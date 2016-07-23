#include "parser.h"

/* checks if a token is a terminal */
int jep_is_term(jep_token* token)
{
	int term = 0;

	/* NULL is considered a terminal */
	if(token == NULL) 
	{
		return 1;
	}

	switch(token->token_code)
	{
		case T_SEMICOLON:
		case T_RPAREN:
		case T_EOF:
		case T_COMMA:
		case T_RSQUARE:
		case T_RBRACE:
			term = 1;
			break;

		default:
			break;
	}

	return term;
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
jep_ast_node* jep_parse(jep_token_builder* tb, jep_ast_node** nodes)
{
	jep_token** tok;      /* the tokens              */
	jep_ast_node* first;  /* the first node          */
	jep_ast_node* root;   /* the root of the AST     */
	int i;                /* index variable          */

	tok = tb->tokens;
	*nodes = malloc(sizeof(jep_ast_node) * tb->size);

	/* create an AST node for each token */
	for(i = 0; i < tb->size; i++)
	{
		(*nodes)[i].leaf_count = 0;
		(*nodes)[i].capacity = 10;
		(*nodes)[i].leaves = NULL;
		(*nodes)[i].token = tok[i];
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
	if(jep_is_term(cur) && cur->token_code != T_SEMICOLON)
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
	while(!jep_is_term(cur))
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
				else if(jep_priority(*nodes) < jep_priority(opr.top))
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

	if(jep_is_term(cur))
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

/* create an AST node */
jep_ast_node* jep_create_ast_node()
{
	jep_ast_node* node = malloc(sizeof(jep_ast_node));
	node->token = NULL;
	node->leaf_count = 0;
	node->capacity = 10;
	node->leaves = NULL;
	return node;
}

/* increases the memory allocated for an AST node's leaf nodes by about 50% */
void jep_resize_ast_node(jep_ast_node* node)
{
	int new_cap = node->capacity + node->capacity / 2;
	jep_ast_node* new_leaves = malloc(sizeof(jep_ast_node) * new_cap);

	int i;
	for(i = 0; i < node->leaf_count; i++)
	{
		new_leaves[i] = node->leaves[i];
	}

	free(node->leaves);

	node->leaves = new_leaves;
	node->capacity = new_cap;
}

/* adds a leaf node to an AST node */
void jep_add_leaf_node(jep_ast_node* root, jep_ast_node* leaf)
{

	if(root->leaves == NULL)
	{
		root->leaves = malloc(sizeof(jep_ast_node) * root->capacity);
	}

	if(root->leaf_count == root->capacity)
	{
		jep_resize_ast_node(root);
	}
	if(leaf != NULL)
	{
		root->leaves[root->leaf_count++] = *leaf;
	}
}

/* prints the AST */
void jep_print_ast(jep_ast_node root)
{
	static int indent = 1;

	if(root.leaf_count > 0)
	{
		printf("%*s\n", indent, root.token->value->buffer);
		indent++;
		int i;
		for(i = 0; i < root.leaf_count; i++)
		{
			jep_print_ast(root.leaves[i]);
		}
		indent--;
	}
	else
	{
		printf("%*s\n", indent, root.token->value->buffer);
	}

}

/* creates a stack */
/*
jep_stack* jep_create_stack()
{
	jep_stack* stack = malloc(sizeof(jep_stack));
	stack->size = 0;
	stack->capacity = 10;
	stack->top = NULL;
	stack->nodes = malloc(sizeof(jep_ast_node*) * stack->capacity);
	return stack;
}
*/

/* pushes an AST node onto the top of the stack */
void jep_push(jep_stack* stack, jep_ast_node* node)
{

	if(stack->size == stack->capacity)
	{
		int new_cap = stack->capacity + stack->capacity / 2;
		jep_ast_node** new_nodes = malloc(sizeof(jep_ast_node*) * new_cap);

		int i;
		for(i = 0; i < stack->capacity; i++)
		{
			new_nodes[i] = stack->nodes[i];
		}

		free(stack->nodes);

		stack->nodes = new_nodes;
		stack->capacity = new_cap;
	}

	stack->nodes[stack->size++] = node;
	stack->top = stack->nodes[stack->size-1];

}

/* pops an AST node from the top of the stack */
jep_ast_node* jep_pop(jep_stack* stack)
{
	if(stack->size > 0)
	{
		jep_ast_node* top = stack->nodes[stack->size-1];
		stack->nodes[stack->size-1] = NULL;
		stack->size = stack->size - 1;
		if(stack->size > 0)
		{
			stack->top = stack->nodes[stack->size-1];
		}
		else
		{
			stack->top = NULL;
		}
		return top;
	}
	else
	{
		return NULL;
	}
}

/* pops all AST nodes from the stack */
void jep_pop_all(jep_stack* stack)
{
	int i;
	for(i = 0; i < stack->size; i++)
	{
		stack->nodes[i] = NULL;
	}
}

/* determines the priority of an operator based on order of operations */
int jep_priority(jep_ast_node* node)
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
		return 8;
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
			priority = 5;
			break;

		case T_STAR:
			priority = 6;
			break;

		case T_LPAREN:
			priority = 7;
			break;

		default:
			priority = 0;
			break;
	}
	return priority;
}

/* checks if an operator is potentially unary */
int jep_check_unary(jep_token* cur, jep_token* prev)
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
