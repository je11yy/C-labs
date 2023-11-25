#include "stack.h"
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
    invalid_brackets = -6,
    invalid_arguments = -7,
    delete_file_error = -8
};

// реализация для чаров
int push_back_char(Char_stack_ptr * head, char data)
{
    Char_stack_ptr last = get_last_char(*head);
    Char_stack_ptr tmp = create_node_char(data);
    if (!tmp)
    {
        free_stack_char(*head);
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

Char_stack_ptr create_node_char(char data)
{
    Char_stack_ptr node = (Char_stack_ptr)malloc(sizeof(Char_stack));
    if (!node) return NULL;
    node -> data = data;
    node -> next = NULL;
    return node;
}

void pop_back_char(Char_stack_ptr * head)
{
    if (!(*head)) return;
    Char_stack_ptr prev = get_prev_last_char(*head);
    Char_stack_ptr last = get_last_char(*head);
    free_node_char(last);
    if (prev) prev -> next = NULL;
    else *head = NULL;
}

void free_node_char(Char_stack_ptr node)
{
    if (!node) return;
    node -> next = NULL;
    free(node);
    node = NULL;
}

Char_stack_ptr get_prev_last_char(Char_stack_ptr head)
{
    if (!head) return NULL;
    Char_stack_ptr prev = NULL;
    Char_stack_ptr tmp = head;
    while (tmp -> next)
    {
        prev = tmp;
        tmp = tmp -> next;
    }
    return prev;
}

Char_stack_ptr get_last_char(Char_stack_ptr head)
{
    if (!head) return NULL;
    Char_stack_ptr tmp = head;
    while (tmp -> next) tmp = tmp -> next;
    return tmp;
}

void free_stack_char(Char_stack_ptr head)
{
    if (!head) return;
    Char_stack_ptr prev = NULL;
    while (head -> next)
    {
        prev = head;
        head = head -> next;
        free_node_char(prev);
    }
    free_node_char(head);
}

// реализация для дерева
int push_back_tree(Tree_stack_ptr * head, Node_ptr tree_node)
{
    Tree_stack_ptr last = get_last_tree(*head);
    Tree_stack_ptr tmp = create_node_tree(tree_node);
    if (!tmp)
    {
        free_stack_tree(*head);
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

Tree_stack_ptr create_node_tree(Node_ptr tree_node)
{
    Tree_stack_ptr node = (Tree_stack_ptr)malloc(sizeof(Tree_stack));
    if (!node) return NULL;
    node -> node = tree_node;
    node -> next = NULL;
    return node;
}

void pop_back_tree(Tree_stack_ptr * head)
{
    if (!(*head)) return;
    Tree_stack_ptr prev = get_prev_last_tree(*head);
    Tree_stack_ptr last = get_last_tree(*head);
    free_node_tree(last);
    if (prev) prev -> next = NULL;
    else *head = NULL;
}

void free_node_tree(Tree_stack_ptr node)
{
    if (!node) return;
    node -> next = NULL;
    node -> node = NULL;
    free(node);
    node = NULL;
}

Tree_stack_ptr get_prev_last_tree(Tree_stack_ptr head)
{
    if (!head) return NULL;
    Tree_stack_ptr prev = NULL;
    Tree_stack_ptr tmp = head;
    while (tmp -> next)
    {
        prev = tmp;
        tmp = tmp -> next;
    }
    return prev;
}

Tree_stack_ptr get_last_tree(Tree_stack_ptr head)
{
    if (!head) return NULL;
    Tree_stack_ptr tmp = head;
    while (tmp -> next) tmp = tmp -> next;
    return tmp;
}

void free_stack_tree(Tree_stack_ptr head)
{
    if (!head) return;
    Tree_stack_ptr prev = NULL;
    while (head -> next)
    {
        prev = head;
        head = head -> next;
        free_node_tree(prev);
    }
    free_node_tree(head);
}