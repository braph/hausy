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

#include "pilight.h"
#include "../hausy.h"

unsigned long pilight_timings_get_callback
 (
   size_t pos,
   void *int_raw_timings
 )
{
   return (unsigned long) ((int*)int_raw_timings)[pos];
}

void pilight_timings_set_callback
 (
   size_t pos,
   unsigned long timing,
   void *int_raw_timings
 )
{
   ((int*)int_raw_timings)[pos] = timing;
}

/*
 * Transforms request held in $data of size $data_size to the
 * pilight raw format (int *).
 *
 * If $pilight_raw is NULL, do nothing but return the number of
 * timings that would have been written to $pilight_raw.
 *
 * Returns the number of timings written to $pilight_raw.
 */
size_t hausy_pilight_create_timings
 (
   hausy_bitstorage *data,
   size_t data_size,
   int *pilight_raw
 )
{
   if (pilight_raw == NULL)
      return hausy_create_timings(data, data_size, NULL, NULL);

   return hausy_create_timings(
      data,
      data_size,
      pilight_timings_set_callback,
      (void*) pilight_raw
   );
}

/*
 * Parses timings of size $timings_size held in $pilight_raw and writes
 * the result into newly allocated $data.
 *
 * On success:
 *    Returns the number of bits written into $data.
 *
 * On failure:
 *    Returns 0.
 */
size_t hausy_pilight_parse_timings
 (
   hausy_bitstorage **data,
   int *pilight_raw,
   size_t pilight_raw_len
 )
{
   return hausy_parse_timings(
      data,
      pilight_raw_len,
      (void*) pilight_raw
   );
}
