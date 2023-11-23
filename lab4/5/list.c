#include "list.h"
#include <stdlib.h>
#include <stdio.h>

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

int push_back(Node_ptr * head, int value)
{
    Node_ptr last = get_last(*head);
    Node_ptr tmp = create_node(value);
    if (!tmp)
    {
        free_list(*head);
        return no_memory;
    }
    if (!last)
    {
        *head = tmp;
        return success;
    }
    last -> next = tmp;
    return success;
}

int push_back_char(Node_ptr * head, char data)
{
    Node_ptr last = get_last(*head);
    Node_ptr tmp = create_node_char(data);
    if (!tmp)
    {
        free_list(*head);
        return no_memory;
    }
    if (!last)
    {
        *head = tmp;
        return success;
    }
    last -> next = tmp;
    return success;
}

void pop_back(Node_ptr * head)
{
    if (!(*head)) return;
    Node_ptr prev = get_prev_last(*head);
    Node_ptr last = get_last(*head);
    free_node(last);
    if (prev) prev -> next = NULL;
    else *head = NULL;
}

Node_ptr get_prev_last(Node_ptr head)
{
    if (!head) return NULL;
    Node_ptr prev = NULL;
    Node_ptr tmp = head;
    while (tmp -> next)
    {
        prev = tmp;
        tmp = tmp -> next;
    }
    return prev;
}

Node_ptr get_last(Node_ptr head)
{
    if (!head) return NULL;
    Node_ptr tmp = head;
    while (tmp -> next) tmp = tmp -> next;
    return tmp;
}

Node_ptr create_node_char(char data)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return NULL;
    node -> data = data;
    node -> value = -999;
    node -> next = NULL;
    return node;
}

Node_ptr create_node(int value)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return NULL;
    node -> value = value;
    node -> data = '_';
    node -> next = NULL;
    return node;
}

void free_list(Node_ptr head)
{
    if (!head) return;
    Node_ptr prev = NULL;
    while (head -> next)
    {
        prev = head;
        head = head -> next;
        free_node(prev);
    }
    free_node(head);
}

void free_node(Node_ptr node)
{
    if (!node) return;
    node -> next = NULL;
    free(node);
    node = NULL;
}

void print_list(Node_ptr head)
{
    while (head)
    {
        if (head -> data == '_') printf("%d\n", head -> value);
        else printf("%c\n", head -> data);
        head = head -> next;
    }
}