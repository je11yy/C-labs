#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

Node_ptr create_node(char data)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    node -> data = data;
    node -> children = NULL;
    node -> next = NULL;
    node -> children_count = 0;
    return node;
}

Node_ptr create_expression_tree(char * expression)
{
    int length = strlen(expression);
    Node_ptr root = create_node(expression[0]);
    Node_ptr current = root;
    Node_ptr node;
    Node_array stack = (Node_array)malloc(length * sizeof(Node_ptr));
    int result;
    int stack_size = 0;
    for (int i = 0; i < length; ++i)
    {
        if (expression[i] == '(')
        {
            node = create_node(expression[i + 1]);
            stack[stack_size] = current;
            stack_size++;
            result = add_child(current, node);
            current = node;
        }
        else if (expression[i] == ',')
        {
            node = create_node(expression[i + 1]);
            result = add_child(stack[stack_size - 1], node);
            current -> next = node;
            current = node;
        }
        else if (expression[i] == ')')
        {
            stack[stack_size - 1] = NULL;
            stack_size--;
        }
    }
    free(stack);
    stack = NULL;
    return root;
}

void delete_tree(Node_ptr root)
{
    if (!root) return;

    for (int i = 0; i < root -> children_count; ++i)
    {
        delete_tree((root -> children)[i]);
    }
    free(root -> children);
    root -> children = NULL;
    free(root);
    root = NULL;
}

int add_child(Node_ptr parent, Node_ptr child)
{
    (parent -> children_count)++;
    Node_array temp = (Node_array)realloc(parent -> children, (parent -> children_count) * sizeof(Node_ptr));
    if (!temp)
    {
        delete_tree(child);
        return no_memory;
    }
    parent -> children = temp;
    (parent -> children)[parent -> children_count - 1] = child;
}

void print_tree(FILE * file, Node_ptr root, int depth)
{
    if (root)
    {
        for (int i = 0; i < depth; ++i) fprintf(file, "    ");
        fprintf(file, "%c\n", root -> data);
        for (int i = 0; i < root -> children_count; i++) print_tree(file, (root -> children)[i], depth + 1);
    }
}