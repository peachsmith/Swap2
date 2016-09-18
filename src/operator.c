#include "operator.h"

/* evaluates the nodes of an AST */
jep_obj* jep_evaluate(jep_ast_node ast, jep_obj* list)
{
	jep_obj* o = NULL;

	if(ast.token.type == T_NUMBER)
	{
		return jep_number(ast.token.val->buffer);
	}
	else if(ast.token.type == T_CHARACTER)
	{
		return jep_character(ast.token.val->buffer);
	}
	else if(ast.token.type == T_STRING)
	{
		return jep_string(ast.token.val->buffer);
	}
	else if(ast.token.type == T_IDENTIFIER)
	{
		return jep_get_object(ast.token.val->buffer, list);
	}

	switch(ast.token.token_code)
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

		case T_LPAREN:
			o = jep_paren(ast, list);
			break;

		case T_LBRACE:
			o = jep_brace(ast, list);
			break;

		case T_EQUALS:
			o = jep_assign(ast, list);
			break;

		default:
			printf("unrecognized token: %s\n", 
				ast.token.val->buffer);
			break;
	}

	return o;
}

/* evaluates an addition expression */
jep_obj* jep_add(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type != JEP_INT && l->type != JEP_LONG
			&& l->type != JEP_DOUBLE && r->type != JEP_INT
			&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
		{
			printf("invalid operand types for operation +\n");
		}
		else if(l->type == r->type)
		{
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) + (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) + (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) + (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_DOUBLE;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) + (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) + (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) + (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) + (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) + (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) + (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) + (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) + (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_LONG;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates subtraction or negation */
jep_obj* jep_sub(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2 && node.leaf_count != 1)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);

	if(node.leaf_count > 1)
	{
		r = jep_evaluate(node.leaves[1], list);
	}

	if(node.leaf_count == 1 && l != NULL)
	{
		if(l->type != JEP_INT && l->type != JEP_LONG
			&& l->type != JEP_DOUBLE)
		{
			printf("invalid operand types for operation -\n");
		}
		if(l->type == JEP_INT)
		{
			int *n = malloc(sizeof(int));
			*n = 0 - (*(int*)(l->val));
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG)
		{
			long *n = malloc(sizeof(int));
			*n = 0 - (*(long*)(l->val));
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_LONG;
		}
		else if(l->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			*n = 0 - (*(double*)(l->val));
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_DOUBLE;
		}
	}
	else if(l != NULL && r != NULL)
	{
		if(l->type != JEP_INT && l->type != JEP_LONG
			&& l->type != JEP_DOUBLE && r->type != JEP_INT
			&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
		{
			printf("invalid operand types for operation -\n");
		}
		if(l->type == r->type)
		{
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) - (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) - (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) - (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_DOUBLE;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) - (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) - (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) - (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) - (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) - (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) - (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) - (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) - (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_LONG;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates a multiplication expression */
jep_obj* jep_mul(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type != JEP_INT && l->type != JEP_LONG
			&& l->type != JEP_DOUBLE && r->type != JEP_INT
			&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
		{
			printf("invalid operand types for operation *\n");
		}
		if(l->type == r->type)
		{
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) * (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) * (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) * (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_DOUBLE;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) * (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) * (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) * (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) * (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			printf("multiplying a long with an int\n");
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) * (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) * (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) * (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) * (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_LONG;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates a division expression */
jep_obj* jep_div(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation /\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) / (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) / (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) / (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_DOUBLE;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) / (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) / (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) / (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) / (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) / (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) / (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) / (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) / (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_LONG;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates a less than expression */
jep_obj* jep_less(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) < (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) < (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) < (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) < (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) < (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) < (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) < (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) < (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) < (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) < (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) < (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates a less than expression */
jep_obj* jep_greater(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) > (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) > (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) > (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) > (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) > (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) > (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) > (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) > (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) > (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) > (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) > (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates a less than or equal to expression */
jep_obj* jep_lorequal(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) <= (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) <= (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) <= (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) <= (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) <= (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) <= (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) <= (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) <= (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) <= (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) <= (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) <= (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates a greater than or equal to expression */
jep_obj* jep_gorequal(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) >= (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) >= (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) >= (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) >= (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) >= (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) >= (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) >= (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) >= (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) >= (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) >= (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) >= (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates an equivalence expression */
jep_obj* jep_equiv(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) == (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) == (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) == (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) == (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) == (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) == (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) == (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) == (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) == (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) == (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) == (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;	
}

/* evaluates a not equivalence expression */
jep_obj* jep_noteq(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) != (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) != (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->val)) != (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) != (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) != (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) != (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) != (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) != (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) != (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) != (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) != (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a not operation */
jep_obj* jep_not(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* operand  */
	jep_obj* result = NULL;  /* result   */

	if(node.leaf_count != 1)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);

	if(l != NULL)
	{
		if(l->type != JEP_INT && l->type != JEP_LONG
			&& l->type != JEP_DOUBLE)
		{
			printf("invalid operand types for operation -\n");
		}
		if(l->type == JEP_INT)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(int*)(l->val));
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(long*)(l->val));
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(double*)(l->val));
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain operand val\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}

	return result;
}

/* performs a logical and operation */
jep_obj* jep_and(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) && (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) && (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->val)) && (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) && (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) && (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) && (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) && (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) && (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) && (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) && (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) && (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a logical or operation */
jep_obj* jep_or(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) || (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) || (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->val)) || (*(double*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_DOUBLE)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(double*)(l->val)) || (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->val)) || (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) || (*(double*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) || (*(double*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) || (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) || (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) || (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) || (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a bitwise operation */
jep_obj* jep_bitand(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) & (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) & (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			printf("invalid bitwise operands\n");
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) & (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) & (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) & (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) & (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a bitwise or operation */
jep_obj* jep_bitor(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) | (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) | (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			printf("invalid bitwise operands\n");
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) | (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) | (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) | (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) | (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a bitwise xor operation */
jep_obj* jep_bitxor(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) ^ (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) ^ (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			printf("invalid bitwise operands\n");
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) ^ (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) ^ (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) ^ (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) ^ (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a left bit shift operation */
jep_obj* jep_lshift(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) << (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) << (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			printf("invalid bitwise operands\n");
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) << (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) << (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) << (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) << (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* performs a right bit shift operation */
jep_obj* jep_rshift(jep_ast_node node, jep_obj* list)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(l != NULL && r != NULL)
	{
		if(l->type == r->type)
		{
			if(l->type != JEP_INT && l->type != JEP_LONG
				&& l->type != JEP_DOUBLE && r->type != JEP_INT
				&& r->type != JEP_LONG && r->type != JEP_DOUBLE)
			{
				printf("invalid operand types for operation <\n");
			}
			if(l->type == JEP_INT)
			{
				int *n = malloc(sizeof(int));
				*n = (*(int*)(l->val)) >> (*(int*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) >> (*(long*)(r->val));
				result = malloc(sizeof(jep_obj));
				result->val = (void*)n;
				result->type = JEP_INT;
			}
		}
		else if(l->type == JEP_DOUBLE || r->type == JEP_DOUBLE)
		{
			printf("invalid bitwise operands\n");
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->val)) >> (*(int*)(r->val));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) >> (*(long*)(r->val));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->val)) >> (*(long*)(r->val));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->val)) >> (*(long*)(r->val));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->val = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->val);
		free(l);
	}
	if(r != NULL)
	{
		free(r->val);
		free(r);
	}

	return result;
}

/* evaluates an assignment */
jep_obj* jep_assign(jep_ast_node node, jep_obj* list)
{
	// printf("BEGIN assignment\n");
	jep_obj* o = NULL; /* the recipient of the assignment */
	jep_obj* l = NULL; /* left operand                    */
	jep_obj* r = NULL; /* right oeprand                   */

	if(node.leaf_count != 2)
	{
		return NULL;
	}

	// printf("evaluating left operand\n");
	l = jep_evaluate(node.leaves[0], list);
	// printf("evaluating right operand\n");
	r = jep_evaluate(node.leaves[1], list);
	// printf("evaluated both operands without segmentation fault\n");

	// if(l == NULL)
	// {
	// 	printf("left operand is NULL\n");
	// }
	// else
	// {
	// 	printf("left operand is not NULL\n");
	// }

	// if(r == NULL)
	// {
	// 	printf("right operand is NULL\n");
	// }
	// else
	// {
	// 	printf("right operand is not NULL\n");
	// }

	if((l != NULL || node.leaves[0].token.type == T_IDENTIFIER) 
		&& (r != NULL || node.leaves[1].token.type == T_IDENTIFIER))
	{
		if(l == NULL)
		{
			o = jep_get_object(node.leaves[0].token.val->buffer, list);
		}
		else
		{
			o = jep_get_object(l->ident, list);	
		}

		if(o == NULL)
		{
			/* create the object if it doesn't exist */
			o = jep_create_object();
			o->ident = node.leaves[0].token.val->buffer;
			jep_add_object(list, o);
		}
		else if(o->val != NULL)
		{
			/* free existing memory */
			free(o->val);
		}
		o->type = r->type;
		if(r->type == JEP_ARRAY)
		{
			o->val = r;
		}
		else
		{
			o->val = r->val;
		}
		
		/* all used objects must have identifiers */
		if(r->ident == NULL)
		{
			r->ident = o->ident;
		}
	}
	else
	{
		printf("could not obtain both operand values\n");
	}

	/* free memory of unused left operands */
	if(l != NULL && l->ident == NULL)
	{
		free(l->val);
		free(l);
	}

	return r;
}

/* evaluates the contents of a set of parentheses */
jep_obj* jep_paren(jep_ast_node node, jep_obj* list)
{
	if(node.leaves == NULL)
	{
		return NULL;
	}

	jep_obj* o = jep_evaluate(node.leaves[0], list);

	int i;
	for(i = 1; i < node.leaf_count; i++)
	{
		if(o->val != NULL)
		{
			free(o->val);
		}
		if(o != NULL)
		{
			free(o);
		}
		o = jep_evaluate(node.leaves[i], list);
	}

	return o;
}

/* evaluates a block of code in curly braces */
jep_obj* jep_brace(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	if(node.array)
	{
		o = jep_create_object();
		o->type = JEP_ARRAY;
		if(node.leaf_count > 0 && node.leaves[0].token.token_code == T_COMMA)
		{
			jep_ast_node delim = node.leaves[0];
			while(delim.token.token_code == T_COMMA)
			{
				jep_obj* e;
				e = jep_evaluate(delim.leaves[1], list);
				jep_add_object(o, e);
				delim = delim.leaves[0];
			}
			jep_obj* e;
			e = jep_evaluate(delim, list);
			jep_add_object(o, e);
		}
		else if(node.leaf_count == 1)
		{
			jep_obj* e = jep_evaluate(node.leaves[0], list);
			jep_add_object(o, e);
		}
	}
	else
	{
		int i;
		for(i = 0; i < node.leaf_count; i++)
		{
			o = jep_evaluate(node.leaves[i], list);
			if(o != NULL)
			{
				free(o->val);
				free(o);
			}
		}
		o = NULL;
	}

	return o;
}
