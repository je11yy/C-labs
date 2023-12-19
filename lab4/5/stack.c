#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    incorrect_command_input = -6,
    incorrect_array_input = -7
};

Node_ptr create_node(int data)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return NULL;
    node -> next = NULL;
    node -> data = data;
    return node;
}

Stack_ptr create_stack()
{
    Stack_ptr stack = (Stack_ptr)malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack -> size = 0;
    stack -> head = NULL;
    return stack;
}

void free_node(Node_ptr node)
{
    if (!node) return;
    node -> next = NULL;
    free(node);
    node = NULL;
}

void free_stack(Stack_ptr stack)
{
    while (stack -> head)
    {
        pop(stack);
    }
    free(stack);
    stack = NULL;
}

int push(Stack_ptr stack, int data)
{
    Node_ptr node = create_node(data);
    if (!node) return no_memory;
    node -> next = stack -> head;
    stack -> head = node;
    return success;
}

int pop(Stack_ptr stack)
{
    if (!(stack -> head)) return -9999;
    int data = stack -> head -> data;
    Node_ptr next = stack -> head -> next;
    free_node(stack -> head);
    stack -> head = next;
    return data;
}