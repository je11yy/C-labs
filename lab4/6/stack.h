#ifndef STACK_H
#define STACK_H

#include "tree.h"

// стек для чаров
typedef struct Char_stack
{
    char data;
    struct Char_stack * next;
} Char_stack, *Char_stack_ptr;

// стек для узлов дерева
typedef struct Tree_stack
{
    Node_ptr node;
    struct Tree_stack * next;
} Tree_stack, *Tree_stack_ptr;


// реализация для чаров
int push_back_char(Char_stack_ptr * head, char data);
Char_stack_ptr create_node_char(char data);
void pop_back_char(Char_stack_ptr * head);
void free_node_char(Char_stack_ptr node);
Char_stack_ptr get_prev_last_char(Char_stack_ptr head);
Char_stack_ptr get_last_char(Char_stack_ptr head);
void free_stack_char(Char_stack_ptr head);

// реализация для дерева
int push_back_tree(Tree_stack_ptr * head, Node_ptr tree_node);
Tree_stack_ptr create_node_tree(Node_ptr tree_node);
void pop_back_tree(Tree_stack_ptr * head);
void free_node_tree(Tree_stack_ptr node);
Tree_stack_ptr get_prev_last_tree(Tree_stack_ptr head);
Tree_stack_ptr get_last_tree(Tree_stack_ptr head);
void free_stack_tree(Tree_stack_ptr head);

#endif