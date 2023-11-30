#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "memory_cell.h"
#include "operations.h"
#include <math.h>

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    invalid_arguments = -6,
    invalid_brackets = -7,
    unknown_variable = -8
};

int print_variable(MemoryCell_array array, int length, char * name)
{
    if (!name)
    {
        print_array(array, length);
        return success;
    }
    MemoryCell_ptr cell = dichotomy_search(array, length, name);
    if (!cell)
    {
        free_memory_cell_array(array, length);
        return unknown_variable;
    }
    printf("%s : %d\n\n", cell -> name, cell -> value);
    return success;
}

void print_array(MemoryCell_array array, int length)
{
    for (int i = 0; i < length; ++i)
    {
        printf("%s : %d\n", array[i] -> name, array[i] -> value);
    }
    printf("\n");
}

/*
переводит выражение, состоящее из названий переменных, в выражение ТОЛЬКО с числами
*/
char* make_expression(int * error, char * expression, MemoryCell_array cell_array, int length, int size)
{
    char * temp;
    int res_size = 10;
    char * result = (char*)calloc(res_size, sizeof(char));
    if (!result)
    {
        free_memory_cell_array(cell_array, length);
        *error = no_memory;
        return NULL;
    }
    int i = 0;
    char * tmp = (char*)malloc(size * sizeof(char));
    if (!tmp)
    {
        free_memory_cell_array(cell_array, length);
        free(result);
        *error = no_memory;
        return NULL;
    }
    char * tmp1;
    int k = 0;
    int index = 0;
    MemoryCell_ptr cell;
    char operator;
    int value;
    while (expression[i] != 0)
    {
        while (is_operator(expression[i]) != success && expression[i] != 0)
        {
            tmp[k] = expression[i];
            k++;
            i++;
        }
        tmp[k] = 0;
        k = 0;
        if (is_number(tmp) == success)
        {
            if (strlen(tmp) > res_size - strlen(result))
            {
                res_size *= 2;
                temp = (char*)realloc(result, res_size * sizeof(char));
                if (!temp)
                {
                    free(result);
                    result = NULL;
                    free_memory_cell_array(cell_array, length);
                    *error = no_memory;
                    return NULL;
                }
                result = temp;
            }
            while (tmp[k] != 0)
            {
                result[index] = tmp[k];
                index++;
                k++;
            }
            for (int j = 0; j < k; ++j) tmp[j] = 0;
            k = 0;
        }
        else if (is_alpha(tmp) == success)
        {
            cell = dichotomy_search(cell_array, length, tmp);
            if (!cell) // использование неинициализированной переменной
            {
                free(result);
                result = NULL;
                free_memory_cell_array(cell_array, length);
                *error = unknown_variable;
                return NULL;
            }
            for (int j = 0; j < k; ++j) tmp[j] = 0;
            k = 0;
            value = cell -> value;
            k = get_size(abs(value));
            tmp1 = (char*)malloc((k + 1) * sizeof(char));
            if (!tmp1)
            {
                free(tmp);
                tmp = NULL;
                free(result);
                result = NULL;
                free_memory_cell_array(cell_array, length);
                *error = no_memory;
                return NULL;
            }
            if (index > 0)
            {
                if (result[index - 1] == '-' && value < 0)
                {
                    value = abs(value);
                    if (index - 1 != 0) result[index - 1] = '+';
                    else index--;
                }
            }
            sprintf(tmp1, "%d", value);
            if (strlen(tmp1) > res_size - strlen(result))
            {
                res_size *= 2;
                temp = (char*)realloc(result, res_size * sizeof(char));
                if (!temp)
                {
                    free(result);
                    result = NULL;
                    free_memory_cell_array(cell_array, length);
                    *error = no_memory;
                    return NULL;
                }
                result = temp;
            }
            for (int j = 0; j < k; ++j)
            {
                result[index] = tmp1[j];
                index++;
            }
            k = 0;
            free(tmp1);
            tmp1 = NULL;
        }
        else
        {
            free_memory_cell_array(cell_array, length);
            *error = incorrect_input;
            return NULL;
        }
        if (expression[i] != 0)
        {
            if (strlen(result) + 1 >= res_size)
            {
                res_size *= 2;
                temp = (char*)realloc(result, res_size * sizeof(char));
                if (!temp)
                {
                    free(result);
                    result = NULL;
                    free_memory_cell_array(cell_array, length);
                    *error = no_memory;
                    return NULL;
                }
                result = temp;
            }
            result[index] = expression[i];
            index++;
            i++;
        }
    }
    free(tmp);
    tmp = NULL;
    *error = success;
    return result;
}

int get_size(int value)
{
    int size = 0;
    while (value)
    {
        value /= 10;
        size++;
    }
    return size;
}

int update_array(MemoryCell_array * cell_array, int * size, int * length, const char * name, const int value)
{
    MemoryCell_ptr cell = dichotomy_search(*cell_array, *length, name);
    int result;
    if (!cell) return append_cell(cell_array, size, length, name, value);
    cell -> value = value;
    return success;
}

int append_cell(MemoryCell_array * cell_array, int * size, int * length, const char * name, const int value)
{
    (*length)++;
    int result;
    if (*length >= *size)
    {
        result = realloc_memory_cell_array(cell_array, size, length);
        if (result != success) return result;
    }
    MemoryCell_ptr cell = create_memory_cell(name, value);
    if (!cell)
    {
        free_memory_cell_array(*cell_array, *length);
        return no_memory;
    }
    (*cell_array)[(*length) - 1] = cell;
    qsort(*cell_array, *length, sizeof(MemoryCell_ptr), (int(*) (const void *, const void *)) memory_cell_comparison);
    return success;
}

MemoryCell_ptr dichotomy_search(MemoryCell_array array, int length, const char * name)
{
    int left = 0;
    int right = length - 1;
    int index;
    int comparison;
    while (left <= right)
    {
        index = left + (right - left) / 2;
        comparison = strcmp(array[index] -> name, name);
        // есть ли в центре
        if (comparison == success) return array[index];
        if (comparison < 0) left = index + 1; // игнорируем левую половину
        else right = index - 1; // игнорируем правую половину
    }
    return NULL; // если не найден в массиве
}

int memory_cell_comparison(const MemoryCell_ptr * first, const MemoryCell_ptr * second)
{
    return strcmp((*first) -> name, (*second) -> name);
}

MemoryCell_ptr create_memory_cell(const char * name, const int value)
{
    char * cell_name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    if (!cell_name) return NULL;
    strcpy(cell_name, name);
    MemoryCell_ptr cell = (MemoryCell_ptr)malloc(sizeof(MemoryCell));
    if (!cell)
    {
        free(cell_name);
        cell_name = NULL;
        return NULL;
    }
    cell -> name = cell_name;
    cell -> value = value;
    return cell;
}

int realloc_memory_cell_array(MemoryCell_array * array, int * size, int * length)
{
    if (!(*array)) (*size) = 1;
    else (*size) *= 2;
    MemoryCell_array tmp = (MemoryCell_array)realloc(*array, *size * sizeof(MemoryCell_ptr));
    if (!tmp)
    {
        free_memory_cell_array(*array, *length);
        return no_memory;
    }
    *array = tmp;
    return success;
}

void free_memory_cell_array(MemoryCell_array array, const int length)
{
    if (!array) return;
    for (int i = 0; i < length; ++i) free_memory_cell(array[i]);
    free(array);
    array = NULL;
}

void free_memory_cell(MemoryCell_ptr cell)
{
    if (!cell) return;
    free(cell -> name);
    cell -> name = NULL;
    free(cell);
    cell = NULL;
}