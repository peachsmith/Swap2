#ifndef JEP_PARSER_H
#define JEP_PARSER_H

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

/* checks if a token is a terminal */
int jep_is_term(jep_token* token);

/* advances the node pointer for a specific token code */
int jep_accept(int token_code, jep_ast_node** nodes);

/* advances the node pointer for a specific token type */
int jep_accept_type(int type, jep_ast_node** nodes);

/* parses a stream of tokens */
jep_ast_node* jep_parse(jep_token_builder* tb, jep_ast_node** nodes);

/* parses an expression */
jep_ast_node* jep_expression(jep_ast_node* root, jep_ast_node** nodes);

/* parses a statement */
jep_ast_node* jep_statement(jep_ast_node* root, jep_ast_node** nodes);

/* create an AST node */
jep_ast_node* jep_create_ast_node();

/* increases the memory allocated for an AST node's leaf nodes by about 50% */
void jep_resize_ast_node(jep_ast_node* node);

/* adds a leaf node to an AST node */
void jep_add_leaf_node(jep_ast_node* root, jep_ast_node* leaf);

/* prints the AST */
void jep_print_ast(jep_ast_node root);

/* creates a stack */
/* jep_stack* jep_create_stack(); */

/* pushes an AST node onto the top of the stack */
void jep_push(jep_stack* stack, jep_ast_node* node);

/* pops an AST node from the top of the stack */
jep_ast_node* jep_pop(jep_stack* stack);

/* pops all AST nodes from the stack */
void jep_pop_all(jep_stack* stack);

/* determines the priority of an operator based on order of operations */
int jep_priority(jep_ast_node* node);

/* checks if an operator is potentially unary */
int jep_check_unary(jep_token* cur, jep_token* prev);

#endif
