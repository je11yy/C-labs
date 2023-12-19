#ifndef STACK_H
#define STACK_H

typedef struct Node
{
    int data;
    struct Node * next;
} Node, *Node_ptr;

typedef struct stack
{
    int size;
    Node_ptr head;
} Stack, *Stack_ptr;

int pop(Stack_ptr stack);
int push(Stack_ptr stack, int data);
void free_stack(Stack_ptr stack);
void free_node(Node_ptr node);
Stack_ptr create_stack();
Node_ptr create_node(int data);

#endif