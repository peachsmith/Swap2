#include "swap/SwapNative.h"

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_len(jep_obj *args)
{
	jep_obj *length;
	jep_obj *obj;

	if (args == NULL || args->size != 1)
	{
		return NULL;
	}

	length = jep_create_object();
	length->type = JEP_INT;
	length->val = malloc(sizeof(int));
	*(int *)(length->val) = 0;

	obj = args->head;

	switch (obj->type)
	{
	case JEP_INT:
	case JEP_LONG:
	case JEP_DOUBLE:
	case JEP_CHARACTER:
		*(int *)(length->val) = 1;
		break;

	case JEP_STRING:
	{
		char *str = (char *)(obj->val);
		*(int *)(length->val) = strlen(str);
	}
	break;

	case JEP_ARRAY:
		*(int *)(length->val) = obj->size;
		break;

	default:
		break;
	}

	return length;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_typeof(jep_obj *args)
{
	jep_obj *i = NULL;

	if (args == NULL || args->size != 1)
	{
		i = jep_create_object();
		i->type = JEP_STRING;
		i->ret = JEP_RETURN | JEP_EXCEPTION;
		i->val = malloc(28);
		strcpy(i->val, "invalid number of arguments");
		((char*)(i->val))[27] = '\0';
		return i;
	}

	if (args->head == NULL)
	{
		return NULL;
	}

	jep_obj *type = jep_create_object();
	type->type = JEP_STRING;
	char *str = NULL;

	switch (args->head->type)
	{
	case JEP_INT:
		str = malloc(4);
		strcpy(str, "int");
		break;

	case JEP_LONG:
		str = malloc(5);
		strcpy(str, "long");
		break;

	case JEP_DOUBLE:
		str = malloc(7);
		strcpy(str, "double");
		break;

	case JEP_CHARACTER:
		str = malloc(5);
		strcpy(str, "char");
		break;

	case JEP_STRING:
		str = malloc(7);
		strcpy(str, "string");
		break;

	case JEP_ARRAY:
		str = malloc(6);
		strcpy(str, "array");
		break;

	case JEP_FUNCTION:
		str = malloc(9);
		strcpy(str, "function");
		break;

	case JEP_STRUCTDEF:
		str = malloc(10);
		strcpy(str, "structdef");
		break;

	case JEP_STRUCT:
		str = malloc(7);
		strcpy(str, "struct");
		break;

	case JEP_REFERENCE:
		str = malloc(10);
		strcpy(str, "reference");
		break;

	case JEP_NULL:
		str = malloc(5);
		strcpy(str, "null");
		break;

	default:
		str = malloc(5);
		strcpy(str, "null");
		break;
	}

	type->val = (void *)str;

	return type;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_int(jep_obj *args)
{
	jep_obj *i = NULL;

	if (args == NULL || args->size != 1)
	{
		i = jep_create_object();
		i->type = JEP_STRING;
		i->ret = JEP_RETURN | JEP_EXCEPTION;
		i->val = malloc(28);
		strcpy(i->val, "invalid number of arguments");
		((char*)(i->val))[27] = '\0';
		return i;
	}

	jep_obj *obj = args->head;

	if (obj->type == JEP_INT)
	{
		i = jep_create_object();
		jep_copy_object(i, obj);
		return i;
	}
	else if (obj->type == JEP_LONG)
	{
		i = jep_create_object();
		i->type = JEP_INT;
		i->val = malloc(sizeof(int));
		*(int *)(i->val) = (int)(*(long *)(obj->val));
		return i;
	}
	else if (obj->type == JEP_DOUBLE)
	{
		i = jep_create_object();
		i->type = JEP_INT;
		i->val = malloc(sizeof(int));
		*(int *)(i->val) = (int)(*(double *)(obj->val));
		return i;
	}
	else if (obj->type == JEP_CHARACTER)
	{
		i = jep_create_object();
		i->type = JEP_INT;
		i->val = malloc(sizeof(int));
		*(int *)(i->val) = (int)(*(char *)(obj->val));
		return i;
	}
	else if (obj->type == JEP_STRING)
	{
		if (obj->val == NULL)
		{
			printf("invalid integer format\n");
			return i;
		}

		int neg = 0;
		char *s;
		char *endptr;
		long int l;
		void *val;

		errno = 0;
		endptr = NULL;
		s = NULL;

		s = (char *)(obj->val);

		/* allow for signed integers */
		if (*s == '-')
		{
			s++;
			neg++;
		}

		/* attempt to convert the string to a long int */
		l = strtol(s, &endptr, 10);

		if (errno == ERANGE)
		{
			printf("value to large to fit in an integer\n");
		}
		else if ((endptr != s && *endptr != '\0') || (strlen(s) > 1 && *s == '0') || !isdigit(*s))
		{
			i = jep_create_object();
			i->type = JEP_STRING;
			i->ret = JEP_RETURN | JEP_EXCEPTION;
			i->val = malloc(23);
			strcpy(i->val, "invalid integer format");
			((char*)(i->val))[22] = '\0';
			return i;
		}
		else if (l >= INT_MIN && l <= INT_MAX)
		{
			/* cast the value as an int if it will fit */
			int *l_ptr = malloc(sizeof(int));
			if (neg)
			{
				*l_ptr = -(int)l;
			}
			else
			{
				*l_ptr = (int)l;
			}
			val = (void *)l_ptr;
			i = jep_create_object();
			i->val = (void *)val;
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
		i = jep_create_object();
		i->type = JEP_STRING;
		i->ret = JEP_RETURN | JEP_EXCEPTION;
		i->val = malloc(22);
		strcpy(i->val, "invalid argument type");
		((char*)(i->val))[21] = '\0';
		return i;
	}

	return i;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_char(jep_obj *args)
{
	jep_obj *c = NULL;

	if (args == NULL || args->size != 1)
	{
		c = jep_create_object();
		c->type = JEP_STRING;
		c->ret = JEP_RETURN | JEP_EXCEPTION;
		c->val = malloc(28);
		strcpy(c->val, "invalid number of arguments");
		((char*)(c->val))[27] = '\0';
		return c;
	}

	jep_obj *obj = args->head;

	if (obj->type == JEP_INT)
	{
		c = jep_create_object();
		c->type = JEP_CHARACTER;
		c->val = malloc(sizeof(char));
		*(char *)(c->val) = *(char *)(obj->val);
		return c;
	}
	else if (obj->type == JEP_CHARACTER)
	{
		c = jep_create_object();
		jep_copy_object(c, obj);
		return c;
	}
	if (obj->type == JEP_BYTE)
	{
		c = jep_create_object();
		c->type = JEP_CHARACTER;
		c->val = malloc(sizeof(char));
		*(char *)(c->val) = *(char *)(obj->val);
		return c;
	}
	else
	{
		c = jep_create_object();
		c->type = JEP_STRING;
		c->ret = JEP_RETURN | JEP_EXCEPTION;
		c->val = malloc(22);
		strcpy(c->val, "invalid argument type");
		((char*)(c->val))[21] = '\0';
		return c;
	}

	return c;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_double(jep_obj *args)
{
	jep_obj *i = NULL;

	if (args == NULL || args->size != 1)
	{
		i = jep_create_object();
		i->type = JEP_STRING;
		i->ret = JEP_RETURN | JEP_EXCEPTION;
		i->val = malloc(28);
		strcpy(i->val, "invalid number of arguments");
		((char*)(i->val))[27] = '\0';
		return i;
	}

	jep_obj *obj = args->head;

	if (obj->type == JEP_INT)
	{
		i = jep_create_object();
		i->type = JEP_DOUBLE;
		i->val = malloc(sizeof(double));
		*(double *)(i->val) = (double)(*(int *)(obj->val));
		return i;
	}
	else if (obj->type == JEP_LONG)
	{
		i = jep_create_object();
		i->type = JEP_DOUBLE;
		i->val = malloc(sizeof(double));
		*(double *)(i->val) = (double)(*(long *)(obj->val));
		return i;
	}
	else if (obj->type == JEP_DOUBLE)
	{
		i = jep_create_object();
		jep_copy_object(i, obj);
		return i;
	}
	else if (obj->type == JEP_CHARACTER)
	{
		i = jep_create_object();
		i->type = JEP_DOUBLE;
		i->val = malloc(sizeof(double));
		*(double *)(i->val) = (double)(*(char *)(obj->val));
		return i;
	}
	else if (obj->type == JEP_STRING)
	{
		if (obj->val == NULL)
		{
			printf("invalid integer format\n");
			return i;
		}

		int neg = 0;
		char *s;
		char *endptr;
		double d;
		void *val;

		errno = 0;
		endptr = NULL;
		s = NULL;

		s = (char *)(obj->val);

		/* allow for signed integers */
		if (*s == '-')
		{
			s++;
			neg++;
		}

		/* attempt to convert the string to a double */
		d = strtod(s, &endptr);

		if (errno == ERANGE)
		{
			printf("value to large to fit in a double\n");
		}
		else if ((endptr != s && *endptr != '\0') || (strlen(s) > 1 && *s == '0') || !isdigit(*s))
		{
			i = jep_create_object();
			i->type = JEP_STRING;
			i->ret = JEP_RETURN | JEP_EXCEPTION;
			i->val = malloc(22);
			strcpy(i->val, "invalid double format");
			((char*)(i->val))[21] = '\0';
			return i;
		}
		else
		{
			/* cast the value as an int if it will fit */
			double *l_ptr = malloc(sizeof(double));
			if (neg)
			{
				*l_ptr = -d;
			}
			else
			{
				*l_ptr = d;
			}
			val = (void *)l_ptr;
			i = jep_create_object();
			i->val = (void *)val;
			i->type = JEP_DOUBLE;
		}

		return i;
	}
	else
	{
		i = jep_create_object();
		i->type = JEP_STRING;
		i->ret = JEP_RETURN | JEP_EXCEPTION;
		i->val = malloc(22);
		strcpy(i->val, "invalid argument type");
		((char*)(i->val))[21] = '\0';
		return i;
	}

	return i;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_byte(jep_obj *args)
{
	jep_obj *o = NULL;

	if (args == NULL || args->size != 1)
	{
		o = jep_create_object();
		o->type = JEP_STRING;
		o->ret = JEP_RETURN | JEP_EXCEPTION;
		o->val = malloc(28);
		strcpy(o->val, "invalid number of arguments");
		((char*)(o->val))[27] = '\0';
		return o;
	}

	jep_obj *arg = args->head;

	if (arg == NULL || arg->val == NULL || (arg->type != JEP_INT && arg->type != JEP_LONG && arg->type != JEP_CHARACTER))
	{
		o = jep_create_object();
		o->type = JEP_STRING;
		o->ret = JEP_RETURN | JEP_EXCEPTION;
		o->val = malloc(22);
		strcpy(o->val, "invalid argument type");
		((char*)(o->val))[21] = '\0';
		return o;
	}

	unsigned char *b = malloc(1);

	if (arg->type == JEP_INT)
	{
		int i = *((int *)(arg->val));
		if (i > UCHAR_MAX)
		{
			i |= UCHAR_MAX;
		}
		*b = i & UCHAR_MAX;
	}
	else if (arg->type == JEP_LONG)
	{
		long int i = *((long int *)(arg->val));
		if (i > UCHAR_MAX)
		{
			i |= UCHAR_MAX;
		}
		*b = i & UCHAR_MAX;
	}
	else if (arg->type == JEP_CHARACTER)
	{
		unsigned char i = *((unsigned char *)(arg->val));
		*b = i & UCHAR_MAX;
	}

	o = jep_create_object();
	o->type = JEP_BYTE;
	o->val = b;

	return o;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_bytes(jep_obj *args)
{
	jep_obj *byte_obj = NULL;
	char* str = NULL;

	if (args == NULL || args->size != 1)
	{
		byte_obj = jep_create_object();
		byte_obj->type = JEP_STRING;
		byte_obj->ret = JEP_RETURN | JEP_EXCEPTION;
		byte_obj->val = malloc(28);
		strcpy(byte_obj->val, "invalid number of arguments");
		((char*)(byte_obj->val))[27] = '\0';
		return byte_obj;
	}

	jep_obj *arg = args->head;

	if (arg->type != JEP_STRING)
	{
		byte_obj = jep_create_object();
		byte_obj->type = JEP_STRING;
		byte_obj->ret = JEP_RETURN | JEP_EXCEPTION;
		byte_obj->val = malloc(22);
		strcpy(byte_obj->val, "invalid argument type");
		((char*)(byte_obj->val))[21] = '\0';
		return byte_obj;
	}

	byte_obj = jep_create_object();
	byte_obj->type = JEP_ARRAY;

	str = (char*)arg->val;

	jep_obj *bytes = NULL;
	unsigned int l = strlen(str);

	bytes = jep_create_object();
	bytes->type = JEP_LIST;

	unsigned int i;
	for (i = 0; i < l; i++)
	{
		jep_obj *byte = jep_create_object();
		byte->type = JEP_BYTE;
		unsigned char *c = malloc(1);
		*c = str[i];
		byte->val = c;
		jep_add_object(bytes, byte);
	}

	byte_obj->size = bytes->size;
	byte_obj->val = bytes;

	return byte_obj;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_write(jep_obj* args)
{
	if (args == NULL || args->size != 1)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *data = args->head;

	char *buffer = jep_to_string(data);

	if (buffer != NULL)
	{
		fputs(buffer, stdout);
		free(buffer);
	}

	return NULL;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_writeln(jep_obj* args)
{
	if (args == NULL || args->size != 1)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *data = args->head;

	char *buffer = jep_to_string(data);

	if (buffer != NULL)
	{
		puts(buffer);
		free(buffer);
	}

	return NULL;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_readln(jep_obj* args)
{
	if (args != NULL && args->size > 0)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *o = NULL;

	jep_file *file_val = malloc(sizeof(jep_file));
	file_val->file = stdin;
	file_val->mode = JEP_READ;
	file_val->open = 1;
	file_val->refs = 1;
	file_val->type = 0;

	jep_obj *file_obj = jep_create_object();
	file_obj->type = JEP_FILE;
	file_obj->val = file_val;

	jep_obj *file_args = jep_create_object();
	file_args->type = JEP_ARGUMENT;
	file_args->head = file_obj;
	file_args->size = 1;

	o = jep_freadln(file_args);

	file_val->open = 0; /* prevent cleanup from trying to close stdin */
	jep_destroy_object(file_obj);
	jep_destroy_object(file_args);

	return o;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_freadln(jep_obj* args)
{
	if (args == NULL || args->size != 1)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *arg = args->head;

	if (arg == NULL || arg->val == NULL)
	{
		printf("could not read from file\n");
		return NULL;
	}

	jep_file *file_obj = (jep_file *)(arg->val);

	if (file_obj->file == NULL || !file_obj->open)
	{
		printf("could not read from file\n");
		return NULL;
	}
	if (file_obj->mode != JEP_READ)
	{
		printf("the file is not open for reading\n");
		return NULL;
	}

	jep_obj *o = NULL;

	size_t size = 0;
	size_t len = 0;
	size_t last = 0;
	char *buffer = NULL;

	FILE *file = file_obj->file;

	do
	{

		size += BUFSIZ;
		buffer = realloc(buffer, size);

		if (buffer == NULL)
		{
			printf("failed to reallocate buffer for reading\n");
			return NULL;
		}
		if (fgets(buffer + last, size, file) == NULL)
		{
			if (buffer != NULL)
			{
				free(buffer);
			}

			o = jep_create_object();

			/* check if EOF or just empty line */
			if (feof(file))
			{
				o->type = JEP_NULL;
			}
			else
			{
				/* create an empty string */
				char *empty = malloc(1);
				empty[0] = '\0';
				o->type = JEP_STRING;
				o->val = empty;
			}

			return o;
		}
		len = strlen(buffer);
		last = len - 1;
	} while (!feof(file) && buffer[last] != '\n');

	if (buffer != NULL)
	{
		/* remove the newline from the end */
		if (buffer[last] == '\n')
		{
			buffer[last] = '\0';
		}

		o = jep_create_object();
		o->type = JEP_STRING;
		o->val = (void *)(buffer);
	}

	return o;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fopen(jep_obj* args)
{
	if (args == NULL || args->size != 2)
	{
		printf("invalid number of arguments for fopen\n");
		return NULL;
	}

	jep_obj *path_obj = args->head;

	if (path_obj->type != JEP_STRING || path_obj->next == NULL || path_obj->next->type != JEP_STRING)
	{
		printf("invalid argument type for fopen\n");
		return NULL;
	}

	char *path_str = (char *)(path_obj->val);
	char *mode = (char *)(path_obj->next->val);

	FILE *file = fopen(path_str, mode);
	jep_obj *o = NULL;
	o = jep_create_object();
	o->type = JEP_NULL;

	if (file != NULL)
	{
		jep_file *file_val = malloc(sizeof(jep_file));
		file_val->file = file;
		file_val->open = 1;
		file_val->refs = 1;
		file_val->type = 0;
		if (!strcmp(mode, "r"))
		{
			file_val->mode = JEP_READ;
		}
		else if (!strcmp(mode, "w"))
		{
			file_val->mode = JEP_WRITE;
		}
		else if (!strcmp(mode, "a"))
		{
			file_val->mode = JEP_APPEND;
		}
		else if (!strcmp(mode, "rb"))
		{
			file_val->mode = JEP_READ_BINARY;
		}
		else if (!strcmp(mode, "wb"))
		{
			file_val->mode = JEP_WRITE_BINARY;
		}
		else if (!strcmp(mode, "ab"))
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

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fwrite(jep_obj* args)
{
	if (args == NULL || args->size != 2)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *arg = args->head;

	if (arg == NULL || arg->val == NULL || arg->next == NULL)
	{
		printf("could not write to file\n");
		return NULL;
	}

	jep_file *file_obj = (jep_file *)(arg->val);
	jep_obj *data = arg->next;

	if (file_obj->file == NULL || !file_obj->open)
	{
		printf("could not write to file\n");
		return NULL;
	}
	if (file_obj->mode != JEP_APPEND && file_obj->mode != JEP_WRITE)
	{
		printf("the file is not open for writing\n");
		return NULL;
	}

	char *str = jep_to_string(data);

	fprintf(file_obj->file, "%s", str);
	fflush(file_obj->file);

	free(str);

	return NULL;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fwriteln(jep_obj* args)
{
	if (args == NULL || args->size != 2)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *arg = args->head;

	if (arg == NULL || arg->val == NULL || arg->next == NULL)
	{
		printf("could not write to file\n");
		return NULL;
	}

	jep_file *file_obj = (jep_file *)(arg->val);
	jep_obj *data = arg->next;

	if (file_obj->file == NULL || !file_obj->open)
	{
		printf("could not write to file\n");
		return NULL;
	}
	if (file_obj->mode != JEP_APPEND && file_obj->mode != JEP_WRITE)
	{
		printf("the file is not open for appending\n");
		return NULL;
	}

	char *str = jep_to_string(data);

	fprintf(file_obj->file, "%s\n", str);
	fflush(file_obj->file);

	free(str);

	return NULL;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_freadb(jep_obj* args)
{
	if (args == NULL || args->size != 2)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *arg = args->head;

	if (arg == NULL || arg->val == NULL || arg->next == NULL)
	{
		printf("could not read from file\n");
		return NULL;
	}

	jep_file *file_obj = (jep_file *)(arg->val);
	jep_obj *size = arg->next;

	if (size->type != JEP_INT)
	{
		printf("invalid buffer size for binary file read\n");
		return NULL;
	}

	int n = *((int *)(size->val));

	if (file_obj->file == NULL || !file_obj->open)
	{
		printf("could not read from file\n");
		return NULL;
	}
	if (file_obj->mode != JEP_READ_BINARY)
	{
		printf("the file is not open for binary reading\n");
		return NULL;
	}

	jep_obj *byte_array = NULL;
	jep_obj *bytes = NULL;
	unsigned char *data = malloc(n);
	size_t read = fread(data, n, 1, file_obj->file);

	if (read)
	{
		byte_array = jep_create_object();
		byte_array->type = JEP_ARRAY;

		bytes = jep_create_object();
		bytes->type = JEP_LIST;

		int i;
		for (i = 0; i < n; i++)
		{
			jep_obj *byte = jep_create_object();
			byte->type = JEP_BYTE;
			unsigned char *c = malloc(1);
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

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_fwriteb(jep_obj* args)
{
	if (args == NULL || args->size != 2)
	{
		printf("invalid number of arguments\n");
		return NULL;
	}

	jep_obj *arg = args->head;

	if (arg == NULL || arg->val == NULL || arg->next == NULL)
	{
		printf("could not write to file\n");
		return NULL;
	}

	jep_file *file_obj = (jep_file *)(arg->val);
	jep_obj *data = arg->next;
	unsigned char *byte_array = NULL;

	if (file_obj->file == NULL || !file_obj->open)
	{
		printf("could not write to file\n");
		return NULL;
	}
	if (file_obj->mode != JEP_APPEND_BINARY && file_obj->mode != JEP_WRITE_BINARY)
	{
		printf("the file is not open for binary appending\n");
		return NULL;
	}

	jep_obj *written = NULL;

	if (data == NULL)
	{
		printf("could not covnert data into binary\n");
		return NULL;
	}
	else
	{
		byte_array = NULL;
		size_t s = data->size;
		jep_obj *b = ((jep_obj *)(data->val))->head;
		byte_array = malloc(s);
		unsigned int i;
		for (i = 0; i < s && b != NULL; i++, b = b->next)
		{
			byte_array[i] = *((unsigned char *)(b->val));
		}

		size_t read = fwrite(byte_array, s, 1, file_obj->file);
		fflush(file_obj->file);

		written = jep_create_object();
		written->type = JEP_INT;
		int *i_val = malloc(sizeof(int));
		*i_val = (int)read;
		written->val = i_val;

		if (byte_array != NULL)
		{
			free(byte_array);
		}
	}

	return written;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_createSocket(jep_obj* args)
{
	jep_obj *s = NULL;
	jep_addrinf *address_info = NULL;
	char* host = NULL;
	char* port = NULL;

	if (args == NULL || args->size != 2)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(28);
		strcpy(s->val, "invalid number of arguments");
		((char*)(s->val))[27] = '\0';
		return s;
	}

	jep_obj *host_arg = args->head;
	jep_obj *port_arg = args->head->next;

	if ((host_arg->type != JEP_STRING && host_arg->type != JEP_NULL)
		|| (port_arg->type != JEP_STRING && port_arg->type != JEP_NULL))
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(22);
		strcpy(s->val, "invalid argument type");
		((char*)(s->val))[21] = '\0';
		return s;
	}

	if (host_arg->type == JEP_STRING) host = (char*)host_arg->val;
	if (port_arg->type == JEP_STRING) port = (char*)port_arg->val;

	int result = jep_get_addr_info(&address_info, host, port);

	if (result != 0)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(34);
		strcpy(s->val, "could not get address information");
		((char*)(s->val))[33] = '\0';
		return s;
	}

	jep_socket socket = jep_socket_create(address_info);

	if (socket == JEP_INVALID_SOCKET)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(24);
		strcpy(s->val, "could not create socket");
		((char*)(s->val))[23] = '\0';
		return s;
	}

	jep_file *file_val = malloc(sizeof(jep_file));
	file_val->socket = socket;
	file_val->open = 1;
	file_val->refs = 1;
	file_val->type = 1;
	file_val->info = address_info;

	s = jep_create_object();
	s->type = JEP_FILE;
	s->val = file_val;

	return s;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_bindSocket(jep_obj* args)
{
	jep_obj *s = NULL;
	jep_file *file = NULL;

	if (args == NULL || args->size != 1)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(28);
		strcpy(s->val, "invalid number of arguments");
		((char*)(s->val))[27] = '\0';
		return s;
	}

	jep_obj *arg = args->head;

	if (arg->type != JEP_FILE)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(22);
		strcpy(s->val, "invalid argument type");
		((char*)(s->val))[21] = '\0';
		return s;
	}

	file = (jep_file*)arg->val;

	int result = jep_socket_bind(file->socket, file->info);

	if (result != 0)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(22);
		strcpy(s->val, "could not bind socket");
		((char*)(s->val))[21] = '\0';
		return s;
	}

	return s;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_listenSocket(jep_obj* args)
{
	jep_obj *s = NULL;
	jep_file *file = NULL;

	if (args == NULL || args->size != 1)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(28);
		strcpy(s->val, "invalid number of arguments");
		((char*)(s->val))[27] = '\0';
		return s;
	}

	jep_obj *arg = args->head;

	if (arg->type != JEP_FILE)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(22);
		strcpy(s->val, "invalid argument type");
		((char*)(s->val))[21] = '\0';
		return s;
	}

	file = (jep_file*)arg->val;

	/* assume SOMAXCONN for now */
	int result = jep_socket_listen(file->socket, SOMAXCONN);

	if (result != 0)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(27);
		strcpy(s->val, "could not listen to socket");
		((char*)(s->val))[26] = '\0';
		return s;
	}

	return s;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_acceptSocket(jep_obj* args)
{
	jep_obj *s = NULL;
	jep_addrinf *address_info = NULL;
	jep_file* file = NULL;

	if (args == NULL || args->size != 1)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(28);
		strcpy(s->val, "invalid number of arguments");
		((char*)(s->val))[27] = '\0';
		return s;
	}

	jep_obj *arg = args->head;

	if (arg->type != JEP_FILE)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(22);
		strcpy(s->val, "invalid argument type");
		((char*)(s->val))[21] = '\0';
		return s;
	}

	file = (jep_file*)arg->val;

	jep_socket socket = jep_socket_accept(file->socket, NULL, NULL);

	if (socket == JEP_INVALID_SOCKET)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(35);
		strcpy(s->val, "could not accept socket connection");
		((char*)(s->val))[34] = '\0';
		return s;
	}

	jep_file *file_val = malloc(sizeof(jep_file));
	file_val->socket = socket;
	file_val->open = 1;
	file_val->refs = 1;
	file_val->type = 1;
	file_val->info = address_info;

	s = jep_create_object();
	s->type = JEP_FILE;
	s->val = file_val;

	return s;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_connectSocket(jep_obj* args)
{
	jep_obj *s = NULL;
	jep_file* file = NULL;

	if (args == NULL || args->size != 1)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(28);
		strcpy(s->val, "invalid number of arguments");
		((char*)(s->val))[27] = '\0';
		return s;
	}

	jep_obj *arg = args->head;

	if (arg->type != JEP_FILE)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(22);
		strcpy(s->val, "invalid argument type");
		((char*)(s->val))[21] = '\0';
		return s;
	}

	file = (jep_file*)arg->val;

	int result = jep_socket_connect(file->socket, file->info);

	if (result == JEP_SOCKET_ERROR)
	{
		s = jep_create_object();
		s->type = JEP_STRING;
		s->ret = JEP_RETURN | JEP_EXCEPTION;
		s->val = malloc(28);
		strcpy(s->val, "could not connect to socket");
		((char*)(s->val))[27] = '\0';
		return s;
	}

	return s;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_readSocket(jep_obj* args)
{
	jep_obj *read = NULL;
	jep_file* file = NULL;

	if (args == NULL || args->size != 3)
	{
		read = jep_create_object();
		read->type = JEP_STRING;
		read->ret = JEP_RETURN | JEP_EXCEPTION;
		read->val = malloc(28);
		strcpy(read->val, "invalid number of arguments");
		((char*)(read->val))[27] = '\0';
		return read;
	}

	jep_obj *arg = args->head;
	jep_obj *in_buffer = arg->next;
	jep_obj *size = arg->next->next;

	if (arg->type != JEP_FILE || in_buffer->type != JEP_REFERENCE || size->type != JEP_INT)
	{
		read = jep_create_object();
		read->type = JEP_STRING;
		read->ret = JEP_RETURN | JEP_EXCEPTION;
		read->val = malloc(22);
		strcpy(read->val, "invalid argument type");
		((char*)(read->val))[21] = '\0';
		return read;
	}

	/* dereference the buffer */
	in_buffer = (jep_obj*)(in_buffer->val);
	if (in_buffer->type != JEP_ARRAY)
	{
		read = jep_create_object();
		read->type = JEP_STRING;
		read->ret = JEP_RETURN | JEP_EXCEPTION;
		read->val = malloc(22);
		strcpy(read->val, "invalid argument type");
		((char*)(read->val))[21] = '\0';
		return read;
	}

	file = (jep_file*)arg->val;
	int n = *((int *)(size->val));

	jep_obj *bytes = NULL;
	unsigned char *data = malloc(n);
	int result = jep_socket_receive(file->socket, data, n, 0);

	if (result == JEP_SOCKET_ERROR)
	{
		free(data);

		read = jep_create_object();
		read->type = JEP_STRING;
		read->ret = JEP_RETURN | JEP_EXCEPTION;
		read->val = malloc(32);
		strcpy(read->val, "error while reading from socket");
		((char*)(read->val))[31] = '\0';
		return read;
	}

	if (result)
	{
		bytes = jep_create_object();
		bytes->type = JEP_LIST;

		int i;
		for (i = 0; i < result; i++)
		{
			jep_obj *byte = jep_create_object();
			byte->type = JEP_BYTE;
			unsigned char *c = malloc(1);
			*c = data[i];
			byte->val = c;
			jep_add_object(bytes, byte);
		}

		in_buffer->size = bytes->size;
		in_buffer->val = bytes;
	}

	free(data);

	read = jep_create_object();
	read->type = JEP_INT;
	read->val = malloc(sizeof(int));
	*(int*)(read->val) = result;

	return read;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_writeSocket(jep_obj* args)
{
	jep_obj *read = NULL;
	jep_file* file = NULL;

	if (args == NULL || args->size != 3)
	{
		read = jep_create_object();
		read->type = JEP_STRING;
		read->ret = JEP_RETURN | JEP_EXCEPTION;
		read->val = malloc(28);
		strcpy(read->val, "invalid number of arguments");
		((char*)(read->val))[27] = '\0';
		return read;
	}

	jep_obj *arg = args->head;
	jep_obj *out_buffer = arg->next;
	jep_obj *size = arg->next->next;

	if (arg->type != JEP_FILE || out_buffer->type != JEP_ARRAY || size->type != JEP_INT)
	{
		read = jep_create_object();
		read->type = JEP_STRING;
		read->ret = JEP_RETURN | JEP_EXCEPTION;
		read->val = malloc(22);
		strcpy(read->val, "invalid argument type");
		((char*)(read->val))[21] = '\0';
		return read;
	}

	file = (jep_file*)arg->val;
	int n = *((int *)(size->val));

	jep_string_builder *sb = jep_create_string_builder();

	jep_obj* element = ((jep_obj*)(out_buffer->val))->head;

	while (element != NULL)
	{
		jep_append_char(sb, *(char*)(element->val));
		element = element->next;
	}

	int result = jep_socket_send(file->socket, sb->buffer, n, 0);

	jep_destroy_string_builder(sb);

	read = jep_create_object();
	read->type = JEP_INT;
	read->val = malloc(sizeof(int));
	*(int*)(read->val) = result;

	return read;
}

SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_closeSocket(jep_obj* args)
{
	jep_obj *result = NULL;
	jep_socket socket;

	if (args == NULL || args->size != 1)
	{
		result = jep_create_object();
		result->type = JEP_STRING;
		result->ret = JEP_RETURN | JEP_EXCEPTION;
		result->val = malloc(28);
		strcpy(result->val, "invalid number of arguments");
		((char*)(result->val))[27] = '\0';
		return result;
	}

	jep_obj *arg = args->head;

	socket = *((jep_socket*)(arg->val));

	jep_socket_close(socket);

	return result;
}

/**
* prints the number of times an an object was created in the native library
*/
SWAPNATIVE_API jep_obj* SWAPNATIVE_CALL jep_print_native_call_count()
{
	print_call_counts();

	return NULL;
}
