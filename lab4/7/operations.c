#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "operations.h"
#include "list.h"

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

int is_alpha(char * string)
{
    for (int i = 0; string[i] != 0; ++i)
    {
        if (!(isalpha(string[i]))) return fail;
    }
    return success;
}

int is_number(char * string)
{
    for (int i = 0; string[i] != 0; ++i)
    {
        if (!(isdigit(string[i]))) return fail;
    }
    return success;
}

int is_operator(char symbol)
{
    char operators[] = "+-/%^*~";
    for (int i = 0; operators[i] != 0; ++i)
    {
        if (symbol == operators[i]) return success;
    }
    return fail;
}

int do_operation(int * result, char operation, int left, int right)
{
    switch (operation)
    {
        case '+':
            *result = left + right;
            break;
        case '-':
            *result = left - right;
            break;
        case '/':
            if (right == 0) return invalid_arguments;
            *result =  left / right;
            break;
        case '*':
            *result = left * right;
            break;
        case '%':
            if (left < 0 || right <= 0) return invalid_arguments;
            *result = (left % right);
            break;
        case '^': 
            if (right < 0 || (left == 0 && right == 0)) return invalid_arguments;
            *result = pow(left, right);
            break;
        case '~':
            *result = -right;
            break;
    }
    return success;
}
