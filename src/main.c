#include "ast.h"
#include "parser.h"
#include "operator.h"

/* command line flag indices */
#define JEP_TOK 0
#define JEP_AST 1
#define JEP_OBJ 2

#define MAX_FLAGS 3

const char *flags[] = 
{
	"-t", /* print tokens  */
	"-a", /* print ast     */ 
	"-o"  /* print objects */
};

int jep_check_flag(const char* arg)
{
	int i;
	for(i = 0; i < MAX_FLAGS; i++)
	{
		if(!strcmp(arg, flags[i]))
		{
			return i;
		}
	}
	return -1;
}

int main(int argc, char** argv)
{
	jep_token_stream* ts = NULL;
	jep_ast_node* nodes = NULL;
	jep_ast_node* root = NULL;
	int flags[MAX_FLAGS] = { 0, 0, 0 };
	int i;
	char* file_name = NULL;

	for(i = 1; i < argc; i++)
	{
		int f = jep_check_flag(argv[i]);
		if(f == -1)
		{
			if(file_name == NULL)
			{
				file_name = argv[i];
			}
		}
		else
		{
			if(!flags[f])
			{
				flags[f] = 1;
			}
			else
			{
				printf("unexpected flag: %s\n", argv[i]);
				return 1;
			}
		}
	}

	if(file_name != NULL)
	{
		ts = jep_create_token_stream();
		jep_tokenize_file(ts, file_name);
	}

	if(ts->error)
	{
		jep_destroy_token_stream(ts);
		return -1;
	}
	
	if(flags[JEP_TOK])
	{
		jep_print_tokens(ts, stdout);	
	}
	
	/* create the root of the AST */
	root = malloc(sizeof(jep_ast_node));
	root->leaf_count = 0;
	root->cap = 10;
	root->leaves = NULL;
	root->token.val = jep_create_string_builder();
	root->token.type = T_SYMBOL;
	root->token.token_code = 0;
	root->token.row = 0;
	root->token.column = 0;
	root->token.unary = 0;
	root->token.postfix = 0;
	root->error = 0;
	root->array = 0;
	root->loop = 0;
	jep_append_string(root->token.val, "root");

	jep_parse(ts, root);

	if(root != NULL)
	{
		if(!root->error && flags[JEP_AST])
		{
			jep_print_ast(*root);
		}

		if(root->leaves != NULL && !root->error 
			&& !flags[JEP_AST] && !flags[JEP_TOK])
		{
			jep_obj* list = jep_create_object();
			list->type = JEP_LIST;
			jep_obj* o;
			int i;
			for(i = 0; i < root->leaf_count; i++)
			{
				o = jep_evaluate(root->leaves[i], list);
				if(o != NULL)
				{
					jep_destroy_object(o);
					o = NULL;
				}
			}
			if(flags[JEP_OBJ])
			{
				jep_print_list(list);
			}
			jep_destroy_list(list);
		}

		/* destroy the AST */
		jep_destroy_string_builder(root->token.val);
		if(root->leaves != NULL)
		{
			free(root->leaves);
		}
		free(root);
	}

	if(nodes != NULL)
	{
		/* destroy all of the individual nodes */
		for(i = 0; i < ts->size; i++)
		{
			if(nodes[i].leaves != NULL)
			{
				free(nodes[i].leaves);
			}
		}
		free(nodes);
	}

	/* destroy the tokens */
	jep_destroy_token_stream(ts);
	
	return 0;
}
