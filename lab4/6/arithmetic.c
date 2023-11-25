#include "arithmetic.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    invalid_brackets = -6,
    invalid_arguments = -7,
    delete_file_error = -8
};

int is_operator(char data)
{
    char * operators = "&|~-+<=!?>";
    int i = 0;
    while (operators[i])
    {
        if (operators[i] == data) return success;
        i++;
    }
    return fail;
}

int priority(char data)
{
    switch(data)
    {
        case '~': return 3;
        case '?': return 2;
        case '!': return 2;
        case '+': return 2;
        case '&': return 2;
        case '|': return 1;
        case '-': return 1;
        case '<': return 1;
        case '=': return 1;
        default: return fail;
    }
}
