#include "ast.h"
#include "parser.h"
#include "operator.h"

int main(int argc, char** argv)
{
	jep_token_stream* ts = NULL;
	jep_ast_node* nodes = NULL;
	jep_ast_node* root = NULL;
	
	if(argc > 1)
	{
		ts = jep_tokenize_file(argv[1]);
	}
	
	if(ts != NULL)
	{
		// jep_print_tokens(ts, stdout);
		root = jep_parse(ts, &nodes);

		if(root != NULL)
		{
			/* print the AST */
			if(!root->error)
			{
				jep_print_ast(*root);
			}
			// if(root->leaves != NULL && !root->error)
			// {
			// 	jep_obj* o;
			// 	int i;
			// 	for(i = 0; i < root->leaf_count; i++)
			// 	{
			// 		o = jep_evaluate(root->leaves[i]);
			// 		if(o != NULL)
			// 		{
			// 			jep_print_obj(o);
			// 			free(o->value);
			// 			free(o);
			// 		}
			// 	}
			// }
			

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
