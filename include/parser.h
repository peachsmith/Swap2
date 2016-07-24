#ifndef JEP_PARSER_H
#define JEP_PARSER_H

#include "tokenizer.h"
#include "ast.h"

/* advances the node pointer for a specific token code */
int jep_accept(int token_code, jep_ast_node** nodes);

/* advances the node pointer for a specific token type */
int jep_accept_type(int type, jep_ast_node** nodes);

/* parses a stream of tokens */
jep_ast_node* jep_parse(jep_token_stream* ts, jep_ast_node** nodes);

/* parses an expression */
jep_ast_node* jep_expression(jep_ast_node* root, jep_ast_node** nodes);

/* parses a statement */
jep_ast_node* jep_statement(jep_ast_node* root, jep_ast_node** nodes);

/* parses a block of code */
jep_ast_node* jep_block(jep_ast_node* root, jep_ast_node** nodes);

#endif
