#ifndef JEP_OBJECT_H
#define JEP_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/* the primitive types */
#define JEP_INT 1
#define JEP_LONG 2
#define JEP_DOUBLE 3
#define JEP_CHARACTER 4
#define JEP_STRING 5

typedef struct Object
{
	void* val;
	int type;
}jep_obj;

/* converts a token into a number object */
jep_obj* jep_number(const char* s);

/* converts a token into a character object */
jep_obj* jep_character(const char* s);

/* converts a token into a string object */
jep_obj* jep_string(const char* s);

void jep_print_obj(jep_obj* obj);

#endif
