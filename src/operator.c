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
		jep_obj* e = jep_get_object(ast.token.val->buffer, list);
		if(e != NULL)
		{
			o = jep_create_object();
			jep_copy_object(o, e);
			o->ident = e->ident;
		}
		return o;
	}
	else if(ast.token.type == T_KEYWORD)
	{
		if(ast.token.token_code == T_FUNCTION)
		{
			return jep_function(ast, list);
		}
		else if(ast.token.token_code == T_RETURN)
		{
			return jep_return(ast, list);
		}
		else if(ast.token.token_code == T_IF)
		{
			return jep_if(ast, list);
		}
		else if(ast.token.token_code == T_FOR)
		{
			return jep_for(ast, list);
		}
		else if(ast.token.token_code == T_WHILE)
		{
			return jep_while(ast, list);
		}
	}
	else if(ast.token.type == T_MODIFIER)
	{
		return jep_modifier(ast, list);
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
		/* string concatenation */
		{
			if(l->type == JEP_STRING || r->type == JEP_STRING)
			{
				char* l_str = jep_to_string(l);
				char* r_str = jep_to_string(r);

				if(l_str != NULL && r_str != NULL)
				{
					result = jep_create_object();
					result->type = JEP_STRING;

					char* str = malloc(strlen(l_str) + strlen(r_str));
					strcpy(str, l_str);
					strcat(str, r_str);
					result->val = (void*)(str);
				}

				if(l_str != NULL)
				{
					free(l_str);
				}

				if(r_str != NULL)
				{
					free(r_str);
				}

				return result;
			}
		}

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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) + (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) + (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG)
		{
			long *n = malloc(sizeof(int));
			*n = 0 - (*(long*)(l->val));
			result = jep_create_object();
			result->val = (void*)n;
			result->type = JEP_LONG;
		}
		else if(l->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			*n = 0 - (*(double*)(l->val));
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) - (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) - (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) * (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) * (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) / (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) / (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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

/* evaluates a modulus expression */
jep_obj* jep_modulus(jep_ast_node node, jep_obj* list)
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
		if(l->type == r->type && l->type == JEP_INT)
		{
			int *n = malloc(sizeof(int));
			*n = (*(int*)(l->val)) % (*(int*)(r->val));
			result = jep_create_object();
			result->val = (void*)n;
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) < (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) < (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) > (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) > (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) <= (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) <= (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) >= (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) >= (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) == (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->val)) == (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) != (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->val)) != (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(long*)(l->val));
			result = jep_create_object();
			result->val = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(double*)(l->val));
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) && (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->val)) && (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) || (*(long*)(r->val));
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->val)) || (*(double*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) & (*(long*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) | (*(long*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) ^ (*(long*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) << (*(long*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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
				result = jep_create_object();
				result->val = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->val)) >> (*(long*)(r->val));
				result = jep_create_object();
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
			result = jep_create_object();
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

/* performs an increment on an integer */
jep_obj* jep_inc(jep_ast_node node, jep_obj* list)
{
	if(node.leaf_count != 1)
	{
		return NULL;
	}

	jep_obj* o = NULL;
	jep_obj* obj = jep_evaluate(node.leaves[0], list);

	if(obj != NULL)
	{
		if(obj->ident == NULL || obj->type != JEP_INT || obj->val == NULL)
		{
			printf("invalid operand\n");
			return NULL;
		}
		else
		{
			jep_obj* actual = jep_get_object(obj->ident, list);
			o = jep_create_object();
			
			int cur_val = *(int*)(actual->val);
			int new_val = cur_val + 1;
			*(int*)(actual->val) = new_val;

			jep_copy_object(o, actual);

			if(node.token.postfix)
			{
				*(int*)(o->val) = cur_val;;
			}
		}
	}

	return o;
}

/* performs a decrement on an integer */
jep_obj* jep_dec(jep_ast_node node, jep_obj* list)
{
	if(node.leaf_count != 1)
	{
		return NULL;
	}

	jep_obj* o = NULL;
	jep_obj* obj = jep_evaluate(node.leaves[0], list);

	if(obj != NULL)
	{
		if(obj->ident == NULL || obj->type != JEP_INT || obj->val == NULL)
		{
			printf("invalid operand\n");
			return NULL;
		}
		else
		{
			jep_obj* actual = jep_get_object(obj->ident, list);
			o = jep_create_object();
			
			int cur_val = *(int*)(actual->val);
			int new_val = cur_val - 1;
			*(int*)(actual->val) = new_val;

			jep_copy_object(o, actual);

			if(node.token.postfix)
			{
				*(int*)(o->val) = cur_val;;
			}
		}
	}

	return o;
}

/* evaluates an assignment */
jep_obj* jep_assign(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL; /* the recipient of the assignment */
	jep_obj* l = NULL; /* left operand                    */
	jep_obj* r = NULL; /* right oeprand                   */

	if(node.leaf_count != 2)
	{
		return NULL;
	}

	l = jep_evaluate(node.leaves[0], list);
	r = jep_evaluate(node.leaves[1], list);

	if(r != NULL 
		&& (l != NULL || node.leaves[0].token.type == T_IDENTIFIER))
	{
		if(l == NULL)
		{
			o = jep_get_object(node.leaves[0].token.val->buffer, list);
		}
		else
		{
			if(l->index == -1)
			{
				o = jep_get_object(l->ident, list);
			}
			else
			{
				o = jep_get_element(node.leaves[0], list);
			}
		}

		if(o == NULL)
		{
			/* create the object if it doesn't exist */
			o = jep_create_object();
			o->ident = node.leaves[0].token.val->buffer;
			jep_add_object(list, o);
		}


		jep_copy_object(o, r);	


		if(r->type == JEP_ARRAY)
		{
			int i = 0;
			jep_obj* head = ((jep_obj*)(o->val))->head;
			while(head != NULL)
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
jep_obj* jep_paren(jep_ast_node node, jep_obj* list)
{
	if(!node.token.unary)
	{
		if(node.leaf_count == 1)
		{
			return jep_evaluate(node.leaves[0], list);
		}
		else
		{
			return NULL;
		}
	}

	jep_obj* o;        /* function return value    */
	jep_ast_node args; /* incoming arguments       */
	jep_obj* func;     /* function being called    */
	jep_obj* arg_list; /* list of argument objects */

	if(node.leaf_count == 0)
	{
		return NULL;
	}

	o = NULL;
	func = NULL;
	arg_list = NULL;

	/* collect the function arguments as objects */
	if(node.leaf_count == 2)
	{
		func = jep_get_object(node.leaves[1].token.val->buffer, list);
		args = node.leaves[0];
		arg_list = jep_create_object();
		arg_list->type = JEP_LIST;
		if(args.leaf_count > 1)
		{
			if(node.leaves[0].token.token_code == T_COMMA)
			{
				jep_sequence(node.leaves[0], list, arg_list);
			}
			else
			{
				jep_obj* a = jep_evaluate(node.leaves[0], list);
				if(a != NULL)
				{
					jep_add_object(arg_list, a);	
				}
				else
				{
					printf("could not evaluate argument\n");
				}
			}
		}
		else if(args.leaf_count == 0)
		{
			jep_obj* a = jep_evaluate(node.leaves[0], list);
			if(a != NULL)
			{
				jep_add_object(arg_list, a);		
			}
			else
			{
				printf("could not evaluate argument\n");
			}
		}
	}
	else if(node.leaf_count == 1)
	{
		func = jep_get_object(node.leaves[0].token.val->buffer, list);	
	}

	if(func != NULL)
	{
		jep_obj* fargs = func->head;
		jep_obj* farg = fargs->head;

		/* native function call */
		if(func->size == 1)
		{
			return jep_call_native(func->ident, arg_list);
		}

		jep_ast_node body = *((jep_ast_node*)(func->head->next->val));
		if(arg_list != NULL)
		{
			jep_obj* arg = arg_list->head;
			while(arg != NULL && farg != NULL)
			{
				arg->ident = farg->ident;
				farg = farg->next;
				arg = arg->next;
			}
			if(arg != NULL || farg != NULL)
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
			if(farg != NULL)
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

	jep_destroy_list(arg_list);
	free(arg_list);

	return o;
}

/* evaluates a block of code in curly braces */
jep_obj* jep_brace(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	if(node.array)
	{
		jep_obj* array = jep_create_object();
		array->type = JEP_LIST;
		o = jep_create_object();
		o->type = JEP_ARRAY;
		o->val = array;
		if(node.leaf_count > 0 && node.leaves[0].token.token_code == T_COMMA)
		{
			jep_sequence(node.leaves[0], list, array);
			int i = 0;
			jep_obj* elem = array->head;
			while(elem != NULL)
			{
				elem->index = i++;
				elem = elem->next;
			}
		}
		else if(node.leaf_count == 1)
		{
			jep_obj* e = jep_evaluate(node.leaves[0], list);
			e->index = 0;
			jep_add_object(array, e);
		}
		o->size = array->size;
	}
	else
	{
		int i;
		for(i = 0; i < node.leaf_count; i++)
		{
			o = jep_evaluate(node.leaves[i], list);
			if(o != NULL && o->ret)
			{
				return o;
			}
			jep_destroy_object(o);
			o = NULL;
		}
	}
	
	return o;
}

/* evaluates an array subscript */
jep_obj* jep_subscript(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	if(node.leaf_count != 2 && node.leaf_count != 1)
	{
		printf("invalid leaf count for ast node\n");
	}

	/* array initialization */
	if(node.leaf_count == 1)
	{
		o = jep_create_object();
		o->type = JEP_ARRAY;
		jep_obj* array = jep_create_object();
		array->type = JEP_LIST;
		o->val = array;

		jep_obj* size = jep_evaluate(node.leaves[0], list);

		if(size == NULL || size->type != JEP_INT || size->val == NULL)
		{
			if(size != NULL)
			{
				jep_destroy_object(size);
			}
			printf("invalid array size\n");
			jep_destroy_object(o);
			jep_destroy_object(array);
			o = NULL;
			return o;
		}

		int s = *((int*)(size->val));
		
		int i;
		for(i = 0; i < s; i++)
		{
			jep_obj* elem = jep_create_object();
			elem->type = JEP_ARGUMENT;
			elem->index = i;
			jep_add_object(array, elem);
		}

		o->size = array->size;

		return o;
	}

	/* array index access */

	jep_obj* index = jep_evaluate(node.leaves[0], list);
	jep_obj* array = jep_evaluate(node.leaves[1], list);

	if(index != NULL && array != NULL)
	{
		if(index->type != JEP_INT)
		{
			printf("array subscript must be an integer\n");
		}
		else if(array->type != JEP_ARRAY)
		{
			printf("cannot access index of non array object\n");
		}
		else if(array->size > 0)
		{
			jep_obj* contents = (jep_obj*)(array->val);
			jep_obj* elem = contents->head;
			int target = *(int*)(index->val);
			int i;
			for(i = 0; elem != NULL; i++)
			{
				if(i == target)
				{
					o = jep_create_object();
					jep_copy_object(o, elem);
					o->array_ident = elem->array_ident;
					o->index = elem->index;
				}
				elem = elem->next;
			}
			if(o == NULL)
			{
				printf("array index out of bounds\n");
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

/* gets the actual element from an array */
jep_obj* jep_get_element(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;

	if(list == NULL || node.leaf_count != 2)
	{
		return o;
	}

	jep_obj* index = jep_evaluate(node.leaves[0], list);
	jep_obj* array = NULL;
	jep_ast_node array_leaf = node.leaves[1];

	/* evaluate existing arrays and function call that return arrays */
	if(array_leaf.token.token_code == T_LSQUARE)
	{
		array = jep_get_element(array_leaf, list);
	}
	else
	{
		array = jep_evaluate(array_leaf, list);
		if(array != NULL)
		{
			/* get the actual array, not just a copy */
			if(array->type == JEP_ARRAY)
			{
				if(array->ident != NULL)
				{
					jep_obj* actual = jep_get_object(array->ident, list);
					jep_destroy_object(array);
					array = actual;	
				}
				else
				{
					/* 
					 * if an array doens't have an array
					 * identifier, then it is most likely
					 * a local object
					 */
				}
			}
			else
			{
				jep_destroy_object(array);
				array = NULL;
			}
		}
	}

	if(index != NULL && array != NULL)
	{
		if(index->type != JEP_INT)
		{
			printf("array subscript must be an integer\n");
		}
		else if(array->type != JEP_ARRAY)
		{
			printf("cannot access index of non array object\n");
		}
		else if(array->size > 0)
		{
			/* get the element from the actual array */
			jep_obj* elem = ((jep_obj*)(array->val))->head;
			int target = *(int*)(index->val);
			int i;
			for(i = 0; elem != NULL; i++)
			{
				if(i == target)
				{
					o = elem;
				}
				elem = elem->next;
			}
			if(o == NULL)
			{
				printf("array index out of bounds\n");
			}
		}
		else
		{
			printf("cannot access an element in an empty array\n");
		}
	}

	jep_destroy_object(index);

	return o;
}

/* evaluates a function definition */
jep_obj* jep_function(jep_ast_node node, jep_obj* list)
{
	jep_obj* copy = jep_create_object();
	jep_obj* func = jep_create_object();
	func->type = JEP_FUNCTION;
	func->ident = node.leaves[0].token.val->buffer;
	jep_obj* args = jep_create_object();
	jep_obj* body = jep_create_object();

	/* function arguments */
	int i;
	for(i = 0; i < node.leaves[1].leaf_count; i++)
	{
		jep_obj* a = jep_create_object();
		a->type = JEP_ARGUMENT;
		a->ident = node.leaves[1].leaves[i].token.val->buffer;
		jep_add_object(args, a);
	}

	if(node.leaf_count == 3)
	{
		/* function body */
		jep_ast_node* n = jep_create_ast_node();
		*n = node.leaves[2];
		body->val = n;
	}

	jep_add_object(func, args);

	if(node.leaf_count == 3)
	{
		jep_add_object(func, body);	
	}

	jep_add_object(list, func);
	jep_copy_object(copy, func);

	return copy;
}

/* returns from a function */
jep_obj* jep_return(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;

	if(node.leaf_count == 1)
	{
		o = jep_evaluate(node.leaves[0], list);
		o->ret = 1;
	}
	else if(node.leaf_count == 0)
	{
		o = jep_create_object();
		o->type = JEP_ARGUMENT;
		o->ret = 1;
	}

	return o;
}

/* evaluates a comma tree */
jep_obj* jep_comma(jep_ast_node node, jep_obj* list)
{
	jep_ast_node l = node.leaves[0]; /* left operand  */
	jep_ast_node r = node.leaves[1]; /* right operand */
	jep_obj* lo = NULL;              /* left object   */
	jep_obj* ro = NULL;              /* right object  */

	if(l.token.token_code == T_COMMA)
	{
		lo = jep_comma(l, list);
		jep_destroy_object(lo);
	}
	else
	{
		lo = jep_evaluate(l, list);
		jep_destroy_object(lo);
	}

	if(r.token.token_code == T_COMMA)
	{
		ro = jep_comma(r, list);
	}
	else
	{
		ro = jep_evaluate(r, list);
	}

	return ro;
}

/* evaluates a comma-delimited sequence of objects */
void jep_sequence(jep_ast_node node, jep_obj* list, jep_obj* seq)
{
	jep_ast_node l = node.leaves[0]; /* left operand  */
	jep_ast_node r = node.leaves[1]; /* right operand */
	jep_obj* lo = NULL;              /* left object   */
	jep_obj* ro = NULL;              /* right object  */

	if(l.token.token_code == T_COMMA)
	{
		jep_sequence(l, list, seq);
	}
	else
	{
		lo = jep_evaluate(l, list);
		jep_add_object(seq, lo);
	}

	if(r.token.token_code == T_COMMA)
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
jep_obj* jep_if(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	jep_ast_node cond; /* condition        */
	jep_ast_node body; /* body             */
	jep_ast_node els;  /* else and else if */

	if(node.leaf_count < 2)
	{
		return o;
	}

	cond = node.leaves[0];
	body = node.leaves[1];

	jep_obj* c = jep_evaluate(cond, list);

	if(c != NULL && c->type == JEP_INT)
	{
		int val = *((int*)(c->val));
		if(val)
		{
			/* add a list for scope */
			jep_obj* scope = jep_create_object();
			scope->type = JEP_LIST;

			jep_add_object(list, scope);

			o = jep_evaluate(body, list);

			/* remove the argument list from the main list */
			jep_remove_scope(list);
			jep_destroy_list(scope);
			free(scope);
		}
		else if(node.leaf_count == 3)
		{
			els = node.leaves[2];
			if(els.token.token_code == T_IF)
			{
				o = jep_if(els, list);
			}
			else if(els.token.token_code == T_ELSE && els.leaf_count == 1)
			{
				/* add a list for scope */
				jep_obj* scope = jep_create_object();
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

/* evaluates an for loop */
jep_obj* jep_for(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	jep_obj* scope = jep_create_object();
	scope->type = JEP_LIST;

	jep_add_object(list, scope);

	jep_ast_node head = node.leaves[0];
	jep_obj* cond = NULL;
	jep_ast_node index_node;
	jep_ast_node cond_node;
	jep_ast_node change_node;

	if(node.loop & JEP_INDEX)
	{
		index_node = head.leaves[0];
		jep_evaluate(index_node, list);
	}

	if(node.loop & JEP_CONDITION)
	{
		if(node.loop & JEP_INDEX)
		{
			cond_node = head.leaves[1];
		}
		else
		{
			cond_node = head.leaves[0];
		}
	}

	if(node.loop & JEP_CHANGE)
	{
		if(node.loop & JEP_INDEX && node.loop & JEP_CONDITION)
		{
			change_node = head.leaves[2];
		}
		else if(node.loop & JEP_INDEX || node.loop & JEP_CONDITION)
		{
			change_node = head.leaves[1];
		}
		else
		{
			change_node = head.leaves[0];
		}
	}

	if(node.loop & JEP_CONDITION)
	{
		cond = jep_evaluate(cond_node, list);
		if(cond != NULL && cond->type == JEP_INT)
		{
			int val = 0;
			if(cond != NULL && cond->val != NULL)
			{
				val = *((int*)(cond->val));
			}
			while(val)
			{
				if(node.leaf_count == 2)
				{
					o = jep_evaluate(node.leaves[1], list);
					if(o != NULL && o->ret)
					{
						jep_remove_scope(list);
						jep_destroy_list(scope);
						free(scope);
						return o;
					}
					else if(o != NULL)
					{
						jep_destroy_object(o);
						o = NULL;
					}
				}
				if(node.loop & JEP_CHANGE)
				{
					jep_evaluate(change_node, list);
				}

				cond = jep_evaluate(cond_node, list);
				if(cond != NULL && cond->val != NULL)
				{
					val = *((int*)(cond->val));
				}
			}
		}
		else
		{
			printf("invalid loop condition expression\n");
		}
	}
	else
	{
		while(1)
		{
			if(node.leaf_count == 2)
			{
				o = jep_evaluate(node.leaves[1], list);
				if(o != NULL && o->ret)
				{
					jep_remove_scope(list);
					jep_destroy_list(scope);
					free(scope);
					return o;
				}
				else if(o != NULL)
				{
					jep_destroy_object(o);
					o = NULL;
				}
			}
			if(node.loop & JEP_CHANGE)
			{
				jep_evaluate(change_node, list);
			}
		}
	}

	jep_remove_scope(list);
	jep_destroy_list(scope);
	free(scope);

	return o;
}

/* evaluates a while loop */
jep_obj* jep_while(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	jep_obj* scope = jep_create_object();
	scope->type = JEP_LIST;

	jep_add_object(list, scope);

	jep_ast_node head = node.leaves[0];
	jep_obj* cond = NULL;
	jep_ast_node cond_node;

	cond_node = head.leaves[0];

	cond = jep_evaluate(cond_node, list);
	if(cond != NULL && cond->type == JEP_INT)
	{
		int val = 0;
		if(cond != NULL && cond->val != NULL)
		{
			val = *((int*)(cond->val));
		}
		while(val)
		{
			if(node.leaf_count == 2)
			{
				o = jep_evaluate(node.leaves[1], list);
				if(o != NULL && o->ret)
				{
					jep_remove_scope(list);
					jep_destroy_list(scope);
					free(scope);
					return o;
				}
				else if(o != NULL)
				{
					jep_destroy_object(o);
					o = NULL;
				}
			}

			cond = jep_evaluate(cond_node, list);
			if(cond != NULL && cond->val != NULL)
			{
				val = *((int*)(cond->val));
			}
		}
	}
	else
	{
		printf("invalid loop condition expression\n");
	}

	jep_remove_scope(list);
	jep_destroy_list(scope);
	free(scope);

	return o;
}

/* evaluates a modifier chain */
jep_obj* jep_modifier(jep_ast_node node, jep_obj* list)
{
	jep_obj* o = NULL;
	int mod = node.mod;
	jep_ast_node exp;
	// jep_obj* seq;       /* modified expression */

	if(node.leaf_count < 1)
	{
		return o;
	}

	exp = node.leaves[node.leaf_count - 1];

	// seq = jep_create_object();
	// seq->type = JEP_LIST;

	jep_mod_sequence(exp, list, mod);

	return o;
}

/* evaluates a comma-delimited sequence of modified expressions */
void jep_mod_sequence(jep_ast_node node, jep_obj* list, int mod)
{
	jep_ast_node l = node.leaves[0]; /* left operand  */
	jep_ast_node r = node.leaves[1]; /* right operand */
	//jep_obj* lo = NULL;              /* left object   */
	//jep_obj* ro = NULL;              /* right object  */

	/*
	 * modified expressions can only be assignments or declarations.
	 * each node in the sequence must be either an identifier, or an 
	 * assignment with an identifier as its left operand.
	 */

	if(l.token.token_code == T_COMMA)
	{
		jep_mod_sequence(l, list, mod);
	}
	else
	{
		jep_evaluate_local(l, list, mod);
		// if(lo != NULL)
		// {
		// 	jep_add_object(seq, lo);
		// }
	}

	if(r.token.token_code == T_COMMA)
	{
		jep_mod_sequence(r, list, mod);
	}
	else
	{
		/* declaration */
		jep_evaluate_local(l, list, mod);
		// if(ro != NULL)
		// {
		// 	jep_add_object(seq, ro);
		// }
	}
}

/* evaluates an AST node within a certain scope*/
jep_obj* jep_evaluate_local(jep_ast_node ast, jep_obj* list, int mod)
{
	jep_obj* o = NULL;

	if(ast.token.type == T_IDENTIFIER)
	{
		/* TODO: implement local declaration */
	}
	else if(ast.token.token_code == T_EQUALS)
	{
		/* TODO: implement local initialization */
	}

	return o;
}
