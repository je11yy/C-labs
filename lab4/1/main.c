#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "operations.h"
#include "hash_table.h"
#include "my_library.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    file_rename_error = -6,
    file_remove_error = -7
};

int replace_words(FILE * file, FILE * output);
int check_is_definition(char ** sub, Hash_table_ptr table, char * string, int min_len, int max_len);
int is_definition(char ** result, Hash_table_ptr table, char * string);
int get_max_len(Hash_table_ptr table);
int get_min_len(Hash_table_ptr table);
int into_table(Hash_table_ptr table, char line[]);
int is_directive(char * line);

int main(int argc, char * argv[])
{
    if (argc != 2) 
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    FILE * input = fopen(argv[1], "r");
    if (!input)
    {
        print_error(file_open_error);
        return file_open_error;
    }
    FILE * output = fopen("output.txt", "w");
    if (!output)
    {
        fclose(input);
        print_error(file_open_error);
        return file_open_error;
    }
    int result = replace_words(input, output);
    if (result != success)
    {
        print_error(result);
        return result;
    }
    result = remove(argv[1]);
    if (result == -1)
    {
        print_error(file_remove_error);
        return file_remove_error;
    }
    result = rename("output.txt", argv[1]);
    if (result == -1)
    {
        print_error(file_rename_error);
        return file_rename_error;
    }
    printf("Program has finished correctly.\n");
    return success;
}

int replace_words(FILE * file, FILE * output)
{
    int hash_size = 128;
    Hash_table_ptr table = create_table(hash_size);
    int result;
    int res;
    char * line = NULL;
    size_t length = 0;
    int flag = 0;
    int max_len;
    char * temp;
    int count;
    char * definition;
    int min_len;
    int directives_count = 0;
    while ((result = getline(&line, &length, file)) != EOF)
    {
        res = is_directive(line);
        if (res == success && !flag)
        {
            directives_count++;
            res = into_table(table, line);
            if (res != success)
            {
                fclose(file);
                fclose(output);
                free(line);
                line = NULL;
                return res;
            }
        }
        else if (res != success && !flag)
        {
            if (directives_count)
            {
                if (table -> hash_size == 0)
                {
                    fclose(file);
                    fclose(output);
                    free_table(table);
                    return success;
                }
                flag = 1;
                max_len = get_max_len(table);
                min_len = get_min_len(table);
                temp = (char*)calloc((max_len + 1), sizeof(char));
                if (!temp)
                {
                    fclose(file);
                    fclose(output);
                    free_table(table);
                    free(line);
                    line = NULL;
                    return no_memory;
                }
            }
            else
            {
                fprintf(output, "%s", line);
                break;
            }
        }
        if (flag == 1) // если директивы закончились
        {
            int j = 0;
            while (line[j] != '\n' && j < max_len)
            {
                temp[j] = line[j];
                j++;
            }
            count = j;
            // если не дошли до конца line
            while (line[j] != '\n' && line[j] != '\0')
            {
                if (count == max_len)
                {
                    res = check_is_definition(&definition, table, temp, min_len, max_len);
                    if (res > 0)
                    {
                        fprintf(output, "%s", definition);
                        for (int k = 0, n = res; n < max_len; ++k, ++n)
                        {
                            temp[k] = temp[n];
                        }
                        for (int k = max_len - res; k < max_len; ++k) temp[k] = 0;
                        count = max_len - res;
                    }
                    else
                    {
                        fprintf(output, "%c", temp[0]);
                        for (int i = 0; i < max_len; ++i) temp[i] = temp[i + 1];
                        count--;
                    }
                }
                temp[count] = line[j];
                count++;
                j++;
            }
            fprintf(output, "%s", temp);
            count = 0;
            if (line[j] == '\n') fprintf(output, "\n");
            break;
        }
    }
    free(line);
    line = NULL;
    char symbol = fgetc(file);
    if (!directives_count)
    {
        while (symbol != EOF)
        {
            fputc(symbol, output);
            symbol = fgetc(file);
        }
        free_table(table);
        fclose(file);
        fclose(output); 
        return success;
    }
    while (symbol != EOF)
    {
        if (count == max_len)
        {
            res = check_is_definition(&definition, table, temp, min_len, max_len);
            if (res > 0)
            {
                fprintf(output, "%s", definition);
                for (int k = 0, n = res; n < max_len; ++k, ++n)
                {
                    temp[k] = temp[n];
                }
                for (int k = max_len - res; k < max_len; ++k) temp[k] = 0;
                count = max_len - res;
            }
            else
            {
                fprintf(output, "%c", temp[0]);
                for (int i = 0; i < max_len; ++i) temp[i] = temp[i + 1];
                count--;
            }
        }
        temp[count] = symbol;
        count++;
        symbol = fgetc(file);
    }
    free_table(table);
    fclose(file);
    fclose(output);
    return success;
}

int check_is_definition(char ** sub, Hash_table_ptr table, char * string, int min_len, int max_len)
{
    char tmp[strlen(string) + 1];
    strcpy(tmp, string);

    char * result = NULL;
    int res;
    for (int i = max_len - 1; i >= min_len - 1; --i)
    {
        res = is_definition(&result, table, tmp);
        if (res == success)
        {
            *sub = result;
            return strlen(tmp);
        }
        tmp[i] = 0;
    }
    return fail;
}

int is_definition(char ** result, Hash_table_ptr table, char * string)
{
    if (is_alpha_digit_string(string) != success) return fail;
    char * hash = NULL;
    int res = hash_function(&hash, string);
    if (res != success) return res;
    int index = get_index(hash, table -> hash_size);
    if (index < 0)
    {
        free(hash);
        hash = NULL;
        return index;
    }
    if (!(table -> items)[index]) return fail;
    Item_ptr item = (table -> items)[index] -> head;
    if (strcmp(hash, item -> hash) == success)
    {
        if (strcmp(string, item -> definition) == success)
        {
            *result = item -> substitution;
            free(hash);
            hash = NULL;
            return success;
        }
    }
    while (item -> next)
    {
        item = item -> next;
        if (item)
        {
            if (strcmp(hash, item -> hash) == success)
            {
                if (strcmp(string, item -> definition) == success)
                {
                    *result = item -> substitution;
                    free(hash);
                    hash = NULL;
                    return success;
                }
            }
        }
    }
    free(hash);
    hash = NULL;
    return fail;
}

// searching for a definition in the table with max length
int get_max_len(Hash_table_ptr table)
{
    Item_ptr item;
    int length, max_length = 0;
    for (int i = 0; i < table -> hash_size; ++i)
    {
        if ((table -> items)[i])
        {
            item = (table -> items)[i] -> head;
            length = strlen(item -> definition);
            if (length > max_length) max_length = length;
            while (item -> next)
            {
                item = item -> next;
                if (item)
                {
                    length = strlen(item -> definition);
                    if (length > max_length) max_length = length;
                }
            }
        }
    }
    return max_length;
}

int get_min_len(Hash_table_ptr table)
{
    Item_ptr item;
    int length, min_length = -1;
    for (int i = 0; i < table -> hash_size; ++i)
    {
        if ((table -> items)[i])
        {
            item = (table -> items)[i] -> head;
            length = strlen(item -> definition);
            if (length < min_length || min_length < 0) min_length = length;
            while (item -> next)
            {
                item = item -> next;
                if (item)
                {
                    length = strlen(item -> definition);
                    if (length < min_length || min_length < 0) min_length = length;
                }
            }
        }
    }
    return min_length;
}

// добавление в таблицу
int into_table(Hash_table_ptr table, char line[])
{
    char * tmp = strtok(line, " \n"); // define
    tmp = strtok(NULL, " \n"); // def_name
    char * definition = tmp;
    tmp = strtok(NULL, " \n"); // value
    int res = insert(table, definition, tmp);
    if (res != success) return res;
    return success;
}

// проверка, является ли директивой
int is_directive(char * line)
{
    // количество пробелов (должно быть два)
    int count = 0;
    int i = 0;
    while (line[i] != '\0')
    {
        if (line[i] == ' ') count++;
        i++;
    }
    if (count != 2) return fail;
    int length = 2;
    char * tmp = (char*)malloc(length * sizeof(char));
    if (!tmp)
    {
        free(line);
        line = NULL;
        return no_memory;
    }
    char * tmp1;
    count = 0;
    i = 0;
    while (line[i] != ' ' && line[i] != '\n')
    {
        if (length < count)
        {
            length *= 2;
            tmp1 = (char*)realloc(tmp, length * sizeof(char));
            if (!tmp1)
            {
                free(tmp);
                tmp = NULL;
                free(line);
                line = 0;
                return no_memory;
            }
            tmp = tmp1;
        }
        tmp[count] = line[i];
        count++;
        i++;
    }
    if (length < count)
    {
        length *= 2;
        tmp1 = (char*)realloc(tmp, length * sizeof(char));
        if (!tmp1)
        {
            free(tmp);
            tmp = NULL;
            free(line);
            line = 0;
            return no_memory;
        }
        tmp = tmp1;
    }

    tmp[count] = '\0';
    if (strcmp(tmp, "#define") != success)
    {
        free(tmp);
        tmp = NULL;
        return fail;
    }
    free(tmp);
    tmp = NULL;
    length = 2;
    tmp = (char*)malloc(length * sizeof(char));
    count = 0;
    i++;
    while (line[i] != ' ' && line[i] != '\n')
    {
        if (length < count)
        {
            length *= 2;
            tmp1 = (char*)realloc(tmp, length * sizeof(char));
            if (!tmp1)
            {
                free(tmp);
                tmp = NULL;
                free(line);
                line = 0;
                return no_memory;
            }
            tmp = tmp1;
        }
        tmp[count] = line[i];
        count++;
        i++;
    }
    if (length < count)
    {
        length *= 2;
        tmp1 = (char*)realloc(tmp, length * sizeof(char));
        if (!tmp1)
        {
            free(tmp);
            tmp = NULL;
            free(line);
            line = 0;
            return no_memory;
        }
        tmp = tmp1;
    }
    tmp[count] = 0;
    if (is_alpha_digit_string(tmp) != success)
    {
        free(tmp);
        tmp = NULL;
        return fail;
    }
    free(tmp);
    tmp = NULL;
    return success;
}