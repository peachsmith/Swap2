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
#define T_IDENTIFIER 3
#define T_NUMBER 4
#define T_CHARACTER 5
#define T_STRING 6

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

/* double character symbol token codes */
#define T_INCREMENT 22
#define T_DECREMENT 23
#define T_ADDASSIGN 24
#define T_SUBASSIGN 25
#define T_MULASSIGN 26
#define T_DIVASSIGN 27
#define T_MODASSIGN 28
#define T_EQUIVALENT 29
#define T_LOREQUAL 30
#define T_GOREQUAL 31
#define T_RSHIFT 32
#define T_LSHIFT 33
#define T_ANDASSIGN 34
#define T_ORASSIGN 35
#define T_XORASSIGN 36
#define T_LOGAND 37
#define T_LOGOR 38
#define T_NOTEQUIVALENT 39

/* tripple character symbol token codes */
#define T_LSHIFTASSIGN 40
#define T_RSHIFTASSIGN 41

/* keyword token codes */
#define T_IF 42
#define T_ELSE 43
#define T_FOR 44
#define T_WHILE 45
#define T_FUNCTION 46
#define T_RETURN 47

/* end of file */
#define T_EOF 48

/**
 * a single token
 */
typedef struct Token
{
	jep_string_builder *value;
	int type;
	int token_code;
	int row;
	int column;
	int unary;
	int postfix;
}jep_token;

/**
 * a sequence of tokens
 */
typedef struct TokenStream
{
	jep_token** tok;
	int size;
	int capacity;
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
void jep_append_token(jep_token_stream* tb, jep_token* t);

/**
 * tokenizes the contents of a file
 */
jep_token_stream* jep_tokenize_file(const char* file_name);

/**
 * prints the tokens
 */
void jep_print_tokens(jep_token_stream* ts, FILE* f);

#endif /* JEP_TOKENIZER_H */
