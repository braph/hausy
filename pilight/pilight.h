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

#ifndef Hausy_Pilight_h
#define Hausy_Pilight_h

#include "../hausy.h"

unsigned long pilight_timings_get_callback(
   size_t pos,
   void *int_raw_timings
);

void pilight_timings_set_callback(
   size_t pos,
   unsigned long timing,
   void *int_raw_timings
);

size_t hausy_pilight_create_timings(
   hausy_bitstorage *data,
   size_t data_size,
   int *pilight_raw
);

size_t hausy_pilight_parse_timings(
   hausy_bitstorage **data,
   size_t timings_size,
   int *pilight_raw
);

#endif
