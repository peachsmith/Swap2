#include "ast.h"

int main(int argc, char** argv)
{
	jep_token_builder* tb = NULL;
	jep_ast_node* nodes = NULL;
	jep_ast_node* root = NULL;
	
	if(argc > 1)
	{
		tb = jep_tokenize_file(argv[1]);
	}
	
	if(tb != NULL)
	{
		// jep_print_tokens(tb, stdout);
		root = jep_parse(tb, &nodes);

		if(root != NULL)
		{
			/* print the AST */
			// jep_print_ast(*root);
			if(root->leaves != NULL)
			{
				jep_obj* o;
				int i;
				for(i = 0; i < root->leaf_count; i++)
				{
					o = jep_evaluate(root->leaves[i]);
					if(o != NULL)
					{
						jep_print_obj(o);
						free(o->value);
						free(o);
					}
				}
			}
			

			/* destroy the AST */
			jep_destroy_token(root->token);
			if(root->leaves != NULL)
			{
				free(root->leaves);
			}
			free(root);
		}

		if(nodes != NULL)
		{
			/* destroy all of the individual nodes */
			int i;
			for(i = 0; i < tb->size; i++)
			{
				if(nodes[i].leaves != NULL)
				{
					free(nodes[i].leaves);
				}
			}
			free(nodes);
		}

		/* destroy the tokens */
		jep_destroy_token_builder(tb);
	}
	else
	{
		printf("could not open file\n");
	}
	
	return 0;
}
