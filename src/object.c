#include "object.h"

jep_obj* jep_number(const char* s)
{
	char* endptr;
	long int l;
	double d;
	void* val;
	jep_obj* obj;

	errno = 0;
	endptr = NULL;
	obj = NULL;

	/* attempt to convert the string to a long int */
	l = strtol(s, &endptr, 10);

	if(errno == ERANGE)
	{
		printf("out of range\n");
		return NULL;
	}
	else if(endptr != s && *endptr != '\0')
	{
		endptr = NULL;
		errno = 0;

		/* attempt to convert the string to a double */
		d = strtod(s, &endptr);

		if(errno == ERANGE)
		{
			printf("out of range\n");
		}
		else if(endptr != s && *endptr != '\0')
		{
			printf("invalid number format\n");
		}
		else
		{
			double* d_ptr = malloc(sizeof(double));
			*d_ptr = d;
			val = (void*)d_ptr;
			obj = malloc(sizeof(jep_obj));
			obj->value = (void*)val;
			obj->type = JEP_DOUBLE;
		}
	}
	else if(l >= INT_MIN && l <= INT_MAX)
	{
		/* cast the value as an int if it will fit */
		int* l_ptr = malloc(sizeof(int));
		*l_ptr = (int)l;
		val = (void*)l_ptr;
		obj = malloc(sizeof(jep_obj));
		obj->value = (void*)val;
		obj->type = JEP_INT;
	}
	else
	{
		long int* l_ptr = malloc(sizeof(long int));
		*l_ptr = l;
		val = (void*)l_ptr;
		obj = malloc(sizeof(jep_obj));
		obj->value = (void*)val;
		obj->type = JEP_LONG;
	}

	return obj;
}

void jep_print_obj(jep_obj* obj)
{
	if(obj != NULL)
	{
		if(obj->type == JEP_INT)
		{
			printf("int: %d\n", *((int*)(obj->value)));
		}
		else if(obj->type == JEP_LONG)
		{
			printf("long: %ld\n", *((long*)(obj->value)));
		}
		else if(obj->type == JEP_DOUBLE)
		{
			printf("double: %.2lf\n", *((double*)(obj->value)));
		}
	}
	else
	{
		printf("NULL\n");
	}
}

/* evaluates the nodes of an AST */
jep_obj* jep_evaluate(jep_ast_node ast)
{
	if(ast.token->type == T_NUMBER)
	{
		return jep_number(ast.token->value->buffer);
	}
	else if(ast.token->token_code == T_PLUS)
	{
		return jep_add(ast);
	}
	else if(ast.token->token_code == T_MINUS)
	{
		return jep_sub(ast);
	}
	else if(ast.token->token_code == T_STAR)
	{
		return jep_mul(ast);
	}
	else if(ast.token->token_code == T_FSLASH)
	{
		return jep_div(ast);
	}
	else if(ast.token->token_code == T_LESS)
	{
		return jep_less(ast);
	}
	else if(ast.token->token_code == T_GREATER)
	{
		return jep_greater(ast);
	}
	else if(ast.token->token_code == T_LOREQUAL)
	{
		return jep_lorequal(ast);
	}
	else if(ast.token->token_code == T_GOREQUAL)
	{
		return jep_gorequal(ast);
	}
	else if(ast.token->token_code == T_EQUIVALENT)
	{
		return jep_equiv(ast);
	}
	else if(ast.token->token_code == T_NOTEQUIVALENT)
	{
		return jep_noteq(ast);
	}
	else if(ast.token->token_code == T_NOT)
	{
		return jep_not(ast);
	}
	else if(ast.token->token_code == T_LOGAND)
	{
		return jep_and(ast);
	}
	else if(ast.token->token_code == T_LOGOR)
	{
		return jep_or(ast);
	}
	else
	{
		printf("unsupported operation: %s\n", 
			ast.token->value->buffer);
		return NULL;
	}
}

/* evaluates an addition expression */
jep_obj* jep_add(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	

	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) + (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) + (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) + (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) + (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) + (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) + (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) + (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) + (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) + (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) + (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) + (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates subtraction or negation */
jep_obj* jep_sub(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2 && node.leaf_count != 1)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);

	if(node.leaf_count > 1)
	{
		r = jep_evaluate(node.leaves[1]);
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
			*n = 0 - (*(int*)(l->value));
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG)
		{
			long *n = malloc(sizeof(int));
			*n = 0 - (*(long*)(l->value));
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_LONG;
		}
		else if(l->type == JEP_DOUBLE)
		{
			double *n = malloc(sizeof(double));
			*n = 0 - (*(double*)(l->value));
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
				*n = (*(int*)(l->value)) - (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) - (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) - (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) - (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) - (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) - (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) - (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) - (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) - (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) - (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) - (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates a multiplication expression */
jep_obj* jep_mul(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) * (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) * (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) * (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) * (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) * (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) * (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) * (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
					*n = (*(long*)(l->value)) * (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) * (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) * (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) * (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates a division expression */
jep_obj* jep_div(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) / (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				long *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) / (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_LONG;
			}
			else if(l->type == JEP_DOUBLE)
			{
				double *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) / (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) / (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) / (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) / (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) / (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_DOUBLE;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			long *n = malloc(sizeof(long));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) / (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) / (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) / (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) / (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates a less than expression */
jep_obj* jep_less(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) < (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) < (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) < (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) < (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) < (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) < (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) < (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) < (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) < (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) < (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) < (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates a less than expression */
jep_obj* jep_greater(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) > (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) > (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) > (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) > (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) > (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) > (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) > (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) > (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) > (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) > (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) > (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates a less than or equal to expression */
jep_obj* jep_lorequal(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) <= (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) <= (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) <= (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) <= (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) <= (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) <= (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) <= (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) <= (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) <= (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) <= (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) <= (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates a greater than or equal to expression */
jep_obj* jep_gorequal(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) >= (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) >= (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) >= (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) >= (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) >= (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) >= (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) >= (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) >= (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) >= (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) >= (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) >= (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* evaluates an equivalence expression */
jep_obj* jep_equiv(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) == (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) == (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(double));
				*n = (*(double*)(l->value)) == (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) == (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) == (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) == (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) == (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) == (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) == (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) == (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) == (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;	
}

/* evaluates a not equivalence expression */
jep_obj* jep_noteq(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) != (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) != (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->value)) != (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) != (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) != (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) != (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) != (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) != (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) != (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) != (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) != (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* performs a not operation */
jep_obj* jep_not(jep_ast_node node)
{
	jep_obj* l = NULL;       /* operand  */
	jep_obj* result = NULL;  /* result   */

	if(node.leaf_count != 1)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);

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
			*n = !(*(int*)(l->value));
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(long*)(l->value));
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_DOUBLE)
		{
			int *n = malloc(sizeof(int));
			*n = !(*(double*)(l->value));
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
	}
	else
	{
		printf("could not obtain operand value\n");
	}

	/* free the memory of the operands */
	if(l != NULL)
	{
		free(l->value);
		free(l);
	}

	return result;
}

/* performs a logical and operation */
jep_obj* jep_and(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) && (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) && (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->value)) && (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) && (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) && (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) && (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) && (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) && (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) && (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) && (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) && (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}

/* performs a logical or operation */
jep_obj* jep_or(jep_ast_node node)
{
	jep_obj* l = NULL;       /* left operand  */
	jep_obj* r = NULL;       /* right operand */
	jep_obj* result = NULL;  /* result        */

	if(node.leaf_count != 2)
	{
		return NULL;
	}
	
	l = jep_evaluate(node.leaves[0]);
	r = jep_evaluate(node.leaves[1]);

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
				*n = (*(int*)(l->value)) || (*(int*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_LONG)
			{
				int *n = malloc(sizeof(int));
				*n = (*(long*)(l->value)) || (*(long*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
				result->type = JEP_INT;
			}
			else if(l->type == JEP_DOUBLE)
			{
				int *n = malloc(sizeof(int));
				*n = (*(double*)(l->value)) || (*(double*)(r->value));
				result = malloc(sizeof(jep_obj));
				result->value = (void*)n;
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
					*n = (*(double*)(l->value)) || (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(double*)(l->value)) || (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_DOUBLE)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) || (*(double*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) || (*(double*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
			result->type = JEP_INT;
		}
		else if(l->type == JEP_LONG || r->type != JEP_LONG)
		{
			int *n = malloc(sizeof(int));
			if(l->type == JEP_LONG)
			{
				if(r->type == JEP_INT)
				{
					*n = (*(long*)(l->value)) || (*(int*)(r->value));
				}
				else if(r->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) || (*(long*)(r->value));
				}
			}
			else if(r->type == JEP_LONG)
			{
				if(l->type == JEP_INT)
				{
					*n = (*(int*)(l->value)) || (*(long*)(r->value));
				}
				else if(l->type == JEP_LONG)
				{
					*n = (*(long*)(l->value)) || (*(long*)(r->value));
				}
			}
			result = malloc(sizeof(jep_obj));
			result->value = (void*)n;
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
		free(l->value);
		free(l);
	}
	if(r != NULL)
	{
		free(r->value);
		free(r);
	}

	return result;
}
