#ifndef JEP_PARSER_H
#define JEP_PARSER_H

#include "tokenizer.h"
#include "ast.h"

/* associativity */
#define JEP_LEFT_ASSOC 1  /* left to right */
#define JEP_RIGHT_ASSOC 2 /* right to left */

/* error codes */
#define ERR_EXPRESSION 1
#define ERR_IDENTIFIER 2
#define ERR_UNEXPECTED 3
#define ERR_EXPECTED 4
#define ERR_UNIQUE 5

/* modifier codes */
#define MOD_LOCAL 1
#define MOD_CONST 2

/**
 * constructs an AST from a stream of tokens
 */
void jep_parse(jep_token_stream* ts, jep_ast_node* root);

#endif
