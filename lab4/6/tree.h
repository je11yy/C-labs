#ifndef TREE_H
#define TREE_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "arithmetic.h"

typedef struct Node // корень дерева
{
    char data; // могут быть двусимвольные операции
    struct Node * left;
    struct Node * right;
} Node, *Node_ptr;

void print_infix(FILE * file, Node_ptr root);
void print_tree(Node_ptr root, int level);

Node_ptr create_tree_node(char data, Node_ptr left, Node_ptr right);

void free_tree_node(Node_ptr node);
void free_tree(Node_ptr root);

#endif