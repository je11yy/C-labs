#ifndef TREE_H
#define TREE_H

#include "string.h"

typedef struct tree
{
    int count;
    String_ptr word;
    struct tree * left;
    struct tree * right;
    struct tree * parent;
} Node, *Node_ptr;

void print_symmetrical_tree(Node_ptr root);
void print_tree(Node_ptr root, int space);

Node_ptr create_root(int count, char * word);
Node_ptr add_node(Node_ptr root, int count, char * word);

Node_ptr min_node(Node_ptr root);
Node_ptr max_node(Node_ptr root);
Node_ptr search_min(Node_ptr root);

Node_ptr search_node_via_word(Node_ptr root, char * word);

void delete_node(Node_ptr root, char * word);
// очистка памяти 
void free_node(Node_ptr node);
void delete_tree(Node_ptr root);

int get_depth(Node_ptr node);

#endif