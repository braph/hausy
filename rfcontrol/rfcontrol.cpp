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

#include "rfcontrol.h"
#include "../hausy.h"

struct get_timings_param_t {
   unsigned int *timings;
   unsigned int pulse_length_divider;
};

unsigned long hausy_rfcontrol_get_timings_callback
 (
   size_t pos,
   void *params
 )
{
   return (
      (unsigned long) ((struct get_timings_param_t*) params)->pulse_length_divider *
      (unsigned long) ((struct get_timings_param_t*) params)->timings[pos]
   );
}

void hausy_rfcontrol_set_timings_callback
 (
   size_t pos,
   unsigned long timing,
   void *int_timings
 )
{
   ((int*) int_timings)[pos] = timing;
}
   
void hausy_rfcontrol_set_compressed_timings_callback
 (
   size_t pos,
   unsigned long timing,
   void *char_timings
 )
{
   if (timing == HAUSY_PULSE_LOW)
      ((char*) char_timings)[pos] = '0';
   else if (timing == HAUSY_PULSE_HIGH)
      ((char*) char_timings)[pos] = '1';
   else if (timing == HAUSY_PULSE_FOOTER)
      ((char*) char_timings)[pos] = '2';
}

/*
 * Parses timings of size $timings_size held in $timings and writes
 * the result to newly allocated $data.
 *
 * On success:
 *    Returns the number of bits written to $data.
 *
 * On failure:
 *    Returns 0
 */
size_t hausy_rfcontrol_parse_timings
 (
   hausy_bitstorage **data,
   unsigned int *timings,
   unsigned int timings_size,
   unsigned int pulse_length_divider
 )
{
   struct get_timings_param_t param;
   param.timings = timings;
   param.pulse_length_divider = pulse_length_divider;

   return hausy_parse_timings(data, timings_size, hausy_rfcontrol_get_timings_callback, &param);
}

/*
 * Transforms request held in $data of size $data_size to the
 * RFControl format (int[]).
 *
 * If $timings is NULL, do nothing but return the number of
 * timings that would have been written to $timings.
 *
 * Returns the number of timings written to $timings.
 */
size_t hausy_rfcontrol_create_timings
 (
   hausy_bitstorage *data,
   size_t data_size,
   unsigned int *timings
 )
{
   if (timings == NULL)
      return hausy_create_timings(data, data_size, NULL, NULL);

   return hausy_create_timings(
      data,
      data_size,
      hausy_rfcontrol_set_timings_callback,
      (void*) timings
   );
}

/*
 * Transforms request held in $data of size $data_size to the
 * compressed RFControl format (char[]).
 *
 * It also allocates and fills the needed buckets[] array.
 *
 * If $timings is NULL, do nothing but return the number of
 * timings that would have been written to $timings.
 *
 * Returns the number of timings written to $timings.
 */
size_t hausy_rfcontrol_create_compressed_timings
 (
   hausy_bitstorage *data,
   size_t data_size,
   unsigned long **buckets,
   char *timings
 )
{
   if (timings == NULL)
      return hausy_create_timings(data, data_size, NULL, NULL) + 1 /*end of string*/;

   *buckets = (unsigned long *) malloc(sizeof(unsigned long) * 3);
   if (! *buckets)
      return 0;

   (*buckets)[0] = HAUSY_PULSE_LOW;
   (*buckets)[1] = HAUSY_PULSE_HIGH;
   (*buckets)[2] = HAUSY_PULSE_FOOTER;

   size_t timings_size = hausy_create_timings(
      data,
      data_size,
      hausy_rfcontrol_set_compressed_timings_callback,
      (void*) timings
   );

   timings[timings_size] = 0 /*end of string*/;
   return timings_size + 1;
}
