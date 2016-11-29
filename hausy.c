/*
	Copyright (C) 2016 Benjamin Abendroth

   This file is part of hausy.
	
	hausy is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	hausy is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Make it possible to directly include the source file.
 * This is used to address the inability of arduino-mk to compile and link
 * source files in subdirectories.
 */
#ifndef _HAUSY_CPP
#define _HAUSY_CPP

#include "hausy.h"

/*
 * Initialize hausy_request structure
 */
void hausy_request_init
 (
   hausy_request *req
 )
{
   req->size = 0;
   req->bufsize = 0;
   req->data = NULL;
}

/*
 * Ensure that the buffer in $request can hold $size bits.
 *
 * On sucess:
 *    Return $size
 *
 * On failure:
 *    Return 0
 */
int hausy_request_require_size
 (
   hausy_request *request,
   size_t size
 )
{
   if (size <= request->size)
      return size;

   size_t needed_bytes = getBytesForBits(size);
   size_t needed_n_bitstorage =
      needed_bytes / sizeof(hausy_bitstorage)
      + !!(needed_bytes % sizeof(hausy_bitstorage));

   if (request->bufsize < needed_n_bitstorage) {
      hausy_bitstorage *new_data = (hausy_bitstorage*) realloc(request->data, sizeof(hausy_bitstorage) * needed_n_bitstorage);
      if (! new_data)
         return 0;
      request->data = new_data;
      request->bufsize = needed_n_bitstorage;
   }

   return size;
}

/*
 * Truncate the buffer in $request to minimum size.
 */
int hausy_request_fit
 (
   hausy_request *request
 )
{
   size_t needed_bytes = getBytesForBits(request->size);
   size_t needed_n_bitstorage =
      needed_bytes / sizeof(hausy_bitstorage)
      + !!(needed_bytes % sizeof(hausy_bitstorage));

   if (needed_n_bitstorage > request->bufsize) {
      hausy_bitstorage *new_data = (hausy_bitstorage*) realloc(request->data, sizeof(hausy_bitstorage) * needed_n_bitstorage);
      if (! new_data)
         return 0;

      request->data = new_data;
      request->bufsize = needed_n_bitstorage;
   }

   return 1;
}

/*
 * Unset an hausy_request.
 */
void hausy_request_free
 (
   hausy_request *request
 )
{
   if (request->data)
      free(request->data);

   request->size = 0;
   request->data = NULL;
   request->bufsize = 0;
}

/*
 * Copies $length bits from $src at $src_start and inserts it into
 * $dest at $dest_start.
 *
 * If $length is 0, copy all available data.
 *
 * $src and $dest may be the same hausy_request, in this case this function
 * behaves like moving the data.
 *
 * On success:
 *    Return pos for new write on $dest
 *
 * On failure:
 *    Return 0
 */
int hausy_request_copy
 (
   hausy_request *src,
   size_t src_start,
   size_t length,
   hausy_request *dest,
   size_t dest_start
 )
{
   if (src_start + length < src->size)
      return 0;

   if (length == 0)
      length = src->size - src_start;

   if (! hausy_request_require_size(dest, dest_start + length))
      return 0;

   /* Moving overlapping sections in the wrong direction will corrupt our data.
    * Therefore we are checking the start positions of $src and $dest lie. */
   size_t i;
   if (src_start < dest_start) {
      for (i = length - 1; i != (size_t)(-1); --i)
         hausy_write_bit(dest, dest_start+i, hausy_read_bit(src, src_start+i));
   }
   else {
      for (i = 0; i < length; ++i) {
         hausy_write_bit(dest, dest_start+i, hausy_read_bit(src, src_start+i));
      }
   }

   if (dest_start + length < dest->size) {
      dest->size = dest_start + length; // update size
   }

   return dest->size;
}

/*
 * Return single bit inside $request.
 * Note: There are no checks if the requested pos exceeds the buffer length.
 */
int hausy_read_bit
 (
   hausy_request *request,
   size_t pos
 )
{
   size_t byte_pos = pos / (CHAR_BIT * sizeof(hausy_bitstorage));
   size_t bit_pos  = pos % (CHAR_BIT * sizeof(hausy_bitstorage));

   return hw_bitRead(
      request->data[byte_pos],
      bit_pos
   );
}

/*
 * Write single bit inside $request.
 * Note: there are no checks if the requested pos exceeds the buffer length.
 */
void hausy_write_bit
 (
   hausy_request *request,
   size_t pos,
   int value
 )
{
   size_t byte_pos = pos / (CHAR_BIT * sizeof(hausy_bitstorage));
   size_t bit_pos  = pos % (CHAR_BIT * sizeof(hausy_bitstorage));

   request->data[byte_pos] = hw_bitWrite(
      request->data[byte_pos],
      bit_pos,
      value 
   );
}

/* 
 * Reads $len bits from $request and saves it in 32bit type $dest.
 *
 * On success:
 *    Return position for the next read.
 *
 * On failure:
 *    Return 0.
 */
size_t hausy_read_32
 (
   hausy_request *request,
   uint32_t *dest,
   uint8_t len,
   size_t pos
 )
{
   if (pos + len > request->size)
      return 0;

   *dest = 0;
   for (; len != (uint8_t)(-1); --len, ++pos)
      *dest = hw_bitWrite(*dest, len, hausy_read_bit(request, pos));

   return pos;
}

/*
 * Pack the first $len bits of 32bit type $src into $request.
 * $request will be resized if necessary.
 *
 * On success:
 *    Return the new position for a write.
 *
 * On error:
 *    Return 0
 */
size_t hausy_write_32
 (
   hausy_request *request,
   uint32_t src,
   uint8_t len,
   size_t pos
 )
{
   if (! hausy_request_require_size(request, pos + len))
      return 0;

   for (; len != (uint8_t)(-1); --len, ++pos)
      hausy_write_bit(request, pos, hw_bitRead(src, len));

   return (request->size = pos);
}

/*
 * Reads $timings_size timings using the $get_timings callback and
 * stores the result in $request.
 *
 * On success:
 *    Returns $request->size.
 *
 * On failure:
 *    Returns 0.
 */
size_t hausy_parse_timings
 (
   hausy_request *request,
   size_t timings_size,
   unsigned long (*get_timings)(size_t pos, void *get_timings_data),
   void *get_timings_data
 )
{
   if (timings_size < (2 + 2)) // at least 1 data packet + footer
      return 0;

   if (timings_size % 2) // hausy requests are always of even length
      return 0;

   if (! hausy_is_high_pulse(get_timings(timings_size - 2, get_timings_data)))
      return 0;

   if (! hausy_is_footer_pulse(get_timings(timings_size - 1, get_timings_data)))
      return 0;

   if (! hausy_request_require_size(request, ((timings_size - 2) / 2)))
      return 0;

   size_t i;
   for (i = 0; i < (timings_size - 2); i += 2) {
      unsigned long p1 = get_timings(i + 0, get_timings_data);
      unsigned long p2 = get_timings(i + 1, get_timings_data);

      if (hausy_is_high_pulse(p1) && hausy_is_low_pulse(p2)) {
         hausy_write_bit(request, i/2, 1);
      }
      else if (hausy_is_low_pulse(p1) && hausy_is_high_pulse(p2)) {
         hausy_write_bit(request, i/2, 0);
      }
      else {
         hausy_request_free(request);
         return 0;
      }
   }

   request->size = ((timings_size - 2 ) / 2);
   return request->size;
}

/*
 * Translates the bits held in $request to timings using the
 * set_timings callback.
 *
 * Returns the size of timings written.
 *
 * If set_timings callback is NULL do nothing but return the number
 * of timings that would have be written.
 */
size_t hausy_create_timings
 (
   hausy_request *request,
   void (*set_timings)(size_t pos, unsigned long timing, void *set_timings_data),
   void *set_timings_data
 )
{
   if (set_timings == NULL)
      return (request->size * 2 + 2);

   size_t i;
   for (i = 0; i < request->size; ++i) {
      if (hausy_read_bit(request, i)) {
         set_timings( ((1+i) * 2 - 2), HAUSY_PULSE_HIGH, set_timings_data );
         set_timings( ((1+i) * 2 - 1), HAUSY_PULSE_LOW,  set_timings_data );
      } else {
         set_timings( ((1+i) * 2 - 2), HAUSY_PULSE_LOW,  set_timings_data );
         set_timings( ((1+i) * 2 - 1), HAUSY_PULSE_HIGH, set_timings_data );
      }
   }

   set_timings( (request->size * 2 + 0), HAUSY_PULSE_HIGH,   set_timings_data );
   set_timings( (request->size * 2 + 1), HAUSY_PULSE_FOOTER, set_timings_data );

   return (request->size * 2 + 2);
}

/*
 * Helper function for parsing an incoming command.
 * The identifiers are saved to the variables and the extracted data
 * will be stripped from the request.
 *
 * On success:
 *    Returns 1.
 *
 * On failure:
 *    Returns 0.
 */
size_t hausy_parse_request
 (
   hausy_request *request,
   hausy_id *protocolID,
   hausy_id *systemID,
   hausy_id *unitID,
   hausy_id *cmdID
 )
{
   if (request->size < (HAUSY_ID_BITLENGTH * 4))
      return 0;

   size_t pos = 0;
   pos = hausy_read_32(request, (uint32_t*) protocolID, HAUSY_ID_BITLENGTH, pos);
   pos = hausy_read_32(request, (uint32_t*) systemID,   HAUSY_ID_BITLENGTH, pos);
   pos = hausy_read_32(request, (uint32_t*) unitID,     HAUSY_ID_BITLENGTH, pos);
   pos = hausy_read_32(request, (uint32_t*) cmdID,      HAUSY_ID_BITLENGTH, pos);

   hausy_request_copy(request, pos, 0, request, 0);
   request->size = request->size - pos;
   hausy_request_fit(request);

   return 1;
}

/*
 * Helper function for filling a hausy_request with IDs.
 * If $request is NULL return the size that would have been written.
 * Returns position for next write.
 */
size_t hausy_create_request
 (
   hausy_request *request,
   hausy_id protocolID,
   hausy_id systemID,
   hausy_id unitID,
   hausy_id cmdID
 )
{
   if (request == NULL)
      return (HAUSY_ID_BITLENGTH * 4);

   hausy_request_require_size(request, HAUSY_ID_BITLENGTH * 4);

   int pos = 0;
   pos = hausy_write_32(request, protocolID, HAUSY_ID_BITLENGTH, pos);
   pos = hausy_write_32(request, systemID,   HAUSY_ID_BITLENGTH, pos);
   pos = hausy_write_32(request, unitID,     HAUSY_ID_BITLENGTH, pos);
   pos = hausy_write_32(request, cmdID,      HAUSY_ID_BITLENGTH, pos);
   return pos;
}

/*
 * Parse string either as alphex or as int.
 *
 * On success:
 *    Return value of id
 *
 * On error:
 *    return -1
 */
unsigned int hausy_parse_id(const char *id) {
   if (id[0] == '0' && id[1] == 'a')
      return alphex_stoui(&(id[2]));

   return (unsigned int) -1;
}

/*
 * Create hausy id (prefixed with "0a") from int
 */
char *hausy_create_id(unsigned int id) {
   char *id_str = alphex_uitos(id);

   int len = 0;
   while (id_str[len] != '\0')
      ++len;

   char *id_with_prefix = (char*) malloc(len + 3);
   id_with_prefix[0] = '0';
   id_with_prefix[1] = 'a';
   id_with_prefix[len + 2] = '\0';
   for (--len; len >= 0; --len)
      id_with_prefix[2 + len] = id_str[len];

   free(id_str);
   return id_with_prefix;
}

/* 
 * Convert "alphex" char (6bit) to integer.
 */
unsigned int alphex_ctoui(char c) {
   if (c >= '0' && c <= '9')
      return (c - '0');
   if (c >= 'a' && c <= 'z')
      return (c - 'a' + 10);
   if (c >= 'A' && c <= 'Z')
      return (c - 'A' + 10 + 26);
   if (c == '_')
      return (62);
   if (c == '@')
      return (63);

   return ((unsigned) -1);
}

/* 
 * Convert "alphex" string to int.
 */
unsigned int alphex_stoui(const char *alphex) {
   unsigned int i = alphex_ctoui(*alphex);
   while (*++alphex) {
      i <<= 6;
      i += alphex_ctoui(*alphex);
   }
   return i;
}

/*
 * Convert first 6 bits of $i to alphex char
 */
char alphex_uitoc(unsigned int i) {
   i = (i & (1U << 6)) - 1;

   if (i == 63U)
      return '@';
   else if (i == 62U)
      return '_';
   else if (i >= 36U)
      return ('A' + i - 36);
   else if (i >= 10U)
      return ('a' + i - 10);
   else 
      return ('0' + i);
}

/*
 * Convert integer to alphex string
 */
char *alphex_uitos(unsigned int i) {
   if (! i) {
      char *result = (char*) malloc(2);
      result[0] = '0'; result[1] = '\0';
      return result;
   }

   int n = sizeof(unsigned int) * CHAR_BIT / HAUSY_ID_BITLENGTH
         + !!(sizeof(unsigned int) * CHAR_BIT % HAUSY_ID_BITLENGTH);
   char buff[n];
   int x = 0;

   while (i && x < n) {
      buff[x++] = alphex_uitoc(i % 64);
      i = i / 64;
   }

   char *result = (char*) malloc(1 + x);
   result[x] = '\0';

   int r = x - 1;
   int b = 0;

   while (b < x)
      result[r--] = buff[b++];

   return result;
}

#endif // _HAUSY_CPP
