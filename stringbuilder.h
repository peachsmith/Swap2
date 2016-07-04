#ifndef JEP_STRING_BUILDER_H
#define JEP_STRING_BUILDER_H

#include <stdlib.h>
#include <stdio.h>

typedef struct StringBuilder
{
	char* buffer;
	int size;
	int capacity;
} jep_string_builder;

/* allocates memory for a StringBuilder */
jep_string_builder* jep_create_string_builder();

/* frees memory that was allocated for a StringBuilder */
void jep_destroy_string_builder(jep_string_builder* sb);

/* initializes a StringBuilder */
void jep_init_string_builder(jep_string_builder* sb);

/* increases a StringBuilder's capacity by approximately 50 percent. */
void jep_resize_string_builder(jep_string_builder* sb);

/* add a new character to a StringBuilder's buffer */
void jep_append_char(jep_string_builder* sb, char c);

/* adds a null-terminated C string to a StringBuilder's buffer */
void jep_append_string(jep_string_builder* sb, const char* str);

#endif /* JEP_STRING_BUILDER_H */
