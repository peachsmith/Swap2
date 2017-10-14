/*
	Functions for performing operations dictated by an abstract syntax tree
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
#ifndef JEP_OPERATOR_H
#define JEP_OPERATOR_H

#include "native.h"

/**
 * evaluates an AST node
 */
jep_obj* jep_evaluate(jep_ast_node ast, jep_obj* list);

/**
 * evaluates an addition expression
 */
jep_obj* jep_add(jep_ast_node node, jep_obj* list);

/**
 * evaluates subtraction or negation
 */
jep_obj* jep_sub(jep_ast_node node, jep_obj* list);

/**
 * evaluates a multiplication expression
 */
jep_obj* jep_mul(jep_ast_node node, jep_obj* list);

/**
 * evaluates a division expression
 */
jep_obj* jep_div(jep_ast_node node, jep_obj* list);

/**
 * evaluates a modulus expression
 */
jep_obj* jep_modulus(jep_ast_node node, jep_obj* list);

/**
 * evaluates a less than expression
 */
jep_obj* jep_less(jep_ast_node node, jep_obj* list);

/**
 * evaluates a greater than expression
 */
jep_obj* jep_greater(jep_ast_node node, jep_obj* list);

/**
 * evaluates a less than or equal to expression
 */
jep_obj* jep_lorequal(jep_ast_node node, jep_obj* list);

/**
 * evaluates a greater than or equal to expression
 */
jep_obj* jep_gorequal(jep_ast_node node, jep_obj* list);

/**
 * evaluates an equivalence expression
 */
jep_obj* jep_equiv(jep_ast_node node, jep_obj* list);

/**
 * evaluates a not equivalence expression
 */
jep_obj* jep_noteq(jep_ast_node node, jep_obj* list);

/**
 * performs a not operation
 */
jep_obj* jep_not(jep_ast_node node, jep_obj* list);

/**
 * performs a logical and operation
 */
jep_obj* jep_and(jep_ast_node node, jep_obj* list);

/**
 * performs a logical or operation
 */
jep_obj* jep_or(jep_ast_node node, jep_obj* list);

/**
 * performs a bitwise operation
 */
jep_obj* jep_bitand(jep_ast_node node, jep_obj* list);

/**
 * performs a bitwise or operation
 */
jep_obj* jep_bitor(jep_ast_node node, jep_obj* list);

/**
 * performs a bitwise xor operation
 */
jep_obj* jep_bitxor(jep_ast_node node, jep_obj* list);

/**
 * performs a left bit shift operation
 */
jep_obj* jep_lshift(jep_ast_node node, jep_obj* list);

/**
 * performs a right bit shift operation
 */
jep_obj* jep_rshift(jep_ast_node node, jep_obj* list);

/**
 * performs an increment on an integer
 */
jep_obj* jep_inc(jep_ast_node node, jep_obj* list);

/**
 * performs a decrement on an integer
 */
jep_obj* jep_dec(jep_ast_node node, jep_obj* list);

/**
 * performs an addition assignmnet
 */
jep_obj* jep_add_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a subtraction assignment
 */
jep_obj* jep_sub_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a multiplication assignment
 */
jep_obj* jep_mul_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a division assignment
 */
jep_obj* jep_div_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a modulus assignment
 */
jep_obj* jep_mod_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a bitwise and assignment on an integer
 */
jep_obj* jep_and_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a bitwise and assignment on an integer
 */
jep_obj* jep_or_assign(jep_ast_node node, jep_obj* list);

/**
 * performs a bitwise exclusive or assignment on an integer
 */
jep_obj* jep_xor_assign(jep_ast_node node, jep_obj* list);

/**
 * evaluates an assignment
 */
jep_obj* jep_assign(jep_ast_node node, jep_obj* list);

/**
 * evaluates the contents of a set of parentheses
 */
jep_obj* jep_paren(jep_ast_node node, jep_obj* list);

/**
 * evaluates the contents of a set of curly braces
 */
jep_obj* jep_brace(jep_ast_node node, jep_obj* list);

/**
 * evaluates an array subscript
 */
jep_obj* jep_subscript(jep_ast_node node, jep_obj* list);

/**
 * gets the actual data member from a struct
 */
jep_obj* jep_get_data_member(jep_ast_node node, jep_obj* list);

/**
 * evaluates a function definition
 */
jep_obj* jep_function(jep_ast_node node, jep_obj* list);

/**
 * returns from a function
 */
jep_obj* jep_return(jep_ast_node node, jep_obj* list);

/**
 * evaluates a comma tree
 */
jep_obj* jep_comma(jep_ast_node node, jep_obj* list);

/**
 * evaluates a reference
 */
jep_obj* jep_reference(jep_ast_node node, jep_obj* list);

/**
 * evaluates a dereference
 */
jep_obj* jep_dereference(jep_ast_node node, jep_obj* list);

/**
 * evaluates a comma-delimited sequence of objects
 */
void jep_sequence(jep_ast_node node, jep_obj* list, jep_obj* seq);

/**
 * evaluates an if statement
 */
jep_obj* jep_if(jep_ast_node node, jep_obj* list);

/**
 * evaluates a switch statement
 */
jep_obj* jep_switch(jep_ast_node node, jep_obj* list);

/**
 * evaluates an for loop
 */
jep_obj* jep_for(jep_ast_node node, jep_obj* list);

/**
 * evaluates a while loop
 */
jep_obj* jep_while(jep_ast_node node, jep_obj* list);

/**
 * evaluates a try/catch block
 */
jep_obj* jep_try(jep_ast_node node, jep_obj* list);

/**
 * evaluates a throw statement
 */
jep_obj* jep_throw(jep_ast_node node, jep_obj* list);

/**
 * checks if a struct has a data member with the specfied identifier
 */
int jep_has_data_member(jep_obj* members, const char* ident);

/**
 * evaluates a structure definition
 */
jep_obj* jep_struct(jep_ast_node node, jep_obj* list);

/**
 * creates a new instance of a certain type of object
 */
jep_obj* jep_new(jep_ast_node node, jep_obj* list);

/**
 * accesses members of an object
 */
jep_obj* jep_member(jep_ast_node node, jep_obj* list);

/**
 * evaluates a modifier chain
 */
jep_obj* jep_modifier(jep_ast_node node, jep_obj* list);

/**
 * evaluates a comma-delimited sequence of modified expressions
 */
jep_obj* jep_mod_sequence(jep_ast_node node, jep_obj* list, int mod);

/**
 * evaluates an AST node within a certain scope
 */
jep_obj* jep_evaluate_local(jep_ast_node ast, jep_obj* list, int mod);

#endif /* JEP_OPERATOR_H */
