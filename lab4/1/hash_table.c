#include "hash_table.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    incorrect_command_input = -6,
    incorrect_array_input = -7
};

int check_table(Hash_table_ptr table)
{
    int max_chain = 0, min_chain = -1;
    int count;
    Items_ptr cur_items;
    Item_ptr cur_item;
    for (int i = 0; i < table -> hash_size; ++i)
    {
        count = 0;
        cur_items = (table -> items)[i];
        if (cur_items)
        {
            cur_item = cur_items -> head;
            if (cur_item)
            {
                count++;
                while (cur_item -> next != NULL)
                {
                    cur_item = cur_item -> next;
                    if (cur_item) count++;
                }
                if (count > max_chain && count != 0) max_chain = count;
                if ((count < min_chain || min_chain == -1) && count != 0) min_chain = count;
            }
        }
    }
    if (min_chain == -1) success; // таблица пустая
    if (max_chain / min_chain >= 2) return fail; // все плохо, надо менять таблицу
    return success; // урааа все ок
}

int get_new_size(int elements_count)
{
    /*
    Ищем следующее простое число, начиная с elements_count + 1;
    */
   elements_count++;
   while (is_prime(elements_count) != success) elements_count++;
   return elements_count;
}

int rebuilding_table(Hash_table_ptr table)
{
    int prev_size = table -> hash_size;
    table -> hash_size = get_new_size(prev_size);

    Items_array tmp = (Items_array)realloc(table -> items, (table -> hash_size) * (sizeof(Items_ptr)));
    if (!tmp)
    {
        free_table(table);
        return no_memory;
    }
    table -> items = tmp;
    for (int i = prev_size; i < table -> hash_size; ++i) (table -> items)[i] = NULL;
    Items_ptr current_items;
    Item_ptr cur_item;
    int index, res;
    for (int i = 0; i < prev_size; ++i)
    {
        if ((table -> items)[i])
        {
            current_items = (table -> items)[i];
            cur_item = current_items -> head;
            if (cur_item)
            {
                index = get_index(cur_item -> hash, table -> hash_size);
                if (index < 0)
                {
                    free_table(table);
                    return index;
                }
                if (index != i)
                {
                    remove_item_from_list(current_items, cur_item);
                    (table -> items)[index] = insert_in_list((table -> items)[index], cur_item);
                    if (!(table -> items)[index])
                    {
                        free_table(table);
                        return no_memory;
                    }
                }
            }
            while (cur_item -> next)
            {
                cur_item = cur_item -> next;
                if (cur_item)
                {
                    index = get_index(cur_item -> hash, table -> hash_size);
                    if (index < 0)
                    {
                        free_table(table);
                        return index;
                    }
                    if (index != i)
                    {
                        remove_item_from_list(current_items, cur_item);
                        insert_in_list((table -> items)[index], cur_item);
                    }
                }
            }
        }
    }
    return success;
}

void remove_item_from_list(Items_ptr items, Item_ptr item)
{
    // если Item - единственное значение в списке
    if (!(item -> prev) && !(item -> next))
    {
        items -> head = NULL;
        items -> tail = NULL;
        free(items);
        items = NULL;
        return;
    }
    // Если Item - голова
    if (!(item -> prev))
    {
        items -> head = item -> next;
        items -> head -> prev = NULL;
        item -> next = NULL;
        return;
    }
    // если item - хвост
    if (!(item -> next))
    {
        items -> tail = item -> prev;
        items -> tail -> next = NULL;
        item -> prev = NULL;
        return;
    }
    // если в середине
    item -> prev -> next = item -> next;
    item -> next -> prev = item -> prev;
    item -> prev = NULL;
    item -> next = NULL;
}

int get_index(char * hash, int hash_size)
{
    char * index_string = NULL;
    int res = get_remainder(&index_string, hash, hash_size);
    if (res != success) return res;
    return atoi(index_string);
}

int insert(Hash_table_ptr table, char * definition, char * substitution)
{
    Item_ptr item = create_item(definition, substitution);
    if (!item) return no_memory;
    int index;
    char * index_string = NULL;
    int res = get_remainder(&index_string, item -> hash, table -> hash_size);
    if (res != success)
    {
        free_item(item);
        free_table(table);
        return res;
    }
    index = atoi(index_string);
    free(index_string);
    index_string = NULL;
    if (!(table -> items)[index])
    {
        (table -> items)[index] = create_item_list();
        if (!(table -> items)[index])
        {
            free_item(item);
            free_table(table);
            return no_memory;
        }
    }
    insert_in_list((table -> items)[index], item);
    (table -> count)++;
    
    int result = check_table(table);
    while (result != success)
    {
        result = rebuilding_table(table);
        if (result != success) return result;
        result = check_table(table);
    }

    return success;
}

// инициализирование ДВУСВЯЗНОГО СПИСКА
Items_ptr create_item_list()
{
    Items_ptr items = (Items_ptr)malloc(sizeof(Items));
    if (!items) return NULL;
    items -> head = NULL;
    items -> tail = NULL;
    return items;
}

int hash_function(char ** hash_result, char * number)
{
    char * base = "62";
    int length = strlen(number);
    char * power = (char*)malloc(2 * sizeof(char));
    if (!power) return no_memory;
    power[0] = '1';
    power[1] = 0;

    char * hash = (char*)malloc(2 * sizeof(char));
    if (!hash)
    {
        free(power);
        power = NULL;
        return no_memory;
    }
    hash[0] = '0';
    hash[1] = 0;

    char * string_digit = NULL;
    int len;

    int digit;
    int result;
    for (int i = length - 1; i >= 0; --i)
    {
        digit = (number[i] >= '0' && number[i] <= '9') ? number[i] - '0' : tolower(number[i]) - 'a' + 10;
        len = get_length(digit);
        string_digit = (char*)malloc((len + 1) * sizeof(char));
        if (!string_digit)
        {
            free(hash);
            hash = NULL;
            free(power);
            power = NULL;
            return no_memory;
        }
        sprintf(string_digit, "%d", digit);
        result = multiply_numbers(&string_digit, power);
        if (result != success)
        {
            free(string_digit);
            string_digit = NULL;
            free(hash);
            hash = NULL;
            free(power);
            power = NULL;
            return no_memory;
        }
        result = summary_two_numbers(&hash, string_digit, 10);
        if (result != success)
        {
            free(string_digit);
            string_digit = NULL;
            free(hash);
            hash = NULL;
            free(power);
            power = NULL;
            return no_memory;
        }
        free(string_digit);
        string_digit = NULL;
        result = multiply_numbers(&power, base);
        if (result != success)
        {
            free(string_digit);
            string_digit = NULL;
            free(hash);
            hash = NULL;
            free(power);
            power = NULL;
            return no_memory;
        }
    }
    free(power);
    power = NULL;
    *hash_result = hash;

    return success;
}

Item_ptr create_item(char * definition, char * substitution)
{
    char * hash = NULL;
    int res = hash_function(&hash, definition);
    if (res != success) return NULL;

    Item_ptr item = (Item_ptr)malloc(sizeof(Item));
    if (!item)
    {
        free(hash);
        hash = NULL;
        return NULL;
    }
    item -> definition = (char*)malloc((strlen(definition) + 1) * sizeof(char));
    if (!(item -> definition))
    {
        free(hash);
        hash = NULL;
        free(item);
        item = NULL;
        return NULL;
    }
    strcpy(item -> definition, definition);
    item -> substitution = (char*)malloc((strlen(substitution) + 1) * sizeof(char));
    if (!(item -> substitution))
    {
        free(hash);
        hash = NULL;
        free(item -> definition);
        item -> definition = NULL;
        free(item);
        item = NULL;
        return NULL;
    }
    strcpy(item -> substitution, substitution);

    item -> hash = (char*)malloc((strlen(hash) + 1) * sizeof(char));
    if (!(item -> hash))
    {
        free(hash);
        hash = NULL;
        free(item -> definition);
        item -> definition = NULL;
        free(item -> substitution);
        item -> substitution = NULL;
        free(item);
        item = NULL;
        return NULL;
    }
    strcpy(item -> hash, hash);
    free(hash);
    hash = NULL;

    item -> prev = NULL;
    item -> next = NULL;
    return item;
}

Hash_table_ptr create_table(int hash_size)
{
    Hash_table_ptr hash_table = (Hash_table_ptr)malloc(sizeof(Hash_table));
    if (!hash_table) return NULL;

    hash_table -> count = 0;
    hash_table -> hash_size = hash_size;
    hash_table -> items = create_items(hash_table -> hash_size);
    if (!(hash_table -> items))
    {
        free(hash_table);
        hash_table = NULL;
        return NULL;
    }

    return hash_table;
}

Items_ptr insert_in_list(Items_ptr items, Item_ptr item)
{
    if (!items)
    {
        items = create_item_list();
        if (!items)
        {
            free_item(item);
            return NULL;
        }
    }
    if (!(items -> head))
    {
        items -> head = item;
        items -> tail = item;
        item -> prev = NULL;
        item -> next = NULL;
        return items;
    }
    items -> tail -> next = item;
    item -> prev = items -> tail;
    items -> tail = item;
    item -> next = NULL;
    return items;
}

// инициализирование списка двусвязных списков
Items_array create_items(int size)
{
    Items_array items = (Items_array)calloc(size, sizeof(Items_ptr));
    if (!items) return NULL;
    return items;
}

void free_items(Items_ptr items)
{
    printf("\nHERE\n");
    if (!items) return;
    Item_ptr item = items -> head;
    if (!item)
    {
        free(items);
        items = NULL;
        return;
    }
    printf("%s\n", item -> definition);
    printf("HERE\n");
    Item_ptr prev;
    prev = item -> next;
    free_item(item);
    item = prev;
    while (item)
    {
        printf("%s\n", item ->definition);
        prev = item -> next;
        free_item(item);
        item = prev;
    }
    free(items);
    items = NULL;
}

void free_item(Item_ptr item)
{
    if (!item) return;
    free(item -> definition);
    item -> definition = NULL;
    free(item -> substitution);
    item -> substitution = NULL;
    free(item);
    item = NULL;
}

void free_table(Hash_table_ptr table)
{
    if (!table) return;
    for (int i = 0; i < table -> hash_size; ++i)
    {
        if ((table -> items)[i]) free_items((table -> items)[i]);
    }
    free(table -> items);
    table -> items = NULL;
    free(table);
    table = NULL;
}