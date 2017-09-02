/*
	The swap interpreter.
	Copyright (C) 2016 John Powell

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ast.h"
#include "parser.h"
#include "operator.h"

/* command line flag indices */
#define JEP_TOK 0
#define JEP_AST 1
#define JEP_OBJ 2
#define JEP_VER 3
#define JEP_VER_LONG 4

#define MAX_FLAGS 5

const char *flags[MAX_FLAGS] =
{
	"-t",		/* print tokens  */
	"-a",		/* print ast     */
	"-o",		/* print objects */
	"-v",		/* version info  */
	"--version" /* version info  */
};

/**
 * checks an input flag to see if it is in the array of acceptable flags
 */
int jep_check_flag(const char *arg)
{
	int i;
	for (i = 0; i < MAX_FLAGS; i++)
	{
		if (!strcmp(arg, flags[i]))
		{
			return i;
		}
	}
	return -1;
}

int main(int argc, char **argv)
{
	jep_token_stream *ts = NULL;
	jep_ast_node *nodes = NULL;
	jep_ast_node *root = NULL;
	int flags[MAX_FLAGS] = { 0, 0, 0, 0, 0 };
	int i;
	char *file_name = NULL;

	for (i = 1; i < argc; i++)
	{
		int f = jep_check_flag(argv[i]);
		if (f == -1 && file_name == NULL)
		{
			file_name = argv[i];
		}
		else
		{
			if (!flags[f])
			{
				flags[f] = 1;
			}
			else
			{
				/* TODO: output usage information */
				printf("unexpected flag: %s\n", argv[i]);
				return 1;
			}
		}
	}

	if (flags[JEP_VER] || flags[JEP_VER_LONG])
	{
		printf("swap 0.0.9\n");
		printf("Copyright (C) 2016 John Powell\n");
		printf("This is free software. There is NO warranty;");
		printf(" not even for\nMERCHANTABILITY or FITNESS FOR ");
		printf("A PARTICULAR PURPOSE.\n");

		if (file_name == NULL)
		{
			return 0;
		}
	}

	if (file_name != NULL)
	{
		ts = jep_create_token_stream();
		jep_tokenize_file(ts, file_name);
	}
	else
	{
		printf("error: no input file\n");
		return 1;
	}

	if (ts->error)
	{
		jep_destroy_token_stream(ts);
		return -1;
	}

	if (flags[JEP_TOK])
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

	/* build the AST */
	jep_parse(ts, root);

	if (root != NULL)
	{
		if (!root->error && flags[JEP_AST])
		{
			jep_print_ast(*root);
		}

		if (root->leaves != NULL && !root->error && !flags[JEP_AST] && !flags[JEP_TOK])
		{
			jep_obj *list = jep_create_object();
			list->type = JEP_LIST;

			/* load the main native library */
			char* app_path = jep_get_app_path();

			size_t a_len = strlen(app_path);
			size_t l_len = strlen(SWAP_NATIVE_LIB);

			char* lib_path = malloc(a_len + l_len + 1);
			strcpy(lib_path, app_path);
			strcat(lib_path, SWAP_NATIVE_LIB);
			lib_path[a_len + l_len] = '\0';

			jep_lib native_lib = jep_load_lib(lib_path);

			free(app_path);
			free(lib_path);

			if (native_lib != NULL)
			{
				jep_obj *l_native = jep_create_object();
				l_native->type = JEP_NULL;
				l_native->ident = " SwapNative"; /* prevent code from obtaining a reference to this */
				l_native->val = native_lib;
				jep_add_object(list, l_native);
			}
			else
			{
				printf("could not load native shared library\n");
			}

			/* add built in functions */
			jep_obj *f_byte = jep_create_object();
			f_byte->type = JEP_FUNCTION;
			f_byte->ident = "byte";
			f_byte->size = 1;
			jep_add_object(list, f_byte);

			jep_obj *f_int = jep_create_object();
			f_int->type = JEP_FUNCTION;
			f_int->ident = "int";
			f_int->size = 1;
			jep_add_object(list, f_int);

			jep_obj *f_double = jep_create_object();
			f_double->type = JEP_FUNCTION;
			f_double->ident = "double";
			f_double->size = 1;
			jep_add_object(list, f_double);

			jep_obj *f_typeof = jep_create_object();
			f_typeof->type = JEP_FUNCTION;
			f_typeof->ident = "typeof";
			f_typeof->size = 1;
			jep_add_object(list, f_typeof);

			jep_obj *f_len = jep_create_object();
			f_len->type = JEP_FUNCTION;
			f_len->ident = "len";
			f_len->size = 1;
			jep_add_object(list, f_len);

			/* traverse and interpret the AST */
			jep_obj *o;
			int i;
			int exception = 0;
			for (i = 0; i < root->leaf_count && !exception; i++)
			{
				o = jep_evaluate(root->leaves[i], list);
				if (o != NULL)
				{
					if (o->ret & JEP_EXCEPTION)
					{
						printf("unhandled exception: %s\n", (char*)(o->val));
						exception = 1;
					}
					jep_destroy_object(o);
					o = NULL;
				}
			}

			if (native_lib != NULL)
			{
				jep_free_lib(native_lib);
			}

			if (flags[JEP_OBJ])
			{
				jep_print_list(list);
			}

			/* destroy all remaining objects */
			jep_destroy_object(list);
		}

		/* destroy the AST */
		jep_destroy_string_builder(root->token.val);
		if (root->leaves != NULL)
		{
			free(root->leaves);
		}
		free(root);
	}

	if (nodes != NULL)
	{
		/* destroy all of the individual AST nodes */
		for (i = 0; i < ts->size; i++)
		{
			if (nodes[i].leaves != NULL)
			{
				free(nodes[i].leaves);
			}
		}
		free(nodes);
	}

	/* destroy the tokens */
	jep_destroy_token_stream(ts);

	print_call_counts();

	return 0;
}
