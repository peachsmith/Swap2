#include "object.h"
#include "tokenizer.h"

/* escape characters */
const char escapes[] = 
{
	'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\'', '"', '?'
};

/* checks for an escape character */
static int jep_is_escape(char c)
{
	int i;
	for(i = 0; i < 11; i++)
	{
		if(escapes[i] == c)
		{
			return i;
		}
	}
	return -1;
}

static void jep_print_array(jep_obj* array)
{
	printf("{ ");
	if(array != NULL && array->size > 0)
	{
		jep_obj* elem = array->head;
		while(elem != NULL)
		{
			if(elem->type == JEP_INT)
			{
				printf("[int] %d", *((int*)(elem->val)));
			}
			else if(elem->type == JEP_LONG)
			{
				printf("[long] %ld", *((long*)(elem->val)));
			}
			else if(elem->type == JEP_DOUBLE)
			{
				printf("[double] %.2lf", *((double*)(elem->val)));
			}
			else if(elem->type == JEP_CHARACTER)
			{
				printf("[character] %c", *((char*)(elem->val)));
			}
			else if(elem->type == JEP_STRING)
			{
				printf("[string] %s", (char*)(elem->val));
			}
			else if(elem->type == JEP_ARRAY)
			{
				printf("[array] ");
				jep_print_array((jep_obj*)(elem->val));
			}
			else if(elem->type == JEP_ARGUMENT)
			{
				printf("[null]");
			}
			if(elem->next != NULL)
			{
				printf(", ");
			}
			elem = elem->next;
		}
	}
	printf(" }");
}

/* creates a string representation of an object */
char* jep_to_string(jep_obj* o)
{
	char* str = NULL;

	if(o == NULL)
	{
		return str;
	}

	if(o->type == JEP_STRING)
	{
		char* s = (char*)(o->val);
		int len = strlen(s);
		str = malloc(len + 1);
		strcpy(str, s);
		str[len] = '\0';
	}
	else if(o->type == JEP_CHARACTER)
	{
		char* c = (char*)(o->val);
		str = malloc(2);
		str[0] = *c;
		str[1] = '\0';
	}
	else if(o->type == JEP_INT)
	{
		int i = *(int*)(o->val);
		char s[100];

		int n = sprintf(s, "%d", i);
	
		if(n > 0)
		{
			if(n >= 100)
			{
				s[99] = '\0';
			}
			else
			{
				s[n] = '\0';
			}
			int len = strlen(s);
			str = malloc(len + 1);
			strcpy(str, s);
			str[len] = '\0';
		}
	}
	else if(o->type == JEP_DOUBLE)
	{
		double d = *(double*)(o->val);
		char s[100];

		int n = sprintf(s, "%.4f", d);
	
		if(n > 0)
		{
			if(n >= 100)
			{
				s[99] = '\0';
			}
			else
			{
				s[n] = '\0';
			}
			int len = strlen(s);
			str = malloc(len + 1);
			strcpy(str, s);
			str[len] = '\0';
		}
	}
	else if(o->type == JEP_ARRAY)
	{
		char* array = "[array]";
		str = malloc(strlen(array) + 1);
		strcpy(str, array);
	}
	else if(o->type == JEP_FUNCTION)
	{
		str = malloc(strlen(o->ident) + 18);
		strcpy(str, "function: ");
		strcat(str, o->ident);
		strcat(str, "(...)");
	}
	else if(o->type == JEP_ARGUMENT)
	{
		if(o->val == NULL)
		{
			str = malloc(7);
			strcpy(str, "[null]");
		}
	}

	return str;
}

/* frees the memory used by an array */
void jep_free_array(jep_obj* array)
{
	if(array != NULL && array->size > 0)
	{
		jep_obj* elem = array->tail;
		jep_obj* prev = NULL;
		while(elem != NULL)
		{
			prev = elem->prev;
			if(elem->type == JEP_INT)
			{
				free(elem->val);
			}
			else if(elem->type == JEP_LONG)
			{
				free(elem->val);
			}
			else if(elem->type == JEP_DOUBLE)
			{
				free(elem->val);
			}
			else if(elem->type == JEP_CHARACTER)
			{
				free(elem->val);
			}
			else if(elem->type == JEP_STRING)
			{
				free(elem->val);
			}
			else if(elem->type == JEP_ARRAY)
			{
				/* frees the memory used by an array */
				jep_free_array(elem);
			}

			free(elem);
			elem = prev;
		}
	}
}

/* frees the memory used by a function */
void jep_free_function(jep_obj* func)
{
	jep_obj* args = func->head;

	/* destroy the body of non-native functions */
	if(func->size == 2)
	{
		jep_obj* body = args->next;
		free(body);	
	}

	jep_destroy_list(args);
	free(args);
}

/* allocates memory for a new object */
jep_obj* jep_create_object()
{
	jep_obj* o = malloc(sizeof(jep_obj));

	o->val = NULL;
	o->ident = NULL;
	o->type = 0;
	o->prev = NULL;
	o->next = NULL;
	o->head = NULL;
	o->tail = NULL;
	o->size = 0;
	o->ret = 0;
	o->index = -1;
	o->array_ident = NULL;

	return o;
}

/* adds an object to a list */
void jep_add_object(jep_obj* list, jep_obj* o)
{
	if(list->head == NULL && list->tail == NULL)
	{
		list->head = o;
		list->tail = o;
		list->size++;
	}
	else if(list->tail->type == JEP_LIST)
	{
		jep_add_object(list->tail, o);
	}
	else
	{
		list->tail->next = o;
		o->prev = list->tail;
		list->tail = o;
		list->size++;
	}
}

/* retreives an object from a list */
jep_obj* jep_get_object(const char* ident, jep_obj* list)
{
	if(list == NULL || ident == NULL)
	{
		return NULL;
	}

	jep_obj* o = NULL;
	jep_obj* obj = list->head;

	while(obj != NULL)
	{
		if(obj->ident != NULL && !strcmp(ident, obj->ident))
		{
			o = obj;
		}
		if(obj->type == JEP_LIST)
		{
			jep_obj* temp = NULL;
			if(o != NULL)
			{
				temp = o;
			}
			o = jep_get_object(ident, obj);
			if(o == NULL && temp != NULL)
			{
				o = temp;
			}
		}
		obj = obj->next;
	}

	return o;
}

/* copies the value of one obect into another */
void jep_copy_object(jep_obj* dest, jep_obj* src)
{
	if(dest == NULL || src == NULL)
	{
		return;
	}

	if(dest->val != NULL)
	{
		if(dest->type == JEP_ARRAY)
		{
			/* frees the memory used by an array */
			jep_free_array((jep_obj*)(dest->val));
			dest->size = 0;
			free(dest->val);
		}
		else if(dest->type == JEP_FUNCTION)
		{
			jep_free_function(dest);
		}
		else
		{
			free(dest->val);
		}
	}
	
	dest->type = src->type;

	if(src->type == JEP_INT)
	{
		int* i = malloc(sizeof(int));
		*i = *(int*)(src->val);
		dest->val = (void*)i;
	}
	else if(src->type == JEP_LONG)
	{
		long* l = malloc(sizeof(long));
		*l = *(long*)(src->val);
		dest->val = (void*)l;
	}
	else if(src->type == JEP_DOUBLE)
	{
		double* d = malloc(sizeof(double));
		*d = *(double*)(src->val);
		dest->val = (void*)d;
	}
	else if(src->type == JEP_CHARACTER)
	{
		char* c = malloc(sizeof(char));
		*c = *(char*)(src->val);
		dest->val = (void*)c;
	}
	else if(src->type == JEP_STRING)
	{
		int len = strlen((char*)(src->val));
		dest->val = malloc(len + 1);
		strcpy(dest->val, (char*)(src->val));
	}
	else if(src->type == JEP_ARRAY)
	{
		jep_obj* array = (jep_obj*)(src->val);
		jep_obj* dest_array = jep_create_object();
		if(array != NULL && array->size > 0)
		{
			jep_obj* orig = array->head; /* original */
			while(orig != NULL)
			{
				jep_obj* copy = jep_create_object();
				copy->index = orig->index;
				copy->array_ident = orig->array_ident;
				jep_copy_object(copy, orig);
				jep_add_object(dest_array, copy);
				orig = orig->next;
			}
		}
		dest->size = src->size;
		dest->val = dest_array;
	}
	else if(src->type == JEP_FUNCTION)
	{
		jep_ast_node* n = jep_create_ast_node();
		jep_obj* args = jep_create_object();
		jep_obj* body = NULL;

		jep_obj* src_args = src->head;
		jep_obj* a = src_args->head;
		while(a != NULL)
		{
			jep_obj* arg = jep_create_object();
			arg->type = JEP_ARGUMENT;
			arg->ident = a->ident;
			jep_add_object(args, arg);
			a = a->next;
		}

		if(src->size == 2)
		{
			body = jep_create_object();
			*n = *((jep_ast_node*)(src_args->next->val));
			body->val = n;	
		}
		
		jep_add_object(dest, args);

		if(src->size == 2)
		{
			jep_add_object(dest, body);	
		}
	}
}

/* frees the memory used by an object */
void jep_destroy_object(jep_obj* obj)
{
	if(obj != NULL)
	{
		if(obj->type == JEP_INT && obj->val != NULL)
		{
			free(obj->val);
		}
		else if(obj->type == JEP_LONG && obj->val != NULL)
		{
			free(obj->val);
		}
		else if(obj->type == JEP_DOUBLE && obj->val != NULL)
		{
			free(obj->val);
		}
		else if(obj->type == JEP_CHARACTER && obj->val != NULL)
		{
			free(obj->val);
		}
		else if(obj->type == JEP_STRING && obj->val != NULL)
		{
			free(obj->val);
		}
		else if(obj->type == JEP_ARRAY && obj->val != NULL)
		{
			jep_obj* array = (jep_obj*)(obj->val);
			jep_free_array(array);
		}
		else if(obj->type == JEP_FUNCTION)
		{
			jep_free_function(obj);
		}
		else if(obj->type == JEP_ARGUMENT)
		{
			/* arguments have no value */
		}
		else if(obj->type == JEP_LIST)
		{
			jep_destroy_list(obj);
		}
		else
		{
			printf("unrecognized type while destroying object %d\n", obj->type);
		}

		obj->val = NULL;
		free(obj);
	}
}

/* frees the memory in a list of objects */
void jep_destroy_list(jep_obj* list)
{
	if(list == NULL)
	{
		return;
	}

	jep_obj* obj = list->head;
	jep_obj* next = NULL;

	if(obj == NULL)
	{
		return;
	}

	do
	{
		next = obj->next;
		jep_destroy_object(obj);
		obj = next;
	}while(obj != NULL);
}

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
		printf("number out of range\n");
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
			printf("number out of range\n");
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
			obj = jep_create_object();
			obj->val = (void*)val;
			obj->type = JEP_DOUBLE;
		}
	}
	else if(l >= INT_MIN && l <= INT_MAX)
	{
		/* valcast the value as an int if it will fit */
		int* l_ptr = malloc(sizeof(int));
		*l_ptr = (int)l;
		val = (void*)l_ptr;
		obj = jep_create_object();
		obj->val = (void*)val;
		obj->type = JEP_INT;
	}
	else
	{
		long int* l_ptr = malloc(sizeof(long int));
		*l_ptr = l;
		val = (void*)l_ptr;
		obj = jep_create_object();
		obj->val = (void*)val;
		obj->type = JEP_LONG;
	}

	return obj;
}

/* converts a token into a character object */
jep_obj* jep_character(const char* s)
{
	jep_obj* obj = NULL;
	char* c = malloc(sizeof(char));

	obj = jep_create_object();
	obj->type = JEP_CHARACTER;

	if(s == NULL || s[0] == '\0')
	{
		obj->val = NULL;
	}
	else if(s[0] == '\\' && strlen(s) > 1)
	{
		switch(jep_is_escape(s[1]))
		{
			case 0:
				*c = '\a';
				break;

			case 1:
				*c = '\b';
				break;

			case 2:
				*c = '\f';
				break;

			case 3:
				*c = '\n';
				break;

			case 4:
				*c = '\r';
				break;

			case 5:
				*c = '\t';
				break;

			case 6:
				*c = '\v';
				break;

			case 7:
				*c = '\\';
				break;

			case 8:
				*c = '\'';
				break;

			case 9:
				*c = '\"';
				break;

			case 10:
				*c = '\?';
				break;

			default:
				printf("invalid escape character %c\n", s[1]);
				free(c);
				free(obj);
				obj = NULL;
				break;
		}

		if(obj != NULL)
		{
			obj->val = (void*)c;
		}
	}
	else if(strlen(s) > 1)
	{
		printf("invalid character length\n");
		free(c);
		free(obj);
		obj = NULL;
	}
	else
	{
		*c = s[0];
		obj->val = (void*)c;
	}

	return obj;
}

/* converts a token into a string object */
jep_obj* jep_string(const char* s)
{
	jep_obj* obj = NULL;
	int len = strlen(s);
	obj = jep_create_object();
	obj->type = JEP_STRING;
	obj->val = malloc(len + 1);
	strcpy(obj->val, s);

	return obj;
}

/* convertes an ast into an array */
jep_obj* jep_array(jep_ast_node* ast)
{
	jep_obj* obj = jep_create_object();
	obj->type = JEP_ARRAY;
	return obj;
}

void jep_print_object(jep_obj* obj)
{
	static int inner_list = 0;
	if(obj != NULL)
	{
		if(obj->type == JEP_INT)
		{
			printf("[int] %s: %d\n", obj->ident, *((int*)(obj->val)));
		}
		else if(obj->type == JEP_LONG)
		{
			printf("[long] %s: %ld\n", obj->ident, *((long*)(obj->val)));
		}
		else if(obj->type == JEP_DOUBLE)
		{
			printf("[double] %s: %.2lf\n", obj->ident, *((double*)(obj->val)));
		}
		else if(obj->type == JEP_CHARACTER)
		{
			printf("[character] %s: %c\n", obj->ident, *((char*)(obj->val)));
		}
		else if(obj->type == JEP_STRING)
		{
			printf("[string] %s: %s\n", obj->ident, (char*)(obj->val));
		}
		else if(obj->type == JEP_ARRAY)
		{
			printf("[array] %s: ", obj->ident);
			jep_obj* array = (jep_obj*)(obj->val);
			jep_print_array(array);
			printf("\n");
		}
		else if(obj->type == JEP_FUNCTION)
		{
			printf("[function] %s (", obj->ident);
			jep_obj* arg = obj->head->head;
			while(arg != NULL)
			{
				printf("%s", arg->ident);
				if(arg->next != NULL)
				{
					printf(", ");
				}
				arg = arg->next;
			}
			printf(")\n");
		}
		else if(obj->type == JEP_LIST)
		{
			inner_list++;
			printf("inner list %d\n", inner_list);
			jep_print_list(obj);
			inner_list--;
		}
		else
		{
			printf("unrecognized type while printing object %d\n", obj->type);
		}
	}
	else
	{
		printf("[null]\n");
	}
}

/* prints a list of objects to stdout */
void jep_print_list(jep_obj* list)
{
	if(list == NULL)
	{
		return;
	}

	jep_obj* obj = NULL; 

	obj = list->head;

	if(obj == NULL)
	{
		return;
	}

	do
	{
		jep_print_object(obj);
		obj = obj->next;
	}while(obj != NULL);
}

/* removes the scope list off the tail of a list */
void jep_remove_scope(jep_obj* list)
{
	if(list == NULL)
	{
		return;
	}
	else if(list->tail == NULL)
	{
		return;
	}
	else if(list->tail->type != JEP_LIST)
	{
		return;
	}

	jep_obj* tail = list->tail;
	
	if(tail->tail != NULL && tail->tail->type == JEP_LIST)
	{
		jep_remove_scope(tail);
	}
	else if(list->tail != NULL)
	{
		list->tail = list->tail->prev;
		if(list->tail != NULL)
		{
			list->tail->next = NULL;	
		}
		list->size--;
		if(list->size == 0)
		{
			list->head = NULL;
		}
	}
}
