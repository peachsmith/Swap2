#include "tokenizer.h"

/* one-character symbols */
const char *symbols[] = 
{
	"+", "-", "/", "*", ";", ",", "(", ")", "[", "]", "{", "}", "<", 
	">", "=", ".", "&", "|", "^"
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
	"if", "else", "for", "while"
};

/* primitives */
const char *primitives[] =
{
	"void", "int", "char", "float", "double"
};


/* escape characters */
const char escapes[] = 
{
	'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\'', '"', '?'
};

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

/* assign a token code to a token */
void jep_classify_token(jep_token* t)
{
	if(t->type == T_SYMBOL)
	{
		if(t->value->size == 1)
		{
			t->token_code = jep_is_symbol(t->value->buffer) + 1;
		}
		else if(t->value->size == 2)
		{
			t->token_code = jep_is_symbol2(t->value->buffer) + 20;
		}
		else if(t->value->size == 3)
		{
			t->token_code = jep_is_symbol3(t->value->buffer) + 37;
		}
	}
	else if(t->type == T_KEYWORD)
	{
		t->token_code = jep_is_keyword(t->value->buffer) + 39;
	}
}

/* creates a new token builder */
jep_token_builder* jep_create_token_builder()
{
	jep_token_builder* tb = malloc(sizeof(jep_token_builder));
	tb->size = 0;
	tb->capacity = 10;
	tb->tokens = malloc(10 * sizeof(jep_token*));
	return tb;
}

/* frees memory allocated for a token builder */
void jep_destroy_token_builder(jep_token_builder* tb)
{
	int i;
	for(i = 0; i < tb->size; i++)
	{
		jep_destroy_token(tb->tokens[i]);
	}
	free(tb->tokens);
	free(tb);
}

/* increases the capacity of a token builder by approximately 50% */
void jep_resize_token_builder(jep_token_builder* tb)
{
	int new_capacity;
	jep_token** new_tokens;
	int i;
	
	new_capacity = tb->capacity + tb->capacity / 2;
	new_tokens = malloc(new_capacity * sizeof(jep_token*));
	
	for(i = 0; i < tb->size; i++)
	{
		new_tokens[i] = tb->tokens[i];
	}
	
	free(tb->tokens);
	tb->tokens = new_tokens;
	tb->capacity = new_capacity;
}

/* adds a token to a token builder */
void jep_append_token(jep_token_builder* tb, jep_token* t)
{
	if(tb->size >= tb->capacity)
	{
		jep_resize_token_builder(tb);
	}
	tb->tokens[tb->size++] = t;
}

/* checks for a symbol character */
int jep_is_symbol_char(char c)
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

/* reads the contents of a file into memory */
void jep_scan_file(FILE* file, jep_string_builder *sb)
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

/* tokenizes the contents of a text file */
jep_token_builder* jep_tokenize_file(const char* file_name)
{
	FILE* in_file;          /* the input file */
	jep_string_builder* sb; /* puts the character data into a string */
	char* s;                /* the character data */
	jep_token_builder* tb;  /* tokens */
	
	in_file = fopen(file_name, "r");
	
	if(in_file != NULL)
	{
		sb = jep_create_string_builder();
		tb = jep_create_token_builder();
		jep_scan_file(in_file, sb);
		s = sb->buffer;
		int i = 0;
		while(i < sb->size)
		{
			/* skip block comments */
			if(s[i] == '/' && s[i+1] == '*')
			{
				i += 2;
				while(!(s[i] == '*' && s[i+1] == '/') && i < sb->size)
					i++;
				i += 2;
			}

			/* skip line comments */
			if(s[i] == '/' && s[i+1] == '/')
			{
				i += 2;
				while(!(s[i] == '\n') && i < sb->size)
					i++;
				i++;
			}
			
			/* detect identifiers */
			if(jep_is_ident_start(s[i]))
			{
				jep_token* ident = jep_create_token();
				do
				{
					jep_append_char(ident->value, s[i++]);
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
				jep_append_token(tb, ident);
			}
			
			/* detect characters */
			if(s[i] == '\'')
			{
				i++;
				jep_token* c = jep_create_token();
				do
				{
					jep_append_char(c->value, s[i]);
					if(s[i] == '\\' && s[i+1] == '\'')
					{
						i++;
						jep_append_char(c->value, s[i]);
					}
					i++;
				}while(s[i] != '\'' && i < sb->size);
				c->type = T_CHARACTER;
				jep_append_token(tb, c);
			}
			
			/* detect strings */
			if(s[i] == '"')
			{
				i++;
				jep_token* str = jep_create_token();
				do
				{
					jep_append_char(str->value, s[i]);
					if(s[i] == '\\' && s[i+1] == '"')
					{
						i++;
						jep_append_char(str->value, s[i]);
					}
					i++;
				}while(s[i] != '"' && i < sb->size);
				str->type = T_STRING;
				jep_append_token(tb, str);
			}
			
			/* detect symbols */
			if(jep_is_symbol_char(s[i]))
			{
				jep_token* sym = jep_create_token();
				char symbol[] = {s[i], '\0', '\0', '\0'};
				if(i < sb->size - 1)
				{
					symbol[1] = s[i+1];
					symbol[2] = s[i+2];
					if(jep_is_symbol3(symbol) >= 0)
					{
						jep_append_string(sym->value, symbol);
						i += 2;
					}
					else
					{
						symbol[2] = '\0';
						if(jep_is_symbol2(symbol) >= 0)
						{
							jep_append_string(sym->value, symbol);
							i++;
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
				jep_append_token(tb, sym);
			}
			
			/* detect numbers */
			if(isdigit(s[i]))
			{
				jep_token* num = jep_create_token();
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
				}while((isdigit(s[i]) || s[i] == '.' ) && dec < 2);
				num->type = T_NUMBER;
				jep_append_token(tb, num);
				i--;
			}
			
			i++;
		}
		
		/* append an END token */
		jep_token* end_token = jep_create_token();
		end_token->type = T_END;
		end_token->token_code = T_EOF;
		jep_append_string(end_token->value, "__END__");
		jep_append_token(tb, end_token);
		
		/* free memory */
		fclose(in_file);
		jep_destroy_string_builder(sb);
		return tb;
	}
	else
	{
		/* failed to open the input file */
		printf("could not open input file\n");
		return NULL;
	}
}

/* checks for a single-character symbol */
int jep_is_symbol(const char* s)
{
	int i;
	for(i = 0; i < 19; i++)
	{
		if(!strcmp(symbols[i], s))
			return i;
	}
	return -1;
}

/* checks for a double-character symbol */
int jep_is_symbol2(const char* s)
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
int jep_is_symbol3(const char* s)
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
int jep_is_keyword(const char* s)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		if(!strcmp(s, keywords[i]))
			return i;
	}
	return -1;
}

/* checks if a token is a primitive */
int jep_is_primitive(const char* s)
{
	int i;
	for(i = 0; i < 5; i++)
	{
		if(!strcmp(s, primitives[i]))
			return i;
	}
	return -1;
}

/* checks for the beginning of an identifier */
int jep_is_ident_start(char c)
{
	if(isalpha(c) || c == '_' || c == '$')
		return 1;
	else
		return 0;
}

/* checks for identifier characters */
int jep_is_ident(char c)
{
	if(isalnum(c) || c == '_' || c == '$')
		return 1;
	else
		return 0;
}

/* checks for an escape character */
int jep_is_escape(char c)
{
	int i;
	for(i = 0; i < 11; i++)
	{
		if(escapes[i] == c)
		{
			return 1;
		}
	}
	return 0;
}

/* prints the tokens */
void jep_print_tokens(jep_token_builder* tb, FILE* f)
{
	fprintf(f,"-------------------------------\n");
	fprintf(f,"%-12s %-12s value\n","type","token code");
	fprintf(f,"-------------------------------\n");
	int t_count;
	for(t_count = 0; t_count < tb->size; t_count++)
	{
		switch(tb->tokens[t_count]->type)
		{
			case T_SYMBOL:
				fprintf(f,"%-12s %-12d %s\n","[symbol]", 
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			case T_IDENTIFIER:
				fprintf(f,"%-12s %-12d %s\n","[identifier]", 
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			case T_CHARACTER:
				fprintf(f,"%-12s %-12d %s\n","[character]", 
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			case T_STRING:
				fprintf(f,"%-12s %-12d %s\n","[string]", 
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			case T_NUMBER:
				fprintf(f,"%-12s %-12d %s\n","[number]", 
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			case T_KEYWORD:
				fprintf(f,"%-12s %-12d %s\n","[keyword]",
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			case T_END:
				fprintf(f,"%-12s %-12d %s\n","[end]",
				tb->tokens[t_count]->token_code,
					tb->tokens[t_count]->value->buffer);
				break;
			default:
				break;
		}
		
	}
}
