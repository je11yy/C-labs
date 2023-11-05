#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Node
{
    char data;
    struct Node ** children;
    struct Node * next;
    int children_count;
} Node, *Node_ptr, **Node_array;

Node_ptr create_node(char data);
int add_child(Node_ptr parent, Node_ptr child);
void print_tree(FILE * file, Node_ptr root, int depth);
void delete_tree(Node_ptr root);
Node_ptr create_expression_tree(char * expression);

#endif