/*
    Functions for evaluating and modifying an abstract syntax tree
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
#ifndef JEP_AST_H
#define JEP_AST_H

#include "tokenizer.h"

/* for loop components */
#define JEP_INDEX 1
#define JEP_CONDITION 2
#define JEP_CHANGE 4

/* a node in an AST */
typedef struct ASTNode
{
	jep_token token;
	int leaf_count;
	int cap;
	struct ASTNode* leaves;
	int error;
	int array;
	int loop;
	int mod;
}jep_ast_node;

/* a stack of nodes */
typedef struct Stack
{
	int size;
	int cap;
	jep_ast_node* top;
	jep_ast_node** nodes;
}jep_stack;

/**
 * create an AST node
 */
jep_ast_node* jep_create_ast_node();

/**
 * adds a leaf node to an AST node
 */
void jep_add_leaf_node(jep_ast_node* root, jep_ast_node* leaf);

/**
 * prints the AST
 */
void jep_print_ast(jep_ast_node root);

/**
 * pushes an AST node onto the top of the stack
 */
void jep_push(jep_stack* stack, jep_ast_node* node);

/**
 * pops an AST node from the top of the stack
 */
jep_ast_node* jep_pop(jep_stack* stack);

/**
 * pops all AST nodes from the stack
 */
void jep_pop_all(jep_stack* stack);

#endif
