/*
    Functions for building a dynamic string of characters
    Copyright (C) 2016 John Powell

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef JEP_STRING_BUILDER_H
#define JEP_STRING_BUILDER_H

#include <stdlib.h>
#include <stdio.h>

/**
 * a utility for building a dynamic string of characters
 */
typedef struct StringBuilder
{
	char* buffer; /* the character array                    */
	int size;     /* the amount of characters in the array  */
	int cap;      /* the current maximum character capacity */
} jep_string_builder;

/**
 * allocates memory for a StringBuilder
 */
jep_string_builder* jep_create_string_builder();

/**
 * frees memory that was allocated for a StringBuilder
 */
void jep_destroy_string_builder(jep_string_builder* sb);

/**
 * add a new character to a StringBuilder's buffer
 */
void jep_append_char(jep_string_builder* sb, char c);

/**
 * adds a null-terminated C string to a StringBuilder's buffer
 */
void jep_append_string(jep_string_builder* sb, const char* str);

/**
 * reads the contents of a file into memory
 */
void jep_scan_file(FILE* file, jep_string_builder *sb);

#endif /* JEP_STRING_BUILDER_H */
