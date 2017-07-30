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
#include "parser.h"

static int jep_accept(int, jep_ast_node **);
static jep_ast_node *jep_expression(jep_ast_node *, jep_ast_node **);
static jep_ast_node *jep_statement(jep_ast_node *, jep_ast_node **);
static void jep_block(jep_ast_node *, jep_ast_node **);

/**
 * sets the error flag for the root of an AST.
 * err - the type of error
 * tok - the token in the stream at which the error was detected
 * root - the root of the AST
 * val - the expected value
 */
static void jep_err(int err, jep_token tok, jep_ast_node *root, char *val)
{
	if (root->error)
	{
		return;
	}

	root->error = 1;

	if (err == ERR_EXPRESSION)
	{
		printf("expected expression before '%s' at %d,%d\n",
			   val, tok.row, tok.column);
	}
	else if (err == ERR_IDENTIFIER)
	{
		printf("expected identifier at %s %d,%d but found %s\n",
			   tok.file, tok.row, tok.column, tok.val->buffer);
	}
	else if (err == ERR_UNEXPECTED)
	{
		printf("unexpected token '%s' at %s %d,%d\n",
			   tok.val->buffer, tok.file, tok.row, tok.column);
	}
	else if (err == ERR_EXPECTED)
	{
		printf("expected '%s' at %s %d,%d but found '%s'\n",
			   val, tok.file, tok.row, tok.column, tok.val->buffer);
	}
	else if (err == ERR_UNIQUE)
	{
		printf("expected unique argument at %d,%d but found '%s'\n",
			   tok.row, tok.column, tok.val->buffer);
	}
}

/**
 * checks if a token is an 'end of expression' token
 */
static int jep_eoe(jep_token token)
{
	switch (token.token_code)
	{
	case T_EOF:
	case T_SEMICOLON:
	case T_RPAREN:
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
static int jep_associativity(jep_ast_node *node)
{
	if (node == NULL)
	{
		return 0;
	}

	switch (node->token.token_code)
	{
	case T_EQUALS:
	case T_ADDASSIGN:
	case T_SUBASSIGN:
	case T_MULASSIGN:
	case T_DIVASSIGN:
	case T_MODASSIGN:
	case T_LSHIFTASSIGN:
	case T_RSHIFTASSIGN:
	case T_NOT:
	case T_COLON:
	case T_DOUBLECOLON:
	case T_NEW:
		return JEP_RIGHT_ASSOC;

	case T_MINUS:
		if (node->token.unary)
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
static int jep_priority(jep_ast_node *node)
{
	if (node == NULL)
	{
		return -1;
	}

	switch (node->token.token_code)
	{
	case T_COMMA:
		return 0;

	case T_SEMICOLON:
	case T_EOF:
	case T_RPAREN:
	case T_EQUALS:
	case T_ADDASSIGN:
	case T_SUBASSIGN:
	case T_MULASSIGN:
	case T_DIVASSIGN:
	case T_MODASSIGN:
	case T_LSHIFTASSIGN:
	case T_RSHIFTASSIGN:
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
		if (node->token.unary)
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
	case T_PERIOD:
		return 7;

	case T_NOT:
	case T_COLON:
	case T_DOUBLECOLON:
	case T_NEW:
		return 8;

	/* this used to be 9, but it might need to stay as 8 */
	case T_INCREMENT:
	case T_DECREMENT:
		return 8;

	default:
		return 0;
	}
}

/**
 * determine whether or not the current operator stack should be emptied
 */
static int jep_prioritize(jep_ast_node *cur, jep_ast_node *top)
{
	if (jep_associativity(cur) == JEP_LEFT_ASSOC)
	{
		if (jep_priority(cur) <= jep_priority(top))
		{
			if (cur->token.postfix)
			{
				if ((top->token.postfix && !top->token.unary) || (!top->token.postfix && top->token.unary))
				{
					return 0;
				}
				return 1;
			}
			return 1;
		}
		return 0;
	}
	else if (jep_associativity(cur) == JEP_RIGHT_ASSOC)
	{
		if (jep_priority(cur) < jep_priority(top))
		{
			if (cur->token.postfix && top->token.postfix)
			{
				return 0;
			}
			return 1;
		}
		return 0;
	}

	return 0;
}

/**
 * checks if an operator is potentially unary
 */
static int jep_check_unary(jep_token *cur, jep_token *prev)
{
	switch (cur->token_code)
	{
	case T_INCREMENT:
	case T_DECREMENT:
	case T_NOT:
	case T_LSQUARE:
	case T_COLON:
	case T_DOUBLECOLON:
	case T_NEW:
		cur->unary = 1;
		return 1;

	case T_MINUS:
		if (prev == NULL || (prev->type == T_SYMBOL && prev->token_code != T_RPAREN && prev->token_code != T_RSQUARE))
		{
			cur->unary = 1;
			return 1;
		}
		return 0;

	case T_LPAREN:
		if (prev == NULL || prev->type == T_IDENTIFIER || prev->token_code == T_RPAREN || prev->token_code == T_RSQUARE)
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
static int jep_check_postfix(jep_token *cur, jep_token *prev)
{
	if (cur == NULL || prev == NULL)
	{
		return 0;
	}

	int code = cur->token_code;

	/* ensure the current token can be a postfix operator */
	if (code != T_INCREMENT && code != T_DECREMENT && code != T_LPAREN && code != T_LSQUARE)
	{
		return 0;
	}

	if (prev->type == T_IDENTIFIER || prev->token_code == T_RPAREN || prev->token_code == T_RSQUARE)
	{
		cur->postfix = 1;
		return 1;
	}
	return 0;
}

/**
 * parses a struct definition
 */
static jep_ast_node *jep_struct(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *struct_node;
	jep_ast_node *ident;
	jep_ast_node *members;

	struct_node = (*nodes)++;

	if ((*nodes)->token.type != T_IDENTIFIER)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, "identifier");
		return NULL;
	}

	ident = (*nodes)++;

	jep_add_leaf_node(struct_node, ident);

	members = (*nodes)++;

	if (members->token.token_code != T_LBRACE)
	{
		jep_err(ERR_IDENTIFIER, (*nodes)->token, root, NULL);
		return NULL;
	}

	if (!jep_accept(T_RBRACE, nodes))
	{
		do
		{
			if ((*nodes)->token.type != T_IDENTIFIER)
			{
				jep_err(ERR_IDENTIFIER, (*nodes)->token, root, NULL);
				return NULL;
			}
			jep_ast_node *mem = (*nodes)++;
			jep_add_leaf_node(members, mem);
		} while (jep_accept(T_SEMICOLON, nodes) && (*nodes)->token.token_code != T_RBRACE);

		if (!jep_accept(T_RBRACE, nodes))
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
			return NULL;
		}
	}

	jep_add_leaf_node(struct_node, members);

	return struct_node;
}

/**
 * parses an if statement
 */
static jep_ast_node *jep_if(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *if_node; /* the node containing the if token */
	jep_ast_node *con;	 /* the condition                    */
	jep_ast_node *body;	/* the body of the if statement     */

	if_node = (*nodes)++;
	if (if_node->token.token_code == T_IF && (*nodes)->token.token_code != T_LPAREN)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, "(");
		return NULL;
	}

	if (if_node->token.token_code == T_IF)
	{
		con = (*nodes)++;
		jep_ast_node *p = jep_expression(root, nodes);
		if (!jep_accept(T_RPAREN, nodes))
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, ")");
			return NULL;
		}

		if (p == NULL)
		{
			jep_err(ERR_EXPRESSION, (*nodes)->token, root, ")");
			return NULL;
		}
		else
		{
			jep_add_leaf_node(con, p);
		}

		if (con != NULL && !root->error)
		{
			jep_add_leaf_node(if_node, con);
		}
	}

	if ((*nodes)->token.token_code == T_LBRACE)
	{
		body = (*nodes)++;
		body->error = 0;
		jep_block(body, nodes);
		root->error = body->error;
		if (!jep_accept(T_RBRACE, nodes) && !root->error)
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
		}
		else
		{
			jep_add_leaf_node(if_node, body);
		}
	}
	else
	{
		body = jep_statement(root, nodes);
		if (body != NULL && !root->error)
		{
			jep_add_leaf_node(if_node, body);
		}
	}

	/* parse else blocks */
	if ((*nodes)->token.token_code == T_ELSE && !root->error)
	{
		if ((*nodes + 1)->token.token_code == T_IF)
		{
			(*nodes)++;
		}
		jep_ast_node *els = jep_if(root, nodes);
		if (els != NULL)
		{
			jep_add_leaf_node(if_node, els);
		}
	}

	return if_node;
}

/**
 * parses a while loop
 */
static jep_ast_node *jep_while(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *wh_node; /* the node containing the while token */
	jep_ast_node *con;	 /* the condition                       */
	jep_ast_node *body;	/* the body of the if statement        */

	wh_node = (*nodes)++;
	if ((*nodes)->token.token_code != T_LPAREN)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
		return NULL;
	}

	con = (*nodes)++;
	jep_ast_node *p = jep_expression(root, nodes);

	if (!jep_accept(T_RPAREN, nodes))
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, ")");
		return NULL;
	}

	if (p == NULL)
	{
		jep_err(ERR_EXPRESSION, (*nodes)->token, root, ")");
		return NULL;
	}
	else
	{
		jep_add_leaf_node(con, p);
	}

	if (con != NULL && !root->error)
	{
		jep_add_leaf_node(wh_node, con);
	}

	if ((*nodes)->token.token_code == T_LBRACE)
	{
		body = (*nodes)++;
		body->error = 0;
		jep_block(body, nodes);
		root->error = body->error;
		if (!jep_accept(T_RBRACE, nodes) && !root->error)
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
		}
		else
		{
			jep_add_leaf_node(wh_node, body);
		}
	}
	else
	{
		body = jep_statement(root, nodes);
		if (body != NULL && !root->error)
		{
			jep_add_leaf_node(wh_node, body);
		}
	}

	return wh_node;
}

/**
 * parses a for loop
 */
static jep_ast_node *jep_for(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *fo_node; /* the node containing the while token */
	jep_ast_node *head;	/* the index, condition, and change    */
	jep_ast_node *ind;	 /* the index                           */
	jep_ast_node *con;	 /* the condition                       */
	jep_ast_node *chg;	 /* the index change                    */
	jep_ast_node *body;	/* the body of the if statement        */

	fo_node = (*nodes)++;
	if ((*nodes)->token.token_code != T_LPAREN)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
		return NULL;
	}

	head = (*nodes)++;

	/* parse the index */
	ind = jep_expression(root, nodes);
	if (!jep_accept(T_SEMICOLON, nodes) || root->error)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, ";");
		return NULL;
	}

	/* parse the condition */
	con = jep_expression(root, nodes);
	if (!jep_accept(T_SEMICOLON, nodes) || root->error)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, ";");
		return NULL;
	}

	/* parse the index change */
	chg = jep_expression(root, nodes);
	if (!jep_accept(T_RPAREN, nodes) || root->error)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, ")");
		return NULL;
	}

	if (ind != NULL && !root->error)
	{
		jep_add_leaf_node(head, ind);
		fo_node->loop |= JEP_INDEX;
	}

	if (con != NULL && !root->error)
	{
		jep_add_leaf_node(head, con);
		fo_node->loop |= JEP_CONDITION;
	}

	if (chg != NULL && !root->error)
	{
		jep_add_leaf_node(head, chg);
		fo_node->loop |= JEP_CHANGE;
	}

	jep_add_leaf_node(fo_node, head);

	if ((*nodes)->token.token_code == T_LBRACE)
	{
		body = (*nodes)++;
		body->error = 0;
		jep_block(body, nodes);
		root->error = body->error;
		if (!jep_accept(T_RBRACE, nodes) && !root->error)
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
		}
		else
		{
			jep_add_leaf_node(fo_node, body);
		}
	}
	else
	{
		body = jep_statement(root, nodes);
		if (body != NULL && !root->error)
		{
			jep_add_leaf_node(fo_node, body);
		}
	}

	return fo_node;
}

/**
 * parses a function definition
 */
static jep_ast_node *jep_function(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *fn_node; /* the node containing the function token */
	jep_ast_node *ident;   /* the function identifier                */
	jep_ast_node *args;	/* the function arguments                 */
	jep_ast_node *body;	/* the function body                      */
	char **arg_names;	  /* argument identifier buffer             */
	int cap = 10;		   /* capacity of argument identifier buffer */
	int size = 0;		   /* size of argument identifier buffer     */

	fn_node = (*nodes)++;

	if ((*nodes)->token.type != T_IDENTIFIER)
	{
		jep_err(ERR_IDENTIFIER, (*nodes)->token, root, NULL);
		return NULL;
	}

	ident = (*nodes)++;

	if ((*nodes)->token.token_code != T_LPAREN)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, "(");
		return NULL;
	}

	jep_add_leaf_node(fn_node, ident);

	/* aprse the function arguments */
	args = (*nodes)++;
	if (!jep_accept(T_RPAREN, nodes))
	{
		arg_names = malloc(sizeof(char *) * cap);
		do
		{
			if ((*nodes)->token.type != T_IDENTIFIER)
			{
				jep_err(ERR_IDENTIFIER, (*nodes)->token, root, NULL);
				return NULL;
			}
			char *arg_ident = (*nodes)->token.val->buffer;
			int i;
			for (i = 0; i < size; i++)
			{
				if (!strcmp(arg_ident, arg_names[i]))
				{
					jep_err(ERR_UNIQUE, (*nodes)->token, root, ")");
					return NULL;
				}
			}
			if (size >= cap)
			{
				cap = cap + cap / 2;
				arg_names = realloc(arg_names, sizeof(char *) * cap);
			}
			arg_names[size++] = arg_ident;
			jep_add_leaf_node(args, (*nodes)++);
		} while (jep_accept(T_COMMA, nodes));
		if (!jep_accept(T_RPAREN, nodes))
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, ")");
			return NULL;
		}
	}

	jep_add_leaf_node(fn_node, args);

	if ((*nodes)->token.token_code != T_LBRACE)
	{
		/* native function definition */
		return fn_node;
	}

	/* parse the function body */
	body = (*nodes)++;
	body->error = 0;
	jep_block(body, nodes);
	root->error = body->error;
	if (!jep_accept(T_RBRACE, nodes) && !root->error)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
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
static int jep_accept(int token_code, jep_ast_node **nodes)
{
	if ((*nodes)->token.token_code == token_code)
	{
		(*nodes)++;
		return 1;
	}
	return 0;
}

/**
 * constructs an AST from a stream of tokens
 */
void jep_parse(jep_token_stream *ts, jep_ast_node *root)
{
	jep_ast_node *nodes; /* the nodes of the AST      */
	jep_ast_node *first; /* the first node of the AST */
	int i;				 /* index variable            */

	nodes = malloc(sizeof(jep_ast_node) * ts->size);

	/* create an AST node for each token */
	for (i = 0; i < ts->size; i++)
	{
		nodes[i].leaf_count = 0;
		nodes[i].cap = 10;
		nodes[i].leaves = NULL;
		nodes[i].token = ts->tok[i];
		nodes[i].error = 0;
		nodes[i].array = 0;
		nodes[i].loop = 0;
		nodes[i].mod = 0;
	}

	first = nodes;

	do
	{
		if (nodes->token.token_code == T_IMPORT)
		{
			/* imports are handled during tokenization */
			nodes++;
			if (nodes->token.type != T_STRING)
			{
				jep_err(ERR_EXPECTED, nodes->token, root, "string");
			}
		}
		else if (nodes->token.token_code == T_LBRACE)
		{
			jep_ast_node *l_brace = nodes++;
			l_brace->error = 0;
			jep_block(l_brace, &nodes);
			root->error = l_brace->error;
			if (!jep_accept(T_RBRACE, &nodes) && !root->error)
			{
				jep_err(ERR_EXPECTED, nodes->token, root, "}");
			}
			else
			{
				jep_add_leaf_node(root, l_brace);
			}
		}
		else if (nodes->token.token_code != T_EOF)
		{
			jep_ast_node *stm = jep_statement(root, &nodes);
			if (stm != NULL && !root->error)
			{
				jep_add_leaf_node(root, stm);
			}
		}
	} while (nodes->token.token_code != T_EOF && !root->error);

	/* position the pointer back at the beginning of the memory chunk */
	nodes = first;
	free(nodes);

	return;
}

/**
 * attaches the operators on the stack to their appropriate operands until the
 * priority of the top of the operator stack is less than or equal to the
 * current operator on the stream
 */
static void jep_attach(jep_stack *exp, jep_stack *opr, jep_ast_node *root, jep_ast_node *nodes)
{
	// printf("attaching some operands to operators\n");
	jep_ast_node *r; /* right operand     */
	jep_ast_node *l; /* left operand      */
	jep_ast_node *o; /* operator          */
	jep_token *cur;  /* the current token */

	cur = &(nodes->token);

	do
	{
		o = jep_pop(opr);

		if (o != NULL && o->token.unary)
		{
			// printf(" operator: %s", o->token.val->buffer);
			r = jep_pop(exp);
			if (r != NULL)
			{
				// printf(" operand: %s\n", r->token.val->buffer);
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
		}
		else if (o != NULL)
		{
			// printf(" operator: %s", o->token.val->buffer);
			r = jep_pop(exp);
			l = jep_pop(exp);
			if (r != NULL && l != NULL)
			{
				// printf(" right operand: %s", r->token.val->buffer);
				// printf(" left operand: %s\n", l->token.val->buffer);
				jep_add_leaf_node(o, l);
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
			else
			{
				jep_err(ERR_EXPRESSION, *cur, root, cur->val->buffer);
			}
		}
	} while (opr->size && jep_priority(opr->top) > jep_priority(nodes));
}

/**
 * attaches all of the operators in the operator stack to their operands on the
 * expression stack
 */
static void jep_attach_all(jep_stack *exp, jep_stack *opr, jep_ast_node *root, jep_ast_node **nodes)
{
	// printf("attaching ALL operands to operators\n");
	jep_ast_node *r; /* right operand     */
	jep_ast_node *l; /* left operand      */
	jep_ast_node *o; /* operator          */
	jep_token *cur;  /* the current token */

	cur = &(*nodes)->token;

	do
	{
		o = jep_pop(opr);

		if (o != NULL && o->token.unary)
		{
			// printf(" operator: %s", o->token.val->buffer);
			r = jep_pop(exp);
			if (r != NULL)
			{
				// printf(" right operand: %s\n", r->token.val->buffer);
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
		}
		else if (o != NULL)
		{
			// printf(" operator: %s", o->token.val->buffer);
			r = jep_pop(exp);
			l = jep_pop(exp);
			if (r != NULL && l != NULL)
			{
				// printf(" right operand: %s", r->token.val->buffer);
				// printf(" left operand: %s\n", l->token.val->buffer);
				jep_add_leaf_node(o, l);
				jep_add_leaf_node(o, r);
				jep_push(exp, o);
			}
			else
			{
				jep_err(ERR_EXPRESSION, *cur, root, cur->val->buffer);
			}
		}
	} while (opr->size);
}

/**
 * parses an expression
 */
static jep_ast_node *jep_expression(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_stack exp;	 /* expresion stack   */
	jep_stack opr;	 /* operator stack    */
	jep_token *prev;   /* previous token    */
	jep_token *cur;	/* current token     */
	jep_token *next;   /* next token        */
	jep_ast_node *ast; /* the resulting AST */

	exp.size = 0;
	exp.cap = 10;
	exp.top = NULL;
	exp.nodes = malloc(sizeof(jep_ast_node *) * 10);

	opr.size = 0;
	opr.cap = 10;
	opr.top = NULL;
	opr.nodes = malloc(sizeof(jep_ast_node *) * 10);

	prev = NULL;
	cur = &(*nodes)->token;
	next = &(*nodes + 1)->token;

	/* check for an empty expression */
	if (jep_eoe(*cur) && cur->token_code)
	{
		/* free the memory allocated for the stacks */
		free(exp.nodes);
		free(opr.nodes);
		return NULL;
	}

	while (!jep_eoe(*cur))
	{
		switch ((*nodes)->token.type)
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
			if (cur->token_code == T_LPAREN && next->token_code != T_EOF)
			{
				jep_ast_node *l_brac = (*nodes)++;
				jep_ast_node *e = jep_expression(root, nodes);
				prev = &(*nodes - 1)->token;
				cur = &(*nodes)->token;
				if (cur->token_code != T_EOF)
				{
					next = &(*nodes + 1)->token;
				}

				if ((*nodes)->token.token_code != T_RPAREN)
				{
					jep_err(ERR_EXPECTED, (*nodes)->token, root, ")");
					free(exp.nodes);
					free(opr.nodes);
					return NULL;
				}

				if (e != NULL)
				{
					jep_add_leaf_node(l_brac, e);
				}
				else
				{
					/* a postfix () might have no contents */
					if (!l_brac->token.postfix)
					{
						jep_err(ERR_EXPRESSION, *cur, root, ")");
						free(exp.nodes);
						free(opr.nodes);
						return NULL;
					}
				}

				if (l_brac->token.postfix)
				{
					if (jep_prioritize(l_brac, opr.top))
					{
						jep_attach(&exp, &opr, root, l_brac);
						jep_push(&opr, l_brac);
					}
					else
					{
						jep_ast_node *e = jep_pop(&exp);
						if (e != NULL)
						{
							jep_add_leaf_node(l_brac, e);
							jep_push(&exp, l_brac);
						}
					}
				}
				else
				{
					jep_push(&exp, l_brac);
				}
			}
			else if (cur->token_code == T_LSQUARE && next->token_code != T_EOF)
			{
				jep_ast_node *l_brac = (*nodes)++;
				jep_ast_node *e = jep_expression(root, nodes);
				prev = &(*nodes - 1)->token;
				cur = &(*nodes)->token;
				if (cur->token_code != T_EOF)
				{
					next = &(*nodes + 1)->token;
				}

				if ((*nodes)->token.token_code != T_RSQUARE)
				{
					jep_err(ERR_EXPECTED, *cur, root, "]");
					free(exp.nodes);
					free(opr.nodes);
					return NULL;
				}

				if (e != NULL)
				{
					jep_add_leaf_node(l_brac, e);
				}
				else
				{
					jep_err(ERR_EXPRESSION, *cur, root, "]");
					free(exp.nodes);
					free(opr.nodes);
					return NULL;
				}

				if (l_brac->token.postfix)
				{
					if (jep_prioritize(l_brac, opr.top))
					{
						jep_attach(&exp, &opr, root, l_brac);
						jep_push(&opr, l_brac);
					}
					else
					{
						jep_ast_node *e = jep_pop(&exp);
						if (e != NULL)
						{
							jep_add_leaf_node(l_brac, e);
							jep_push(&exp, l_brac);
						}
					}
				}
				else
				{
					jep_push(&exp, l_brac);
				}
			}
			else if (cur->token_code == T_LBRACE && next->token_code != T_EOF)
			{
				jep_ast_node *l_brac = (*nodes)++;
				l_brac->array = 1;
				jep_ast_node *e = jep_expression(root, nodes);
				prev = &(*nodes - 1)->token;
				cur = &(*nodes)->token;
				if (cur->token_code != T_EOF)
				{
					next = &(*nodes + 1)->token;
				}

				if ((*nodes)->token.token_code != T_RBRACE)
				{
					jep_err(ERR_EXPECTED, *cur, root, "}");
					free(exp.nodes);
					free(opr.nodes);
					return NULL;
				}

				if (e != NULL)
				{
					jep_add_leaf_node(l_brac, e);
				}
				jep_push(&exp, l_brac);
			}
			else if (jep_prioritize(*nodes, opr.top))
			{
				jep_attach(&exp, &opr, root, *nodes);
				jep_push(&opr, *nodes);
			}
			else
			{
				jep_push(&opr, *nodes);
			}
		}
		break;

		case T_KEYWORD:
		{
			if ((*nodes)->token.token_code != T_NULL)
			{
				jep_err(ERR_UNEXPECTED, *cur, root, NULL);
				free(exp.nodes);
				free(opr.nodes);
				return NULL;
			}
			else
			{
				jep_push(&exp, *nodes);
			}
		}
		break;

		default:
			jep_err(ERR_UNEXPECTED, *cur, root, NULL);
			free(exp.nodes);
			free(opr.nodes);
			return NULL;
		}

		(*nodes)++;

		prev = cur;
		cur = next;
		if (cur->token_code != T_EOF)
		{
			next = &(*nodes + 1)->token;
		}
	}

	if (jep_eoe(*cur))
	{
		jep_attach_all(&exp, &opr, root, nodes);
	}
	else
	{
		printf("invalid end of statement\n");
	}

	if (exp.size > 1)
	{
		jep_err(ERR_UNEXPECTED, exp.top->token, root, NULL);
		ast = NULL;
	}
	else if (opr.size > 0)
	{
		jep_err(ERR_UNEXPECTED, exp.top->token, root, NULL);
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
 * parses a sequence of modifiers followed by an expression.
 * where modn is the nth modifier in the chain, the following ast is produced
 * mod0
 *   modn
 *   exp
 */
static jep_ast_node *jep_modifier(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *mod_node; /* the initial modifier                    */
	jep_ast_node *exp;		/* the modified expression                 */
	int mod;				/* bit flags to keep track of modification */

	mod_node = (*nodes)++;
	mod = 0;

	if (mod_node->token.token_code == T_LOCAL)
	{
		mod |= 1;
	}
	else if (mod_node->token.token_code == T_CONST)
	{
		mod |= 2;
	}

	while ((*nodes)->token.type == T_MODIFIER && !root->error)
	{
		jep_ast_node *next = (*nodes)++;
		if (next->token.token_code == T_LOCAL)
		{
			if (mod & 1)
			{
				printf("duplicate modifier %s detected at %d,%d\n",
					   next->token.val->buffer,
					   next->token.row,
					   next->token.column);
				root->error = 1;
				return NULL;
			}
			else
			{
				mod |= 1;
			}
		}
		else if (next->token.token_code == T_CONST)
		{
			if (mod & 2)
			{
				printf("duplicate modifier %s detected at %d,%d\n",
					   next->token.val->buffer,
					   next->token.row,
					   next->token.column);
				root->error = 1;
				return NULL;
			}
			else
			{
				mod |= 2;
			}
		}
		jep_add_leaf_node(mod_node, next);
	}

	exp = jep_expression(root, nodes);
	if (!jep_accept(T_SEMICOLON, nodes) || root->error)
	{
		jep_err(ERR_EXPECTED, (*nodes)->token, root, ";");
		return NULL;
	}

	if (exp != NULL)
	{
		jep_add_leaf_node(mod_node, exp);
	}

	mod_node->mod = mod;

	return mod_node;
}

/**
 * parses a statement
 */
static jep_ast_node *jep_statement(jep_ast_node *root, jep_ast_node **nodes)
{
	jep_ast_node *statement = NULL;

	if ((*nodes)->token.token_code == T_STRUCT)
	{
		statement = jep_struct(root, nodes);
	}
	else if ((*nodes)->token.token_code == T_IF)
	{
		statement = jep_if(root, nodes);
	}
	else if ((*nodes)->token.token_code == T_WHILE)
	{
		statement = jep_while(root, nodes);
	}
	else if ((*nodes)->token.token_code == T_FOR)
	{
		statement = jep_for(root, nodes);
	}
	else if ((*nodes)->token.token_code == T_FUNCTION)
	{
		statement = jep_function(root, nodes);
	}
	else if ((*nodes)->token.token_code == T_RETURN)
	{
		statement = (*nodes)++;
		jep_ast_node *ret_val = jep_expression(root, nodes);
		if (!jep_accept(T_SEMICOLON, nodes) && !root->error)
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, ";");
		}
		else
		{
			jep_add_leaf_node(statement, ret_val);
		}
	}
	else if ((*nodes)->token.type == T_MODIFIER)
	{
		statement = jep_modifier(root, nodes);
	}
	else
	{
		statement = jep_expression(root, nodes);
		if (!jep_accept(T_SEMICOLON, nodes) && !root->error)
		{
			jep_err(ERR_EXPECTED, (*nodes)->token, root, ";");
		}
	}

	return statement;
}

/**
 * parses a block of code
 */
static void jep_block(jep_ast_node *root, jep_ast_node **nodes)
{
	while ((*nodes)->token.token_code != T_RBRACE && !root->error)
	{
		if ((*nodes)->token.token_code == T_LBRACE)
		{
			jep_ast_node *l_brace = (*nodes)++;
			l_brace->error = 0;
			jep_block(l_brace, nodes);
			root->error = l_brace->error;
			if (!jep_accept(T_RBRACE, nodes) && !root->error)
			{
				jep_err(ERR_EXPECTED, (*nodes)->token, root, "}");
			}
			else
			{
				jep_add_leaf_node(root, l_brace);
			}
		}
		else
		{
			jep_ast_node *stm = jep_statement(root, nodes);
			if (stm != NULL && !root->error)
			{
				jep_add_leaf_node(root, stm);
			}
		}
	}
}
