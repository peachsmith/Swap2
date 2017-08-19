/*
	Functions for object creation and manipulation
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
#ifndef JEP_OBJECT_H
#define JEP_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "ast.h"

/* return flags */
#define JEP_RETURN 1
#define JEP_EXCEPTION 2

/* types of jep_objects */
#define JEP_BYTE 1
#define JEP_INT 2
#define JEP_LONG 3
#define JEP_DOUBLE 4
#define JEP_CHARACTER 5
#define JEP_STRING 6
#define JEP_ARRAY 7
#define JEP_FUNCTION 8
#define JEP_ARGUMENT 9
#define JEP_LIST 10
#define JEP_NATIVE 11
#define JEP_REFERENCE 12
#define JEP_FILE 13
#define JEP_NULL 14
#define JEP_STRUCT 15
#define JEP_STRUCTDEF 16

/* file modes */
#define JEP_READ 1
#define JEP_WRITE 2
#define JEP_APPEND 3
#define JEP_READ_BINARY 4
#define JEP_WRITE_BINARY 5
#define JEP_APPEND_BINARY 6

/**
 * a structure representing all objects and lists of objects
 */
typedef struct Object
{
	char *ident;		 /* identifier                    */
	void *val;			 /* stored value                  */
	int type;			 /* type of object                */
	struct Object *prev; /* previous object               */
	struct Object *next; /* next object                   */
	struct Object *head; /* beginning of list             */
	struct Object *tail; /* end of list                   */
	int size;            /* number of objects in the list */
	unsigned char ret;   /* return flag                   */
	int index;           /* index in an array             */
	int mod;             /* modifier flags                */
	char *array_ident;   /* array identifier              */
	struct Object *self; /* the actual object             */
} jep_obj;

/**
 * a structre representing a file for file io
 */
typedef struct File
{
	FILE *file;		   /* the file pointer                   */
	int open;		   /* open flag                          */
	int mode;		   /* what will be done with the file    */
	unsigned int refs; /* amount of objects referencing this */
} jep_file;

/* allocates memory for a new object */
jep_obj *jep_create_object();

/* frees the memory used by an object */
void jep_destroy_object(jep_obj *obj);

/* creates a string representation of an object */
char *jep_to_string(jep_obj *o);

/* converts an object into an array of bytes */
jep_obj *jep_get_bytes(jep_obj *o);

/* compares the values of two objects */
int jep_compare_object(jep_obj *a, jep_obj *b);

/* adds an object to a list */
void jep_add_object(jep_obj *list, jep_obj *o);

/* retreives an object from a list */
jep_obj *jep_get_object(const char *ident, jep_obj *list);

/* copies the value of one obect into another */
void jep_copy_object(jep_obj *dest, jep_obj *src);

/* copies the value of the self pointer of an object */
void jep_copy_self(jep_obj *dest, jep_obj *src);

/* frees the memory in a list of objects */
void jep_destroy_list(jep_obj *list);

/* converts a token into a number object */
jep_obj *jep_number(const char *s);

/* converts a token into a character object */
jep_obj *jep_character(const char *s);

/* converts a token into a string object */
jep_obj *jep_string(const char *s);

/* convertes an ast into an array */
jep_obj *jep_array(jep_ast_node *ast);

/* prints an object to stdout */
void jep_print_object(jep_obj *obj);

/* prints a list of objects to stdout */
void jep_print_list(jep_obj *list);

/* removes the scope list off the tail of a list */
void jep_remove_scope(jep_obj *list);

#endif
