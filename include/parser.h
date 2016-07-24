#ifndef JEP_PARSER_H
#define JEP_PARSER_H

#include "tokenizer.h"
#include "ast.h"

/* checks if a token is an 'end of expression' token */
int jep_eoe(jep_token* token);

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

/* parses a block of code */
jep_ast_node* jep_block(jep_ast_node* root, jep_ast_node** nodes);

/* determines the priority of an operator based on order of operations */
int jep_priority(jep_ast_node* node);

/* checks if an operator is potentially unary */
int jep_check_unary(jep_token* cur, jep_token* prev);

#endif
