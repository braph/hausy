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

#ifndef _HAUSY_H
#define _HAUSY_H

#include <stdint.h>
#include <stdlib.h>
#include <limits.h> // CHAR_BIT

#ifdef __cplusplus
extern "C" {
#endif

#define HAUSY_PULSE_LOW             300
#define HAUSY_PULSE_HIGH            900
#define HAUSY_PULSE_TOLERANCE       200
#define HAUSY_PULSE_FOOTER          12000
#define HAUSY_FOOTER_TOLERANCE      (HAUSY_PULSE_TOLERANCE * 20)

// Identifier used for broadcasting to all units inside a system,
// or to broadcast to all systems inside a protocol.
#define HAUSY_BROADCAST_ID          63 // '@'

// Bitlength of identifiers such as protocolID, systemID,...
#define HAUSY_ID_BITLENGTH          6 // alphex returns 6bit integer
#define HAUSY_ID_MAX                ((1 << HAUSY_ID_BITLENGTH) - 1)
#define HAUSY_BOOL_BITLENGTH        1

typedef uint8_t hausy_id;
typedef uint8_t hausy_bool;
typedef uint8_t hausy_bitstorage;

typedef struct hausy_request {
   uint16_t size;    /* holds the request size of data */
   uint8_t bufsize;  /* holds the number of hausy_bitstorage fields */
   hausy_bitstorage *data;
} hausy_request;

unsigned int alphex_ctoui(char c);
unsigned int alphex_stoui(const char *alphex);
char alphex_uitoc(unsigned int i);
char *alphex_uitos(unsigned int i);

unsigned int hausy_parse_id(const char *id);
char *hausy_create_id(unsigned int id);

/* Alternative to hausy_request_init() */
#define HAUSY_REQUEST_INITIALIZER { .size = 0, .bufsize = 0, .data = NULL }
void hausy_request_init(hausy_request *request);
size_t hausy_request_require_size(hausy_request *request, size_t size);
int hausy_request_fit(hausy_request *request);
void hausy_request_free(hausy_request *request);
int hausy_request_eqcmp(hausy_request *a, hausy_request *b);

size_t hausy_request_copy(
   hausy_request *src,
   size_t src_start,
   size_t length,
   hausy_request *dest,
   size_t dest_start
);

size_t hausy_read_32(
   hausy_request *request,
   uint32_t *dest,
   uint8_t len,
   size_t pos
);

size_t hausy_write_32(
   hausy_request *request,
   uint32_t src,
   uint8_t len,
   size_t pos
);

size_t hausy_parse_request(
   hausy_request *request,
   hausy_id *protocolID,
   hausy_id *systemID,
   hausy_id *unitID,
   hausy_id *cmdID
);

size_t hausy_create_request(
   hausy_request *request,
   hausy_id protocolID,
   hausy_id systemID,
   hausy_id unitID,
   hausy_id cmdID
);

size_t hausy_create_timings(
   hausy_request *request,
   void (*set_timings)(size_t pos , unsigned long timing, void *callback_data),
   void *set_timings_data
);

size_t hausy_parse_timings(
   hausy_request *request,
   size_t timings_size,
   unsigned long (*get_timings)(size_t pos, void *callback_data),
   void *get_timings_data
);

/*
 * Return true if $pulse is a low pulse.
 */
static inline __attribute__((always_inline))
int hausy_is_low_pulse(unsigned long pulse) {
   return (
      pulse <= ((unsigned long) HAUSY_PULSE_LOW + HAUSY_PULSE_TOLERANCE) &&
      pulse >= ((unsigned long) HAUSY_PULSE_LOW - HAUSY_PULSE_TOLERANCE)
   );
}

/*
 * Return true if $pulse is a high pulse.
 */
static inline __attribute__((always_inline))
int hausy_is_high_pulse(unsigned long pulse) {
   return (
      pulse <= ((unsigned long) HAUSY_PULSE_HIGH + HAUSY_PULSE_TOLERANCE) &&
      pulse >= ((unsigned long) HAUSY_PULSE_HIGH - HAUSY_PULSE_TOLERANCE)
   );
}

/*
 * Return true if $pulse is a footer pulse.
 */
static inline __attribute__((always_inline))
int hausy_is_footer_pulse(unsigned long pulse) {
   return (
      pulse <= ((unsigned long) HAUSY_PULSE_FOOTER + HAUSY_FOOTER_TOLERANCE) &&
      pulse >= ((unsigned long) HAUSY_PULSE_FOOTER - HAUSY_FOOTER_TOLERANCE)
   );
}

/*
 * Helper function for reading single bit of an integer.
 * Returns value of bit.
 */
static inline __attribute__((always_inline))
int hw_bitRead(unsigned int n, int bit) {
   return ((n >> bit) & 1U);
}

/*
 * Helper function for setting single bit of an integer.
 * Returns new integer.
 */
static inline __attribute__((always_inline))
unsigned int hw_bitSet(unsigned int n, int bit) {
   return (n | 1U << bit);
}

/*
 * Helper function for clearing single bit of an integer.
 * Returns new integer.
 */
static inline __attribute__((always_inline))
unsigned int hw_bitClear(unsigned int n, int bit) {
   return (n & ~(1U << bit));
}

/*
 * Helper function for writing single bit of an integer.
 * Returns new integer.
 */
static inline __attribute__((always_inline))
unsigned int hw_bitWrite(unsigned int n, int bit, int value) {
   if (value)
      return (n | 1U << bit);
   else
      return (n & ~(1U << bit));
}

/*
 * Return the number of bytes that are needed to hold $size bits.
 */
static inline __attribute__((always_inline))
size_t getBytesForBits(unsigned int size) {
   return (size / CHAR_BIT + !!(size % CHAR_BIT));
}

/*
 * Return single bit inside $request.
 * Note: There are no checks if the requested pos exceeds the buffer length.
 */
static inline __attribute__((always_inline))
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
 * Set single bit to 1 inside $request.
 * Note: there are no checks if the requested pos exceeds the buffer length.
 */
static inline __attribute__((always_inline))
void hausy_set_bit
 (
   hausy_request *request,
   size_t pos
 )
{
   size_t byte_pos = pos / (CHAR_BIT * sizeof(hausy_bitstorage));
   size_t bit_pos  = pos % (CHAR_BIT * sizeof(hausy_bitstorage));

   request->data[byte_pos] = hw_bitSet(
      request->data[byte_pos],
      bit_pos
   );
}

/*
 * Set single bit to 0 inside $request.
 * Note: there are no checks if the requested pos exceeds the buffer length.
 */
static inline __attribute__((always_inline))
void hausy_clear_bit
 (
   hausy_request *request,
   size_t pos
 )
{
   size_t byte_pos = pos / (CHAR_BIT * sizeof(hausy_bitstorage));
   size_t bit_pos  = pos % (CHAR_BIT * sizeof(hausy_bitstorage));

   request->data[byte_pos] = hw_bitClear(
      request->data[byte_pos],
      bit_pos
   );
}

/*
 * Write single bit inside $request.
 * Note: there are no checks if the requested pos exceeds the buffer length.
 */
static inline __attribute__((always_inline))
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

#ifdef __cplusplus
}
#endif

#endif
