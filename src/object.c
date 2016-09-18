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

	return o;
}

/* adds an object to a list */
void jep_add_object(jep_obj* list, jep_obj* o)
{
	if(list->head == NULL && list->tail == NULL)
	{
		list->head = o;
		list->tail = o;
	}
	else
	{
		list->tail->next = o;
		o->prev = list->tail;
		list->tail = o;
	}
}

/* retreives an object from a list */
jep_obj* jep_get_object(const char* ident, jep_obj* list)
{
	if(list == NULL || ident == NULL)
	{
		return NULL;
	}

	jep_obj* node = list->head;
	// jep_obj* head = node;

	if(node == NULL)
	{
		return NULL;
	}

	do
	{
		if(node->ident != NULL && !strcmp(ident, node->ident))
		{
			return node;
		}
		node = node->next;
	}while(node != NULL);

	return NULL;
}

/* copies the value of one obect into another */
void jep_copy_object(jep_obj* dest, jep_obj* src)
{
	if(dest == NULL || src == NULL)
	{
		return;
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
}

/* frees the memory in a list of objects */
void jep_destroy_list(jep_obj* list)
{
	if(list == NULL)
	{
		return;
	}

	jep_obj* node = list->head;
	jep_obj* next = NULL;

	if(node == NULL)
	{
		return;
	}

	do
	{
		next = node->next;
		free(node->val);
		free(node);
		node = next;
	}while(next != NULL);
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

void jep_print_obj(jep_obj* obj)
{
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

	jep_obj* node = list->head;

	if(node == NULL)
	{
		return;
	}

	do
	{
		jep_print_obj(node);
		node = node->next;
	}while(node != NULL);
}
