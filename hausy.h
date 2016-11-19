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

#ifndef Hausy_h
#define Hausy_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HAUSY_PULSE_DIV             34
#define HAUSY_PULSE_LOW             300
#define HAUSY_PULSE_HIGH            900
#define HAUSY_PULSE_FOOTER          HAUSY_PULSE_LOW * HAUSY_PULSE_DIV
#define HAUSY_PULSE_TOLERANCE       200

// Identifier used for broadcasting to all units inside a system,
// or to broadcast to all systems inside a protocol.
#define HAUSY_BROADCAST_ID          63 // '@'

// Bitlength of identifiers such as protocolID, systemID,...
#define HAUSY_ID_BITLENGTH          6 // alphex returns 6bit integer
#define HAUSY_BOOL_BITLENGTH        1

typedef unsigned char hausy_id;
typedef unsigned char hausy_bool;
typedef unsigned char hausy_bitstorage;

unsigned int alphex_ctoui(char c);
unsigned int alphex_stoui(const char *alphex);

int hw_bitRead(unsigned int n, int bit);
unsigned int hw_bitSet(unsigned int n, int bit);
unsigned int hw_bitClear(unsigned int n, int bit);
unsigned int hw_bitWrite(unsigned int n, int bit, int value);

hausy_bitstorage *hausy_allocate(size_t size);

int hausy_read_bit(
   hausy_bitstorage *data,
   size_t pos
);

void hausy_write_bit(
   hausy_bitstorage *data,
   size_t pos,
   int value
);

unsigned long hausy_read_long(
   hausy_bitstorage *data,
   size_t len,
   size_t *at
);

size_t hausy_write_long(
   hausy_bitstorage *data,
   unsigned long long_data,
   size_t len,
   size_t at
);

size_t hausy_parse_request(
   hausy_bitstorage *data,
   size_t data_size,
   hausy_id *protocolID,
   hausy_id *systemID,
   hausy_id *unitID,
   hausy_id *cmdID
);

size_t hausy_create_command(
   hausy_bitstorage *data,
   hausy_id protocolID,
   hausy_id systemID,
   hausy_id unitID,
   hausy_id cmdID
);

size_t hausy_create_timings(
   hausy_bitstorage *data,
   size_t data_size,
   void (*set_timings)(size_t pos , unsigned long timing, void *callback_data),
   void *set_timings_data
);

size_t hausy_parse_timings(
   hausy_bitstorage **data,
   size_t timings_size,
   unsigned long (*get_timings)(size_t pos, void *callback_data),
   void *get_timings_data
);

#ifdef __cplusplus
}
#endif

#endif
