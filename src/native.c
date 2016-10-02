#include "native.h"

#define JEP_NATIVE_COUNT 3

/* native function identifiers */
const char *natives[] = 
{
	"print", "println", "scanln"
};

/* native function forward declarations */
static jep_obj* jep_print(const char*);
static jep_obj* jep_println(const char*);
static jep_obj* jep_scanln();

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

	if(native == 0 || native == 1)
	{
		if(args == NULL || args->size != 1)
		{
			printf("invalid number of arguments\n");
			return o;
		}

		jep_obj* data = args->head;

		if(data->type != JEP_STRING)
		{
			printf("invalid argument type\n");
			return o;
		}

		char* str = (char*)(data->val);

		if(native == 0)
		{
			o = jep_print(str);	
		}
		else
		{
			o = jep_println(str);
		}
	}
	else if(native == 2)
	{
		if(args != NULL && args->size > 0)
		{
			printf("invalid number of arguments\n");
			return o;
		}
		o = jep_scanln();
	}
	else
	{
		printf("native is somehow: %d\n", native);
	}
	
	return o;
}

/* writes a string to standard out */
static jep_obj* jep_print(const char* buffer)
{
	fputs(buffer, stdout);
	return NULL;
}

/* writes a string to standard out appended by a newline */
static jep_obj* jep_println(const char* buffer)
{
	puts(buffer);
	return NULL;
}

/* reads a string from standard in */
static jep_obj* jep_scanln()
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
			return NULL;
		}
		if(fgets(buffer + last, size, stdin) == NULL)
		{
			if(buffer != NULL)
			{
				free(buffer);
			}
			return o;
		}
		len = strlen(buffer);
		last = len - 1;
	} while (!feof(stdin) && buffer[last] != '\n');

	if(buffer != NULL)
	{
		o = jep_create_object();
		o->type = JEP_STRING;
		o->val = (void*)(buffer);
	}

	return o;
}
