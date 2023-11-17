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
    file_rename_error = -6,
    file_remove_error = -7
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
        case file_rename_error:
            printf("\nCannot rename file.\n");
            break;
        case file_remove_error:
            printf("\nCannot delete file.\n");
            break;
    }
}

int is_lower_string(char * string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (!islower(string[i])) return fail;
        i++;
    }
    return success;
}

int is_alpha_string(char * string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (!isalpha(string[i])) return fail;
        i++;
    }
    return success;
}

int is_digit_string(char * string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (!isdigit(string[i])) return fail;
        i++;
    }
    return success;
}

int is_alpha_digit_string(char * string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if (!isdigit(string[i]) && !isalpha(string[i])) return fail;
        i++;
    }
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