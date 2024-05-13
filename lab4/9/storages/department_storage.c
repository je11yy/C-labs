#include "department_storage.h"

Department_storage_ptr department_storage_create(department_type type)
{
    Department_storage_ptr department_storage = (Department_storage_ptr)malloc(sizeof(Department_storage));
    if (!department_storage) return NULL;
    department_storage->type = type;
    void * storage = NULL;
    switch (type)
    {
        case HashSet_T:
            storage = HS_create();
            if (!storage)
            {
                free(department_storage);
                return NULL;
            }
            department_storage->insert =   (status (*) (void *, int, Department_ptr)) HS_insert;
            department_storage->find   = (status (*) (void *, int, Department_ptr *)) HS_find;
            department_storage->free   =                        (status (*) (void *)) HS_free;
            break;
        case DynamicArray_T:
            storage = dynamic_array_create();
            if (!storage)
            {
                free(department_storage);
                return NULL;
            }
            department_storage->insert =   (status (*) (void *, int, Department_ptr)) dynamic_array_insert;
            department_storage->find   = (status (*) (void *, int, Department_ptr *)) dynamic_array_find;
            department_storage->free   =                        (status (*) (void *)) dynamic_array_free;
            break;
        case BST_T:
            storage = BST_create();
            if (!storage)
            {
                free(department_storage);
                return NULL;
            }
            department_storage->insert =   (status (*) (void *, int, Department_ptr)) BST_insert;
            department_storage->find   = (status (*) (void *, int, Department_ptr *)) BST_find;
            department_storage->free   =                        (status (*) (void *)) BST_free;
            break;
        case Trie_T:
            storage = Trie_create();
            if (!storage)
            {
                free(department_storage);
                return NULL;
            }
            department_storage->insert =   (status (*) (void *, int, Department_ptr)) Trie_insert;
            department_storage->find   = (status (*) (void *, int, Department_ptr *)) Trie_find;
            department_storage->free   =                        (status (*) (void *)) Trie_free;
            break;
    }
    return department_storage;
}