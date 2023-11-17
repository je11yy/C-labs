#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "operations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
Метод цепочек: если хеш-функция выделяет один индекс сразу двум элементам, то храниться они 
будут в одном и том же индексе, но уже с помощью двусвязного списка.

Поэтому сразу создадим указатели на следующий и предыдущий элементы в структуре Item
*/

typedef struct Item
{
    char * definition; // макрос
    char * substitution; // макроподстановка

    char * hash; // хэш

    struct Item * prev;
    struct Item * next;
} Item, *Item_ptr, **Item_array;

typedef struct Items
{
    Item_ptr head;
    Item_ptr tail;
} Items, *Items_ptr, **Items_array;

typedef struct Hash_table
{
    int hash_size; // размер таблицы
    int count; // количество элементов
    Items_array items; // список двусвязных списков
} Hash_table, *Hash_table_ptr;

int hash_function(char ** hash_result, char * number);
int get_index(char * hash, int hash_size);

int insert(Hash_table_ptr table, char * definition, char * substitution);
void insert_in_list(Items_ptr items, Item_ptr item);

Items_ptr create_item_list();
Item_ptr create_item(char * definition, char * substitution);
Hash_table_ptr create_table(int hash_size);
Items_array create_items(int size);

void free_items(Items_ptr items);
void free_item(Item_ptr item);
void free_table(Hash_table_ptr table);

void remove_item_from_list(Items_ptr items, Item_ptr item);

#endif