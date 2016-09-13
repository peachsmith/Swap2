#include "ast.h"
#include "parser.h"
#include "operator.h"

/* command line flag indices */
#define TOK 0
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
		ts = jep_tokenize_file(file_name);
	}
	
	if(ts != NULL)
	{
		if(flags[TOK])
		{
			jep_print_tokens(ts, stdout);	
		}
		
		root = jep_parse(ts, &nodes);

		if(root != NULL)
		{
			/* print the AST if the -a flag was passed in */
			if(!root->error && flags[JEP_AST])
			{
				jep_print_ast(*root);
			}
			if(root->leaves != NULL && !root->error && flags[JEP_OBJ])
			{
				jep_obj list = 
				{
					NULL, NULL, 0, NULL, NULL, NULL, NULL
				};
				jep_obj* o;
				int i;
				for(i = 0; i < root->leaf_count; i++)
				{
					o = jep_evaluate(root->leaves[i], &list);
					if(o != NULL)
					{
						free(o->val);
						free(o);
					}
				}
				jep_print_list(&list);
				jep_destroy_list(&list);
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
	}
	else
	{
		printf("could not open file\n");
	}
	
	return 0;
}
