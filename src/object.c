#include "object.h"

jep_obj* jep_number(const char* s)
{
	char* endptr;
	long int l;
	double d;
	void* val;
	jep_obj* obj;

	errno = 0;
	endptr = NULL;
	obj = NULL;

	/* attempt to convert the string to a long int */
	l = strtol(s, &endptr, 10);

	if(errno == ERANGE)
	{
		printf("number out of range\n");
		return NULL;
	}
	else if(endptr != s && *endptr != '\0')
	{
		endptr = NULL;
		errno = 0;

		/* attempt to convert the string to a double */
		d = strtod(s, &endptr);

		if(errno == ERANGE)
		{
			printf("number out of range\n");
		}
		else if(endptr != s && *endptr != '\0')
		{
			printf("invalid number format\n");
		}
		else
		{
			double* d_ptr = malloc(sizeof(double));
			*d_ptr = d;
			val = (void*)d_ptr;
			obj = malloc(sizeof(jep_obj));
			obj->value = (void*)val;
			obj->type = JEP_DOUBLE;
		}
	}
	else if(l >= INT_MIN && l <= INT_MAX)
	{
		/* cast the value as an int if it will fit */
		int* l_ptr = malloc(sizeof(int));
		*l_ptr = (int)l;
		val = (void*)l_ptr;
		obj = malloc(sizeof(jep_obj));
		obj->value = (void*)val;
		obj->type = JEP_INT;
	}
	else
	{
		long int* l_ptr = malloc(sizeof(long int));
		*l_ptr = l;
		val = (void*)l_ptr;
		obj = malloc(sizeof(jep_obj));
		obj->value = (void*)val;
		obj->type = JEP_LONG;
	}

	return obj;
}

void jep_print_obj(jep_obj* obj)
{
	if(obj != NULL)
	{
		if(obj->type == JEP_INT)
		{
			printf("int: %d\n", *((int*)(obj->value)));
		}
		else if(obj->type == JEP_LONG)
		{
			printf("long: %ld\n", *((long*)(obj->value)));
		}
		else if(obj->type == JEP_DOUBLE)
		{
			printf("double: %.2lf\n", *((double*)(obj->value)));
		}
	}
	else
	{
		printf("NULL\n");
	}
}
