#include <stdio.h>
#include <stdlib.h>
#include "arrays.h"
#include "functions.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5
};

int array_exists(Arrays_ptr arrays, char name)
{
    if (!arrays) return fail;
    for (int i = 0; i < arrays -> quantity; ++i)
    {
        if ((arrays -> arrays)[i] -> name == name) return i;
    }
    return fail;
}

Array_ptr create_array(char name)
{
    Array_ptr array = (Array_ptr)malloc(sizeof(Array));
    if (!array) return NULL;
    array -> size = 0;
    array -> name = name;
    array -> array = NULL;
    return array;
}

Arrays_ptr create_arrays()
{
    Arrays_ptr arrays = (Arrays_ptr)malloc(sizeof(Arrays));
    if (!arrays) return NULL;
    arrays -> quantity = 0;
    arrays -> arrays = NULL;
    return arrays;
}

int append_array(Arrays_ptr * arrays, Array_ptr new_array)
{
    int quantity = (*arrays) -> quantity + 1;
    Array_array tmp = (Array_array)realloc((*arrays) -> arrays, quantity * sizeof(Array_ptr));
    if (!tmp)
    {
        delete_arrays(*arrays);
        delete_array(new_array);
        return no_memory;
    }
    (*arrays) -> arrays = tmp;
    ((*arrays) -> arrays)[quantity - 1] = new_array;
    (*arrays) -> quantity = quantity;
    return success;
}

int append_element(Array_ptr * array, int element)
{
    int size = (*array) -> size + 1;
    int * tmp = (int*)realloc((*array) -> array, size * sizeof(int));
    if (!tmp)
    {
        delete_array(*array);
        return no_memory;
    }
    (*array) -> array = tmp;
    ((*array) -> array)[size - 1] = element;
    (*array) -> size = size;
    return success;
}

void delete_arrays(Arrays_ptr arrays)
{
    if (!arrays) return;
    for (int i = 0; i < arrays -> quantity; ++i)
    {
        if ((arrays -> arrays)[i]) delete_array((arrays -> arrays)[i]);
    }
    free(arrays -> arrays);
    arrays -> arrays = NULL;
    free(arrays);
    arrays = NULL;
}

void delete_array(Array_ptr array)
{
    if (!array) return;
    if (array -> array)
    {
        free(array -> array);
        array -> array = NULL;
    }
    free(array);
    array = NULL;
}