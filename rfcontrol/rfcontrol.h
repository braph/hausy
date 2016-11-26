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

#ifndef _HAUSY_RFCONTROL_H
#define _HAUSY_RFCONTROL_H

#include "../hausy.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t hausy_rfcontrol_parse_timings(
   hausy_bitstorage **data,
   unsigned int *timings,
   unsigned int timings_size,
   unsigned int pulse_length_divider
);

size_t hausy_rfcontrol_create_timings(
   hausy_bitstorage *data,
   size_t data_size,
   unsigned int *timings
);

size_t hausy_rfcontrol_create_compressed_timings(
   hausy_bitstorage *data,
   size_t data_size,
   unsigned long **buckets,
   char *timings
);

#ifdef __cplusplus
}
#endif

#endif // _HAUSY_RFCONTROL_H
