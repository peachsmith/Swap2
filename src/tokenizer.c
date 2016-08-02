#include "tokenizer.h"

/* one-character symbols */
const char *symbols[] = 
{
	"+", "-", "/", "*", ";", ",", "(", ")", "[", "]", "{", "}", "<", 
	">", "=", ".", "&", "|", "^", "!"
};

/* two-character symbols */
const char *symbols2[] = 
{
	"++", "--", "+=", "-=", "*=", "/=", "==", "<=", ">=", ">>", "<<",
	"&=", "|=", "^=", "&&", "||", "!="
};

/* three-character symbols */
const char *symbols3[] = 
{
	"<<=", ">>="
};

/* keywords */
const char *keywords[] = 
{
	"if", "else", "for", "while", "function", "return"
};

/* checks for a symbol character */
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
			return 1;
		default:
			return 0;
	}
}

/* checks for a single-character symbol */
static int jep_is_symbol(const char* s)
{
	int i;
	for(i = 0; i < 20; i++)
	{
		if(!strcmp(symbols[i], s))
			return i;
	}
	return -1;
}

/* checks for a double-character symbol */
static int jep_is_symbol2(const char* s)
{
	int i;
	for(i = 0; i < 17; i++)
	{
		if(!strcmp(symbols2[i], s))
			return i;
	}
	return -1;
}

/* checks for a tripple-character symbol */
static int jep_is_symbol3(const char* s)
{
	int i;
	for(i = 0; i < 2; i++)
	{
		if(!strcmp(symbols3[i], s))
			return i;
	}
	return -1;
}

/* checks for a keyword */
static int jep_is_keyword(const char* s)
{
	int i;
	for(i = 0; i < 6; i++)
	{
		if(!strcmp(s, keywords[i]))
			return i;
	}
	return -1;
}

/* checks for the beginning of an identifier */
static int jep_is_ident_start(char c)
{
	if(isalpha(c) || c == '_' || c == '$')
		return 1;
	else
		return 0;
}

/* checks for identifier characters */
static int jep_is_ident(char c)
{
	if(isalnum(c) || c == '_' || c == '$')
		return 1;
	else
		return 0;
}

/* reads the contents of a file into memory */
static void jep_scan_file(FILE* file, jep_string_builder *sb)
{
	char buffer[1025];
	size_t s;
	do
	{
		s = fread(buffer, 1, 1024, file);
		if(s > 0)
		{
			buffer[s] = '\0';
			jep_append_string(sb, buffer);
		}
	}while(s > 0);
}

/* increases the capacity of a token stream by approximately 50% */
static void jep_resize_token_stream(jep_token_stream* ts)
{
	int new_capacity;
	jep_token** new_tokens;
	int i;
	
	new_capacity = ts->capacity + ts->capacity / 2;
	new_tokens = malloc(new_capacity * sizeof(jep_token*));
	
	for(i = 0; i < ts->size; i++)
	{
		new_tokens[i] = ts->tok[i];
	}
	
	free(ts->tok);
	ts->tok = new_tokens;
	ts->capacity = new_capacity;
}

/* assign a token code to a token */
static void jep_classify_token(jep_token* t)
{
	if(t->type == T_SYMBOL)
	{
		if(t->value->size == 1)
		{
			t->token_code = jep_is_symbol(t->value->buffer) + 1;
		}
		else if(t->value->size == 2)
		{
			t->token_code = jep_is_symbol2(t->value->buffer) + 21;
		}
		else if(t->value->size == 3)
		{
			t->token_code = jep_is_symbol3(t->value->buffer) + 38;
		}
	}
	else if(t->type == T_KEYWORD)
	{
		t->token_code = jep_is_keyword(t->value->buffer) + 40;
	}
}

/* allocates memory for a new token */
jep_token* jep_create_token()
{
	jep_token *t = malloc(sizeof(jep_token));
	t->value = jep_create_string_builder();
	t->type = 0;
	t->token_code = 0;
	t->row = 0;
	t->column = 0;
	t->unary = 0;
	return t;
}

/* frees the memory allocated for a token */
void jep_destroy_token(jep_token* t)
{
	jep_destroy_string_builder(t->value);
	free(t);
}

/* creates a new token stream */
jep_token_stream* jep_create_token_stream()
{
	jep_token_stream* ts = malloc(sizeof(jep_token_stream));
	ts->size = 0;
	ts->capacity = 10;
	ts->tok = malloc(10 * sizeof(jep_token*));
	return ts;
}

/* frees memory allocated for a token stream */
void jep_destroy_token_stream(jep_token_stream* ts)
{
	int i;
	for(i = 0; i < ts->size; i++)
	{
		jep_destroy_token(ts->tok[i]);
	}
	free(ts->tok);
	free(ts);
}

/* adds a token to a token stream */
void jep_append_token(jep_token_stream* ts, jep_token* t)
{
	if(ts->size >= ts->capacity)
	{
		jep_resize_token_stream(ts);
	}
	ts->tok[ts->size++] = t;
}

/* tokenizes the contents of a text file */
jep_token_stream* jep_tokenize_file(const char* file_name)
{
	FILE* in_file;          /* the input file */
	jep_string_builder* sb; /* puts the character data into a string */
	char* s;                /* the string of character data */
	jep_token_stream* ts;   /* tokens */
	
	in_file = fopen(file_name, "r");
	int row = 1;
	int col = 1;
	
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
	int i = 0;
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
		if(s[i] == '/' && s[i+1] == '/')
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
			jep_token* ident = jep_create_token();
			ident->row = row;
			ident->column = col;
			do
			{
				jep_append_char(ident->value, s[i++]);
				col++;
			}while(jep_is_ident(s[i]));
			if(jep_is_keyword(ident->value->buffer) >= 0)
			{
				ident->type = T_KEYWORD;
			}
			else
			{
				ident->type = T_IDENTIFIER;
			}
			jep_classify_token(ident);
			jep_append_token(ts, ident);
		}
			
		/* detect characters */
		if(s[i] == '\'')
		{
			i++;
			jep_token* c = jep_create_token();
			c->row = row;
			c->column = col;
			col++;
			do
			{
				jep_append_char(c->value, s[i]);
				if(s[i] == '\\')
				{
					i++;
					col++;
					jep_append_char(c->value, s[i]);
				}
				i++;
				col++;
			}while(s[i] != '\'' && i < sb->size);
			c->type = T_CHARACTER;
			jep_append_token(ts, c);
		}
		
		/* detect strings */
		if(s[i] == '"')
		{
			jep_token* str = jep_create_token();
			str->row= row;
			str->column = col;
			i++;
			col++;
			do
			{
				jep_append_char(str->value, s[i]);
				if(s[i] == '\\' && s[i+1] == '"')
				{
					i++;
					col++;
					jep_append_char(str->value, s[i]);
				}
				i++;
				col++;
			}while(s[i] != '"' && i < sb->size);
			str->type = T_STRING;
			jep_append_token(ts, str);
		}
		
		/* detect symbols */
		if(jep_is_symbol_char(s[i]))
		{
			jep_token* sym = jep_create_token();
			sym->row = row;
			sym->column = col;
			char symbol[] = {s[i], '\0', '\0', '\0'};
			if(i < sb->size - 1)
			{
				symbol[1] = s[i+1];
				symbol[2] = s[i+2];
				if(jep_is_symbol3(symbol) >= 0)
				{
					jep_append_string(sym->value, symbol);
					i += 2;
					col += 2;
				}
				else
				{
					symbol[2] = '\0';
					if(jep_is_symbol2(symbol) >= 0)
					{
						jep_append_string(sym->value, symbol);
						i++;
						col++;
					}
					else
					{
						symbol[1] = '\0';
						if(jep_is_symbol(symbol) >= 0)
						{
							jep_append_string(sym->value, symbol);
						}
					}
				}
			}
			else
			{
				symbol[1] = s[i+1];
				if(jep_is_symbol2(symbol) >= 0)
				{
					jep_append_string(sym->value, symbol);
					i++;
					col++;
				}
				else
				{
					symbol[1] = '\0';
					if(jep_is_symbol(symbol) >= 0)
					{
						jep_append_string(sym->value, symbol);
					}
				}
			}
			sym->type = T_SYMBOL;
			jep_classify_token(sym);
			jep_append_token(ts, sym);
		}
		
		/* detect numbers */
		if(isdigit(s[i]))
		{
			jep_token* num = jep_create_token();
			num->row = row;
			num->column = col;
			int dec = 0;
			do
			{
				if(s[i] == '.')
				{
					dec++;
				}
				if(dec < 2)
				{
					jep_append_char(num->value, s[i]);
					i++;
				}
				col++;
			}while((isdigit(s[i]) || s[i] == '.' ) && dec < 2);
			num->type = T_NUMBER;
			jep_append_token(ts, num);
			i--;
			col--;
		}

		if(s[i] == '\n')
		{
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
	jep_token* end_token = jep_create_token();
	end_token->type = T_END;
	end_token->token_code = T_EOF;
	end_token->row = row;
	end_token->column = col;
	jep_append_string(end_token->value, "EOF");
	jep_append_token(ts, end_token);
	
	/* free memory */
	fclose(in_file);
	jep_destroy_string_builder(sb);

	return ts;
}

/* prints the tokens */
void jep_print_tokens(jep_token_stream* ts, FILE* f)
{
	fprintf(f,"-----------------------------------------------\n");
	fprintf(f,"%-12s %-7s %-7s %-12s value\n","type", "row", "column", "token code");
	fprintf(f,"-----------------------------------------------\n");
	int t_count;
	for(t_count = 0; t_count < ts->size; t_count++)
	{
		switch(ts->tok[t_count]->type)
		{
			case T_SYMBOL:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[symbol]", 
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			case T_IDENTIFIER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[identifier]", 
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			case T_CHARACTER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[character]", 
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			case T_STRING:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[string]", 
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			case T_NUMBER:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[number]", 
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			case T_KEYWORD:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[keyword]",
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			case T_END:
				fprintf(f,"%-12s %-7d %-7d %-12d %s\n","[end]",
				ts->tok[t_count]->row, ts->tok[t_count]->column, 
				ts->tok[t_count]->token_code,
					ts->tok[t_count]->value->buffer);
				break;
			default:
				break;
		}
		
	}
}
