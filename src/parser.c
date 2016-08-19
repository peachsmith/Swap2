#include "parser.h"

static int jep_accept(int, jep_ast_node**);
static jep_ast_node* jep_expression(jep_ast_node*, jep_ast_node**);
static jep_ast_node* jep_statement(jep_ast_node*, jep_ast_node**);
static void jep_block(jep_ast_node*, jep_ast_node**);

/**
 * checks if a token is an 'end of expression' token
 */
static int jep_eoe(jep_token* token)
{
	switch(token->token_code)
	{
		case T_SEMICOLON:
		case T_RPAREN:
		case T_EOF:
		case T_RSQUARE:
		case T_RBRACE:
			return 1;

		default:
			return 0;
	}
}

/**
 * determines an operator's associativity
 * JEP_RIGHT_ASSOC - right to left associativity
 * JEP_LEFT_ASSOC - left to right associativity
 */
static int jep_associativity(jep_ast_node* node)
{
	if(node == NULL)
	{
		return 0;
	}

	switch(node->token->token_code)
	{
		case T_EQUALS:
		case T_ADDASSIGN:
		case T_SUBASSIGN:
		case T_MULASSIGN:
		case T_DIVASSIGN:
		case T_LSHIFTASSIGN:
		case T_RSHIFTASSIGN:
		case T_NOT:
			return JEP_RIGHT_ASSOC;

		case T_MINUS:
			if(node->token->unary)
			{
				return JEP_RIGHT_ASSOC;
			}
			return JEP_LEFT_ASSOC;

		default:
			return JEP_LEFT_ASSOC;
	}
}

/**
 * determines the priority of an operator based on order of operations
 */
static int jep_priority(jep_ast_node* node)
{
	if(node == NULL)
	{
		return -1;
	}

	switch(node->token->token_code)
	{
		case T_COMMA:
			return 0;

		case T_RPAREN:
		case T_EQUALS:
		case T_ADDASSIGN:
		case T_SUBASSIGN:
		case T_MULASSIGN:
		case T_DIVASSIGN:
		case T_MODASSIGN:
		case T_LSHIFTASSIGN:
		case T_RSHIFTASSIGN:
		case T_SEMICOLON:
		case T_EOF:
			return 1;

		case T_LOGAND:
		case T_LOGOR:
			return 2;

		case T_BITAND:
		case T_BITOR:
		case T_BITXOR:
		case T_LSHIFT:
		case T_RSHIFT:
			return 3;

		case T_LESS:
		case T_GREATER:
		case T_LOREQUAL:
		case T_GOREQUAL:
		case T_EQUIVALENT:
		case T_NOTEQUIVALENT:
			return 4;

		case T_PLUS:
			return 5;

		case T_MINUS:
			if(node->token->unary)
			{
				return 8;
			}
			return 5;

		case T_FSLASH:
		case T_STAR:
		case T_MODULUS:
			return 6;

		case T_LPAREN:
		case T_LSQUARE:
			return 7;

		case T_NOT:
			return 8;

		case T_INCREMENT:
		case T_DECREMENT:
			return 9;

		default:
			return 0;
	}
}

/**
 * determine whether or not the current oeprator stack should be emptied
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

/**
 * checks if an operator is potentially unary
 */
static int jep_check_unary(jep_token* cur, jep_token* prev)
{
	switch(cur->token_code)
	{
		case T_INCREMENT:
		case T_DECREMENT:
		case T_NOT:
			cur->unary = 1;
			return 1;

		case T_MINUS:
			if(prev == NULL || (prev->type == T_SYMBOL 
				&& !prev->unary && prev->token_code != T_RPAREN))
			{
				cur->unary = 1;
				return 1;
			}
			return 0;

		case T_LPAREN:
			if(prev == NULL || prev->type == T_IDENTIFIER 
				|| prev->token_code == T_RPAREN 
				|| prev->token_code == T_RSQUARE)
			{
				cur->unary = 1;
				return 1;
			}
			return 0;

		default:
			return 0;
	}
}

/**
 * checks if an operator is a postfix operator
 * possible postfix operators are
 * ++ increment
 * -- decrement
 * () function call
 * [] array subscripting
 */
static int jep_check_postfix(jep_token* cur, jep_token* prev)
{
	if(cur == NULL || prev == NULL)
	{
		return 0;
	}

	int code = cur->token_code;

	/* ensure the current token can be a postfix operator */
	if(code != T_INCREMENT && code != T_DECREMENT
		&& code != T_LPAREN && code != T_LSQUARE)
	{
		return 0;
	}

	if(prev->type == T_IDENTIFIER || prev->token_code == T_RPAREN
		|| prev->token_code == T_RSQUARE)
	{
		cur->postfix = 1;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * parses an if statement
 */
static jep_ast_node* jep_if(jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* if_node; /* the node containing the if token */
	jep_ast_node* con;     /* the condition                    */
	jep_ast_node* body;    /* the body of the if statement     */

	if_node = (*nodes)++;
	if((*nodes)->token->token_code != T_LPAREN)
	{
		if(!root->error)
		{
			printf("expected } at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
		}
		root->error = 1;
		return NULL;
	}

	con = (*nodes)++;
	jep_ast_node* p = jep_expression(root, nodes);
	if(p != NULL)
	{
		jep_add_leaf_node(con, p);
	}
	else
	{
		if(!root->error)
		{
			printf("expected expression before ')'\n");
			root->error = 1;
		}
		return NULL;
	}
	if(!jep_accept(T_RPAREN, nodes))
	{
		if(!root->error)
		{
			printf("expected ')' at %d,%d but found '%s'\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		return NULL;
	}

 	if(con != NULL && !root->error)
	{
		jep_add_leaf_node(if_node, con);
	}

	if((*nodes)->token->token_code == T_LBRACE)
	{
		body = (*nodes)++;
		body->error = 0;
		jep_block(body, nodes);
		root->error = body->error;
		if(!jep_accept(T_RBRACE, nodes) && !root->error)
		{
			printf("expected } at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		else
		{
			jep_add_leaf_node(if_node, body);
		}
	}
	else
	{
		body = jep_statement(root, nodes);
		if(body != NULL && !root->error)
		{
			jep_add_leaf_node(if_node, body);
		}
	}

	return if_node;
}

/**
 * parses a while loop
 */
static jep_ast_node* jep_while(jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* wh_node; /* the node containing the while token */
	jep_ast_node* con;     /* the condition                       */
	jep_ast_node* body;    /* the body of the if statement        */

	wh_node = (*nodes)++;
	if((*nodes)->token->token_code != T_LPAREN)
	{
		if(!root->error)
		{
			printf("expected } at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
		}
		root->error = 1;
		return NULL;
	}

	con = (*nodes)++;
	jep_ast_node* p = jep_expression(root, nodes);
	if(p != NULL)
	{
		jep_add_leaf_node(con, p);
	}
	else
	{
		if(!root->error)
		{
			printf("expected expression before ')'\n");
			root->error = 1;
		}
		return NULL;
	}
	if(!jep_accept(T_RPAREN, nodes))
	{
		if(!root->error)
		{
			printf("expected ')' at %d,%d but found '%s'\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		return NULL;
	}

 	if(con != NULL && !root->error)
	{
		jep_add_leaf_node(wh_node, con);
	}

	if((*nodes)->token->token_code == T_LBRACE)
	{
		body = (*nodes)++;
		body->error = 0;
		jep_block(body, nodes);
		root->error = body->error;
		if(!jep_accept(T_RBRACE, nodes) && !root->error)
		{
			printf("expected } at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		else
		{
			jep_add_leaf_node(wh_node, body);
		}
	}
	else
	{
		body = jep_statement(root, nodes);
		if(body != NULL && !root->error)
		{
			jep_add_leaf_node(wh_node, body);
		}
	}

	return wh_node;
}

/**
 * parses a for loop
 */
static jep_ast_node* jep_for(jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* fo_node; /* the node containing the while token */
	jep_ast_node* head;    /* the index, condition, and change    */
	jep_ast_node* ind;     /* the index                           */
	jep_ast_node* con;     /* the condition                       */
	jep_ast_node* chg;     /* the index change                    */
	jep_ast_node* body;    /* the body of the if statement        */

	fo_node = (*nodes)++;
	if((*nodes)->token->token_code != T_LPAREN)
	{
		if(!root->error)
		{
			printf("expected } at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
		}
		root->error = 1;
		return NULL;
	}

	head = (*nodes)++;
	/* parse the index */
	ind = jep_expression(root, nodes);
	if(!jep_accept(T_SEMICOLON, nodes) || root->error)
	{
		if(!root->error)
		{
			printf("expected ';' at %d,%d but found '%s'\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		return NULL;
	}
	/* parse the condition */
	con = jep_expression(root, nodes);
	if(!jep_accept(T_SEMICOLON, nodes) || root->error)
	{
		if(!root->error)
		{
			printf("expected ';' at %d,%d but found '%s'\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		return NULL;
	}
	/* parse the index change */
	chg = jep_expression(root, nodes);
	if(!jep_accept(T_RPAREN, nodes) || root->error)
	{
		if(!root->error)
		{
			printf("expected ')' at %d,%d but found '%s'\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		return NULL;
	}

 	if(ind != NULL && !root->error)
	{
		jep_add_leaf_node(head, ind);
	}

 	if(con != NULL && !root->error)
	{
		jep_add_leaf_node(head, con);
	}

 	if(chg != NULL && !root->error)
	{
		jep_add_leaf_node(head, chg);
	}

	jep_add_leaf_node(fo_node, head);

	if((*nodes)->token->token_code == T_LBRACE)
	{
		body = (*nodes)++;
		body->error = 0;
		jep_block(body, nodes);
		root->error = body->error;
		if(!jep_accept(T_RBRACE, nodes) && !root->error)
		{
			printf("expected } at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		else
		{
			jep_add_leaf_node(fo_node, body);
		}
	}
	else
	{
		body = jep_statement(root, nodes);
		if(body != NULL && !root->error)
		{
			jep_add_leaf_node(fo_node, body);
		}
	}

	return fo_node;
}

/**
 * parses a function call
 */
static jep_ast_node* jep_function(jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* fn_node; /* the node containing the function token */
	jep_ast_node* ident;   /* the function identifier                */ 
	jep_ast_node* args;    /* the function arguments                 */
	jep_ast_node* body;    /* the function body                      */

	fn_node = (*nodes)++;

	if((*nodes)->token->type != T_IDENTIFIER)
	{
		if(!root->error)
		{
			printf("expected identifier at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
		}
		root->error = 1;
		return NULL;
	}

	ident = (*nodes)++;

	if((*nodes)->token->token_code != T_LPAREN)
	{
		if(!root->error)
		{
			printf("expected '(' at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
		}
		root->error = 1;
		return NULL;
	}

	jep_add_leaf_node(fn_node, ident);

	args = (*nodes)++;
	if(!jep_accept(T_RPAREN, nodes))
	{
		do
		{
			if((*nodes)->token->type != T_IDENTIFIER)
			{
				if(!root->error)
				{
					printf("expected identifier at %d,%d but found %s\n", 
						(*nodes)->token->row, 
						(*nodes)->token->column, 
						(*nodes)->token->value->buffer);
				}
				root->error = 1;
				return NULL;
			}
			jep_add_leaf_node(args, (*nodes)++);
		}while(jep_accept(T_COMMA, nodes));
		if(!jep_accept(T_RPAREN, nodes))
		{
			if(!root->error)
			{
				printf("expected ')' at %d,%d but found %s\n", 
					(*nodes)->token->row, 
					(*nodes)->token->column, 
					(*nodes)->token->value->buffer);
			}
			root->error = 1;
			return NULL;
		}
	}

	jep_add_leaf_node(fn_node, args);

	if((*nodes)->token->token_code != T_LBRACE)
	{
		if(!root->error)
		{
			printf("expected '{' at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
		}
		root->error = 1;
		return NULL;
	}

	body = (*nodes)++;
	body->error = 0;
	jep_block(body, nodes);
	root->error = body->error;
	if(!jep_accept(T_RBRACE, nodes) && !root->error)
	{
		printf("expected } at %d,%d but found %s\n", 
			(*nodes)->token->row, 
			(*nodes)->token->column, 
			(*nodes)->token->value->buffer);
		root->error = 1;
	}
	else
	{
		jep_add_leaf_node(fn_node, body);
	}

	return fn_node;
}

/**
 * advances the node pointer for a specfic token code
 */
static int jep_accept(int token_code, jep_ast_node** nodes)
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

/**
 * constructs an AST from a stream of tokens
 */
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
		if((*nodes)->token->token_code == T_LBRACE)
		{
			jep_ast_node* l_brace = (*nodes)++;
			l_brace->error = 0;
			jep_block(l_brace, nodes);
			root->error = l_brace->error;
			if(!jep_accept(T_RBRACE, nodes) && !root->error)
			{
				printf("expected } at %d,%d but found %s\n", 
					(*nodes)->token->row, 
					(*nodes)->token->column, 
					(*nodes)->token->value->buffer);
				root->error = 1;
			}
			else
			{
				jep_add_leaf_node(root, l_brace);
			}
		}
		else if((*nodes)->token->token_code != T_EOF)
		{
			jep_ast_node* stm = jep_statement(root, nodes);
			if(stm != NULL && !root->error)
			{
				jep_add_leaf_node(root, stm);
			}
		}
	}while((*nodes)->token->token_code != T_EOF && !root->error);

	(*nodes) = first;

	return root;

}

/**
 * attaches the operators on the stack to their appropriate operands until the
 * priority of the top of the operator stack is less than or equal to the
 * current operator on the stream
 */
static void jep_attach(jep_stack* exp, jep_stack* opr, jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* r; /* right operand     */
	jep_ast_node* l; /* left operand      */
	jep_ast_node* o; /* operator          */
	jep_token* cur;  /* the current token */

	cur = (*nodes)->token;

	do
	{
		o = jep_pop(opr);
		if(o != NULL && o->token->unary)
		{
			r = jep_pop(exp);
			if(o != NULL && r != NULL)
			{
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
		}
		else if(o != NULL)
		{
			r = jep_pop(exp);
			l = jep_pop(exp);
			if(o != NULL && r != NULL && l != NULL)
			{
				jep_add_leaf_node(o, l);
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
			else
			{
				printf("expected expression before '%s' \n", 
					cur->value->buffer);
				root->error = 1;
			}
		}
	}
	while(opr->top != NULL && jep_priority(opr->top) > jep_priority(*nodes));
}

/**
 * attaches all of the operators in the operator stack to their operands on the
 * expression stack
 */
static void jep_attach_all(jep_stack* exp, jep_stack* opr, jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* r; /* right operand     */
	jep_ast_node* l; /* left operand      */
	jep_ast_node* o; /* operator          */
	jep_token* cur;  /* the current token */

	cur = (*nodes)->token;

	do
	{
		o = jep_pop(opr);
		if(o != NULL && o->token->unary)
		{
			r = jep_pop(exp);
			if(o != NULL && r != NULL)
			{
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
		}
		else if(o != NULL)
		{
			r = jep_pop(exp);
			l = jep_pop(exp);
			if(o != NULL && r != NULL && l != NULL)
			{
				jep_add_leaf_node(o, l);
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
			else
			{
				printf("expected expression before '%s' \n", 
					cur->value->buffer);
				root->error = 1;
			}
		}
	}
	while(opr->top != NULL);
}

/**
 * parses an expression
 */
static jep_ast_node* jep_expression(jep_ast_node* root, jep_ast_node** nodes)
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

	/* check for an empty expression */
	if(jep_eoe(cur) && cur->token_code)
	{
		/* free the memory allocated for the stacks */
		free(exp.nodes);
		free(opr.nodes);
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

				/* check if the symbol is a postfix operator */
				jep_check_postfix(cur, prev);

				/* check for parentheses */
				if(cur->token_code == T_LPAREN && next->token_code != T_EOF)
				{
					jep_ast_node* l_paren = (*nodes)++;
					jep_ast_node* par = jep_expression(root, nodes);
					prev = (*nodes - 1)->token;
					cur = (*nodes)->token;
					if(cur->token_code != T_EOF)
					{
						next = (*nodes + 1)->token;
					}
					if(par != NULL)
					{
						jep_add_leaf_node(l_paren, par);
					}
					else
					{
						/* a postfix () might have no contents */
						if(!l_paren->token->postfix)
						{
							if(!root->error)
							{
								printf("expected expression before ')'\n");
								root->error = 1;
							}
							free(exp.nodes);
							free(opr.nodes);
							return NULL;
						}
					}
					if((*nodes)->token->token_code != T_RPAREN)
					{
						free(exp.nodes);
						free(opr.nodes);
						printf("expected ')' at %d,%d but found '%s'\n", 
							(*nodes)->token->row, 
							(*nodes)->token->column, 
							(*nodes)->token->value->buffer);
						root->error = 1;
						return NULL;
					}
					if(l_paren->token->postfix)
					{
						if(jep_prioritize(l_paren, opr.top))
						{
							jep_attach(&exp, &opr, root, nodes);
						}
						jep_push(&opr, l_paren);
					}
					else
					{
						jep_push(&exp, l_paren);
					}
				}
				else if(cur->token_code == T_LSQUARE && next->token_code != T_EOF)
				{
					jep_ast_node* l_square = (*nodes)++;
					jep_ast_node* sqr = jep_expression(root, nodes);
					prev = (*nodes - 1)->token;
					cur = (*nodes)->token;
					if(cur->token_code != T_EOF)
					{
						next = (*nodes + 1)->token;
					}
					if(sqr != NULL)
					{
						jep_add_leaf_node(l_square, sqr);
					}
					else
					{
						if(!root->error)
						{
							printf("expected expression before ']'\n");
							root->error = 1;
						}
						free(exp.nodes);
						free(opr.nodes);
						return NULL;
					}
					if((*nodes)->token->token_code != T_RSQUARE)
					{
						free(exp.nodes);
						free(opr.nodes);
						printf("expected ']' at %d,%d but found '%s'\n", 
							(*nodes)->token->row, 
							(*nodes)->token->column, 
							(*nodes)->token->value->buffer);
						root->error = 1;
						return NULL;
					}
					if(jep_prioritize(l_square, opr.top))
					{
						jep_attach(&exp, &opr, root, nodes);
					}
					jep_push(&opr, l_square);
				}
				else if(cur->token_code == T_LBRACE && next->token_code != T_EOF)
				{
					jep_ast_node* l_brace = (*nodes)++;
					jep_ast_node* brc = jep_expression(root, nodes);
					prev = (*nodes - 1)->token;
					cur = (*nodes)->token;
					if(cur->token_code != T_EOF)
					{
						next = (*nodes + 1)->token;
					}
					if(brc != NULL)
					{
						jep_add_leaf_node(l_brace, brc);
					}
					if((*nodes)->token->token_code != T_RBRACE)
					{
						free(exp.nodes);
						free(opr.nodes);
						printf("expected '}' at %d,%d but found '%s'\n", 
							(*nodes)->token->row, 
							(*nodes)->token->column, 
							(*nodes)->token->value->buffer);
						root->error = 1;
						return NULL;
					}
					jep_push(&exp, l_brace);
				}
				else if(jep_prioritize(*nodes, opr.top))
				{
					jep_attach(&exp, &opr, root, nodes);
					jep_push(&opr, *nodes);
				}
				else
				{
					jep_push(&opr, *nodes);	
				}
				
			}
				break;

			default:
				printf("unexpected idon't even know token '%s' at %d,%d\n", 
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
		jep_attach_all(&exp, &opr, root, nodes);
	}
	else
	{
		printf("invalid end of statement\n");
	}

	if(exp.size > 1)
	{
		printf("unexpected exp token '%s' at %d,%d\n",
			exp.top->token->value->buffer,
			exp.top->token->row, 
			exp.top->token->column);
		root->error = 1;
		ast = NULL;
	}
	else if(opr.size > 0)
	{
		printf("unexpected opr token '%s' at %d,%d\n",
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

/**
 * parses a statement
 */
static jep_ast_node* jep_statement(jep_ast_node* root, jep_ast_node** nodes)
{
	jep_ast_node* statement = NULL;
	
	if((*nodes)->token->token_code == T_IF)
	{
		statement = jep_if(root, nodes);
	}
	else if((*nodes)->token->token_code == T_WHILE)
	{
		statement = jep_while(root, nodes);
	}
	else if((*nodes)->token->token_code == T_FOR)
	{
		statement = jep_for(root, nodes);
	}
	else if((*nodes)->token->token_code == T_FUNCTION)
	{
		statement = jep_function(root, nodes);
	}
	else if((*nodes)->token->token_code == T_RETURN)
	{
		statement = (*nodes)++;
		jep_ast_node* ret_val = jep_expression(root, nodes);
		if(!jep_accept(T_SEMICOLON, nodes) && !root->error)
		{
			printf("expected ; at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
		else
		{
			jep_add_leaf_node(statement, ret_val);
		}
	}
	else
	{
		statement = jep_expression(root, nodes);
		if(!jep_accept(T_SEMICOLON, nodes) && !root->error)
		{
			printf("expected ; at %d,%d but found %s\n", 
				(*nodes)->token->row, 
				(*nodes)->token->column, 
				(*nodes)->token->value->buffer);
			root->error = 1;
		}
	}

	return statement;
}

/**
 * parses a block of code
 */
static void jep_block(jep_ast_node* root, jep_ast_node** nodes)
{
	while((*nodes)->token->token_code != T_RBRACE && !root->error)
	{
		if((*nodes)->token->token_code == T_LBRACE)
		{
			jep_ast_node* l_brace = (*nodes)++;
			l_brace->error = 0;
			jep_block(l_brace, nodes);
			root->error = l_brace->error;
			if(!jep_accept(T_RBRACE, nodes) && !root->error)
			{
				printf("expected } at %d,%d but found %s\n", 
					(*nodes)->token->row, 
					(*nodes)->token->column, 
					(*nodes)->token->value->buffer);
				root->error = 1;
			}
			else
			{
				jep_add_leaf_node(root, l_brace);
			}
		}
		else
		{
			jep_ast_node* stm = jep_statement(root, nodes);
			if(stm != NULL && !root->error)
			{
				jep_add_leaf_node(root, stm);
			}
		}
	}
}
