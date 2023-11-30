#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "list.h"
#include "operations.h"
#include "memory_cell.h"

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
    unknown_variable = -8,
    print_operation = -9
};

void print_error(int error)
{
    switch (error)
    {
        case no_memory:
            printf("No memory.\n\n");
            break;
        case incorrect_input:
            printf("Incorrect input.\n\n");
            break;
        case file_open_error:
            printf("Can't open file.\n\n");
            break;
        case invalid_arguments:
            printf("Invalid arguments.\n\n");
            break;
        case invalid_brackets:
            printf("Invalid brackets.\n\n");
            break;
        case unknown_variable:
            printf("Unknown variable.\n\n");
            break;
        default: break;
    }
}

int command_handler(int * error, FILE * file);
int make_operation(char * expression, MemoryCell_array * array, int * length, int * size, char * name);
int print(char * string, char ** argument);
int get_name_expression(char * line, char ** name, char ** expression);
int validate(char * expression);

int main(int argc, char * argv[])
{
    if (argc != 2) 
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    FILE * file = fopen(argv[1], "r");
    if (!file)
    {
        print_error(file_open_error);
        return file_open_error;
    }
    int error = 0;
    int result = command_handler(&error, file);
    if (result != success)
    {
        print_error(error);
        return error;
    }
    printf("Program has finished correctly.\n\n");
    return success;
}

int command_handler(int * error, FILE * file)
{
    char * line = NULL;
    size_t len = 0;
    int result;
    int length;
    char * name = NULL;
    char * expression = NULL;
    char * argument;
    MemoryCell_array array = NULL;
    int array_len = 0, array_size = 0;
    int commands_count = 0;
    while ((result = getline(&line, &len, file)) != EOF)
    {
        length = strlen(line);
        if (line[length - 1] == '\n') line[length - 1] = 0;
        result = get_name_expression(line, &name, &expression);
        if (result == print_operation)
        {
            result = print(line, &argument);
            if (result != success)
            {
                free_memory_cell_array(array, array_len);
                free(line);
                line = NULL;
                *error = result;
                fclose(file);
                return runtime_error;
            }
            if (!argument)
            {
                free(argument);
                argument = NULL;
                print_array(array, array_len);
                continue;
            }
            result = print_variable(array, array_len, argument);
            if (result != success)
            {
                free_memory_cell_array(array, array_len);
                free(line);
                line = NULL;
                *error = result;
                fclose(file);
                return runtime_error;
            }
            free(argument);
            argument = NULL;
            continue;
        }
        else if (result != success)
        {
            free_memory_cell_array(array, array_len);
            free(line);
            line = NULL;
            *error = result;
            fclose(file);
            return runtime_error;
        }
        result = make_operation(expression, &array, &array_len, &array_size, name);
        if (result != success)
        {
            free_memory_cell_array(array, array_len);
            free(line);
            line = NULL;
            *error = result;
            fclose(file);
            return runtime_error;
        }
        commands_count++;
        free(name);
        name = NULL;
        free(expression);
        expression = NULL;
    }
    free(line);
    line = NULL;
    free_memory_cell_array(array, array_len);
    fclose(file);
    if (!commands_count)
    {
        *error = incorrect_input;
        return runtime_error;
    }
    *error = success;
    return success;
}

int validate(char * expression)
{
    int i = 0;
    int operators_count = 0;
    while (expression[i] != '=' && expression[i] != ';' && expression[i] != ' ' && expression[i] != 0)
    {
        if (!isalpha(expression[i])) return incorrect_input;
        i++;
    }
    if (expression[i] == 0) return success; // принт
    if (expression[i] == ';' || expression[i] == ' ') return success; // принт
    int k = i + 1;
    if (expression[i + 1] == ';' || expression[i + 1] == 0) return incorrect_input;
    while (expression[i] != 0 && expression[i] != ';')
    {
        if (is_operator(expression[i]) == success && i != k) operators_count++;
        i++;
    }
    if (expression[i] == 0) return incorrect_input;
    if (operators_count != 0 && operators_count != 1) return incorrect_input;
    return success;
}

int make_operation(char * expression, MemoryCell_array * array, int * length, int * size, char * name)
{
    char operation;
    // найдем операцию
    int i = 0;
    while (expression[i] != 0 && is_operator(expression[i]) != success) i++;
    operation = expression[i]; // если нет операции, то там терминирующий нуль
    if (operation == '-' && i == 0)
    {
        i++;
        while (expression[i] != 0 && is_operator(expression[i]) != success) i++;
        operation = expression[i];
    }
    int error;
    int first;
    MemoryCell_ptr cell;
    int flag = 0;
    if (operation == 0)
    {
        if (is_number(expression) == success || (expression[0] == '-' && is_number(expression + 1) == success))
        {
            first = atoi(expression);
        }
        else
        {
            if (expression[0] == '-')
            {
                flag = 1;
                expression++;
            }
            cell = dichotomy_search(*array, *length, expression);
            if (!cell) return unknown_variable;
            else
            {
                first = cell -> value;
                if (flag) first = -first;
            }
        }
        error = update_array(array, size, length, name, first);
        if (error != success) return error;
        return success;
    }
    i++;
    if (!isdigit(expression[i]) && !isalpha(expression[i])) return incorrect_input;
    char * new_expression = make_expression(&error, expression, *array, *length, *size);
    if (error != success) return error;
    char * tmp;
    i = 0;
    if (new_expression[0] == '-') // унарный минус
    {
        flag = 1;
    }
    tmp = strtok(new_expression, "+-*/%");
    first = atoi(tmp);
    if (flag) first = -first;
    tmp = strtok(NULL, "+-*/%");
    if (!tmp)
    {
        free(new_expression);
        new_expression = NULL;
        free(tmp);
        tmp = NULL;
        return incorrect_input;
    }
    int second = atoi(tmp);
    free(new_expression);
    new_expression = NULL;
    int res;
    error = do_operation(&res, operation, first, second);
    if (error != success) return invalid_arguments;
    error = update_array(array, size, length, name, res);
    if (error != success) return error;
    return success;
}

int print(char * string, char ** argument)
{
    int flag = 0;
    int i = 0;
    while (string[i] != 0)
    {
        if (string[i] == ' ')
        {
            flag = 1;
            break;
        }
        i++;
    }
    if (!flag)
    {
        if (strcmp(string, "print;") == success)
        {
            *argument = NULL;
            return success;
        }
        return incorrect_input;
    }
    char * tmp = strtok(string, " ");
    if (strcmp(tmp, "print") != success) return incorrect_input;
    tmp = strtok(NULL, " ");
    int length = strlen(tmp);
    if (tmp[length - 1] != ';') return incorrect_input;
    tmp[length - 1] = 0;
    if (is_alpha(tmp) != success) return incorrect_input;
    *argument = (char*)malloc((length) * sizeof(char));
    if (!(*argument)) return no_memory;
    strcpy(*argument, tmp);
    return success;
}

int get_name_expression(char * line, char ** name, char ** expression)
{
    int result = validate(line);
    if (result != success) return result;
    int i = 0;
    int flag = 0;
    while (line[i] != 0)
    {
        if (line[i] == '=')
        {
            flag = 1;
            break;
        }
        i++;
    }
    if (!flag) return print_operation;
    char * tmp = strtok(line, "=");
    if (is_alpha(tmp) != success) return incorrect_input;
    int length = strlen(tmp);
    *name = (char*)malloc((length + 1) * sizeof(char));
    if (!(*name)) return no_memory;
    strcpy(*name, tmp);
    tmp = strtok(NULL, "=");
    length = strlen(tmp);
    if (tmp[length - 1] != ';')
    {
        free(*name);
        *name = NULL;
        return incorrect_input;
    }
    tmp[length - 1] = 0;
    *expression = (char*)malloc((length + 1) * sizeof(char));
    if (!(*expression))
    {
        free(*name);
        *name = NULL;
        return no_memory;
    }
    strcpy(*expression, tmp);
    return success;
}