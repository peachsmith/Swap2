/*
	Functions for performing operations dictated by an abstract syntax tree
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
#include "swap/operator.h"

/* evaluates the nodes of an AST */
/* TODO ensure that this doesn't return a NULL pointer */
jep_obj *jep_evaluate(jep_ast_node ast, jep_obj *list)
{
	jep_obj *o = NULL;

	if (ast.token.type == T_NUMBER)
	{
		return jep_number(ast.token.val->buffer);
	}
	else if (ast.token.type == T_CHARACTER)
	{
		return jep_character(ast.token.val->buffer);
	}
	else if (ast.token.type == T_STRING)
	{
		return jep_string(ast.token.val->buffer);
	}
	else if (ast.token.type == T_IDENTIFIER)
	{
		jep_obj *e = jep_get_object(ast.token.val->buffer, list);
		if (e != NULL)
		{
			o = jep_create_object();
			jep_copy_object(o, e);
			jep_copy_self(o, e);
			o->ident = e->ident;
		}
		return o;
	}
	else if (ast.token.type == T_KEYWORD)
	{
		if (ast.token.token_code == T_FUNCTION)
		{
			return jep_function(ast, list);
		}
		else if (ast.token.token_code == T_RETURN)
		{
			return jep_return(ast, list);
		}
		else if (ast.token.token_code == T_IF)
		{
			return jep_if(ast, list);
		}
		else if (ast.token.token_code == T_SWITCH)
		{
			return jep_switch(ast, list);
		}
		else if (ast.token.token_code == T_FOR)
		{
			return jep_for(ast, list);
		}
		else if (ast.token.token_code == T_WHILE)
		{
			return jep_while(ast, list);
		}
		else if (ast.token.token_code == T_TRY)
		{
			return jep_try(ast, list);
		}
		else if (ast.token.token_code == T_THROW)
		{
			return jep_throw(ast, list);
		}
		else if (ast.token.token_code == T_NULL)
		{
			jep_obj *n = jep_create_object();
			n->type = JEP_NULL;
			return n;
		}
		else if (ast.token.token_code == T_STRUCT)
		{
			return jep_struct(ast, list);
		}
	}
	else if (ast.token.type == T_MODIFIER)
	{
		return jep_modifier(ast, list);
	}

	switch (ast.token.token_code)
	{
	case T_PLUS:
		o = jep_add(ast, list);
		break;

	case T_MINUS:
		o = jep_sub(ast, list);
		break;

	case T_STAR:
		o = jep_mul(ast, list);
		break;

	case T_FSLASH:
		o = jep_div(ast, list);
		break;

	case T_MODULUS:
		o = jep_modulus(ast, list);
		break;

	case T_LESS:
		o = jep_less(ast, list);
		break;

	case T_GREATER:
		o = jep_greater(ast, list);
		break;

	case T_LOREQUAL:
		o = jep_lorequal(ast, list);
		break;

	case T_GOREQUAL:
		o = jep_gorequal(ast, list);
		break;

	case T_EQUIVALENT:
		o = jep_equiv(ast, list);
		break;

	case T_NOTEQUIVALENT:
		o = jep_noteq(ast, list);
		break;

	case T_NOT:
		o = jep_not(ast, list);
		break;

	case T_LOGAND:
		o = jep_and(ast, list);
		break;

	case T_LOGOR:
		o = jep_or(ast, list);
		break;

	case T_BITAND:
		o = jep_bitand(ast, list);
		break;

	case T_BITOR:
		o = jep_bitor(ast, list);
		break;

	case T_BITXOR:
		o = jep_bitxor(ast, list);
		break;

	case T_LSHIFT:
		o = jep_lshift(ast, list);
		break;

	case T_RSHIFT:
		o = jep_rshift(ast, list);
		break;

	case T_INCREMENT:
		o = jep_inc(ast, list);
		break;

	case T_DECREMENT:
		o = jep_dec(ast, list);
		break;

	case T_LPAREN:
		o = jep_paren(ast, list);
		break;

	case T_LBRACE:
		o = jep_brace(ast, list);
		break;

	case T_LSQUARE:
		o = jep_subscript(ast, list);
		break;

	case T_EQUALS:
		o = jep_assign(ast, list);
		break;

	case T_COMMA:
		o = jep_comma(ast, list);
		break;

	case T_COLON:
		o = jep_reference(ast, list);
		break;

	case T_DOUBLECOLON:
		o = jep_dereference(ast, list);
		break;

	case T_ADDASSIGN:
		o = jep_add_assign(ast, list);
		break;

	case T_SUBASSIGN:
		o = jep_sub_assign(ast, list);
		break;

	case T_DIVASSIGN:
		o = jep_div_assign(ast, list);
		break;

	case T_MULASSIGN:
		o = jep_mul_assign(ast, list);
		break;

	case T_MODASSIGN:
		o = jep_mod_assign(ast, list);
		break;

	case T_ANDASSIGN:
		o = jep_and_assign(ast, list);
		break;

	case T_ORASSIGN:
		o = jep_or_assign(ast, list);
		break;

	case T_XORASSIGN:
		o = jep_xor_assign(ast, list);
		break;

	case T_LSHIFTASSIGN:
		o = jep_lshift_assign(ast, list);
		break;

	case T_RSHIFTASSIGN:
		o = jep_rshift_assign(ast, list);
		break;

	case T_NEW:
		o = jep_new(ast, list);
		break;

	case T_PERIOD:
		o = jep_member(ast, list);
		break;

	default:
		printf("unrecognized token: %s\n",
			ast.token.val->buffer);
		break;
	}

	return o;
}

/* evaluates an addition expression */
jep_obj *jep_add(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		/* string concatenation */
		if (l->type == JEP_STRING || r->type == JEP_STRING)
		{
			char *l_str = jep_to_string(l);
			char *r_str = jep_to_string(r);

			if (l_str != NULL && r_str != NULL)
			{
				result = jep_create_object();
				result->type = JEP_STRING;

				char *str = malloc(strlen(l_str) + strlen(r_str) + 1);
				strcpy(str, l_str);
				strcat(str, r_str);
				result->val = (void *)(str);
			}

			if (l_str != NULL)
			{
				free(l_str);
			}

			if (r_str != NULL)
			{
				free(r_str);
			}

			jep_destroy_object(l);
			jep_destroy_object(r);

			return result;
		}

		if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
			&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
		{
			printf("invalid operand types for operation -\n");
		}
		else if (l->type == r->type)
		{
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) + (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) + (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_LONG;
			}
			else if (l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) + (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_DOUBLE;
			}
			else if (l->type == JEP_BYTE)
			{
				unsigned char *n = malloc(sizeof(unsigned char));
				*n = (*(unsigned char *)(l->val)) + (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_DOUBLE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) + (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) + (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) + (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) + (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_DOUBLE;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) + (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) + (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) + (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) + (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_LONG;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) + (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) + (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) + (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) + (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_BYTE;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates subtraction or negation */
jep_obj *jep_sub(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2 && node.leaf_count != 1)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);

	if (node.leaf_count > 1)
	{
		r = jep_evaluate(node.leaves[1], list);
	}

	if (node.leaf_count == 1 && l != NULL)
	{
		if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE)
		{
			printf("invalid operand types for operation -\n");
			result = jep_create_object();
			result->type = JEP_ARGUMENT;
		}
		else if (l->type == JEP_INT)
		{
			int *n = malloc(sizeof(int));
			*n = 0 - (*(int *)(l->val));
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_LONG)
		{
			long *n = malloc(sizeof(int));
			*n = 0 - (*(long *)(l->val));
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_LONG;
		}
		else if (l->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			*n = 0 - (*(double *)(l->val));
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_DOUBLE;
		}
		else if (l->type == JEP_BYTE)
		{
			unsigned char *n = malloc(sizeof(unsigned char));
			*n = 0 - (*(unsigned char *)(l->val));
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_BYTE;
		}
	}
	else if (l != NULL && r != NULL)
	{
		if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
			&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
		{
			printf("invalid operand types for operation -\n");
		}
		if (l->type == r->type)
		{
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) - (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) - (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_LONG;
			}
			else if (l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) - (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_DOUBLE;
			}
			else if (l->type == JEP_BYTE)
			{
				unsigned char *n = malloc(sizeof(unsigned char));
				*n = (*(unsigned char *)(l->val)) - (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) - (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) - (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) - (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) - (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_DOUBLE;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) - (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) - (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) - (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) - (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_LONG;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) - (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) - (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) - (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) - (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_BYTE;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a multiplication expression */
jep_obj *jep_mul(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
			&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
		{
			printf("invalid operand types for operation -\n");
		}
		if (l->type == r->type)
		{
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) * (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) * (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_LONG;
			}
			else if (l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) * (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_DOUBLE;
			}
			else if (l->type == JEP_BYTE)
			{
				unsigned char *n = malloc(sizeof(unsigned char));
				*n = (*(unsigned char *)(l->val)) * (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) * (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) * (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) * (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) * (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_DOUBLE;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			printf("multiplying a long with an int\n");
			long *n = malloc(sizeof(long));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) * (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) * (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) * (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) * (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_LONG;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) * (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) * (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) * (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) * (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_BYTE;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a division expression */
jep_obj *jep_div(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type == r->type)
		{
			if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
				&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
			{
				printf("invalid operand types for operation -\n");
			}
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) / (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) / (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_LONG;
			}
			else if (l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) / (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_DOUBLE;
			}
			else if (l->type == JEP_BYTE)
			{
				unsigned char *n = malloc(sizeof(unsigned char));
				*n = (*(unsigned char *)(l->val)) / (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) / (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) / (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) / (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) / (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_DOUBLE;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) / (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) / (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) / (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) / (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_LONG;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			long *n = malloc(sizeof(long));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) / (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) / (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) / (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) / (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_BYTE;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a modulus expression */
jep_obj *jep_modulus(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type == r->type && l->type == JEP_INT)
		{
			int *n = malloc(sizeof(int));
			*n = (*(int *)(l->val)) % (*(int *)(r->val));
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else
		{
			printf("invalid operand types for %% operator\n");
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a less than expression */
jep_obj *jep_less(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type == r->type)
		{
			if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
				&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
			{
				printf("invalid operand types for operation -\n");
			}
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) < (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) < (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) < (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_BYTE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(unsigned char *)(l->val)) < (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) < (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) < (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) < (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) < (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) < (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) < (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) < (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) < (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) < (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) < (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) < (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) < (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a less than expression */
jep_obj *jep_greater(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type == r->type)
		{
			if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
				&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
			{
				printf("invalid operand types for operation -\n");
			}
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) > (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) > (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) > (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_BYTE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(unsigned char *)(l->val)) > (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) > (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) > (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) > (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) > (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) > (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) > (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) > (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) > (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) > (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) > (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) > (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) > (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a less than or equal to expression */
jep_obj *jep_lorequal(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type == r->type)
		{
			if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
				&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
			{
				printf("invalid operand types for operation >=\n");
			}
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) <= (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) <= (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) <= (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_BYTE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(unsigned char *)(l->val)) <= (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) <= (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) <= (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) <= (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) <= (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) <= (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) <= (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) <= (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) <= (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) <= (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) <= (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) <= (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) <= (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a greater than or equal to expression */
jep_obj *jep_gorequal(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		if (l->type == r->type)
		{
			if (l->type != JEP_INT && l->type != JEP_LONG && l->type != JEP_DOUBLE && l->type != JEP_BYTE
				&& r->type != JEP_INT && r->type != JEP_LONG && r->type != JEP_DOUBLE && r->type != JEP_BYTE)
			{
				printf("invalid operand types for operation >=\n");
			}
			if (l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int *)(l->val)) >= (*(int *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long *)(l->val)) >= (*(long *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double *)(l->val)) >= (*(double *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_INT;
			}
			else if (l->type == JEP_BYTE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(unsigned char *)(l->val)) >= (*(unsigned char *)(r->val));
				result = jep_create_object();
				result->val = (void *)n;
				result->type = JEP_BYTE;
			}
		}
		else if (l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(double *)(l->val)) >= (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(double *)(l->val)) >= (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_DOUBLE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) >= (*(double *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) >= (*(double *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(long *)(l->val)) >= (*(int *)(r->val));
				}
				else if (r->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) >= (*(long *)(r->val));
				}
			}
			else if (r->type == JEP_LONG)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) >= (*(long *)(r->val));
				}
				else if (l->type == JEP_LONG)
				{
					*n = (*(long *)(l->val)) >= (*(long *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
		else if (l->type == JEP_BYTE || r->type == JEP_BYTE)
		{
			int *n = malloc(sizeof(int));
			if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)
				{
					*n = (*(unsigned char *)(l->val)) >= (*(int *)(r->val));
				}
				else if (r->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) >= (*(unsigned char *)(r->val));
				}
			}
			else if (r->type == JEP_BYTE)
			{
				if (l->type == JEP_INT)
				{
					*n = (*(int *)(l->val)) >= (*(unsigned char *)(r->val));
				}
				else if (l->type == JEP_BYTE)
				{
					*n = (*(unsigned char *)(l->val)) >= (*(unsigned char *)(r->val));
				}
			}
			result = jep_create_object();
			result->val = (void *)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/**
 * operand type check
 *
 * verifies that the type of an operand
 * is acceptable for a given operator
 */
static int jep_otc(const char* operator, jep_obj* operand)
{
	int valid = 0;

	if (operand == NULL)
	{
		return 0;
	}

	if (!strcmp("!=", operator)
		|| !strcmp("==", operator)
		|| !strcmp("&&", operator)
		|| !strcmp("||", operator))
	{
		switch (operand->type)
		{
		case JEP_INT:
		case JEP_LONG:
		case JEP_DOUBLE:
		case JEP_BYTE:
		case JEP_CHARACTER:
		case JEP_STRING:
		case JEP_NULL:
			valid = 1;
		default:
			break;
		}
	}
	else if (!strcmp("<", operator)
		|| !strcmp(">", operator)
		|| !strcmp("<=", operator)
		|| !strcmp(">=", operator))
	{
		switch (operand->type)
		{
		case JEP_INT:
		case JEP_LONG:
		case JEP_DOUBLE:
		case JEP_BYTE:
		case JEP_CHARACTER:
			valid = 1;
		default:
			break;
		}
	}
	else if (!strcmp("&", operator)
		|| !strcmp("|", operator)
		|| !strcmp("^", operator)
		|| !strcmp("<<", operator)
		|| !strcmp(">>", operator)
		|| !strcmp("&=", operator)
		|| !strcmp("|=", operator)
		|| !strcmp("^=", operator)
		|| !strcmp("<<=", operator)
		|| !strcmp(">>=", operator)
		|| !strcmp("++", operator)
		|| !strcmp("--", operator))
	{
		switch (operand->type)
		{
		case JEP_INT:
		case JEP_LONG:
		case JEP_BYTE:
		case JEP_CHARACTER:
			valid = 1;
		default:
			break;
		}
	}
	else if (!strcmp("!", operator))
	{
		switch (operand->type)
		{
		case JEP_INT:
		case JEP_LONG:
		case JEP_DOUBLE:
		case JEP_BYTE:
		case JEP_CHARACTER:
		case JEP_NULL:
			valid = 1;
		default:
			break;
		}
	}

	return valid;
}

/* evaluates an equivalence expression */
jep_obj *jep_equiv(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc("==", l) || !jep_otc("==", r))
	{
		printf("invalid operand type for operator ==\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			/* handle operands of the same type */
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) == (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) == (*(long *)(r->val));
			else if (l->type == JEP_DOUBLE)    *n = (*(double *)(l->val)) == (*(double *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) == (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) == (*(char *)(r->val));
			else if (l->type == JEP_STRING)    *n = !strcmp((char *)(l->val), (char *)(r->val));
			else if (l->type == JEP_NULL)      *n = 1;
		}
		else if (l->type == JEP_STRING || r->type == JEP_STRING
			|| l->type == JEP_NULL || r->type == JEP_NULL)
		{
			/* handle null and string */
			*n = 0;
		}
		else
		{
			/*
			 * handle operands of different types
			 * (e.g. int, long, double, byte, or character)
			 */
			if (l->type == JEP_INT)
			{
				if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) == (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(int *)(l->val)) == (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) == (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) == (*(char *)(r->val));
			}
			else if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)            *n = (*(long *)(l->val)) == (*(int *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(long *)(l->val)) == (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) == (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) == (*(char *)(r->val));
			}
			else if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)            *n = (*(double *)(l->val)) == (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(double *)(l->val)) == (*(long *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(double *)(l->val)) == (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(double *)(l->val)) == (*(char *)(r->val));
			}
			else if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) == (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) == (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(unsigned char *)(l->val)) == (*(double *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) == (*(char *)(r->val));
			}
			else if (l->type == JEP_CHARACTER)
			{
				if (r->type == JEP_INT)         *n = (*(char *)(l->val)) == (*(int *)(r->val));
				else if (r->type == JEP_LONG)   *n = (*(char *)(l->val)) == (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE) *n = (*(char *)(l->val)) == (*(double *)(r->val));
				else if (r->type == JEP_BYTE)   *n = (*(char *)(l->val)) == (*(unsigned char *)(r->val));
			}
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* evaluates a not equivalence expression */
jep_obj *jep_noteq(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc("!=", l) || !jep_otc("!=", r))
	{
		printf("invalid operand type for operator !=\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			/* handle operands of the same type */
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) != (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) != (*(long *)(r->val));
			else if (l->type == JEP_DOUBLE)    *n = (*(double *)(l->val)) != (*(double *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) != (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) != (*(char *)(r->val));
			else if (l->type == JEP_STRING)    *n = !!strcmp((char *)(l->val), (char *)(r->val));
			else if (l->type == JEP_NULL)      *n = 0;
		}
		else if (l->type == JEP_STRING || r->type == JEP_STRING
			|| l->type == JEP_NULL || r->type == JEP_NULL)
		{
			/* handle null and string */
			*n = 1;
		}
		else
		{
			/*
			* handle operands of different types
			* (e.g. int, long, double, byte, or character)
			*/
			if (l->type == JEP_INT)
			{
				if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) != (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(int *)(l->val)) != (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) != (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) != (*(char *)(r->val));
			}
			else if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)            *n = (*(long *)(l->val)) != (*(int *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(long *)(l->val)) != (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) != (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) != (*(char *)(r->val));
			}
			else if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)            *n = (*(double *)(l->val)) != (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(double *)(l->val)) != (*(long *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(double *)(l->val)) != (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(double *)(l->val)) != (*(char *)(r->val));
			}
			else if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) != (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) != (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(unsigned char *)(l->val)) != (*(double *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) != (*(char *)(r->val));
			}
			else if (l->type == JEP_CHARACTER)
			{
				if (r->type == JEP_INT)         *n = (*(char *)(l->val)) != (*(int *)(r->val));
				else if (r->type == JEP_LONG)   *n = (*(char *)(l->val)) != (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE) *n = (*(char *)(l->val)) != (*(double *)(r->val));
				else if (r->type == JEP_BYTE)   *n = (*(char *)(l->val)) != (*(unsigned char *)(r->val));
			}
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a not operation */
jep_obj *jep_not(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* operand  */
	jep_obj *result = NULL; /* result   */

	if (node.leaf_count != 1)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);

	/* operand type check */
	if (!jep_otc("!", l))
	{
		printf("invalid operand type for operator !\n");
	}

	int *n = malloc(sizeof(int));
	result = jep_create_object();
	result->type = JEP_INT;

	if (l != NULL)
	{
		if (l->type == JEP_INT)            *n = !(*(int *)(l->val));
		else if (l->type == JEP_LONG)      *n = !(*(long *)(l->val));
		else if (l->type == JEP_DOUBLE)    *n = !(*(double *)(l->val));
		else if (l->type == JEP_BYTE)      *n = !(*(unsigned char *)(l->val));
		else if (l->type == JEP_CHARACTER) *n = !(*(char *)(l->val));
		else if (l->type == JEP_NULL)      *n = 0;

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain operand val\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}

	return result;
}

/* performs a logical and operation */
jep_obj *jep_and(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);

	/* operand type check */
	if (!jep_otc("&&", l))
	{
		printf("invalid operand type for operator &&\n");
	}

	int *n = malloc(sizeof(int));
	*n = -1;
	result = jep_create_object();
	result->type = JEP_INT;

	/* check result of left operand first */
	if (l != NULL)
	{
		switch (l->type)
		{
		case JEP_INT:
			if ((*(int *)(l->val)) == 0) *n = 0;
			break;
		case JEP_LONG:
			if ((*(long *)(l->val)) == 0) *n = 0;
			break;
		case JEP_DOUBLE:
			if ((*(double *)(l->val)) == 0) *n = 0;
			break;
		case JEP_BYTE:
			if ((*(unsigned char *)(l->val)) == 0) *n = 0;
			break;
		case JEP_CHARACTER:
			if ((*(char *)(l->val)) == 0) *n = 0;
			break;
		case JEP_STRING:
			if (strlen((char *)(l->val)) < 1) *n = 0;
			break;
		case JEP_NULL:
			*n = 0;
			break;
		default:
			break;
		}
	}

	/* return if the left operand is false */
	if (*n > -1)
	{
		result->val = (void *)n;
		jep_destroy_object(l);
		return result;
	}
	else *n = 0;

	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		/* operand type check */
		if (!jep_otc("&&", r))
		{
			printf("invalid operand type for operator &&\n");
		}

		if (l->type == r->type)
		{
			/* handle operands of the same type */
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) && (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) && (*(long *)(r->val));
			else if (l->type == JEP_DOUBLE)    *n = (*(double *)(l->val)) && (*(double *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) && (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) && (*(char *)(r->val));
			else if (l->type == JEP_STRING)
			{
				if (strlen((char *)(l->val)) < 1 || strlen((char *)(r->val)) < 1)
				{
					*n = 0;
				}
				else *n = 1;
			}
			else if (l->type == JEP_NULL) *n = 0;
		}
		else if (l->type == JEP_NULL || r->type == JEP_NULL)
		{
			/* handle null */
			*n = 0;
		}
		else
		{
			/*
			* handle operands of different types
			* (e.g. int, long, double, byte, or character)
			*/
			if (l->type == JEP_INT)
			{
				if (r->type == JEP_INT)            *n = (*(int *)(l->val)) && (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(int *)(l->val)) && (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(int *)(l->val)) && (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) && (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) && (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(int *)(l->val)) && strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)            *n = (*(long *)(l->val)) && (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(long *)(l->val)) && (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(long *)(l->val)) && (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) && (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) && (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(long *)(l->val)) && strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)            *n = (*(double *)(l->val)) && (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(double *)(l->val)) && (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(double *)(l->val)) && (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(double *)(l->val)) && (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(double *)(l->val)) && (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(double *)(l->val)) && strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) && (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) && (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(unsigned char *)(l->val)) && (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) && (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) && (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(unsigned char *)(l->val)) && strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_CHARACTER)
			{
				if (r->type == JEP_INT)            *n = (*(char *)(l->val)) && (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(char *)(l->val)) && (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(char *)(l->val)) && (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(char *)(l->val)) && (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(char *)(l->val)) && (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(char *)(l->val)) && strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_STRING)
			{
				if (r->type == JEP_LONG)        *n = strlen((char *)(l->val)) > 0 && (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE) *n = strlen((char *)(l->val)) > 0 && (*(double *)(r->val));
				else if (r->type == JEP_BYTE)   *n = strlen((char *)(l->val)) > 0 && (*(unsigned char *)(r->val));
				else if (r->type == JEP_INT)    *n = strlen((char *)(l->val)) > 0 && (*(int *)(r->val));
				else if (r->type == JEP_STRING) *n = strlen((char *)(l->val)) > 0 && strlen((char *)(r->val)) > 0;
			}
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	result->val = (void *)n;

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a logical or operation */
jep_obj *jep_or(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);

	/* operand type check */
	if (!jep_otc("||", l))
	{
		printf("invalid operand type for operator ||\n");
	}

	int *n = malloc(sizeof(int));
	*n = -1;
	result = jep_create_object();
	result->type = JEP_INT;

	/* check result of left operand first */
	if (l != NULL)
	{
		switch (l->type)
		{
		case JEP_INT:
			if ((*(int *)(l->val)) != 0) *n = 1;
			break;
		case JEP_LONG:
			if ((*(long *)(l->val)) != 0) *n = 1;
			break;
		case JEP_DOUBLE:
			if ((*(double *)(l->val)) != 0) *n = 1;
			break;
		case JEP_BYTE:
			if ((*(unsigned char *)(l->val)) != 0) *n = 1;
			break;
		case JEP_CHARACTER:
			if ((*(char *)(l->val)) != 0) *n = 1;
			break;
		case JEP_STRING:
			if (strlen((char *)(l->val)) > 0) *n = 1;
			break;
		case JEP_NULL:
			*n = 0;
			break;
		default:
			break;
		}
	}

	/* return if the left operand is false */
	if (*n > -1)
	{
		result->val = (void *)n;
		jep_destroy_object(l);
		return result;
	}
	else *n = 0;

	r = jep_evaluate(node.leaves[1], list);

	if (l != NULL && r != NULL)
	{
		/* operand type check */
		if (!jep_otc("||", r))
		{
			printf("invalid operand type for operator &&\n");
		}

		if (l->type == r->type)
		{
			/* handle operands of the same type */
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) || (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) || (*(long *)(r->val));
			else if (l->type == JEP_DOUBLE)    *n = (*(double *)(l->val)) || (*(double *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) || (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) || (*(char *)(r->val));
			else if (l->type == JEP_STRING)
			{
				if (strlen((char *)(l->val)) > 0 || strlen((char *)(r->val)) > 0)
				{
					*n = 1;
				}
				else *n = 0;
			}
			else if (l->type == JEP_NULL) *n = 0;
		}
		else
		{
			/*
			* handle operands of different types
			* (e.g. int, long, double, byte, or character)
			*/
			if (l->type == JEP_INT)
			{
				if (r->type == JEP_INT)            *n = (*(int *)(l->val)) || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(int *)(l->val)) || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(int *)(l->val)) || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(int *)(l->val)) || strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_LONG)
			{
				if (r->type == JEP_INT)            *n = (*(long *)(l->val)) || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(long *)(l->val)) || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(long *)(l->val)) || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(long *)(l->val)) || strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_DOUBLE)
			{
				if (r->type == JEP_INT)            *n = (*(double *)(l->val)) || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(double *)(l->val)) || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(double *)(l->val)) || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(double *)(l->val)) || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(double *)(l->val)) || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(double *)(l->val)) || strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_BYTE)
			{
				if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(unsigned char *)(l->val)) || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(unsigned char *)(l->val)) || strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_CHARACTER)
			{
				if (r->type == JEP_INT)            *n = (*(char *)(l->val)) || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = (*(char *)(l->val)) || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = (*(char *)(l->val)) || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = (*(char *)(l->val)) || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = (*(char *)(l->val)) || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = (*(char *)(l->val)) || strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_STRING)
			{
				if (r->type == JEP_INT)            *n = strlen((char *)(l->val)) > 0 || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = strlen((char *)(l->val)) > 0 || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = strlen((char *)(l->val)) > 0 || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = strlen((char *)(l->val)) > 0 || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = strlen((char *)(l->val)) > 0 || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = strlen((char *)(l->val)) > 0 || strlen((char *)(r->val)) > 0;
			}
			else if (l->type == JEP_NULL)
			{
				if (r->type == JEP_INT)            *n = 0 || (*(int *)(r->val));
				else if (r->type == JEP_LONG)      *n = 0 || (*(long *)(r->val));
				else if (r->type == JEP_DOUBLE)    *n = 0 || (*(double *)(r->val));
				else if (r->type == JEP_BYTE)      *n = 0 || (*(unsigned char *)(r->val));
				else if (r->type == JEP_CHARACTER) *n = 0 || (*(char *)(r->val));
				else if (r->type == JEP_STRING)    *n = 0 || strlen((char *)(r->val)) > 0;
			}
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	result->val = (void *)n;

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a bitwise operation */
jep_obj *jep_bitand(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc("&", l) || !jep_otc("&", r))
	{
		printf("invalid operand type for operator &\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) & (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) & (*(long *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) & (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) & (*(char *)(r->val));
		}
		else if (l->type == JEP_INT)
		{
			if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) & (*(long *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) & (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) & (*(char *)(r->val));
		}
		else if (l->type == JEP_LONG)
		{
			if (r->type == JEP_INT)            *n = (*(long *)(l->val)) & (*(int *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) & (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) & (*(char *)(r->val));
		}
		else if (l->type == JEP_BYTE)
		{
			if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) & (*(int *)(r->val));
			else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) & (*(long *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) & (*(char *)(r->val));
		}
		else if (l->type == JEP_CHARACTER)
		{
			if (r->type == JEP_INT)       *n = (*(char *)(l->val)) & (*(int *)(r->val));
			else if (r->type == JEP_LONG) *n = (*(char *)(l->val)) & (*(long *)(r->val));
			else if (r->type == JEP_BYTE) *n = (*(char *)(l->val)) & (*(unsigned char *)(r->val));
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a bitwise or operation */
jep_obj *jep_bitor(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc("|", l) || !jep_otc("|", r))
	{
		printf("invalid operand type for operator |\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) | (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) | (*(long *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) | (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) | (*(char *)(r->val));
		}
		else if (l->type == JEP_INT)
		{
			if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) | (*(long *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) | (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) | (*(char *)(r->val));
		}
		else if (l->type == JEP_LONG)
		{
			if (r->type == JEP_INT)            *n = (*(long *)(l->val)) | (*(int *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) | (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) | (*(char *)(r->val));
		}
		else if (l->type == JEP_BYTE)
		{
			if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) | (*(int *)(r->val));
			else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) | (*(long *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) | (*(char *)(r->val));
		}
		else if (l->type == JEP_CHARACTER)
		{
			if (r->type == JEP_INT)       *n = (*(char *)(l->val)) | (*(int *)(r->val));
			else if (r->type == JEP_LONG) *n = (*(char *)(l->val)) | (*(long *)(r->val));
			else if (r->type == JEP_BYTE) *n = (*(char *)(l->val)) | (*(unsigned char *)(r->val));
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a bitwise xor operation */
jep_obj *jep_bitxor(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc("^", l) || !jep_otc("^", r))
	{
		printf("invalid operand type for operator ^\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) ^ (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) ^ (*(long *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) ^ (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) ^ (*(char *)(r->val));
		}
		else if (l->type == JEP_INT)
		{
			if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) ^ (*(long *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) ^ (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) ^ (*(char *)(r->val));
		}
		else if (l->type == JEP_LONG)
		{
			if (r->type == JEP_INT)            *n = (*(long *)(l->val)) ^ (*(int *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) ^ (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) ^ (*(char *)(r->val));
		}
		else if (l->type == JEP_BYTE)
		{
			if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) ^ (*(int *)(r->val));
			else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) ^ (*(long *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) ^ (*(char *)(r->val));
		}
		else if (l->type == JEP_CHARACTER)
		{
			if (r->type == JEP_INT)       *n = (*(char *)(l->val)) ^ (*(int *)(r->val));
			else if (r->type == JEP_LONG) *n = (*(char *)(l->val)) ^ (*(long *)(r->val));
			else if (r->type == JEP_BYTE) *n = (*(char *)(l->val)) ^ (*(unsigned char *)(r->val));
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a left bit shift operation */
jep_obj *jep_lshift(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc("<<", l) || !jep_otc("<<", r))
	{
		printf("invalid operand type for operator <<\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) << (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) << (*(long *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) << (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) << (*(char *)(r->val));
		}
		else if (l->type == JEP_INT)
		{
			if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) << (*(long *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) << (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) << (*(char *)(r->val));
		}
		else if (l->type == JEP_LONG)
		{
			if (r->type == JEP_INT)            *n = (*(long *)(l->val)) << (*(int *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) << (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) << (*(char *)(r->val));
		}
		else if (l->type == JEP_BYTE)
		{
			if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) << (*(int *)(r->val));
			else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) << (*(long *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) << (*(char *)(r->val));
		}
		else if (l->type == JEP_CHARACTER)
		{
			if (r->type == JEP_INT)       *n = (*(char *)(l->val)) << (*(int *)(r->val));
			else if (r->type == JEP_LONG) *n = (*(char *)(l->val)) << (*(long *)(r->val));
			else if (r->type == JEP_BYTE) *n = (*(char *)(l->val)) << (*(unsigned char *)(r->val));
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs a right bit shift operation */
jep_obj *jep_rshift(jep_ast_node node, jep_obj *list)
{
	jep_obj *l = NULL;		/* left operand  */
	jep_obj *r = NULL;		/* right operand */
	jep_obj *result = NULL; /* result        */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	/* operand type check */
	if (!jep_otc(">>", l) || !jep_otc(">>", r))
	{
		printf("invalid operand type for operator >>\n");
	}

	if (l != NULL && r != NULL)
	{
		int *n = malloc(sizeof(int));
		result = jep_create_object();
		result->type = JEP_INT;

		if (l->type == r->type)
		{
			if (l->type == JEP_INT)            *n = (*(int *)(l->val)) >> (*(int *)(r->val));
			else if (l->type == JEP_LONG)      *n = (*(long *)(l->val)) >> (*(long *)(r->val));
			else if (l->type == JEP_BYTE)      *n = (*(unsigned char *)(l->val)) >> (*(unsigned char *)(r->val));
			else if (l->type == JEP_CHARACTER) *n = (*(char *)(l->val)) >> (*(char *)(r->val));
		}
		else if (l->type == JEP_INT)
		{
			if (r->type == JEP_LONG)           *n = (*(int *)(l->val)) >> (*(long *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(int *)(l->val)) >> (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(int *)(l->val)) >> (*(char *)(r->val));
		}
		else if (l->type == JEP_LONG)
		{
			if (r->type == JEP_INT)            *n = (*(long *)(l->val)) >> (*(int *)(r->val));
			else if (r->type == JEP_BYTE)      *n = (*(long *)(l->val)) >> (*(unsigned char *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(long *)(l->val)) >> (*(char *)(r->val));
		}
		else if (l->type == JEP_BYTE)
		{
			if (r->type == JEP_INT)            *n = (*(unsigned char *)(l->val)) >> (*(int *)(r->val));
			else if (r->type == JEP_LONG)      *n = (*(unsigned char *)(l->val)) >> (*(long *)(r->val));
			else if (r->type == JEP_CHARACTER) *n = (*(unsigned char *)(l->val)) >> (*(char *)(r->val));
		}
		else if (l->type == JEP_CHARACTER)
		{
			if (r->type == JEP_INT)       *n = (*(char *)(l->val)) >> (*(int *)(r->val));
			else if (r->type == JEP_LONG) *n = (*(char *)(l->val)) >> (*(long *)(r->val));
			else if (r->type == JEP_BYTE) *n = (*(char *)(l->val)) >> (*(unsigned char *)(r->val));
		}

		result->val = (void *)n;
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if (l != NULL)
	{
		jep_destroy_object(l);
	}
	if (r != NULL)
	{
		jep_destroy_object(r);
	}

	return result;
}

/* performs an increment on an integer */
jep_obj *jep_inc(jep_ast_node node, jep_obj *list)
{
	if (node.leaf_count != 1)
	{
		return NULL;
	}

	jep_obj *o = NULL;
	jep_obj *obj = jep_evaluate(node.leaves[0], list);

	if (obj != NULL)
	{
		/* operand type check */
		if (!jep_otc("++", obj))
		{
			printf("invalid operand type for operator ++\n");
		}

		jep_obj *actual = obj->self;
		o = jep_create_object();

		int cur_val = *(int *)(actual->val);
		int new_val = cur_val + 1;
		*(int *)(actual->val) = new_val;

		jep_copy_object(o, actual);

		if (node.token.postfix)
		{
			*(int *)(o->val) = cur_val;
		}

		jep_destroy_object(obj);
	}

	return o;
}

/* performs a decrement on an integer */
jep_obj *jep_dec(jep_ast_node node, jep_obj *list)
{
	if (node.leaf_count != 1)
	{
		return NULL;
	}

	jep_obj *o = NULL;
	jep_obj *obj = jep_evaluate(node.leaves[0], list);

	if (obj != NULL)
	{
		/* operand type check */
		if (!jep_otc("--", obj))
		{
			printf("invalid operand type for operator --\n");
		}

		jep_obj *actual = obj->self;
		o = jep_create_object();

		int cur_val = *(int *)(actual->val);
		int new_val = cur_val - 1;
		*(int *)(actual->val) = new_val;

		jep_copy_object(o, actual);

		if (node.token.postfix)
		{
			*(int *)(o->val) = cur_val;
		}

		jep_destroy_object(obj);
	}

	return o;
}

/* performs an addition assignmnet */
jep_obj *jep_add_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_PLUS, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a subtraction assignment */
jep_obj *jep_sub_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_MINUS, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a multiplication assignment */
jep_obj *jep_mul_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_STAR, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a division assignment */
jep_obj *jep_div_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_FSLASH, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a modulus assignment */
jep_obj *jep_mod_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_MODULUS, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a bitwise and assignment on an integer */
jep_obj *jep_and_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_BITAND, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a bitwise and assignment on an integer */
jep_obj *jep_or_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_BITOR, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a bitwise exclusive or assignment on an integer */
jep_obj *jep_xor_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_BITXOR, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a bitwise exclusive or assignment on an integer */
jep_obj *jep_lshift_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_LSHIFT, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* performs a bitwise exclusive or assignment on an integer */
jep_obj *jep_rshift_assign(jep_ast_node node, jep_obj *list)
{
	jep_token equals = { NULL, 0, T_EQUALS, 0, 0, 0, 0, NULL };

	jep_token operator ={ NULL, 0, T_RSHIFT, 0, 0, 0, 0, NULL };

	jep_ast_node operands[] = { node.leaves[0], node.leaves[1] };

	jep_ast_node operation = { operator, 2, 0, operands, 0, 0, 0, 0 };

	jep_ast_node asign_operands[] = { node.leaves[0], operation };

	jep_ast_node assignment = { equals, 2, 0, asign_operands, 0, 0, 0, 0 };

	return jep_evaluate(assignment, list);
}

/* evaluates an assignment */
jep_obj *jep_assign(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL; /* the recipient of the assignment */
	jep_obj *l = NULL; /* left operand                    */
	jep_obj *r = NULL; /* right oeprand                   */

	if (node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);

	if (l != NULL && l->ret & JEP_EXCEPTION)
	{
		return l;
	}

	r = jep_evaluate(node.leaves[1], list);

	if (r != NULL && r->ret & JEP_EXCEPTION)
	{
		if (l != NULL)
		{
			jep_destroy_object(l);
		}
		return r;
	}

	if (l != NULL || node.leaves[0].token.type == T_IDENTIFIER)
	{
		if (l == NULL)
		{
			o = jep_get_object(node.leaves[0].token.val->buffer, list);
		}
		else
		{
			/* assign the actual object to o */
			o = l->self;
		}

		if (o == NULL)
		{
			/* create the object if it doesn't exist */
			o = jep_create_object();
			o->ident = node.leaves[0].token.val->buffer;
			jep_add_object(list, o);
		}
		else if (o->mod & 2)
		{
			/* don't allow reassignment of constants */
			printf("error: cannot reassign a constant\n");
			jep_destroy_object(l);
			jep_destroy_object(r);
			return NULL;
		}

		if (r == NULL)
		{
			o->type = JEP_NULL;
			jep_destroy_object(l);
			return NULL;
		}

		jep_copy_object(o, r);

		if (r->type == JEP_ARRAY)
		{
			int i = 0;
			jep_obj *head = ((jep_obj *)(o->val))->head;
			while (head != NULL)
			{
				head->array_ident = o->ident;
				head->index = i++;
				head = head->next;
			}
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free memory of unused left operands */
	jep_destroy_object(l);

	return r;
}

/* evaluates the contents of a set of parentheses */
jep_obj *jep_paren(jep_ast_node node, jep_obj *list)
{
	if (!node.token.postfix)
	{
		if (node.leaf_count == 1)
		{
			return jep_evaluate(node.leaves[0], list);
		}
		else
		{
			return NULL;
		}
	}

	jep_obj *o;		   /* function return value    */
	jep_ast_node args; /* incoming arguments       */
	jep_obj *func;	   /* function being called    */
	jep_obj *arg_list; /* list of argument objects */

	if (node.leaf_count == 0)
	{
		return NULL;
	}

	o = NULL;
	func = NULL;
	arg_list = NULL;

	/* collect the function arguments as objects */
	if (node.leaf_count == 2)
	{
		func = jep_get_object(node.leaves[1].token.val->buffer, list);
		args = node.leaves[0];
		arg_list = jep_create_object();
		arg_list->type = JEP_LIST;
		if (args.leaf_count >= 1)
		{
			if (node.leaves[0].token.token_code == T_COMMA)
			{
				jep_obj* objects = jep_create_object();
				objects->type = JEP_LIST;
				jep_sequence(node.leaves[0], list, objects);
				jep_copy_object(arg_list, objects);
				jep_destroy_object(objects);
			}
			else
			{
				jep_obj *a = jep_evaluate(node.leaves[0], list);
				if (a != NULL)
				{
					jep_obj* local_a = jep_create_object();
					jep_copy_object(local_a, a);
					jep_destroy_object(a);
					/* jep_destroy_object removes the excess reference
					if (a->type == JEP_FILE && a->val != NULL)
					{
						jep_file *file_obj = (jep_file *)(a->val);
						(file_obj->refs)--;
					}
					*/
					jep_add_object(arg_list, local_a);
				}
				else
				{
					printf("could not evaluate argument\n");
				}
			}
		}
		else if (args.leaf_count == 0)
		{
			jep_obj *a = jep_evaluate(node.leaves[0], list);

			if (a != NULL)
			{
				jep_obj* local_a = jep_create_object();
				jep_copy_object(local_a, a);
				jep_destroy_object(a);
				/* jep_destroy_object removes the excess reference
				if (a->type == JEP_FILE && a->val != NULL)
				{
					jep_file *file_obj = (jep_file *)(a->val);
					(file_obj->refs)--;
				}
				*/
				jep_add_object(arg_list, local_a);
			}
			else
			{
				printf("could not evaluate argument\n");
			}
		}
	}
	else if (node.leaf_count == 1)
	{
		func = jep_get_object(node.leaves[0].token.val->buffer, list);
	}

	if (func != NULL)
	{
		jep_obj *fargs = func->head;
		jep_obj *farg = NULL;

		if (fargs != NULL)
		{
			farg = fargs->head;
		}

		/* native function call */
		if (func->size == 1)
		{
			jep_obj* l_native = jep_get_object(" SwapNative", list);

			jep_obj* native_result = jep_call_shared((jep_lib)(l_native->val), func->ident, arg_list, list);

			if (arg_list != NULL)
			{
				jep_destroy_object(arg_list);
			}

			return native_result;
		}

		jep_ast_node body = *((jep_ast_node *)(func->head->next->val));
		if (arg_list != NULL)
		{
			jep_obj *arg = arg_list->head;
			while (arg != NULL && farg != NULL)
			{
				arg->ident = farg->ident;
				farg = farg->next;
				arg = arg->next;
			}
			if (arg != NULL || farg != NULL)
			{
				printf("woops, apparently there weren't the right amount of arguments!\n");
			}
			else
			{
				jep_add_object(list, arg_list);

				o = jep_evaluate(body, list);

				/* remove the argument list from the main list */
				jep_remove_scope(list);
			}
		}
		else
		{
			if (farg != NULL)
			{
				printf("woops, apparently there weren't the right amount of arguments!\n");
			}
			else
			{
				arg_list = jep_create_object();
				arg_list->type = JEP_LIST;

				jep_add_object(list, arg_list);

				o = jep_evaluate(body, list);

				/* remove the argument list from the main list */
				jep_remove_scope(list);
			}
		}
	}
	else
	{
		printf("couldn't find a function with the specified identifer\n");
	}

	jep_destroy_object(arg_list);

	if (o != NULL && o->ret & JEP_RETURN)
	{
		/*
		 * mark a returned object as returned
		 * so it doesn't returned recursively
		 */
		o->ret |= JEP_RETURNED;
	}

	return o;
}

/* evaluates a block of code in curly braces */
jep_obj *jep_brace(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;
	if (node.array)
	{
		jep_obj *array = jep_create_object();
		array->type = JEP_LIST;
		o = jep_create_object();
		o->type = JEP_ARRAY;
		o->val = array;
		if (node.leaf_count > 0 && node.leaves[0].token.token_code == T_COMMA)
		{
			jep_sequence(node.leaves[0], list, array);
			int i = 0;
			jep_obj *elem = array->head;
			while (elem != NULL)
			{
				elem->index = i++;
				elem = elem->next;
			}
		}
		else if (node.leaf_count == 1)
		{
			jep_obj *e = jep_evaluate(node.leaves[0], list);
			e->index = 0;
			jep_add_object(array, e);
		}
		o->size = array->size;
	}
	else
	{
		int i;
		for (i = 0; i < node.leaf_count; i++)
		{
			o = jep_evaluate(node.leaves[i], list);
			if (o != NULL && o->ret)
			{
				if (o->ret & JEP_EXCEPTION || !(o->ret & JEP_RETURNED))
				{
					return o;
				}
				jep_destroy_object(o);
			}
			else if (o != NULL)
			{
				jep_destroy_object(o);
			}
			o = NULL;
		}
	}

	return o;
}

/* evaluates an array subscript */
jep_obj *jep_subscript(jep_ast_node node, jep_obj *list)
{

	jep_obj *o = NULL;
	if (node.leaf_count != 2 && node.leaf_count != 1)
	{
		printf("invalid leaf count for ast node\n");
	}

	/* array initialization */
	if (node.leaf_count == 1)
	{
		o = jep_create_object();
		o->type = JEP_ARRAY;
		jep_obj *array = jep_create_object();
		array->type = JEP_LIST;
		o->val = array;

		jep_obj *size = jep_evaluate(node.leaves[0], list);

		if (size == NULL || size->type != JEP_INT || size->val == NULL)
		{
			if (size != NULL)
			{
				jep_destroy_object(size);
			}
			printf("invalid array size\n");
			jep_destroy_object(o);
			jep_destroy_object(array);
			o = NULL;
			return o;
		}

		int s = *((int *)(size->val));

		jep_destroy_object(size);

		int i;
		for (i = 0; i < s; i++)
		{
			jep_obj *elem = jep_create_object();
			elem->type = JEP_NULL;
			elem->index = i;
			jep_add_object(array, elem);
		}

		o->size = array->size;

		return o;
	}

	/* array index access */

	jep_obj *index = jep_evaluate(node.leaves[0], list);
	jep_obj *array = jep_evaluate(node.leaves[1], list);

	if (index != NULL && array != NULL)
	{
		if (index->ret & JEP_EXCEPTION)
		{
			return index;
		}

		if (array->ret & JEP_EXCEPTION)
		{
			jep_destroy_object(index);
			return array;
		}

		if (index->type != JEP_INT)
		{
			printf("array subscript must be an integer\n");
		}
		else if (array->type != JEP_ARRAY)
		{
			printf("cannot access index of non array object: ");
			jep_print_object(array);
		}
		else if (array->size > 0)
		{
			jep_obj *contents = (jep_obj *)(array->val);
			jep_obj *elem = contents->head;
			int target = *(int *)(index->val);
			int i;
			int found = 0;
			for (i = 0; elem != NULL && !found; i++)
			{
				if (i == target)
				{
					o = jep_create_object();
					jep_copy_object(o, elem);
					o->array_ident = elem->array_ident;
					o->index = elem->index;
					jep_copy_self(o, elem);
					found = 1;
				}
				elem = elem->next;
			}
			if (o == NULL)
			{
				jep_destroy_object(index);
				jep_destroy_object(array);
				o = jep_create_object();
				o->type = JEP_STRING;
				o->ret = JEP_RETURN | JEP_EXCEPTION;
				o->val = malloc(26);
				strcpy(o->val, "array index out of bounds");
				((char*)(o->val))[25] = '\0';
				return o;
			}
		}
		else
		{
			printf("cannot access an element in an empty array\n");
		}
	}
	else
	{
		printf("could not evaluate either the array or index\n");
	}

	jep_destroy_object(index);
	jep_destroy_object(array);

	return o;
}

jep_obj *jep_get_data_member(jep_ast_node node, jep_obj *list)
{
	if (node.leaf_count != 2)
	{
		return NULL;
	}

	jep_obj *mem;
	jep_obj *struc;
	jep_obj *members;

	if (node.leaves[0].token.token_code == T_PERIOD)
	{
		struc = jep_get_data_member(node.leaves[0], list);
	}
	else if (node.leaves[0].token.type == T_IDENTIFIER)
	{
		struc = jep_get_object(node.leaves[0].token.val->buffer, list);
	}
	else if (node.leaves[0].token.token_code == T_DOUBLECOLON)
	{
		struc = jep_evaluate(node.leaves[0], list);
		if (struc != NULL)
		{
			jep_obj *tmp = struc;
			struc = jep_get_object(struc->ident, list);
			jep_destroy_object(tmp);
		}
	}
	else if (node.leaves[0].token.token_code == T_LPAREN)
	{
		jep_ast_node paren = node.leaves[0];
		/* handle parentheses */

		while (paren.token.token_code == T_LPAREN)
		{
			paren = paren.leaves[0];
			if (paren.token.token_code == T_COMMA)
			{
				while (paren.token.token_code == T_COMMA)
				{
					paren = paren.leaves[1];
				}
			}
		}
		struc = jep_get_data_member(paren, list);
	}
	else
	{
		struc = NULL;
	}

	if (struc == NULL)
	{
		printf("could not obtain object with identifier %s\n",
			node.leaves[0].token.val->buffer);
		return NULL;
	}
	else if (struc->type != JEP_STRUCT)
	{
		printf("%s is not a struct\n",
			node.leaves[0].token.val->buffer);
		return NULL;
	}

	if (node.leaves[1].token.type != T_IDENTIFIER)
	{
		printf("an identifier must be used to access data members\n");
		return NULL;
	}

	members = (jep_obj *)(struc->val);
	mem = NULL;

	if (members->size > 0)
	{
		jep_obj *m = members->head;
		while (m != NULL && mem == NULL)
		{
			if (!strcmp(m->ident, node.leaves[1].token.val->buffer))
			{
				mem = m;
			}
			m = m->next;
		}
		if (mem == NULL)
		{
			printf("%s does not have a member with the identifier %s\n",
				struc->ident, node.leaves[1].token.val->buffer);
		}
	}
	else
	{
		return NULL;
	}

	return mem;
}

/* evaluates a function definition */
jep_obj *jep_function(jep_ast_node node, jep_obj *list)
{
	/* get the current scope */
	jep_obj *scope = list;
	while (scope->tail != NULL && scope->tail->type == JEP_LIST)
	{
		scope = scope->tail;
	}

	jep_obj *exist = jep_get_object(node.leaves[0].token.val->buffer, scope);

	if (exist != NULL)
	{
		jep_obj* redef_except;
		redef_except = jep_create_object();
		redef_except->type = JEP_STRING;
		redef_except->ret = JEP_RETURN | JEP_EXCEPTION;
		redef_except->val = malloc(22);
		strcpy(redef_except->val, "function redefinition");
		((char*)(redef_except->val))[21] = '\0';
		return redef_except;
	}

	jep_obj *copy = jep_create_object();
	jep_obj *func = jep_create_object();
	func->type = JEP_FUNCTION;
	func->ident = node.leaves[0].token.val->buffer;
	jep_obj *args = jep_create_object();

	/* function arguments */
	int i;
	for (i = 0; i < node.leaves[1].leaf_count; i++)
	{
		jep_obj *a = jep_create_object();
		a->type = JEP_ARGUMENT;
		a->ident = node.leaves[1].leaves[i].token.val->buffer;
		jep_add_object(args, a);
	}

	jep_add_object(func, args);

	if (node.leaf_count == 3)
	{
		/* function body */
		jep_obj *body = jep_create_object();
		body->type = JEP_FUNCTION_BODY;
		jep_ast_node *n = jep_create_ast_node();
		*n = node.leaves[2];
		body->val = n;
		jep_add_object(func, body);
	}

	func->head->type = JEP_LIST;

	jep_add_object(list, func);
	jep_copy_object(copy, func);
	copy->self = func->self;

	return copy;
}

/* returns from a function */
jep_obj *jep_return(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;

	if (node.leaf_count == 1)
	{
		o = jep_evaluate(node.leaves[0], list);
		o->ret |= 1;
	}
	else if (node.leaf_count == 0)
	{
		o = jep_create_object();
		o->type = JEP_ARGUMENT;
		o->ret = 1;
	}

	return o;
}

/* evaluates a comma tree */
jep_obj *jep_comma(jep_ast_node node, jep_obj *list)
{
	jep_ast_node l = node.leaves[0]; /* left operand  */
	jep_ast_node r = node.leaves[1]; /* right operand */
	jep_obj *lo = NULL;				 /* left object   */
	jep_obj *ro = NULL;				 /* right object  */

	if (l.token.token_code == T_COMMA)
	{
		lo = jep_comma(l, list);
		jep_destroy_object(lo);
	}
	else
	{
		lo = jep_evaluate(l, list);
		jep_destroy_object(lo);
	}

	if (r.token.token_code == T_COMMA)
	{
		ro = jep_comma(r, list);
	}
	else
	{
		ro = jep_evaluate(r, list);
	}

	return ro;
}

/* evaluates a reference */
jep_obj *jep_reference(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;

	jep_obj *v = jep_evaluate(node.leaves[0], list);

	if (v != NULL)
	{
		o = jep_create_object();
		o->type = JEP_REFERENCE;
		o->val = v->self;

		jep_destroy_object(v);
	}

	return o;
}

/* evaluates a dereference */
jep_obj *jep_dereference(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;

	jep_obj *v = jep_evaluate(node.leaves[0], list);

	if (v != NULL)
	{
		if (v->type == JEP_REFERENCE)
		{
			jep_obj *ref = (jep_obj *)(v->val);
			o = jep_create_object();
			o->ident = ref->ident;
			jep_copy_object(o, ref);
			jep_copy_self(o, ref);
			jep_destroy_object(v);
		}
		else
		{
			jep_destroy_object(v);
			o = jep_create_object();
			o->type = JEP_STRING;
			o->ret = JEP_RETURN | JEP_EXCEPTION;
			o->val = malloc(53);
			strcpy(o->val, "cannot dereference something that is not a reference");
			((char*)(o->val))[52] = '\0';
			return o;
		}
	}
	else
	{
		printf("could not evaluate dereference operand\n");
	}

	return o;
}

/* evaluates a comma-delimited sequence of objects */
void jep_sequence(jep_ast_node node, jep_obj *list, jep_obj *seq)
{
	jep_ast_node l = node.leaves[0]; /* left operand  */
	jep_ast_node r = node.leaves[1]; /* right operand */
	jep_obj *lo = NULL;				 /* left object   */
	jep_obj *ro = NULL;				 /* right object  */

	if (l.token.token_code == T_COMMA)
	{
		jep_sequence(l, list, seq);
	}
	else
	{
		lo = jep_evaluate(l, list);
		jep_add_object(seq, lo);
	}

	if (r.token.token_code == T_COMMA)
	{
		jep_sequence(r, list, seq);
	}
	else
	{
		ro = jep_evaluate(r, list);
		jep_add_object(seq, ro);
	}
}

/* evaluates an if statement */
jep_obj *jep_if(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;
	jep_ast_node cond; /* condition        */
	jep_ast_node body; /* body             */
	jep_ast_node els;  /* else and else if */

	if (node.leaf_count < 2)
	{
		return o;
	}

	cond = node.leaves[0];
	body = node.leaves[1];

	jep_obj *c = jep_evaluate(cond, list);

	if (c != NULL && c->type == JEP_INT)
	{
		int val = *((int *)(c->val));
		if (val)
		{
			/* add a list for scope */
			jep_obj *scope = jep_create_object();
			scope->type = JEP_LIST;
			jep_add_object(list, scope);

			o = jep_evaluate(body, list);

			/* remove the argument list from the main list */
			jep_remove_scope(list);
			jep_destroy_list(scope);
			free(scope);
		}
		else if (node.leaf_count == 3)
		{
			els = node.leaves[2];
			if (els.token.token_code == T_IF)
			{
				o = jep_if(els, list);
			}
			else if (els.token.token_code == T_ELSE && els.leaf_count == 1)
			{
				/* add a list for scope */
				jep_obj *scope = jep_create_object();
				scope->type = JEP_LIST;
				jep_add_object(list, scope);

				o = jep_evaluate(els.leaves[0], list);

				/* remove the argument list from the main list */
				jep_remove_scope(list);
				jep_destroy_list(scope);
				free(scope);
			}
		}
		jep_destroy_object(c);
	}

	return o;
}

/* evaluates a switch statement */
jep_obj* jep_switch(jep_ast_node node, jep_obj* list)
{
	jep_obj *o = NULL;

	jep_ast_node exp;  /* the switch expression            */
	jep_ast_node body; /* the body of the switch statement */

	exp = node.leaves[0].leaves[0];
	body = node.leaves[1];

	jep_obj *check = jep_evaluate(exp, list);

	if (check == NULL)
	{
		return o;
	}

	if (check->ret && check->ret & JEP_EXCEPTION)
	{
		return check;
	}

	/* add a list for scope */
	jep_obj *scope = jep_create_object();
	scope->type = JEP_LIST;
	jep_add_object(list, scope);

	int match = 0;
	int i;
	for (i = 0; i < body.leaf_count && !match; i++)
	{
		int j;
		for (j = 0; j < body.leaves[i].leaf_count && !match; j++)
		{
			if (body.leaves[i].token.token_code == T_DEFAULT)
			{
				match = 1;
				o = jep_brace(body.leaves[i], list);
			}
			else
			{
				if (body.leaves[i].leaves[j].token.token_code == T_DEFAULT)
				{
					match = 1;
					o = jep_brace(body.leaves[i].leaves[j], list);
				}
				else
				{
					jep_obj *cond = jep_evaluate(body.leaves[i].leaves[j], list);
					if (cond != NULL && cond->ret && cond->ret & JEP_EXCEPTION)
					{
						/* remove the argument list from the main list */
						jep_remove_scope(list);
						jep_destroy_list(scope);
						free(scope);

						jep_destroy_object(check);

						return cond;
					}

					if (jep_compare_object(check, cond))
					{
						match = 1;
						o = jep_brace(body.leaves[i].leaves[body.leaves[i].leaf_count - 1], list);
					}

					jep_destroy_object(cond);
				}
			}
		}
	}

	/* remove the argument list from the main list */
	jep_remove_scope(list);
	jep_destroy_list(scope);
	free(scope);

	jep_destroy_object(check);

	return o;
}

/* evaluates an for loop */
jep_obj *jep_for(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;
	jep_obj *scope;

	jep_ast_node head = node.leaves[0];
	jep_obj *cond = NULL;
	jep_ast_node index_node;
	jep_ast_node cond_node;
	jep_ast_node change_node;

	if (node.loop & JEP_INDEX)
	{
		index_node = head.leaves[0];
		jep_obj* index_obj = jep_evaluate(index_node, list);
		if (index_obj != NULL)
		{
			jep_destroy_object(index_obj);
		}
	}

	if (node.loop & JEP_CONDITION)
	{
		if (node.loop & JEP_INDEX)
		{
			cond_node = head.leaves[1];
		}
		else
		{
			cond_node = head.leaves[0];
		}
	}

	if (node.loop & JEP_CHANGE)
	{
		if (node.loop & JEP_INDEX && node.loop & JEP_CONDITION)
		{
			change_node = head.leaves[2];
		}
		else if (node.loop & JEP_INDEX || node.loop & JEP_CONDITION)
		{
			change_node = head.leaves[1];
		}
		else
		{
			change_node = head.leaves[0];
		}
	}

	if (node.loop & JEP_CONDITION)
	{
		cond = jep_evaluate(cond_node, list);
		if (cond != NULL && cond->type == JEP_INT)
		{
			int val = 0;
			if (cond != NULL && cond->val != NULL)
			{
				val = *((int *)(cond->val));
				jep_destroy_object(cond);
				cond = NULL;
			}
			while (val)
			{
				/* create the scope for this iteration */
				scope = jep_create_object();
				scope->type = JEP_LIST;
				jep_add_object(list, scope);

				if (node.leaf_count == 2)
				{
					o = jep_evaluate(node.leaves[1], list);
					if (o != NULL && o->ret)
					{
						jep_remove_scope(list);
						jep_destroy_object(scope);
						scope = NULL;
						return o;
					}
					else if (o != NULL)
					{
						jep_destroy_object(o);
						o = NULL;
					}
				}
				if (node.loop & JEP_CHANGE)
				{
					jep_obj* change_obj = jep_evaluate(change_node, list);
					if (change_obj != NULL)
					{
						jep_destroy_object(change_obj);
					}
				}

				cond = jep_evaluate(cond_node, list);
				if (cond != NULL && cond->val != NULL)
				{
					val = *((int *)(cond->val));
					jep_destroy_object(cond);
					cond = NULL;
				}

				/* destroy the scope at the end of each iteration */
				jep_remove_scope(list);
				jep_destroy_object(scope);
				scope = NULL;
			}
		}
		else
		{
			printf("invalid loop condition expression\n");
		}
	}
	else
	{
		while (1)
		{
			/* create the scope for this iteration */
			scope = jep_create_object();
			scope->type = JEP_LIST;
			jep_add_object(list, scope);

			if (node.leaf_count == 2)
			{
				o = jep_evaluate(node.leaves[1], list);
				if (o != NULL && o->ret)
				{
					jep_remove_scope(list);
					jep_destroy_object(scope);
					return o;
				}
				else if (o != NULL)
				{
					jep_destroy_object(o);
					o = NULL;
				}
			}
			if (node.loop & JEP_CHANGE)
			{
				jep_obj* change_obj = jep_evaluate(change_node, list);
				if (change_obj != NULL)
				{
					jep_destroy_object(change_obj);
				}
			}

			/* destroy the scope at the end of each iteration */
			jep_remove_scope(list);
			jep_destroy_object(scope);
			scope = NULL;
		}
	}

	return o;
}

/* evaluates a while loop */
jep_obj *jep_while(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;
	jep_obj *scope = NULL;

	jep_ast_node head = node.leaves[0];
	jep_obj *cond = NULL;
	jep_ast_node cond_node;

	cond_node = head.leaves[0];

	cond = jep_evaluate(cond_node, list);
	if (cond != NULL && cond->type == JEP_INT)
	{
		int val = 0;
		if (cond != NULL && cond->val != NULL)
		{
			val = *((int *)(cond->val));
			jep_destroy_object(cond);
			cond = NULL;
		}
		while (val)
		{
			/* create the scope for this iteration */
			scope = jep_create_object();
			scope->type = JEP_LIST;
			jep_add_object(list, scope);

			if (node.leaf_count == 2)
			{
				o = jep_evaluate(node.leaves[1], list);
				if (o != NULL && o->ret)
				{
					jep_remove_scope(list);
					jep_destroy_object(scope);
					scope = NULL;
					return o;
				}
				else if (o != NULL)
				{
					jep_destroy_object(o);
					o = NULL;
				}
			}

			cond = jep_evaluate(cond_node, list);
			if (cond != NULL && cond->val != NULL)
			{
				val = *((int *)(cond->val));
				jep_destroy_object(cond);
				cond = NULL;
			}

			/* destroy the scope at the end of each iteration */
			jep_remove_scope(list);
			jep_destroy_object(scope);
			scope = NULL;
		}
	}
	else
	{
		printf("invalid loop condition expression\n");
	}

	return o;
}

/* evaluates a try/catch block */
jep_obj* jep_try(jep_ast_node node, jep_obj* list)
{
	jep_obj *o = NULL;
	jep_obj *scope = NULL;

	jep_ast_node try_body = node.leaves[0];
	jep_ast_node ex = node.leaves[1].leaves[0];
	jep_ast_node catch_body = node.leaves[1].leaves[1];

	/* create the scope for the try block */
	scope = jep_create_object();
	scope->type = JEP_LIST;
	jep_add_object(list, scope);

	/* evaluate the body of the try block */
	o = jep_evaluate(try_body, list);

	/* destroy the scope for the try block */
	jep_remove_scope(list);
	jep_destroy_object(scope);
	scope = NULL;

	if (o != NULL && !o->ret)
	{
		jep_destroy_object(o);
		o = NULL;
	}
	else if (o != NULL)
	{
		if (o->ret & JEP_EXCEPTION)
		{
			jep_obj *exception = jep_create_object();
			exception->ident = ex.token.val->buffer;
			jep_copy_object(exception, o);
			jep_destroy_object(o);
			o = NULL;

			/* create the scope for the catch block */
			scope = jep_create_object();
			scope->type = JEP_LIST;
			jep_add_object(list, scope);

			/* add the exception to the scope */
			jep_add_object(list, exception);

			/* evaluate the body of the catch block */
			o = jep_evaluate(catch_body, list);

			/* destroy the scope for the catch block */
			jep_remove_scope(list);
			jep_destroy_object(scope);
			scope = NULL;
		}
	}

	if (o != NULL && !o->ret)
	{
		jep_destroy_object(o);
		o = NULL;
	}

	return o;
}

/* evaluates a throw statement */
jep_obj* jep_throw(jep_ast_node node, jep_obj* list)
{
	jep_obj *o = NULL;

	o = jep_evaluate(node.leaves[0], list);
	o->ret = JEP_RETURN | JEP_EXCEPTION;

	return o;
}

/* checks if a struct has a data member with the specfied identifier */
int jep_has_data_member(jep_obj *members, const char *ident)
{
	int has = 0;

	if (members->size > 0)
	{
		jep_obj *mem = members->head;
		while (mem != NULL)
		{
			if (!strcmp(mem->ident, ident))
			{
				has = 1;
			}
			mem = mem->next;
		}
	}

	return has;
}

/* evaluates a structure definition */
jep_obj *jep_struct(jep_ast_node node, jep_obj *list)
{
	jep_obj *struc = NULL;
	jep_obj *copy = NULL;
	int dup = 0;

	if (node.leaf_count != 2)
	{
		return struc;
	}

	struc = jep_create_object();
	struc->type = JEP_STRUCTDEF;

	struc->ident = node.leaves[0].token.val->buffer;

	jep_obj *members = jep_create_object();
	members->type = JEP_LIST;

	if (node.leaves[1].leaf_count > 0)
	{
		int i;
		for (i = 0; i < node.leaves[1].leaf_count && !dup; i++)
		{
			if (!jep_has_data_member(members,
				node.leaves[1].leaves[i].token.val->buffer))
			{
				jep_obj *mem = jep_create_object();
				mem->type = JEP_NULL;
				mem->ident = node.leaves[1].leaves[i].token.val->buffer;
				jep_add_object(members, mem);
			}
			else
			{
				printf("duplicate member identifiers\n");
				dup = 1;
			}
		}
	}

	struc->val = members;

	if (dup)
	{
		jep_destroy_object(struc);
		return NULL;
	}

	copy = jep_create_object();

	jep_add_object(list, struc);
	jep_copy_object(copy, struc);

	return copy;
}

/* creates a new instance of a certain type of object */
jep_obj *jep_new(jep_ast_node node, jep_obj *list)
{
	jep_obj *new_obj = NULL;
	jep_obj *struct_def;
	jep_obj *def_members;
	jep_obj *def_mem;
	jep_obj *members;
	jep_obj* init = NULL;

	if (node.leaf_count != 1 && node.leaf_count != 2)
	{
		return new_obj;
	}

	struct_def = jep_get_object(node.leaves[0].token.val->buffer, list);

	/* ensure that the structure definition exists */
	if (struct_def == NULL || struct_def->type != JEP_STRUCTDEF)
	{
		printf("no structure definition of type %s\n",
			node.token.val->buffer);
		return new_obj;
	}

	/* get the values from a structure initialization */
	if (node.leaf_count == 2)
	{
		init = jep_evaluate(node.leaves[1], list);
	}

	new_obj = jep_create_object();
	new_obj->type = JEP_STRUCT;

	members = jep_create_object();
	members->type = JEP_LIST;

	def_members = (jep_obj *)(struct_def->val);

	def_mem = def_members->head;
	if (init != NULL && init->size > 0)
	{
		jep_obj* init_obj = ((jep_obj*)(init->val))->head;
		while (def_mem != NULL && init_obj != NULL)
		{
			jep_obj *mem = jep_create_object();
			mem->ident = def_mem->ident;
			jep_copy_object(mem, init_obj);
			mem->index = -2;
			jep_add_object(members, mem);

			def_mem = def_mem->next;
			init_obj = init_obj->next;
		}

		jep_destroy_object(init);
	}
	else
	{
		while (def_mem != NULL)
		{
			jep_obj *mem = jep_create_object();
			mem->type = JEP_NULL;
			mem->ident = def_mem->ident;
			mem->index = -2;
			jep_add_object(members, mem);

			def_mem = def_mem->next;
		}
	}

	new_obj->val = members;

	return new_obj;
}

/* accesses members of an object */
jep_obj *jep_member(jep_ast_node node, jep_obj *list)
{
	if (node.leaf_count != 2)
	{
		printf("invalid leaf_count for data member access\n");
		return NULL;
	}

	jep_obj *mem;
	jep_obj *struc;
	jep_obj *members;

	struc = jep_evaluate(node.leaves[0], list);

	if (struc == NULL)
	{
		printf("could not obtain object with identifier %s\n",
			node.leaves[0].token.val->buffer);
		return NULL;
	}
	else if (struc->type != JEP_STRUCT)
	{
		printf("%s is not a struct\n",
			node.leaves[0].token.val->buffer);
		jep_destroy_object(struc);
		return NULL;
	}

	if (node.leaves[1].token.type != T_IDENTIFIER)
	{
		printf("an identifier must be used to access data members\n");
		jep_destroy_object(struc);
		return NULL;
	}

	members = (jep_obj *)(struc->val);
	mem = NULL;

	if (members->size > 0)
	{
		jep_obj *m = members->head;
		int found = 0;
		while (m != NULL && mem == NULL && !found)
		{
			if (!strcmp(m->ident, node.leaves[1].token.val->buffer))
			{
				mem = jep_create_object();
				jep_copy_object(mem, m);
				mem->index = -2;
				mem->ident = m->ident;
				jep_copy_self(mem, m);
				found = 1;
			}
			m = m->next;
		}
		if (mem == NULL)
		{
			printf("%s does not have a member with the identifier %s\n",
				struc->ident, node.leaves[1].token.val->buffer);
		}
	}
	else
	{
		printf("returning NULL here for some reason\n");
		jep_destroy_object(struc);
		return NULL;
	}

	jep_destroy_object(struc);

	return mem;
}

/* evaluates a modifier chain */
jep_obj *jep_modifier(jep_ast_node node, jep_obj *list)
{
	jep_obj *o = NULL;
	int mod = node.mod;
	jep_ast_node exp;

	if (node.leaf_count < 1)
	{
		return o;
	}

	exp = node.leaves[node.leaf_count - 1];

	if (exp.leaf_count > 0 && exp.token.token_code == T_COMMA)
	{
		o = jep_mod_sequence(exp, list, mod);
	}
	else
	{
		o = jep_evaluate_local(exp, list, mod);
	}

	return o;
}

/* evaluates a comma-delimited sequence of modified expressions */
jep_obj* jep_mod_sequence(jep_ast_node node, jep_obj *list, int mod)
{
	jep_obj *o = NULL;
	jep_ast_node l = node.leaves[0]; /* left operand  */
	jep_ast_node r = node.leaves[1]; /* right operand */

	/*
	 * modified expressions can only be assignments or declarations.
	 * each node in the sequence must be either an identifier, or an
	 * assignment with an identifier as its left operand.
	 */

	if (l.token.token_code == T_COMMA)
	{
		o = jep_mod_sequence(l, list, mod);
	}
	else
	{
		o = jep_evaluate_local(l, list, mod);
	}

	if (o != NULL)
	{
		if (o->ret & JEP_EXCEPTION)
		{
			return o;
		}
		jep_destroy_object(o);
	}

	if (r.token.token_code == T_COMMA)
	{
		o = jep_mod_sequence(r, list, mod);
	}
	else
	{
		o = jep_evaluate_local(r, list, mod);
	}

	return o;
}

/* evaluates an AST node within a certain scope*/
jep_obj *jep_evaluate_local(jep_ast_node ast, jep_obj *list, int mod)
{
	jep_obj *o = NULL;

	if (list == NULL)
	{
		return o;
	}

	/*
	 * modifiers
	 * 1 - local
	 * 2 - constant
	 */

	if (ast.token.type == T_IDENTIFIER)
	{
		/* get the current scope */
		jep_obj *scope = list;
		while (scope->tail != NULL && scope->tail->type == JEP_LIST)
		{
			scope = scope->tail;
		}

		/* get any existing object in the current scope */
		jep_obj *existing = jep_get_object(ast.token.val->buffer, scope);

		if (existing == NULL)
		{
			if (mod & 3)
			{
				jep_obj *local = jep_create_object();
				local->type = JEP_NULL;
				local->ident = ast.token.val->buffer;
				jep_add_object(scope, local);
				jep_obj *con = jep_get_object(local->ident, scope);
				con->mod = mod;
			}
		}
		else if (mod & 1 && !(mod & 2))
		{
			jep_obj *local = jep_create_object();
			local->type = JEP_NULL;
			local->ident = ast.token.val->buffer;
			jep_add_object(scope, local);
			jep_obj *con = jep_get_object(local->ident, scope);
			con->mod = mod;
		}
		else
		{
			printf("the object %s has already been declared in this scope\n",
				ast.token.val->buffer);
		}
	}
	else if (ast.token.token_code == T_EQUALS)
	{
		if (ast.leaf_count != 2)
		{
			return o;
		}

		if (ast.leaves[0].token.type != T_IDENTIFIER)
		{
			printf("invalid local declaration. expected identifier, found %s\n",
				ast.leaves[0].token.val->buffer);
		}

		/* get the current scope */
		jep_obj *scope = list;
		while (scope->tail != NULL && scope->tail->type == JEP_LIST)
		{
			scope = scope->tail;
		}

		/* get any existing object in the current scope */
		jep_obj *existing = jep_get_object(ast.leaves[0].token.val->buffer, scope);

		if (existing == NULL)
		{
			if (mod & 3)
			{
				jep_obj *local = jep_create_object();
				local->type = JEP_NULL;
				local->ident = ast.leaves[0].token.val->buffer;
				jep_add_object(scope, local);
				o = jep_assign(ast, list);
				if (o != NULL && (o->ret & JEP_EXCEPTION))
				{
					jep_pop_object(scope);
					jep_destroy_object(local);
					return o;
				}
				else if (o != NULL)
				{
					jep_obj *con = jep_get_object(local->ident, scope);
					con->mod = mod;
				}
			}
		}
		else if (mod & 1 && !(mod & 2))
		{
			jep_obj *local = jep_create_object();
			local->type = JEP_NULL;
			local->ident = ast.leaves[0].token.val->buffer;
			jep_add_object(scope, local);
			o = jep_assign(ast, list);
			if (o != NULL && (o->ret & JEP_EXCEPTION))
			{
				return o;
			}
			jep_obj *con = jep_get_object(local->ident, scope);
			con->mod = mod;
		}
		else
		{
			printf("the object %s has already been declared in this scope\n",
				ast.leaves[0].token.val->buffer);
		}
	}

	if (o != NULL && o->ret)
	{
		o->ret ^= JEP_RETURN;
	}

	return o;
}
