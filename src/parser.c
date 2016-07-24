#include "parser.h"

/* checks if a token is an 'end of expression' token */
static int jep_eoe(jep_token* token)
{
	switch(token->token_code)
	{
		case T_SEMICOLON:
		case T_RPAREN:
		case T_EOF:
		case T_COMMA:
		case T_RSQUARE:
		case T_RBRACE:
			return 1;

		default:
			return 0;
	}
}

/* determines an operator's associativity */
static int jep_associativity(jep_ast_node* node)
{
	if(node == NULL)
	{
		return 0;
	}
	
	switch(node->token->token_code)
	{
		case T_EQUALS:
			return JEP_RIGHT_ASSOC;

		default:
			return JEP_LEFT_ASSOC;
	}
}

/* determines the priority of an operator based on order of operations */
static int jep_priority(jep_ast_node* node)
{
	int priority = -1;

	/* NULL nodes take lowest riority */
	if(node == NULL)
	{
		return -1;
	}

	/* unary operators take highest priority */
	if(node->token->unary)
	{
		return 7;
	}

	switch(node->token->token_code)
	{
		case T_RPAREN:
		case T_EQUALS:
		case T_ADDASSIGN:
		case T_SUBASSIGN:
		case T_MULASSIGN:
		case T_DIVASSIGN:
		case T_SEMICOLON:
		case T_EOF:
			priority = 0;
			break;

		case T_LOGAND:
		case T_LOGOR:
			priority = 1;
			break;

		case T_BITAND:
		case T_BITOR:
		case T_BITXOR:
		case T_LSHIFT:
		case T_RSHIFT:
			priority = 2;
			break;

		case T_LESS:
		case T_GREATER:
		case T_LOREQUAL:
		case T_GOREQUAL:
		case T_EQUIVALENT:
		case T_NOTEQUIVALENT:
			priority = 3;
			break;

		case T_PLUS:
		case T_MINUS:
			priority = 4;
			break;

		case T_FSLASH:
		case T_STAR:
			priority = 5;
			break;

		case T_LPAREN:
			priority = 6;
			break;

		default:
			priority = 0;
			break;
	}
	return priority;
}

/* 
 * determine whether or not the current oeprator stack should be emptied 
 * cur is the current operator on the token stream
 * top is the operator on the top of the operator stack
 */
static int jep_prioritize(jep_ast_node* cur, jep_ast_node* top)
{
	if(jep_associativity(top) == JEP_LEFT_ASSOC)
	{
		if(jep_priority(cur) <= jep_priority(top))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(jep_associativity(top) == JEP_RIGHT_ASSOC)
	{
		if(jep_priority(cur) < jep_priority(top))
		{
			return 1;
		}
		else
		{
			return 0;
		}	
	}

	return 0;
}

/* checks if an operator is potentially unary */
static int jep_check_unary(jep_token* cur, jep_token* prev)
{
	int unary = 0;
	switch(cur->token_code)
	{
		case T_INCREMENT:
		case T_DECREMENT:
		case T_NOT:
			unary = 1;
			cur->unary = 1;
			break;

		case T_MINUS:
			if(prev == NULL || (prev->type == T_SYMBOL 
				&& !prev->unary && prev->token_code != T_RPAREN))
			{
				unary = 1;
				cur->unary = 1;
			}
			break;

		default:
			break;
	}

	return unary;
}

/* advances the node pointer for a specfic token code */
int jep_accept(int token_code, jep_ast_node** nodes)
{
	if((*nodes)->token->token_code == token_code)
	{
		(*nodes)++;
		return 1;
	}
	else
	{
		return 0;
	}
}

/* advances the node pointer for a specfic token type */
int jep_accept_type(int type, jep_ast_node** nodes)
{
	if((*nodes)->token->type == type)
	{
		(*nodes)++;
		return 1;
	}
	else
	{
		return 0;
	}
}

/* parses a stream of tokens */
jep_ast_node* jep_parse(jep_token_stream* ts, jep_ast_node** nodes)
{
	jep_ast_node* first;  /* the first node          */
	jep_ast_node* root;   /* the root of the AST     */
	int i;                /* index variable          */

	*nodes = malloc(sizeof(jep_ast_node) * ts->size);

	/* create an AST node for each token */
	for(i = 0; i < ts->size; i++)
	{
		(*nodes)[i].leaf_count = 0;
		(*nodes)[i].capacity = 10;
		(*nodes)[i].leaves = NULL;
		(*nodes)[i].token = ts->tok[i];
	}

	/* create the root of the AST */
	root = NULL;
	root = malloc(sizeof(jep_ast_node));
	root->leaf_count = 0;
	root->capacity = 10;
	root->leaves = NULL;
	root->token = jep_create_token();
	root->token->type = T_SYMBOL;
	root->token->token_code = 0;
	root->error = 0;
	jep_append_string(root->token->value, "root");

	first = *nodes;

	do
	{
		jep_statement(root, nodes);
	}while((*nodes)->token->token_code != T_EOF && root->error == 0);

	(*nodes) = first;

	return root;

}

jep_ast_node* jep_expression(jep_ast_node* root, jep_ast_node** nodes)
{
	jep_stack exp;      /* expresion stack   */
	jep_stack opr;      /* operator stack    */
	jep_token* prev;    /* previous token    */
	jep_token* cur;     /* current token     */
	jep_token* next;    /* next token        */
	jep_ast_node* ast;  /* the resulting AST */

	exp.size = 0;
	exp.capacity = 10;
	exp.top = NULL;
	exp.nodes = malloc(sizeof(jep_ast_node*) * exp.capacity);

	opr.size = 0;
	opr.capacity = 10;
	opr.top = NULL;
	opr.nodes = malloc(sizeof(jep_ast_node*) * opr.capacity);

	prev = NULL;
	cur = (*nodes)->token;
	next = (*nodes + 1)->token;
	if(jep_eoe(cur) && cur->token_code != T_SEMICOLON)
	{
		printf("unexpected token '%s' at %d,%d\n", 
			(*nodes)->token->value->buffer,
			(*nodes)->token->row, 
			(*nodes)->token->column);

		/* free the memory allocated for the stacks */
		free(exp.nodes);
		free(opr.nodes);
		root->error = 1;
		return NULL;
	}
	while(!jep_eoe(cur))
	{
		switch((*nodes)->token->type)
		{
			case T_IDENTIFIER:
			case T_NUMBER:
			case T_STRING:
			case T_CHARACTER:
				jep_push(&exp, *nodes);
				break;

			case T_SYMBOL:
			{
				/* check if the symbol is a unary operator */
				jep_check_unary(cur, prev);

				/* check for parentheses */
				if(cur->token_code == T_LPAREN && next->token_code != T_EOF)
				{
					jep_ast_node* l_paren = (*nodes);
					(*nodes)++;
					do
					{
						jep_ast_node* p = jep_expression(root, nodes);
						prev = (*nodes - 1)->token;
						cur = (*nodes)->token;
						if(cur->token_code != T_EOF)
						{
							next = (*nodes + 1)->token;
						}
						if(p != NULL)
						{
							jep_add_leaf_node(l_paren, p);
						}
						else
						{
							free(exp.nodes);
							free(opr.nodes);
							return NULL;
						}
					}
					while(jep_accept(T_COMMA, nodes) 
						&& (*nodes)->token->token_code != T_EOF);
					if((*nodes)->token->token_code != T_RPAREN)
					{
						free(exp.nodes);
						free(opr.nodes);
						root->error = 1;
						printf("expected ')' at %d,%d but found '%s'\n", 
							(*nodes)->token->row, 
							(*nodes)->token->column, 
							(*nodes)->token->value->buffer);
						return NULL;
					}

					jep_push(&exp, l_paren);
				}
				else if(cur->token_code == T_LBRACE && next->token_code != T_EOF)
				{
					jep_ast_node* l_brace = (*nodes);
					(*nodes)++;
					do
					{
						jep_ast_node* p = jep_expression(root, nodes);
						prev = (*nodes - 1)->token;
						cur = (*nodes)->token;
						if(cur->token_code != T_EOF)
						{
							next = (*nodes + 1)->token;
						}
						if(p != NULL)
						{
							jep_add_leaf_node(l_brace, p);
						}
						else
						{
							free(exp.nodes);
							free(opr.nodes);
							return NULL;
						}
					}
					while(jep_accept(T_COMMA, nodes) 
						&& (*nodes)->token->token_code != T_EOF);
					if((*nodes)->token->token_code != T_RBRACE)
					{
						free(exp.nodes);
						free(opr.nodes);
						root->error = 1;
						printf("expected '}' at %d,%d but found '%s'\n", 
							(*nodes)->token->row, 
							(*nodes)->token->column, 
							(*nodes)->token->value->buffer);
						return NULL;
					}
					jep_push(&exp, l_brace);
				}
				else if(jep_prioritize(*nodes, opr.top))
				{
					jep_ast_node* r; /* right operand */
					jep_ast_node* l; /* left operand */
					jep_ast_node* o; /* operator */
					do
					{
						o = jep_pop(&opr);
						if(o != NULL && o->token->unary)
						{
							r = jep_pop(&exp);
							if(o != NULL && r != NULL)
							{
								jep_add_leaf_node(o, r);
								jep_push(&exp, o);
							}
						}
						else if(o != NULL)
						{
							r = jep_pop(&exp);
							l = jep_pop(&exp);
							if(o != NULL && r != NULL && l != NULL)
							{
								jep_add_leaf_node(o, l);
								jep_add_leaf_node(o, r);
								jep_push(&exp, o);
							}
							else
							{
								printf("error: expected two operands for binary operator %s\n", 
									o->token->value->buffer);
							}
						}
					}while(opr.top != NULL && jep_priority(opr.top) > jep_priority(*nodes));
					jep_push(&opr, *nodes);
				}
				else
				{
					jep_push(&opr, *nodes);	
				}
				
			}
				break;

			default:
				printf("unexpected token '%s' at %d,%d\n", 
					(*nodes)->token->value->buffer,
					(*nodes)->token->row, 
					(*nodes)->token->column);
				root->error = 1;
				break;
		}

		(*nodes)++;

		prev = cur;
		cur = next;
		if(cur->token_code != T_EOF)
		{
			next = (*nodes + 1)->token;
		}
	}

	if(jep_eoe(cur))
	{
		do
		{
			jep_ast_node* r; /* right operand */
			jep_ast_node* l; /* left operand */
			jep_ast_node* o; /* operator */
			o = jep_pop(&opr);
			if(o != NULL && o->token->unary)
			{
				r = jep_pop(&exp);
				if(o != NULL && r != NULL)
				{
					jep_add_leaf_node(o, r);
					jep_push(&exp, o);
				}
			}
			else if(o != NULL)
			{
				r = jep_pop(&exp);
				l = jep_pop(&exp);
				if(o != NULL && r != NULL && l != NULL)
				{
					jep_add_leaf_node(o, l);
					jep_add_leaf_node(o, r);
					jep_push(&exp, o);
				}
				else
				{
					printf("error: expected two operands for binary operator %s\n", 
						o->token->value->buffer);
				}
			}
		}while(opr.size > 0);
	}
	else
	{
		printf("invalid end of statement\n");
	}

	if(exp.size > 1)
	{
		printf("unexpected token '%s' at %d,%d\n", 
			exp.top->token->value->buffer,
			exp.top->token->row, 
			exp.top->token->column);
		root->error = 1;
		ast = NULL;
	}
	else if(opr.size > 0)
	{
		printf("unexpected token '%s' at %d,%d\n", 
			opr.top->token->value->buffer,
			opr.top->token->row, 
			opr.top->token->column);
		root->error = 1;
		ast = NULL;
	}
	else
	{
		/* the top node of the expression stack is the AST */
		ast = exp.top;	
	}

	jep_pop_all(&exp);
	jep_pop_all(&opr);

	/* free the memory allocated for the stacks */
	free(exp.nodes);
	free(opr.nodes);

	return ast;
}

/* parses a statement */
jep_ast_node* jep_statement(jep_ast_node* root, jep_ast_node** nodes)
{
	do
	{
		jep_ast_node* statement = jep_expression(root, nodes);
		if(statement != NULL)
		{
			jep_add_leaf_node(root, statement);
		}
	}while(jep_accept(T_COMMA, nodes));

	if(jep_accept(T_SEMICOLON, nodes))
	{
		return root;	
	}
	else
	{
		printf("expected ; at %d,%d but found %s\n", 
			(*nodes)->token->row, 
			(*nodes)->token->column, 
			(*nodes)->token->value->buffer);
		root->error = 1;
		return NULL;
	}
}

/* parses a block of code */
jep_ast_node* jep_block(jep_ast_node* root, jep_ast_node** nodes)
{
	return NULL;
}
