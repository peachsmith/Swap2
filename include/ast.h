#ifndef JEP_AST_H
#define JEP_AST_H

#include "object.h"
#include "tokenizer.h"

typedef struct ASTNode
{
	jep_token* token;
	int leaf_count;
	int capacity;
	struct ASTNode* leaves;
	int error;
}jep_ast_node;

typedef struct Stack
{
	int size;
	int capacity;
	jep_ast_node* top;
	jep_ast_node** nodes;
}jep_stack;

/*
 * functions for creating an AST
 */

/* create an AST node */
jep_ast_node* jep_create_ast_node();

/* adds a leaf node to an AST node */
void jep_add_leaf_node(jep_ast_node* root, jep_ast_node* leaf);

/* prints the AST */
void jep_print_ast(jep_ast_node root);

/* pushes an AST node onto the top of the stack */
void jep_push(jep_stack* stack, jep_ast_node* node);

/* pops an AST node from the top of the stack */
jep_ast_node* jep_pop(jep_stack* stack);

/* pops all AST nodes from the stack */
void jep_pop_all(jep_stack* stack);

#endif
