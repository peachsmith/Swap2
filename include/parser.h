#ifndef JEP_PARSER_H
#define JEP_PARSER_H

#include "tokenizer.h"
#include "ast.h"

#define JEP_LEFT_ASSOC 1
#define JEP_RIGHT_ASSOC 2

/* constructs  an AST from a stream of tokens */
jep_ast_node* jep_parse(jep_token_stream* ts, jep_ast_node** nodes);

#endif
