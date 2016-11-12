/*
    Functions for building an abstract syntax tree
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
