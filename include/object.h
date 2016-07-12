#ifndef JEP_OBJECT_H
#define JEP_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "parser.h"

/* the primitive types */
#define JEP_INT 1
#define JEP_LONG 2
#define JEP_DOUBLE 3
#define JEP_CHAR 4

typedef struct Object
{
	void* value;
	int type;
}jep_obj;

/* converts a token into a number object */
jep_obj* jep_number(const char* s);

void jep_print_obj(jep_obj* obj);

#endif
