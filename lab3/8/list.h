#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int odd;
    int power;
    struct Node * next;
} Node, *Node_ptr;

typedef struct
{
    Node_ptr head;
    int size;
    int power;
} List, *List_ptr;

List_ptr copy_list(List_ptr list);
List_ptr create_list();
Node_ptr create_node(int odd, int power);

void free_node(Node_ptr node);
void free_list(List_ptr list);

Node_ptr get_last(Node_ptr head);
Node_ptr get_prev_last(Node_ptr head);

int push(List_ptr * list, int odd, int power);
void pop_back(List_ptr * list);

#endif