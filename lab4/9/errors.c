#include "errors.h"

void print_error(status const error)
{
    const char * main_error = "[ERROR]";
    const char * errors[] = 
    {
        "Incorrect arguments.",
        "Memory error."
    };
    switch (error)
    {
        case arguments_error:
            printf("%s %s\n", main_error, errors[0]);
            break;
        case no_memory:
            printf("%s %s\n", main_error, errors[1]);
            break;
        default: break;
    }
}