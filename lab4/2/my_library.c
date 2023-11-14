#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_library.h"

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

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.\n");
            break;
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_open_error:
            printf("\nCannot open file!\n");
            break;
        case runtime_error:
            printf("\nRuntime error.\n");
            break;
        case incorrect_command_input:
            printf("\nIncorrect input in commands.\n");
            break;
        case incorrect_array_input:
            printf("\nIncorrect input for array.\n");
    }
}

int is_lower_string(char * string)
{
    int i = 0;
    while(string[i++] != '\0') if (!islower(string[i])) return fail;
    return success;
}

int is_alpha_string(char * string)
{
    int i = 0;
    while(string[i++] != '\0') if (!isalpha(string[i])) return fail;
    return success;
}

int is_digit_string(char * string)
{
    int i = 0;
    while(string[i++] != '\0') if (!isdigit(string[i])) return fail;
    return success;
}

int is_alpha_digit_string(char * string)
{
    int i = 0;
    while(string[i++] != '\0') if (!isdigit(string[i]) && !isalpha(string[i])) return fail;
    return success;
}

void make_lower(char ** text)
{
    int i = 0;
    while ((*text)[i] != '\0')
    {
        if (isalpha((*text)[i])) (*text)[i] = tolower((*text)[i]);
        i++;
    }
}