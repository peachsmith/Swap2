/*
    Functions for evaluating and modifying an abstract syntax tree
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

/**
 * create an AST node
 */
jep_ast_node *jep_create_ast_node()
{
	jep_ast_node *node = malloc(sizeof(jep_ast_node));
	node->leaf_count = 0;
	node->cap = 10;
	node->leaves = NULL;
	node->array = 0;
	node->loop = 0;
	node->mod = 0;
	return node;
}

/**
 * adds a leaf node to an AST node
 */
void jep_add_leaf_node(jep_ast_node *root, jep_ast_node *leaf)
{

	if (root->leaves == NULL)
	{
		root->leaves = malloc(root->cap * sizeof(jep_ast_node));
	}

	if (root->leaf_count == root->cap)
	{
		int new_cap = root->cap + root->cap / 2;
		int new_size = sizeof(jep_ast_node) * new_cap;
		jep_ast_node *new_leaves = realloc(root->leaves, new_size);
		if (new_leaves != NULL)
		{
			root->leaves = new_leaves;
			root->cap = new_cap;
		}
		else
		{
			if (!root->error)
			{
				printf("failed to reallocate memory for AST\n");
			}
			return;
		}
	}
	if (leaf != NULL)
	{
		root->leaves[root->leaf_count++] = *leaf;
	}
}

/**
 * prints the AST
 */
void jep_print_ast(jep_ast_node root)
{
	static int indent = 1;

	if (root.leaf_count > 0)
	{
		printf("%*s\n", indent, root.token.val->buffer);
		indent++;
		int i;
		for (i = 0; i < root.leaf_count; i++)
		{
			jep_print_ast(root.leaves[i]);
		}
		indent--;
	}
	else
	{
		printf("%*s\n", indent, root.token.val->buffer);
	}
}

/**
 * pushes an AST node onto the top of the stack
 */
void jep_push(jep_stack *stack, jep_ast_node *node)
{

	if (stack->size == stack->cap)
	{
		int new_cap = stack->cap + stack->cap / 2;
		jep_ast_node **new_nodes = malloc(new_cap * sizeof(jep_ast_node *));

		int i;
		for (i = 0; i < stack->cap; i++)
		{
			new_nodes[i] = stack->nodes[i];
		}

		free(stack->nodes);

		stack->nodes = new_nodes;
		stack->cap = new_cap;
	}

	stack->nodes[stack->size++] = node;
	stack->top = stack->nodes[stack->size - 1];
}

/**
 * pops an AST node from the top of the stack
 */
jep_ast_node *jep_pop(jep_stack *stack)
{
	if (stack->size > 0)
	{
		jep_ast_node *top = stack->nodes[stack->size - 1];
		stack->nodes[stack->size - 1] = NULL;
		stack->size = stack->size - 1;
		if (stack->size > 0)
		{
			stack->top = stack->nodes[stack->size - 1];
		}
		else
		{
			stack->top = NULL;
		}
		return top;
	}
	else
	{
		return NULL;
	}
}

/**
 * pops all AST nodes from the stack
 */
void jep_pop_all(jep_stack *stack)
{
	int i;
	for (i = 0; i < stack->size; i++)
	{
		stack->nodes[i] = NULL;
	}
}
