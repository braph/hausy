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

#include "hausy.h"
#include <limits.h> // CHAR_BIT

/*
 * Reads $timings_size timings using the $get_timings callback and
 * stores the result in the newly allocated $data buffer.
 *
 * On success:
 *    Returns the number of bits written to $data.
 *    Remember to free $data.
 *
 * On failure:
 *    Returns 0, $data will be NULL.
 *    You don't need to free $data.
 */
size_t hausy_parse_timings
 (
   hausy_bitstorage **data,
   size_t timings_size,
   unsigned long (*get_timings)(size_t pos, void *get_timings_data),
   void *get_timings_data
 )
{
   *data = NULL;

   if (timings_size < (2 + 2)) // at least 1 data packet + footer
      return 0;

   if (timings_size % 2) // hausy requests are always of even length
      return 0;

   if (! hausy_is_high_pulse(get_timings(timings_size - 2, get_timings_data)))
      return 0;

   if (! hausy_is_footer_pulse(get_timings(timings_size - 1, get_timings_data)))
      return 0;

   *data = hausy_allocate((timings_size - 2) / 2);
   if (! *data)
      return 0;

   size_t i;
   for (i = 0; i < (timings_size - 2); i += 2) {
      unsigned long p1 = get_timings(i + 0, get_timings_data);
      unsigned long p2 = get_timings(i + 1, get_timings_data);

      if (hausy_is_high_pulse(p1) && hausy_is_low_pulse(p2)) {
         hausy_write_bit(*data, i/2, 1);
      }
      else if (hausy_is_low_pulse(p1) && hausy_is_high_pulse(p2)) {
         hausy_write_bit(*data, i/2, 0);
      }
      else {
         free(*data);
         *data = NULL;
         return 0;
      }
   }

   return ((timings_size - 2 ) / 2);
}

/*
 * Translates the bits held in $data of size $data_size using the
 * set_timings callback.
 *
 * Returns the size of timings written.
 *
 * If set_timings callback is NULL do nothing but return the number
 * of timings that would have be written.
 */
size_t hausy_create_timings
 (
   hausy_bitstorage *data,
   size_t data_size,
   void (*set_timings)(size_t pos, unsigned long timing, void *set_timings_data),
   void *set_timings_data
 )
{
   if (set_timings == NULL)
      return (data_size * 2 + 2);

   size_t i;
   for (i = 0; i < data_size; ++i) {
      if (hausy_read_bit(data, i)) {
         set_timings( ((1+i) * 2 - 2), HAUSY_PULSE_HIGH, set_timings_data );
         set_timings( ((1+i) * 2 - 1), HAUSY_PULSE_LOW,  set_timings_data );
      } else {
         set_timings( ((1+i) * 2 - 2), HAUSY_PULSE_LOW,  set_timings_data );
         set_timings( ((1+i) * 2 - 1), HAUSY_PULSE_HIGH, set_timings_data );
      }
   }

   set_timings( (data_size * 2 + 0), HAUSY_PULSE_HIGH,   set_timings_data );
   set_timings( (data_size * 2 + 1), HAUSY_PULSE_FOOTER, set_timings_data );

   return (data_size * 2 + 2);
}

/*
 * Allocates enough space for holding $data_size bits and returns
 * a pointer the allocated memory.
 *
 * On sucess:
 *    Returns pointer to allocated memory.
 *
 * On failure:
 *    Returns NULL.
 */
hausy_bitstorage* hausy_allocate
 (
   size_t data_size
 )
{
   size_t needed_bytes = data_size / CHAR_BIT + !!(data_size % CHAR_BIT);

   size_t needed_n_bitstorage =
      needed_bytes / sizeof(hausy_bitstorage)
      + !!(needed_bytes % sizeof(hausy_bitstorage))
      ;

   return (hausy_bitstorage*) malloc(sizeof(hausy_bitstorage) * needed_n_bitstorage);
}

/*
 * Return single bit inside hausy_bitstorage
 */
int hausy_read_bit
 (
   hausy_bitstorage *data,
   size_t pos
 )
{
   size_t byte_pos = pos / (CHAR_BIT * sizeof(hausy_bitstorage));
   size_t bit_pos  = pos % (CHAR_BIT * sizeof(hausy_bitstorage));

   return hw_bitRead(
      data[byte_pos],
      bit_pos
   );
}

/*
 * Write single bit inside hausy_bitstorage
 */
void hausy_write_bit
 (
   hausy_bitstorage *data,
   size_t pos,
   int value
 )
{
   size_t byte_pos = pos / (CHAR_BIT * sizeof(hausy_bitstorage));
   size_t bit_pos  = pos % (CHAR_BIT * sizeof(hausy_bitstorage));

   data[byte_pos] = hw_bitWrite(
      data[byte_pos],
      bit_pos,
      value 
   );
}

/* 
 * Reads $len bits from data and returns it as a long type.
 * Returns the value that was read from data and
 * sets $at to the position for the next read.
 */
unsigned long hausy_read_long
 (
   hausy_bitstorage *data,
   size_t len,
   size_t *at
 )
{
   unsigned long rByte = 0;

   size_t i;
   for (i = 0; i < len; ++i)
      rByte = hw_bitWrite(rByte, i, hausy_read_bit(data, *at+i));

   *at = *at + len;
   return rByte;
}

/*
 * Pack the first $len bits of $long_data into $data.
 * Returns the new position for a write.
 */
size_t hausy_write_long
 (
   hausy_bitstorage *data,
   unsigned long long_data,
   size_t len,
   size_t at
 )
{
   size_t i;
   for (i = 0; i < len; ++i)
      hausy_write_bit(data, at+i, hw_bitRead(long_data, i));

   return (at + len);
}

/*
 * Helper function for parsing an incoming command.
 * If $data is NULL return the size that would have been read.
 *
 * On success:
 *    Returns position for next read.
 *
 * On failure:
 *    Returns 0.
 */
size_t hausy_parse_request
 (
   hausy_bitstorage *data,
   size_t data_size,
   hausy_id *protocolID,
   hausy_id *systemID,
   hausy_id *unitID,
   hausy_id *cmdID
 )
{
   if (data == NULL)
      return (HAUSY_ID_BITLENGTH * 4);

   if (data_size < (HAUSY_ID_BITLENGTH * 4))
      return 0;

   size_t pos = 0;
   *protocolID = hausy_read_long(data, HAUSY_ID_BITLENGTH, &pos);
   *systemID   = hausy_read_long(data, HAUSY_ID_BITLENGTH, &pos);
   *unitID     = hausy_read_long(data, HAUSY_ID_BITLENGTH, &pos);
   *cmdID      = hausy_read_long(data, HAUSY_ID_BITLENGTH, &pos);
   return pos;
}

/*
 * Helper function for filling a hausy_bitstorage buffer with IDs.
 * If $data is NULL return the size that would have been written.
 * Returns position for next write.
 */
size_t hausy_create_command
 (
   hausy_bitstorage *data,
   hausy_id protocolID,
   hausy_id systemID,
   hausy_id unitID,
   hausy_id cmdID
 )
{
   if (data == NULL)
      return (HAUSY_ID_BITLENGTH * 4);

   int at = 0;
   at = hausy_write_long(data, protocolID, HAUSY_ID_BITLENGTH, at);
   at = hausy_write_long(data, systemID,   HAUSY_ID_BITLENGTH, at);
   at = hausy_write_long(data, unitID,     HAUSY_ID_BITLENGTH, at);
   at = hausy_write_long(data, cmdID,      HAUSY_ID_BITLENGTH, at);
   return at;
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
   i = (i & (1U << 6) - 1);

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
char *alphex_uitos(unsigned int i) {
}
*/

/*
 * Helper function for reading single bit of an integer.
 * Returns value of bit.
 */
int hw_bitRead(unsigned int n, int bit) {
   return ((n >> (bit + 0)) & 1U);
}

/*
 * Helper function for setting single bit of an integer.
 * Returns new integer.
 */
unsigned int hw_bitSet(unsigned int n, int bit) {
   return (n | 1U << (bit + 0));
}

/*
 * Helper function for clearing single bit of an integer.
 * Returns new integer.
 */
unsigned int hw_bitClear(unsigned int n, int bit) {
   return (n & ~(1U << (bit + 0)));
}

/*
 * Helper function for writing single bit of an integer.
 * Returns new integer.
 */
unsigned int hw_bitWrite(unsigned int n, int bit, int value) {
   if (value)
      return hw_bitSet(n, bit);
   else
      return hw_bitClear(n, bit);
   //return (n ^ (-state ^ n) & (1U << bit)); TODO?
}

