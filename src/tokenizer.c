#include "tokenizer.h"

/**
 * one-character symbols
 */
const char *symbols[] = 
{
	"+", "-", "/", "*", ";", ",", "(", ")", "[", "]", "{", 
	"}", "<", ">", "=", ".", "&", "|", "^", "!", "%"
};

/**
 * two-character symbols
 */
const char *symbols2[] = 
{
	"++", "--", "+=", "-=", "*=", "/=", "%=", "==", "<=", 
	">=", ">>", "<<", "&=", "|=", "^=", "&&", "||", "!="
};

/**
 * three-character symbols
 */
const char *symbols3[] = 
{
	"<<=", ">>="
};

/**
 * keywords
 */
const char *keywords[] = 
{
	"if", "else", "for", "while", "function", "return", "local", "const"
};

/**
 * checks for a symbol character
 */
static int jep_is_symbol_char(char c)
{
	switch(c)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '<':
		case '>':
		case '!':
		case '=':
		case '.':
		case ',':
		case ';':
		case '(':
		case ')':
		case '{':
		case '}':
		case '[':
		case ']':
		case '&':
		case '|':
		case '^':
		case '%':
			return 1;
		default:
			return 0;
	}
}

/**
 * checks for a single-character symbol
 */
static int jep_is_symbol(const char* s)
{
	int i;
	for(i = 0; i < 21; i++)
	{
		if(!strcmp(symbols[i], s))
		{
			return i;
		}
	}
	return -1;
}

/**
 * checks for a double-character symbol
 */
static int jep_is_symbol2(const char* s)
{
	int i;
	for(i = 0; i < 18; i++)
	{
		if(!strcmp(symbols2[i], s))
		{
			return i;
		}
	}
	return -1;
}

/**
 * checks for a tripple-character symbol
 */
static int jep_is_symbol3(const char* s)
{
	int i;
	for(i = 0; i < 2; i++)
	{
		if(!strcmp(symbols3[i], s))
		{
			return i;
		}
	}
	return -1;
}

/**
 * checks for a keyword
 */
static int jep_is_keyword(const char* s)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		if(!strcmp(s, keywords[i]))
		{
			return i;
		}
	}

	return -1;
}

/**
 * checks for the beginning of an identifier
 */
static int jep_is_ident_start(char c)
{
	if(isalpha(c) || c == '_' || c == '$')
	{
		return 1;
	}

	return 0;
}

/**
 * checks for identifier characters
 */
static int jep_is_ident(char c)
{
	if(isalnum(c) || c == '_' || c == '$')
	{
		return 1;
	}

	return 0;
}

/**
 * gives a token a token code. Only symbols and keywords get token codes
 */
static void jep_classify_token(jep_token* t)
{
	if(t->type == T_SYMBOL)
	{
		if(t->val->size == 1)
		{
			t->token_code = jep_is_symbol(t->val->buffer) + 1;
		}
		else if(t->val->size == 2)
		{
			t->token_code = jep_is_symbol2(t->val->buffer) + 22;
		}
		else if(t->val->size == 3)
		{
			t->token_code = jep_is_symbol3(t->val->buffer) + 40;
		}
	}
	else if(t->type == T_KEYWORD)
	{
		int code = jep_is_keyword(t->val->buffer);
		t->token_code = code + 42;
		if(code > 5)
		{
			t->type = T_MODIFIER;
		}
	}
}

/**
 * allocates memory for a new token
 */
jep_token* jep_create_token()
{
	jep_token *t = malloc(sizeof(jep_token));
	t->val = jep_create_string_builder();
	t->type = 0;
	t->token_code = 0;
	t->row = 0;
	t->column = 0;
	t->unary = 0;
	t->postfix = 0;
	return t;
}

/**
 * frees the memory allocated for a token
 */
void jep_destroy_token(jep_token* t)
{
	jep_destroy_string_builder(t->val);
	free(t);
}

/**
 * creates a new token stream 
 */
jep_token_stream* jep_create_token_stream()
{
	jep_token_stream* ts = malloc(sizeof(jep_token_stream));
	ts->size = 0;
	ts->cap = 50;
	ts->tok = malloc(50 * sizeof(jep_token));
	return ts;
}

/**
 * frees memory allocated for a token stream
 */
void jep_destroy_token_stream(jep_token_stream* ts)
{
	int i;
	for(i = 0; i < ts->size; i++)
	{
		jep_destroy_string_builder(ts->tok[i].val);
	}

	free(ts->tok);
	free(ts);
}

/**
 * adds a token to a token stream
 */
void jep_append_token(jep_token_stream* ts, jep_token t)
{
	if(ts->size >= ts->cap)
	{
		int new_cap = ts->cap + ts->cap / 2;
		ts->tok = realloc(ts->tok, sizeof(jep_token) * new_cap);
		ts->cap = new_cap;
	}

	ts->tok[ts->size++] = t;
}

/**
 * tokenizes the contents of a text file
 */
jep_token_stream* jep_tokenize_file(const char* file_name)
{
	FILE* in_file;          /* the input file                        */
	jep_string_builder* sb; /* puts the character data into a string */
	char* s;                /* the string of character data          */
	jep_token_stream* ts;   /* tokens                                */
	int row;                /* the row of each token in the file     */
	int col;                /* the column of each token in the file  */
	int i;                  /* loop index                            */

	in_file = fopen(file_name, "r");
	row = 1;
	col = 1;
	
	if(in_file == NULL)
	{
		/* failed to open the input file */
		printf("could not open input file\n");
		return NULL;
	}

	sb = jep_create_string_builder();
	ts = jep_create_token_stream();
	jep_scan_file(in_file, sb);
	s = sb->buffer;
	i = 0;
	while(i < sb->size)
	{
		/* skip block comments */
		if(s[i] == '/' && s[i+1] == '*')
		{
			i += 2;
			col += 2;
			while(!(s[i] == '*' && s[i+1] == '/') && i < sb->size)
			{
				if(s[i] == '\n')
				{
					col = 1;
					row++;
				}
				else
				{
					col++;
				}

				i++;
			}
			i += 2;
			col +=2;
		}

		/* skip line comments */
		if((s[i] == '/' && s[i+1] == '/') || s[i] == '#')
		{
			i += 2;
			col += 2;
			while(!(s[i] == '\n') && i < sb->size)
			{
				i++;
				col++;
			}
		}
			
		/* detect identifiers */
		if(jep_is_ident_start(s[i]))
		{
			jep_string_builder* val = jep_create_string_builder();
			jep_token ident = 
			{
				val, T_IDENTIFIER, 0, row, col, 0, 0
			};
			do
			{
				jep_append_char(ident.val, s[i++]);
				col++;
			}while(jep_is_ident(s[i]));
			if(jep_is_keyword(ident.val->buffer) >= 0)
			{
				ident.type = T_KEYWORD;
			}
			jep_classify_token(&ident);
			jep_append_token(ts, ident);
		}
			
		/* detect characters */
		if(s[i] == '\'')
		{
			jep_string_builder* val = jep_create_string_builder();
			jep_token c = 
			{
				val, T_CHARACTER, 0, row, col, 0, 0
			};
			i++;
			col++;
			do
			{
				jep_append_char(c.val, s[i]);
				/* check for escape sequences */
				if(s[i] == '\\')
				{
					i++;
					col++;
					jep_append_char(c.val, s[i]);
				}
				i++;
				col++;
			}while(s[i] != '\'' && i < sb->size);
			jep_append_token(ts, c);
		}
		
		/* detect strings */
		if(s[i] == '"')
		{
			jep_string_builder* val = jep_create_string_builder();
			jep_token str = 
			{
				val, T_STRING, 0, row, col, 0, 0
			};
			i++;
			col++;
			while(s[i] != '"' && i < sb->size)
			{
				jep_append_char(str.val, s[i]);
				/* check for escape sequences */
				if(s[i] == '\\' && s[i+1] == '"')
				{
					i++;
					col++;
					jep_append_char(str.val, s[i]);
				}
				i++;
				col++;
			}
			jep_append_token(ts, str);
		}
		
		/* detect symbols */
		if(jep_is_symbol_char(s[i]))
		{
			jep_string_builder* val = jep_create_string_builder();
			jep_token sym = 
			{
				val, T_SYMBOL, 0, row, col, 0, 0
			};
			char symbol[] = { s[i], '\0', '\0', '\0' };
			if(i < sb->size - 1)
			{
				symbol[1] = s[i+1];
				symbol[2] = s[i+2];
			}
			if(jep_is_symbol3(symbol) >= 0)
			{
				jep_append_string(sym.val, symbol);
				i += 2;
				col += 2;
			}
			else if(symbol[2] = '\0', jep_is_symbol2(symbol) >= 0)
			{
				jep_append_string(sym.val, symbol);
				i++;
				col++;
			}
			else if(symbol[1] = '\0', jep_is_symbol(symbol) >= 0)
			{
				jep_append_string(sym.val, symbol);
			}

			jep_classify_token(&sym);
			jep_append_token(ts, sym);
		}
		
		/* detect numbers */
		if(isdigit(s[i]))
		{
			jep_string_builder* val = jep_create_string_builder();
			jep_token num = 
			{
				val, T_NUMBER, 0, row, col, 0, 0
			};
			int dec = 0;
			do
			{
				/* keep track of occurrences of '.' */
				if(s[i] == '.') 
					dec++;

				if(dec < 2)
				{
					jep_append_char(num.val, s[i]);
					i++;
				}
				col++;
			}while((isdigit(s[i]) || s[i] == '.' ) && dec < 2);
			jep_append_token(ts, num);
			i--;
			col--;
		}

		if(s[i] == '\n')
		{
			/* increment the row when we find a '\n' */
			row++;
			col = 1;
		}
		else
		{
			col++;
		}
		i++;
	}

	/* append an END token */
	jep_token end_token = 
	{
		jep_create_string_builder(), T_END, T_EOF, row, col, 0, 0
	};
	jep_append_string(end_token.val, "EOF");
	jep_append_token(ts, end_token);
	
	/* free memory */
	fclose(in_file);
	jep_destroy_string_builder(sb);

	return ts;
}

/**
 * prints the tokens in a token stream
 */
void jep_print_tokens(jep_token_stream* ts, FILE* f)
{
	int i;
	fprintf(f,"-----------------------------------------------\n");
	fprintf(f,"%-12s %-7s %-7s %-12s value\n","type", "row", "column", "token code");
	fprintf(f,"-----------------------------------------------\n");
	for(i = 0; i < ts->size; i++)
	{
		switch(ts->tok[i].type)
		{
			case T_SYMBOL:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[symbol]", 
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_IDENTIFIER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[identifier]", 
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_CHARACTER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[character]", 
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_STRING:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[string]", 
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_NUMBER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[number]", 
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_KEYWORD:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[keyword]",
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_MODIFIER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[modifier]",
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			case T_END:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[end]",
				ts->tok[i].row, ts->tok[i].column, 
				ts->tok[i].token_code, ts->tok[i].val->buffer);
				break;
			default:
				break;
		}
	}
}
