#ifndef JEP_AST_H
#define JEP_AST_H

#include "object.h"

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

/* performs a bitwise operation */
jep_obj* jep_bitand(jep_ast_node node);

/* performs a bitwise or operation */
jep_obj* jep_bitor(jep_ast_node node);

/* performs a bitwise xor operation */
jep_obj* jep_bitxor(jep_ast_node node);

/* performs a left bit shift operation */
jep_obj* jep_lshift(jep_ast_node node);

/* performs a right bit shift operation */
jep_obj* jep_rshift(jep_ast_node node);

/* evaluates the contents of a set of parentheses */
jep_obj* jep_paren(jep_ast_node node);

/* evaluates a block of code in curly braces */
void jep_brace(jep_ast_node node);

#endif
