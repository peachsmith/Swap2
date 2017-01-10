/*
    Natively implemented functions
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
#include "native.h"

#define JEP_NATIVE_COUNT 11

/* native function identifiers */
const char *natives[] = 
{
	"write", "writeln", "readln", "fopen", "freadln", "fwriteln", "fwrite",
	"freadb", "fwriteb", "byte", "int"
};

/* native function forward declarations */
static jep_obj* jep_write(const char*);
static jep_obj* jep_writeln(const char*);
static jep_obj* jep_readln();
static jep_obj* jep_fopen(const char*, const char*);
static jep_obj* jep_freadln(FILE*);
static jep_obj* jep_fwriteln(FILE*, const char*);
static jep_obj* jep_fwrite(FILE*, const char*);
static jep_obj* jep_freadb(FILE*, size_t);
static jep_obj* jep_fwriteb(FILE*, const unsigned char*, size_t);
static jep_obj* jep_int(jep_obj* obj);

/* calls a native function */
jep_obj* jep_call_native(const char* ident, jep_obj* args)
{
	jep_obj* o = NULL;
	int native = -1;
	int i;
	for(i = 0; i < JEP_NATIVE_COUNT; i++)
	{
		if(!strcmp(ident, natives[i]))
		{
			native = i;
		}
	}

	if(native == -1)
	{
		printf("error: invalid native identifier\n");
		return o;
	}

	if(native == 0 || native == 1) /* write or writeln */
	{
		if(args == NULL || args->size != 1)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* data = args->head;

		char* str = jep_to_string(data);

		if(native == 0)
		{
			o = jep_write(str);	
		}
		else
		{
			o = jep_writeln(str);
		}

		if(str != NULL)
		{
			free(str);
		}
	}
	else if(native == 2) /* readln */
	{
		if(args != NULL && args->size > 0)
		{
			printf("invalid number of arguments\n");
			return o;
		}
		o = jep_readln();
	}
	else if(native == 3) /* open a file */
	{
		if(args == NULL || args->size != 2)
		{
			printf("invalid number of arguments for fopen\n");
			return o;
		}
		jep_obj* path_obj = args->head;
		if(path_obj->type != JEP_STRING 
			|| path_obj->next == NULL
			|| path_obj->next->type != JEP_STRING)
		{
			printf("invalid argument type for fopen\n");
			return o;
		}
		char* path_str = (char*)(path_obj->val);
		if(path_str != NULL)
		{
			o = jep_fopen(path_str, (char*)(path_obj->next->val));
		}
	}
	else if(native == 4) /* freadln */
	{
		if(args == NULL || args->size != 1)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* arg = args->head;

		if(arg == NULL || arg->val == NULL)
		{
			printf("could not read from file\n");
			return o;
		}

		jep_file* file_obj = (jep_file*)(arg->val);

		if(file_obj->file == NULL || !file_obj->open)
		{
			printf("could not read from file\n");
			return o;
		}
		if(file_obj->mode != JEP_READ)
		{
			printf("the file is not open for reading\n");
			return o;
		}
		o = jep_freadln(file_obj->file);
	}
	else if(native == 5) /* fwriteln */
	{
		if(args == NULL || args->size != 2)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* arg = args->head;

		if(arg == NULL || arg->val == NULL || arg->next == NULL)
		{
			printf("could not write to file\n");
			return o;
		}

		jep_file* file_obj = (jep_file*)(arg->val);
		jep_obj* data = arg->next;

		if(file_obj->file == NULL || !file_obj->open)
		{
			printf("could not write to file\n");
			return o;
		}
		if(file_obj->mode != JEP_APPEND)
		{
			printf("the file is not open for appending\n");
			return o;
		}

		char* str = jep_to_string(data);

		o = jep_fwriteln(file_obj->file, str);

		if(str != NULL)
		{
			free(str);
		}
	}
	else if(native == 6) /* fwrite */
	{
		if(args == NULL || args->size != 2)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* arg = args->head;

		if(arg == NULL || arg->val == NULL || arg->next == NULL)
		{
			printf("could not write to file\n");
			return o;
		}

		jep_file* file_obj = (jep_file*)(arg->val);
		jep_obj* data = arg->next;

		if(file_obj->file == NULL || !file_obj->open)
		{
			printf("could not write to file\n");
			return o;
		}
		if(file_obj->mode != JEP_APPEND)
		{
			printf("the file is not open for appending\n");
			return o;
		}

		char* str = jep_to_string(data);

		o = jep_fwrite(file_obj->file, str);

		if(str != NULL)
		{
			free(str);
		}
	}
	else if(native == 7) /* freadb */
	{
		if(args == NULL || args->size != 2)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* arg = args->head;

		if(arg == NULL || arg->val == NULL || arg->next == NULL)
		{
			printf("could not read from file\n");
			return o;
		}

		jep_file* file_obj = (jep_file*)(arg->val);
		jep_obj* size = arg->next;
		
		if(size->type != JEP_INT)
		{
			printf("invalid buffer size for binary file read\n");
			return o;
		}

		int n = *((int*)(size->val));

		if(file_obj->file == NULL || !file_obj->open)
		{
			printf("could not read from file\n");
			return o;
		}
		if(file_obj->mode != JEP_READ_BINARY)
		{
			printf("the file is not open for binary reading\n");
			return o;
		}
		o = jep_freadb(file_obj->file, n);
	}
	else if(native == 8) /* fwriteb */
	{
		if(args == NULL || args->size != 2)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* arg = args->head;

		if(arg == NULL || arg->val == NULL || arg->next == NULL)
		{
			printf("could not write to file\n");
			return o;
		}

		jep_file* file_obj = (jep_file*)(arg->val);
		jep_obj* data = arg->next;
		unsigned char* byte_array = NULL;

		if(file_obj->file == NULL || !file_obj->open)
		{
			printf("could not write to file\n");
			return o;
		}
		if(file_obj->mode != JEP_APPEND_BINARY)
		{
			printf("the file is not open for binary appending\n");
			return o;
		}

		/* jep_obj* bytes = jep_get_bytes(data); */

		if(data == NULL)
		{
			printf("could not covnert data into binary\n");
			return o;
		}
		else
		{
			byte_array = NULL;
			size_t s = data->size;
			jep_obj* b = ((jep_obj*)(data->val))->head;
			byte_array = malloc(s);
			unsigned int i;
			for(i = 0; i < s && b != NULL; i++, b = b->next)
			{
				byte_array[i] = *((unsigned char*)(b->val));
			}
			
			o = jep_fwriteb(file_obj->file, byte_array, s);
		}

		if(byte_array != NULL)
		{
			free(byte_array);
		}
	}
	else if(native == 9) /* byte */
	{
		if(args == NULL || args->size != 1)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* arg = args->head;

		if(arg == NULL || arg->val == NULL 
			|| (arg->type != JEP_INT && arg->type != JEP_LONG))
		{
			printf("invalid argument for byte truncation\n");
			return o;
		}

		unsigned char *b = malloc(1);

		if(arg->type == JEP_INT)
		{
			int i = *((int*)(arg->val));
			*b = i & UCHAR_MAX;
		}
		else if(arg->type == JEP_LONG)
		{
			long int i = *((long int*)(arg->val));
			*b = i & UCHAR_MAX;
		}

		o = jep_create_object();
		o->type = JEP_BYTE;
		o->val = b;
	}
	else if(native == 10) /* int */
	{
		if(args == NULL || args->size != 1)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		return jep_int(args->head);
	}
	else
	{
		printf("native is somehow: %d\n", native);
	}
	
	return o;
}

/* writes a string to standard out */
static jep_obj* jep_write(const char* buffer)
{
	if(buffer != NULL)
	{
		fputs(buffer, stdout);	
	}
	return NULL;
}

/* writes a string to standard out appended by a newline */
static jep_obj* jep_writeln(const char* buffer)
{
	if(buffer != NULL)
	{
		puts(buffer);	
	}
	return NULL;
}

/* reads a string from standard in */
static jep_obj* jep_readln()
{
	jep_obj* o = NULL;

	o = jep_freadln(stdin);

	return o;
}

/**
 * opens a file
 */
static jep_obj* jep_fopen(const char* path_str, const char* mode)
{
	FILE* file = fopen(path_str, mode);
	jep_obj* o = NULL;
	o = jep_create_object();
	o->type = JEP_NULL;

	if(file != NULL)
	{
		jep_file* file_val = malloc(sizeof(jep_file));
		file_val->file = file;
		file_val->open = 1;
		file_val->refs = 1;
		if(!strcmp(mode, "r"))
		{
			file_val->mode = JEP_READ;
		}
		else if(!strcmp(mode, "a"))
		{
			file_val->mode = JEP_APPEND;
		}
		else if(!strcmp(mode, "rb"))
		{
			file_val->mode = JEP_READ_BINARY;
		}
		else if(!strcmp(mode, "ab"))
		{
			file_val->mode = JEP_APPEND_BINARY;
		}
		else
		{
			file_val->mode = 0;
		}
		
		o->type = JEP_FILE;
		o->val = file_val;
	}

	return o;
}

static jep_obj* jep_freadln(FILE* file)
{
	jep_obj* o = NULL;

	size_t size = 0;
	size_t len  = 0;
	size_t last = 0;
	char *buffer = NULL;

	do {

		size += BUFSIZ;
		buffer = realloc(buffer, size);

		if (buffer == NULL) 
		{
			printf("failed to reallocate buffer for reading\n");
			return NULL;
		}
		if(fgets(buffer + last, size, file) == NULL)
		{
			if(buffer != NULL)
			{
				free(buffer);
			}

			o = jep_create_object();

			/* check if EOF or just empty line */
			if(feof(file))
			{
				o->type = JEP_NULL;
			}
			else
			{
				/* create an empty string */
				char* empty = malloc(1);
				empty[0] = '\0';
				o->type = JEP_STRING;
				o->val = empty;
			}	

			return o;
		}
		len = strlen(buffer);
		last = len - 1;
	} while (!feof(file) && buffer[last] != '\n');

	if(buffer != NULL)
	{
		/* remove the newline from the end */
		if(buffer[last] == '\n')
		{
			buffer[last] = '\0';
		}

		o = jep_create_object();
		o->type = JEP_STRING;
		o->val = (void*)(buffer);
	}

	return o;
}

static jep_obj* jep_fwriteln(FILE* file, const char* data)
{
	jep_obj* o = NULL;

	fprintf(file, "%s\n", data);
	fflush(file);

	return o;
}

static jep_obj* jep_fwrite(FILE* file, const char* data)
{
	jep_obj* o = NULL;

	fprintf(file, "%s", data);
	fflush(file);

	return o;
}

static jep_obj* jep_freadb(FILE* file, size_t n)
{
	jep_obj* byte_array = NULL;
	jep_obj* bytes = NULL;
	unsigned char *data = malloc(n);
	size_t read = fread(data, n, 1, file);

	if(read)
	{
		byte_array = jep_create_object();
		byte_array->type = JEP_ARRAY;

		bytes = jep_create_object();
		bytes->type = JEP_LIST;

		unsigned int i;
		for(i = 0; i < n; i++)
		{
			jep_obj* byte = jep_create_object();
			byte->type = JEP_BYTE;
			unsigned char* c = malloc(1);
			*c = data[i];
			byte->val = c;
			jep_add_object(bytes, byte);
		}

		byte_array->size = bytes->size;
		byte_array->val = bytes;
	}

	free(data);

	return byte_array;
}

static jep_obj* jep_fwriteb(FILE* file, const unsigned char* data, size_t n)
{
	jep_obj* written = NULL;

	size_t read = fwrite(data, n, 1, file);
	fflush(file);

	written = jep_create_object();
	written->type = JEP_INT;
	int* i = malloc(sizeof(int));
	*i = (int)read;
	written->val = i;

	return written;
}

static jep_obj* jep_int(jep_obj* obj)
{
	jep_obj* i = NULL;
	
	if(obj->type == JEP_INT)
	{
		i = jep_create_object();
		jep_copy_object(i, obj);
		return i;
	}
	else if(obj->type == JEP_LONG)
	{
		i = jep_create_object();
		i->type = JEP_INT;
		*(int*)(i->val) = (int)(*(long*)(obj->val));
		return i;
	}
	else if(obj->type == JEP_DOUBLE)
	{
		i = jep_create_object();
		i->type = JEP_INT;
		*(int*)(i->val) = (int)(*(double*)(obj->val));
		return i;
	}
	else if(obj->type == JEP_CHARACTER)
	{
		i = jep_create_object();
		i->type = JEP_INT;
		*(int*)(i->val) = (int)(*(char*)(obj->val) - '0');
		return i;
	}
	else if(obj->type == JEP_STRING)
	{
		if(obj->val == NULL)
		{
			printf("invalid integer format\n");
			return i;
		}

		int neg = 0;
		char* s;
		char* endptr;
		long int l;
		void* val;
		
		errno = 0;
		endptr = NULL;
		s = NULL;

		s = (char*)(obj->val);

		/* allow for signed integers */
		if(*s == '-')
		{
			s++;
			neg++;
		}

		/* attempt to convert the string to a long int */
		l = strtol(s, &endptr, 10);

		if(errno == ERANGE)
		{
			printf("value to large to fit in an integer\n");
		}
		else if((endptr != s && *endptr != '\0') 
			|| (strlen(s) > 1 && *s == '0') || !isdigit(*s))
		{
			printf("invalid integer format\n");
		}
		else if(l >= INT_MIN && l <= INT_MAX)
		{
			/* cast the value as an int if it will fit */
			int* l_ptr = malloc(sizeof(int));
			if(neg)
			{
				*l_ptr = -(int)l;	
			}
			else
			{
				*l_ptr = (int)l;	
			}
			val = (void*)l_ptr;
			i = jep_create_object();
			i->val = (void*)val;
			i->type = JEP_INT;
		}
		else
		{
			printf("value to large to fit in an integer\n");
		}

		return i;
	}
	else
	{
		printf("cannot convert object to integer\n");
		printf("must be int, long, double, character, or string\n");
	}

	return i;
}
