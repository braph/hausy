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
#include <string.h>

#include "../hausy.h"

int main(int argc, char **argv) {
   if (argc < 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "help"))
      return printf("Usage: %s <int>\n", argv[0]), 1;

   char *alphex_str = hausy_create_id(atoi(argv[1]));
   printf("%s\n", alphex_str);
   free(alphex_str);

   return 0;
}
