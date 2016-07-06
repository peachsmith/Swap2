#include "stringbuilder.h"

/* allocates memory for a StringBuilder */
jep_string_builder* jep_create_string_builder()
{
	jep_string_builder* sb = malloc(sizeof(jep_string_builder));
	sb->size = 0;
	sb->capacity = 50;
	sb->buffer = malloc(sb->capacity);
	sb->buffer[0] = '\0';
	return sb;
}

/* frees memory that was allocated for a StringBuilder */
void jep_destroy_string_builder(jep_string_builder* sb)
{
	free(sb->buffer);
	free(sb);
}

/* initialize a new StringBuilder */
void jep_init_string_builder(jep_string_builder* sb)
{
	sb->size = 0;
	sb->capacity = 50;
	sb->buffer = malloc(sb->capacity);
	sb->buffer[0] = '\0';
}

/* increases a StringBuilder's capacity by approximately 50 percent. */
void jep_resize_string_builder(jep_string_builder* sb)
{
	int new_capacity;
	char* new_buffer;
	int i;

	new_capacity = sb->capacity + sb->capacity / 2;
	new_buffer = malloc(new_capacity);

	for(i = 0; i < sb->size; i++)
		new_buffer[i] = sb->buffer[i];

	free(sb->buffer);
	sb->buffer = new_buffer;
	sb->capacity = new_capacity;
}

/* add a new character to a StringBuilder's buffer */
void jep_append_char(jep_string_builder* sb, char c)
{
	if(sb->size >= sb->capacity - 1)
		jep_resize_string_builder(sb);

	sb->buffer[sb->size++] = c;
	sb->buffer[sb->size] = '\0';
}

/* adds a null-terminated C string to a StringBuilder's buffer */
void jep_append_string(jep_string_builder* sb, const char* str)
{
	int  i;
	i = 0;
	while(str[i] != '\0')
		jep_append_char(sb, str[i++]);
}
