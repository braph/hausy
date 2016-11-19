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

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "../hausy.h"

#define MAX_BITS        1024
#define MAX_INT_DIGITS  10

void print_timing
 (
   size_t pos,
   unsigned long timing,
   void *data
 )
{
   printf("%lu ", timing);
}

int main(int argc, char **argv) {
   if (argc < 2 || !strcmp(argv[1], "help") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
      printf("Usage: %s value[:bitlength] ...\n", argv[0]);
      return 1;
   }

   hausy_bitstorage *data = hausy_allocate(MAX_BITS);
   size_t data_pos = 0;

   for (char **arg = &argv[1]; *arg; ++arg) {
      char buffer[MAX_INT_DIGITS + 1];
      unsigned int value;
      unsigned int length;
      char *endptr;
      size_t i;
      size_t stri;

      for (stri = 0, i = 0; stri < strlen(*arg); ++stri, ++i) {
         if ((*arg)[i] == ':') {
            ++stri;
            break;
         }

         if (i >= MAX_INT_DIGITS) {
            printf("Too many digits: %s\n", *arg);
            return 1;
         }

         buffer[i] = (*arg)[stri];
      }
      buffer[i] = 0;

      if (buffer[0] == '0' && buffer[1] == 'a') {
         value = alphex_stoui(&buffer[2]);
      }
      else {
         value = strtoimax(buffer, &endptr, 0);
         if (*endptr != 0) {
            printf("invalid value: %s", buffer);
            return 1;
         }
      }

      if (stri >= strlen(*arg)) {
         length = HAUSY_ID_BITLENGTH;
      }
      else {
         length = strtoumax(&(*arg)[stri], &endptr, 0);
         if (*endptr != 0) {
            printf("invalid length: %s\n", &(*arg)[stri]);
            return 1;
         }
      }

      if (data_pos + length >= MAX_BITS) {
         printf("Too many data, max %d bits\n", MAX_BITS);
         return 1;
      }

      size_t old_pos = data_pos;
      data_pos = hausy_write_long(data, value, length, data_pos);

      while (old_pos++ < data_pos)
         printf("%d", hausy_read_bit(data, old_pos));
      printf(" ");
   }

   printf("\n");
   hausy_create_timings(data, data_pos, print_timing, NULL);
   printf("\n");

   free(data);

   return 0;
}
