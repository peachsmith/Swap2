#ifndef JEP_OBJECT_H
#define JEP_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "parser.h"

#define JEP_INT 0
#define JEP_LONG 1
#define JEP_DOUBLE 2

typedef struct Object
{
	void* value;
	int type;
}jep_obj;

/* converts a token into a number object */
jep_obj* jep_number(const char* s);

void jep_print_obj(jep_obj* obj);

/* evaluates an AST node */
jep_obj* jep_evaluate(jep_ast_node ast);

/* evaluates an addition expression */
jep_obj* jep_add(jep_ast_node node);

/* evaluates subtraction or negation */
jep_obj* jep_sub(jep_ast_node node);

/* evaluates a multiplication expression */
jep_obj* jep_mul(jep_ast_node node);

/* evaluates a division expression */
jep_obj* jep_div(jep_ast_node node);

/* evaluates a less than expression */
jep_obj* jep_less(jep_ast_node node);

/* evaluates a greater than expression */
jep_obj* jep_greater(jep_ast_node node);

/* evaluates a less than or equal to expression */
jep_obj* jep_lorequal(jep_ast_node node);

/* evaluates a greater than or equal to expression */
jep_obj* jep_gorequal(jep_ast_node node);

/* evaluates an equivalence expression */
jep_obj* jep_equiv(jep_ast_node node);

/* evaluates a not equivalence expression */
jep_obj* jep_noteq(jep_ast_node node);

/* performs a not operation */
jep_obj* jep_not(jep_ast_node node);

/* performs a logical and operation */
jep_obj* jep_and(jep_ast_node node);

/* performs a logical or operation */
jep_obj* jep_or(jep_ast_node node);

#endif
