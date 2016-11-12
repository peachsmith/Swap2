/*
    Functions for tokenizing a string of characters
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
#ifndef JEP_TOKENIZER_H
#define JEP_TOKENIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stringbuilder.h"

/* token types */
#define T_END 0
#define T_SYMBOL 1
#define T_KEYWORD 2
#define T_MODIFIER 3
#define T_IDENTIFIER 4
#define T_NUMBER 5
#define T_BYTE 6
#define T_CHARACTER 7
#define T_STRING 8

/* single character symbol token codes */
#define T_PLUS 1
#define T_MINUS 2
#define T_FSLASH 3
#define T_STAR 4
#define T_SEMICOLON 5
#define T_COMMA 6
#define T_LPAREN 7
#define T_RPAREN 8
#define T_LSQUARE 9
#define T_RSQUARE 10
#define T_LBRACE 11
#define T_RBRACE 12
#define T_LESS 13
#define T_GREATER 14
#define T_EQUALS 15
#define T_PERIOD 16
#define T_BITAND 17
#define T_BITOR 18
#define T_BITXOR 19
#define T_NOT 20
#define T_MODULUS 21
/* unused single character symbols */
#define T_COLON 22

/* double character symbol token codes */
#define T_INCREMENT 23
#define T_DECREMENT 24
#define T_ADDASSIGN 25
#define T_SUBASSIGN 26
#define T_MULASSIGN 27
#define T_DIVASSIGN 28
#define T_MODASSIGN 29
#define T_EQUIVALENT 30
#define T_LOREQUAL 31
#define T_GOREQUAL 32
#define T_RSHIFT 33
#define T_LSHIFT 34
#define T_ANDASSIGN 35
#define T_ORASSIGN 36
#define T_XORASSIGN 37
#define T_LOGAND 38
#define T_LOGOR 39
#define T_NOTEQUIVALENT 40
/* unused double character symbols */
#define T_DOUBLECOLON 41

/* tripple character symbol token codes */
#define T_LSHIFTASSIGN 42
#define T_RSHIFTASSIGN 43

/* keyword token codes */
#define T_IF 44
#define T_ELSE 45
#define T_FOR 46
#define T_WHILE 47
#define T_FUNCTION 48
#define T_RETURN 49
#define T_IMPORT 50
#define T_NULL 51
#define T_LOCAL 52
#define T_CONST 53

/* end of file */
#define T_EOF 54

/**
 * a single token
 */
typedef struct Token
{
	jep_string_builder *val; /* the text of the token               */
	int type;                /* the type of token                   */
	int token_code;          /* identifies symbols and keywords     */
	int row;                 /* the row of the token in the file    */
	int column;              /* the column of the token in the file */
	int unary;               /* flag indicating a unary operator    */
	int postfix;             /* flag indicating a postfix operator  */
	const char* file;        /* file in which the token is located  */
}jep_token;

/**
 * a utility for building a dynamic sequence of tokens
 */
typedef struct TokenStream
{
	jep_token* tok; /* the tokens                           */
	int size;       /* the amount of tokens in the stream   */
	int cap;        /* the maximum amount of tokens allowed */
	int error;      /* error flag */
}jep_token_stream;

/**
 * allocates memory for a new token
 */
jep_token* jep_create_token();

/**
 * frees the memory allocated for a token
 */
void jep_destroy_token(jep_token* t);

/**
 * creates a new token stream
 */
jep_token_stream* jep_create_token_stream();

/**
 * frees memory allocated for a token stream
 */
void jep_destroy_token_stream(jep_token_stream* ts);

/**
 * adds a token to a token stream
 */
void jep_append_token(jep_token_stream* tb, jep_token t);

/**
 * tokenizes the contents of a file
 */
void jep_tokenize_file(jep_token_stream* ts, const char* file_name);

/**
 * prints the tokens
 */
void jep_print_tokens(jep_token_stream* ts, FILE* f);

#endif /* JEP_TOKENIZER_H */
