#ifndef JEP_OBJECT_H
#define JEP_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "ast.h"

/* the primitive types */
#define JEP_INT 1
#define JEP_LONG 2
#define JEP_DOUBLE 3
#define JEP_CHARACTER 4
#define JEP_STRING 5
#define JEP_ARRAY 6
#define JEP_FUNCTION 7

typedef struct Object
{
	char* ident;         /* identifier        */
	void* val;           /* stored value      */
	int type;            /* type of object    */
	struct Object* prev; /* previous object   */
	struct Object* next; /* next object       */
	struct Object* head; /* beginning of list */
	struct Object* tail; /* end of list       */
}jep_obj;

/* allocates memory for a new object */
jep_obj* jep_create_object();

/* adds an object to a list */
void jep_add_object(jep_obj* list, jep_obj* o);

/* retreives an object from a list */
jep_obj* jep_get_object(const char* ident, jep_obj* list);

/* copies the value of one obect into another */
void jep_copy_object(jep_obj* dest, jep_obj* src);

/* frees the memory in a list of objects */
void jep_destroy_list(jep_obj* list);

/* converts a token into a number object */
jep_obj* jep_number(const char* s);

/* converts a token into a character object */
jep_obj* jep_character(const char* s);

/* converts a token into a string object */
jep_obj* jep_string(const char* s);

/* prints an object to stdout */
void jep_print_obj(jep_obj* obj);

/* prints a list of objects to stdout */
void jep_print_list(jep_obj* list);

#endif
