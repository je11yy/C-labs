#include "list.h"
#include "polynomial.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

List_ptr copy_list(List_ptr list)
{
    List_ptr new_list = create_list();
    if (!new_list) return NULL;
    Node_ptr tmp = list -> head;
    int res;
    while (tmp)
    {
        res = push(&new_list, tmp -> odd, tmp -> power);
        if (res != success)
        {
            free_list(list);
            free_list(new_list);
            return NULL;
        }
        tmp = tmp -> next;
    }
    return new_list;
}

List_ptr create_list()
{
    List_ptr list = (List_ptr)malloc(sizeof(List));
    if (!list) return NULL;
    list -> head = NULL;
    list -> size = 0;
    return list;
}

Node_ptr create_node(int odd, int power)
{
    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return NULL;
    node -> odd = odd;
    node -> power = power;
    node -> next = NULL;
    return node;
}

void free_node(Node_ptr node)
{
    node -> next = NULL;
    free(node);
    node = NULL;
}

void free_list(List_ptr list)
{
    if (!list) return;
    if (!(list -> head))
    {
        free(list);
        list = NULL;
        return;
    }
    Node_ptr tmp = list -> head;
    Node_ptr next;
    while (tmp)
    {
        next = tmp -> next;
        free_node(tmp);
        tmp = next;
    }
    free(list);
    list = NULL;
}

Node_ptr get_last(Node_ptr head)
{
    if (!head) return head;
    Node_ptr tmp = head;
    while (tmp -> next) tmp = tmp -> next;
    return tmp;
}

Node_ptr get_prev_last(Node_ptr head)
{
    if (!head) return head;
    Node_ptr tmp = head;
    Node_ptr prev = tmp;
    while (tmp -> next)
    {
        prev = tmp;
        tmp = tmp -> next;
    }
    return prev;
}

int push_back(List_ptr * list, int odd, int power)
{
    Node_ptr node = create_node(odd, power);
    if (!node) return no_memory;
    Node_ptr last = get_last((*list) -> head);
    last -> next = node;
    ((*list) -> size)++;
    return success;
}

int push(List_ptr * list, int odd, int power)
{
    if (odd == 0) power = 0;
    ((*list) -> size)++;
    if (power > (*list) -> power) (*list) -> power = power;
    Node_ptr node = create_node(odd, power);
    if (!node) return no_memory;
    Node_ptr tmp = (*list) -> head;
    // если еще нет элементов
    if (!tmp)
    {
        (*list) -> head = node;
        return success;
    }
    Node_ptr tmp1;
    Node_ptr prev = tmp;
    while (tmp)
    {
        if (power < tmp -> power)
        {
            if (prev == tmp)
            {
                (*list) -> head = node;
                node -> next = prev;
                return success;
            }
            prev -> next = node;
            node -> next = tmp;
            return success;
        }
        else if (power == tmp -> power)
        {
            tmp -> odd += odd;
            free_node(node);
            return success;
        }
        prev = tmp;
        tmp = tmp -> next;
    }
    // если степень больше всех степеней в многочлене
    prev -> next = node;
    return success;
}

void remove_node(List_ptr * list, Node_ptr node)
{
    Node_ptr tmp = (*list) -> head;
    Node_ptr prev = NULL;
    while (tmp != node)
    {
        prev = tmp;
        tmp = tmp -> next;
    }
    if (!prev)
    {
        free_list(*list);
        return;
    }
    prev -> next = node -> next;
    free_node(node);
}

void pop_back(List_ptr * list)
{
    Node_ptr last = get_last((*list) -> head);
    Node_ptr prev = get_prev_last((*list) -> head);
    if (last == prev)
    {
        free_list(*list);
        return;
    }
    free_node(last);
    prev -> next = NULL;
}