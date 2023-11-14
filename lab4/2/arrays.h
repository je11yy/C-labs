#ifndef LISTS_H
#define LISTS_H

/*
Для хранения массивов создадим структуру, где будут содержаться: количество массивов и массив указателей на структуры 
с названием массива (от A до Z), самим массивом и его длиной
(файл lists.h)
*/

typedef struct Array
{
    char name; // от A до Z
    int size; // размер массива
    int * array; // сам массив
} Array, *Array_ptr, **Array_array;

typedef struct Arrays
{
    int quantity; // количество массивов
    Array_array arrays; // массив указателей на массивы
} Arrays, *Arrays_ptr;

Array_ptr create_array(char name);
Arrays_ptr create_arrays();

int append_array(Arrays_ptr * arrays, Array_ptr new_array);
int append_element(Array_ptr * array, int element);

void delete_arrays(Arrays_ptr arrays);
void delete_array(Array_ptr array);

int array_exists(Arrays_ptr arrays, char name);

#endif