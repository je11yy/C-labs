#ifndef _DEPARTMENT_STORAGE_H_
#define _DEPARTMENT_STORAGE_H_

#include "departments/HashSet.h"
#include "departments/DynamicArray.h"
#include "departments/BinarySearchTree.h"
#include "departments/Trie.h"

typedef enum department_type
{
    HashSet_T,
    DynamicArray_T,
    BST_T,
    Trie_T
} department_type;

typedef struct Department_storage
{
    void * storage;
    department_type type;

    status (*insert) (void * storage, int key, Department_ptr dep);
    status (*find) (void * storage, int key, Department_ptr * dep);

    void (*free) (void * storage);

} Department_storage, *Department_storage_ptr;

Department_storage_ptr department_storage_create(void * storage, department_type type);


#endif