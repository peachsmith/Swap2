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

/* double character symbol token codes */
#define T_INCREMENT 20
#define T_DECREMENT 21
#define T_ADDASSIGN 22
#define T_SUBASSIGN 23
#define T_MULASSIGN 24
#define T_DIVASSIGN 25
#define T_EQUIVALENT 26
#define T_LOREQUAL 27
#define T_GOREQUAL 28
#define T_RSHIFT 29
#define T_LSHIFT 30
#define T_ANDASSIGN 31
#define T_ORASSIGN 32
#define T_XORASSIGN 33
#define T_LOGAND 34
#define T_LOGOR 35
#define T_NOTEQUIVALENT 36

/* tripple character symbol token codes */
#define T_LSHIFTASSIGN 37
#define T_RSHIFTASSIGN 38

/* keyword token codes */
#define T_IF 39
#define T_ELSE 40
#define T_FOR 41
#define T_WHILE 42

/* end of file */
#define T_EOF 43

typedef struct Token
{
	jep_string_builder *value;
	int type;
	int token_code;
	int row;
	int column;
	int unary;
}jep_token;

typedef struct TokenBuilder
{
	jep_token** tokens;
	int size;
	int capacity;
}jep_token_builder;

/* allocates memory for a new token */
jep_token* jep_create_token();

/* frees the memory allocated for a token */
void jep_destroy_token(jep_token* t);

/* assign a token code to a token */
void jep_classify_token(jep_token* t);

/* creates a new token builder */
jep_token_builder* jep_create_token_builder();

/* increases the capacity of a token builder by approximately 50% */
void jep_resize_token_builder(jep_token_builder* tb);

/* adds a token to a token builder */
void jep_append_token(jep_token_builder* tb, jep_token* t);

/* frees memory allocated for a token builder */
void jep_destroy_token_builder();

/* reads the contents of a file into memory */
void jep_scan_file(FILE* file, jep_string_builder* sb);

/* tokenizes the contents of a text file */
jep_token_builder* jep_tokenize_file(const char* file_name);

/* checks for a symbol character */
int jep_is_symbol_char(char c);

/* checks for a single-character symbol */
int jep_is_symbol(const char* s);

/* checks for a double-character symbol */
int jep_is_symbol2(const char* s);

/* checks for a tripple-character symbol */
int jep_is_symbol3(const char* s);

/* checks for a keyword */
int jep_is_keyword(const char* s);

/* checks if a token is a primitive */
int jep_is_primitive(const char* s);

/* checks for the beginning of an identifier */
int jep_is_ident_start(char c);

/* checks for identifier characters */
int jep_is_ident(char c);

/* checks for an escape character */
int jep_is_escape(char c);

/* prints the tokens */
void jep_print_tokens(jep_token_builder* tb, FILE* f);

#endif /* JEP_TOKENIZER_H */
