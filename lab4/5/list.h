#ifndef LIST_H
#define LIST_H

typedef struct Node
{
    int value;
    char data; // для скобок и операций
    struct Node * next;
} Node, *Node_ptr;

typedef struct Stack
{
    Node_ptr head;
    Node_ptr tail;
} Stack, *Stack_ptr;

int push_back(Node_ptr * head, int value);
int push_back_char(Node_ptr * head, char data);
void pop_back(Node_ptr * head);

Node_ptr get_prev_last(Node_ptr head);
Node_ptr get_last(Node_ptr head);

Node_ptr create_node(int value);
Node_ptr create_node_char(char data);

void free_list(Node_ptr head);
void free_node(Node_ptr node);

void print_list(Node_ptr head);

#endif