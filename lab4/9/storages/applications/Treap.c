#include "Treap.h"

Treap_ptr Treap_create()
{
    Treap_ptr heap = (Treap_ptr)malloc(sizeof(Treap));
    if (!heap) return NULL;
    heap->root = NULL;
    heap->size = 0;
    return heap;
}

Treap_node_ptr Treap_node_create(Application_ptr application)
{
    Treap_node_ptr node = (Treap_node_ptr)malloc(sizeof(Treap_node));
    if (!node)
    {
        free_application(application);
        return NULL;
    }
    node->application = application;
    node->prev = NULL;
    node->next = NULL;

    srand(time(NULL));
    node->priority = rand();
    return node;
}

void Treap_node_free(Treap_node_ptr node)
{
    if (!node) return;
    Treap_node_free(node->next);
    Treap_node_free(node->prev);

    if (node->application) free_application(node->application);
    free(node);
    node = NULL;
}

void Treap_free(Treap_ptr * storage)
{
    if (!(*storage)) return;
    if ((*storage)->root) Treap_node_free((*storage)->root);
    free(*storage);
    *storage = NULL;
}

status Treap_copy(Treap_ptr * to, Treap_ptr from)
{
    if (*to || !from) return invalid_function_argument;
    status error;
    if ((error = Treap_node_copy(&(*to)->root, from->root)) != success)
    {
        Treap_free(to);
        Treap_free(&from);
        return error;
    }

    (*to)->size = from->size;

    return success;
}

status Treap_node_copy(Treap_node_ptr * to, Treap_node_ptr from)
{
    if (*to)
    {
        Treap_node_free(from);
        return invalid_function_argument;
    }

    if (!from)
    {
        *to = NULL;
        return success;
    }

    Treap_node_ptr new = Treap_node_create(copy_application(from->application));
    if (!new)
    {
        Treap_node_free(from);
        return no_memory;
    }

    status error;
    if ((error = Treap_node_copy(&new->next, from->next)) != success || (error = Treap_node_copy(&new->prev, from->prev)) != success)
    {
        Treap_node_free(from);
        Treap_node_free(*to);
        return error;
    }

    *to = new;

    return success;
}

status Treap_insert(Treap_ptr * storage, Application_ptr application)
{
    if (!(*storage) || !application) return invalid_function_argument;

    Treap_node_ptr node = Treap_node_create(application);
    if (!node)
    {
        Treap_free(storage);
        free_application(application);
        return no_memory;
    }

    Treap_node_ptr less = NULL;
    Treap_node_ptr greater = NULL;

    status error;

    if ((error = Treap_split((*storage)->root, application, &less, &greater)) != success)
    {
        Treap_node_free(node);
        return error;
    }

    (*storage)->root = Treap_node_merge(Treap_node_merge(less, node), greater);

    (*storage)->size++;
    return success;
}

status Treap_split(Treap_node_ptr node, Application_ptr application, Treap_node_ptr * first, Treap_node_ptr * second)
{
    if (!application) return invalid_function_argument;

    if (!node) return success;
    status error;
    if (applications_comparator(node->application, application) > 0)
    {
        if ((error = Treap_split(node->next, application, &(node->next), second)) != success) return error;
        *first = node;
    }
    else
    {
        if ((error = Treap_split(node->prev, application, first, &(node->prev))) != success) return error;
        *second = node;
    }
    return success;
}

Treap_node_ptr Treap_node_merge(Treap_node_ptr first, Treap_node_ptr second)
{
    if (!first && !second) return NULL;
    if (!first) return second;
    if (!second) return first;

    if (first->priority > second->priority)
    {
        first->next = Treap_node_merge(first->next, second);
        return first;
    }
    second->next = Treap_node_merge(first, second->prev);
    return second;
}

status Treap_meld(Treap_ptr * res, Treap_ptr * first, Treap_ptr * second)
{
    if (!first && !second) return invalid_function_argument;
    if (!((*first)->root) && !((*second)->root)) return success;

    *res = Treap_create();
    if (!(*res))
    {
        Treap_free(first);
        Treap_free(second);
        return no_memory;
    }

    (*res)->root = Treap_node_merge((*first)->root, (*second)->root);
    (*first)->root = (*second)->root = NULL;
    Treap_free(first);
    Treap_free(second);
    return success;
}

status Treap_merge(Treap_ptr * res, Treap_ptr * first, Treap_ptr * second)
{
    Treap_ptr first_copy = NULL;
    Treap_ptr second_copy = NULL;
    status error;
    if ((error = Treap_copy(&first_copy, *first)) != success || (error = Treap_copy(&second_copy, *second)) != success)
    {
        Treap_free(first);
        Treap_free(second);
        return error;
    }

    return Treap_meld(res, &first_copy, &second_copy);
}

status Treap_get_max(Treap_ptr * storage, Application_ptr * res_application)
{
    if (!(*storage) || *res_application) return invalid_function_argument;

    if (!(*storage)->root) return success;

    
    Treap_node_ptr current = (*storage)->root;
    while (current->next) current = current->next;

    *res_application = current->application;

    return success;
}

status Treap_delete_max(Treap_ptr * storage, Application_ptr * res_application)
{
    if (!(*storage) || *res_application) return invalid_function_argument;

    if (!(*storage)->root) return success;

    Treap_node_ptr root = (*storage)->root;

    if (!(root->next))
    {
        *res_application = root->application;
        (*storage)->root = root->prev;
        free(root);
        return success;
    }

    Treap_node_ptr prev = root;
    root = root->next;

    while (root->next)
    {
        prev = root;
        root = root->next;
    }

    *res_application = root->application;
    prev->next = root->prev;

    free(root);
    (*storage)->size--;

    return success;
}