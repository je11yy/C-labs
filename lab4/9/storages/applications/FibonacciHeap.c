#include "FibonacciHeap.h"
#include <math.h>

Fibonacci_Heap_ptr Fibonacci_Heap_create()
{
    Fibonacci_Heap_ptr heap = (Fibonacci_Heap_ptr)malloc(sizeof(Fibonacci_Heap));
    if (!heap) return NULL;
    heap->size = 0;
    heap->max = NULL;
    return heap;
}

status Fibonacci_Heap_copy(Fibonacci_Heap_ptr * to, Fibonacci_Heap_ptr from)
{
    if (*to)
    {
        Fibonacci_Heap_free(&from);
        return invalid_function_argument;
    }
    if (!from)
    {
        *to = NULL;
        return success;
    }
    *to = Fibonacci_Heap_create();
    if (!(*to))
    {
        Fibonacci_Heap_free(&from);
        return no_memory;
    }
}

FH_node_ptr Fibonacci_Heap_node_create(Application_ptr application)
{
    FH_node_ptr node = (FH_node_ptr)malloc(sizeof(FH_node));
    if (!node)
    {
        free_application(application);
        return NULL;
    }
    node->application = application;
    node->rank = 0;
    node->child = node->prev = node->next = NULL;
    return node;
}

void Fibonacci_Heap_node_free(FH_node_ptr node)
{
    if (!node) return;
    if (node->prev) node->prev->next = NULL;

    Fibonacci_Heap_node_free(node->child);
    Fibonacci_Heap_node_free(node->next);

    if (node->application) free_application(node->application);

    node->child = node->prev = node->next = NULL;
    free(node);
    node = NULL;
}

status Fibonacci_Heap_node_copy(FH_node_ptr * to, FH_node_ptr from)
{
    if (*to) return invalid_function_argument;

    if (!from) return success;

    *to = Fibonacci_Heap_node_create(copy_application(from->application));
    if (!(*to))
    {
        Fibonacci_Heap_node_free(from);
        return no_memory;
    }
    (*to)->rank = from->rank;

    status error;

    if ((error = Fibonacci_Heap_node_copy(&(*to)->child, from->child)) != success || (error = Fibonacci_Heap_node_copy(&(*to)->next, from->next)) != success)
    {
        Fibonacci_Heap_node_free(*to);
        Fibonacci_Heap_node_free(from);
        return error;
    }

    if ((*to)->next)
    {
        (*to)->prev = (*to)->next->prev;
        (*to)->prev->next = (*to);
        (*to)->next->prev = (*to);
    }
    else (*to)->prev = (*to);

    return success;
}

status Fibonacci_Heap_set_null(Fibonacci_Heap_ptr * storage)
{
    if (!(*storage)) return invalid_function_argument;

    (*storage)->max = NULL;
    (*storage)->size = 0;
    return success;
}

void Fibonacci_Heap_free(Fibonacci_Heap_ptr * storage)
{
    if (!(*storage)) return;
    if ((*storage)->max) Fibonacci_Heap_node_free((*storage)->max);
    Fibonacci_Heap_set_null(storage);
    free(*storage);
    *storage = NULL;
}

status Fibonacci_Heap_meld(Fibonacci_Heap_ptr * res, Fibonacci_Heap_ptr * first, Fibonacci_Heap_ptr * second)
{
    if (*res) return invalid_function_argument;
    if (!(*first) && !(*second)) return success;
    if (!(*first))
    {
        *res = *second;
        *second = NULL;
        return success;
    }
    if (!(*second))
    {
        *res = *first;
        *first = NULL;
        return success;
    }
    *res = *first;

    FH_node_ptr tmp = (*res)->max->next;
    (*res)->max->next = (*second)->max;
    (*second)->max->prev = (*res)->max;
    tmp->prev = (*second)->max;
    (*second)->max->next = tmp;

    if (applications_comparator((*res)->max->application, (*second)->max->application) < 0) (*res)->max = (*second)->max;

    free(*second);
    *second = NULL;
    *first = NULL;

    return success;
}

status Fibonacci_Heap_merge(Fibonacci_Heap_ptr * res, Fibonacci_Heap_ptr * first, Fibonacci_Heap_ptr * second)
{
    Fibonacci_Heap_ptr first_copy = NULL;
    Fibonacci_Heap_ptr second_copy = NULL;
    status error;
    if ((error = Fibonacci_Heap_copy(&first_copy, *first)) != success || (error = Fibonacci_Heap_copy(&second_copy, *second)) != success)
    {
        Fibonacci_Heap_free(first);
        Fibonacci_Heap_free(second);
        return error;
    }

    return Fibonacci_Heap_meld(res, &first_copy, &second_copy);
}

status Fibonacci_Heap_node_merge(Fibonacci_Heap_ptr * storage, FH_node_ptr first, FH_node_ptr second, FH_node_ptr * res)
{
    if (!(*storage) || !first || !second || !res) return invalid_function_argument;

    if (first == second || first->rank != second->rank) return invalid_function_argument;

    if (applications_comparator(first->application, second->application) < 0)
    {
        FH_node_ptr tmp = first;
        first = second;
        second = first;
    }

    if (first->child)
    {
        second->next = first->child;
        second->prev = first->child->prev;
        second->prev->next = second;
        second->next->prev = second;
    }
    else
    {
        second->prev = second;
        second->next = second;
    }

    first->child = second;
    first->rank++;

    *res = first;

    return success;
}

status Fibonacci_Heap_consolidate(Fibonacci_Heap_ptr * storage)
{
    if (!(*storage)) return invalid_function_argument;

    if (!(*storage)->max || (*storage)->max->next == (*storage)->max) return success;

    FH_node_ptr current = (*storage)->max;
    FH_node_ptr next = current->next;
    size_t size = log2(1.0 * (*storage)->size) + 3;

    FH_node_ptr * array = (FH_node_ptr*)calloc(size, sizeof(FH_node_ptr));
    if (!array)
    {
        Fibonacci_Heap_free(storage);
        return no_memory;
    }

    (*storage)->max->prev->next = NULL;

    status error;

    while (current)
    {
        next = current->next;
        if (current->next == current) current->next = NULL;

        while (array[current->rank])
        {
            FH_node_ptr tmp = array[current->rank];
            array[current->rank] = NULL;
            if ((error = Fibonacci_Heap_node_merge(storage, current, tmp, &current)) != success)
            {
                Fibonacci_Heap_free(storage);
                return error;
            }
            array[size - 1] = current;
        }

        array[current->rank] = current;
        current = next;
    }

    FH_node_ptr top = NULL;
    FH_node_ptr start = NULL;
    FH_node_ptr prev = NULL;

    for (size_t i = 0; i < size; ++i)
    {
        if (!array[i]) continue;
        start = start ? start : array[i];
        top = top ? top : array[i];

        if (applications_comparator(array[i]->application, top->application) > 0) top = array[i];
        if (prev) prev->next = array[i];

        array[i]->prev = prev;
        prev = array[i];
    }

    prev->next = start;
    start->prev = prev;
    (*storage)->max = top;
    free(array);

    return success;
}

status Fibonacci_Heap_get_max(Fibonacci_Heap_ptr * storage, Application_ptr * res_application)
{
    if (!(*storage) || !((*storage)->max))
    {
        Fibonacci_Heap_free(storage);
        return invalid_function_argument;
    }
    *res_application = (*storage)->max->application;
    return success;
}

status Fibonacci_Heap_delete_max(Fibonacci_Heap_ptr * storage, Application_ptr * res_application)
{
    if (!(*storage) || !((*storage)->max))
    {
        Fibonacci_Heap_free(storage);
        return invalid_function_argument;
    }
    *res_application = (*storage)->max->application;

    FH_node_ptr top = (*storage)->max;

    if (top->next == top && !top->child) (*storage)->max = NULL;
    else if (top->next == top) (*storage)->max = (*storage)->max->child;
    else if (!top->child)
    {
        (*storage)->max->prev->next = (*storage)->max->next;
        (*storage)->max->next->prev = (*storage)->max->prev;
        (*storage)->max = (*storage)->max->prev;
    }
    else
    {
        (*storage)->max->prev->next = (*storage)->max->child;
        (*storage)->max->next->prev = (*storage)->max->child->prev;
        (*storage)->max->prev->next->prev = (*storage)->max->prev;
        (*storage)->max->next->prev->next = (*storage)->max->next;
        (*storage)->max = (*storage)->max->prev;
    }

    free(top);
    top = NULL;
    (*storage)->size--;

    return Fibonacci_Heap_consolidate(storage);
}

status Fibonacci_Heap_insert(Fibonacci_Heap_ptr * storage, Application_ptr application)
{
    if (!(*storage) || !application) return invalid_function_argument;

    FH_node_ptr node = Fibonacci_Heap_node_create(application);
    if (!node)
    {
        Fibonacci_Heap_free(storage);
        return no_memory;
    }

    if (!(*storage)->max)
    {
        node->prev = node->next = node;
        (*storage)->max = node;
    }
    else
    {
        node->prev = (*storage)->max->prev;
        node->next = (*storage)->max;
        node->prev->next = node;
        node->next->prev = node;

        if (applications_comparator(node->application, (*storage)->max->application) > 0) (*storage)->max = node;
    }

    (*storage)->size++;

    return success;
}