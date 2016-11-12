#include "stringbuilder.h"

/**
 * allocates memory for a StringBuilder
 */
jep_string_builder* jep_create_string_builder()
{
	jep_string_builder* sb = malloc(sizeof(jep_string_builder));
	sb->size = 0;
	sb->cap = 50;
	sb->buffer = malloc(sb->cap);
	sb->buffer[0] = '\0';
	return sb;
}

/**
 * frees memory that was allocated for a StringBuilder
 */
void jep_destroy_string_builder(jep_string_builder* sb)
{
	free(sb->buffer);
	free(sb);
}

/**
 * initialize a new StringBuilder
 */
void jep_init_string_builder(jep_string_builder* sb)
{
	sb->size = 0;
	sb->cap = 50;
	sb->buffer = malloc(sb->cap);
	sb->buffer[0] = '\0';
}

/**
 * adds a new character to a StringBuilder's buffer
 */
void jep_append_char(jep_string_builder* sb, char c)
{
	if(sb->size >= sb->cap - 1)
	{
		int new_cap = sb->cap + sb->cap / 2;
		sb->buffer = realloc(sb->buffer, new_cap);
		sb->cap = new_cap;
	}

	sb->buffer[sb->size++] = c;
	sb->buffer[sb->size] = '\0';
}

/**
 * adds a null-terminated C string to a StringBuilder's buffer
 */
void jep_append_string(jep_string_builder* sb, const char* str)
{
	int i = 0;
	while(str[i] != '\0')
		jep_append_char(sb, str[i++]);
}

/**
 * reads the contents of a file into memory
 */
void jep_scan_file(FILE* file, jep_string_builder *sb)
{
	char buffer[1025];
	size_t s;
	do
	{
		s = fread(buffer, 1, 1024, file);
		if(s > 0)
		{
			buffer[s] = '\0';
			jep_append_string(sb, buffer);
		}
	}while(s > 0);
}
