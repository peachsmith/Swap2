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
		jep_obj* elem = array->tail;
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
				jep_print_array(elem);
			}
			if(elem->prev != NULL)
			{
				printf(", ");
			}
			elem = elem->prev;
		}
	}
	printf(" }");
}

static void jep_free_array(jep_obj* array, jep_mem* mem)
{
	if(array != NULL && array->size > 0)
	{
		jep_obj* elem = array->tail;
		jep_obj* prev = NULL;
		while(elem != NULL)
		{
			prev = elem->prev;
			int i;
			int freed = 0;
			for(i = 0; i < mem->size; i++)
			{
				if((void*)(elem->val) == (void*)(mem->addr[i]))
				{
					freed = 1;
				}
			}
			if(!freed)
			{
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
					jep_free_array(elem, mem);
				}	

				if(mem->size >= mem->cap)
				{
					mem->cap = mem->cap + mem->cap / 2;
					mem->addr = realloc(mem->addr, sizeof(void*) * mem->cap);
				}
				mem->addr[mem->size++] = elem->val;
			}
			free(elem);
			elem = prev;
		}
	}
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

	list->size++;
}

/* retreives an object from a list */
jep_obj* jep_get_object(const char* ident, jep_obj* list)
{
	if(list == NULL || ident == NULL)
	{
		return NULL;
	}

	jep_obj* obj = list->head;
	// jep_obj* head = node;

	if(obj == NULL)
	{
		return NULL;
	}

	do
	{
		if(obj->ident != NULL && !strcmp(ident, obj->ident))
		{
			return obj;
		}
		obj = obj->next;
	}while(obj != NULL);

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

// /* frees the memory in a list of objects */
// void jep_destroy_list(jep_obj* list, jep_mem* mem)
// {
// 	printf("BEGIN jep_destroy_list\n");
// 	if(list == NULL)
// 	{
// 		printf("list is NULL\n");
// 		return;
// 	}

// 	jep_obj* node = list->tail;
// 	jep_obj* next = NULL;

// 	if(node == NULL)
// 	{
// 		printf("list head is NULL\n");
// 		return;
// 	}

// 	do
// 	{
// 		printf("freeing: ");
// 		jep_print_obj(node);
// 		next = node->prev;
// 		int i;
// 		int freed = 0;
// 		for(i = 0; i < mem->size; i++)
// 		{
// 			if((void*)(node->val) == (void*)(mem->addr[i]))
// 			{
// 				freed = 1;
// 			}
// 		}
// 		if(!freed)
// 		{
// 			if(node->type == JEP_ARRAY)
// 			{
// 				if(node == NULL)
// 				{
// 					printf("node is NULL\n");
// 				}
// 				else if(node->val == NULL)
// 				{
// 					printf("node's value is somehow already NULL\n");
// 				}
// 				jep_obj* array = (jep_obj*)(node->val);
// 				jep_destroy_list(array, mem);
// 			}
// 			free(node->val);
// 			if(mem->size >= mem->cap)
// 			{
// 				mem->cap = mem->cap + mem->cap / 2;
// 				mem->addr = realloc(mem->addr, sizeof(void*) * mem->cap);
// 			}
// 			mem->addr[mem->size++] = node->val;
// 		}
// 		else
// 		{
// 			printf("this memory at %p has already been freed\n", node);
// 		}
// 		free(node);
// 		node = next;
// 	}while(node != NULL);

// 	printf("END jep_destroy_list\n");
// }

static void jep_destroy_object(jep_obj* obj, jep_mem* mem)
{
	if(obj != NULL)
	{
		int i;
		int freed = 0;
		for(i = 0; i < mem->size; i++)
		{
			if((void*)(obj->val) == (void*)(mem->addr[i]))
			{
				freed = 1;
			}
		}
		if(!freed)
		{
			if(obj != NULL)
			{
				if(obj->type == JEP_INT)
				{
					free(obj->val);
				}
				else if(obj->type == JEP_LONG)
				{
					free(obj->val);
				}
				else if(obj->type == JEP_DOUBLE)
				{
					free(obj->val);
				}
				else if(obj->type == JEP_CHARACTER)
				{
					free(obj->val);
				}
				else if(obj->type == JEP_STRING)
				{
					free(obj->val);
				}
				else if(obj->type == JEP_ARRAY)
				{
					jep_obj* array = (jep_obj*)(obj->val);
					jep_free_array(array, mem);
				}
				else
				{
					printf("unrecognized type\n");
				}
			}
			else
			{
				printf("[null]\n");
			}
			if(mem->size >= mem->cap)
			{
				mem->cap = mem->cap + mem->cap / 2;
				mem->addr = realloc(mem->addr, sizeof(void*) * mem->cap);
			}
			mem->addr[mem->size++] = obj->val;
		}
		else
		{
			// printf("this memory at %p has already been freed\n", obj);
		}
		free(obj);
	}
	else
	{
		printf("obj is NULL\n");
	}
}

/* frees the memory in a list of objects */
void jep_destroy_list(jep_obj* list, jep_mem* mem)
{
	if(list == NULL)
	{
		printf("list is NULL\n");
		return;
	}

	jep_obj* obj = list->head;
	jep_obj* next = NULL;

	if(obj == NULL)
	{
		printf("head is NULL\n");
		return;
	}

	do
	{
		next = obj->next;
		jep_destroy_object(obj, mem);
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
		else if(obj->type == JEP_ARRAY)
		{
			printf("[array] %s: ", obj->ident);
			jep_obj* array = (jep_obj*)(obj->val);
			jep_print_array(array);
			printf("\n");
		}
		else
		{
			printf("unrecognized type\n");
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
		jep_print_obj(obj);	
		obj = obj->next;
	}while(obj != NULL);
}
