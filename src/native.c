#include "native.h"

#define JEP_NATIVE_COUNT 2

/* native function identifiers */
const char *natives[] = 
{
	"print", "println"
};

/* native function forward declarations */
static jep_obj* jep_print(const char*);
static jep_obj* jep_println(const char*);

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
