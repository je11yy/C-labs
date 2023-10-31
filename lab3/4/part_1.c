#include <stdio.h>
#include <stdlib.h>
#include "string.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2
};

void print_error(int error);

int main()
{
    String_ptr string = create_string("mama string");
    if (!string)
    {
        print_error(no_memory);
        return no_memory;
    }
    printf("Creation of string: %s, length: %d\n", string -> buffer, string -> length);
    String_ptr string1 = create_string("papa string");
    if (!string1)
    {
        delete_string(string);
        print_error(no_memory);
        return no_memory;
    }
    printf("Creation of string: %s, length: %d\n\n", string1 -> buffer, string1 -> length);

    printf("Order relation function: %d\n\n", strings_order_relation(string, string1));

    printf("Concatenation:\n");
    printf("Before: %s %s, lengths: %d %d\n", string -> buffer, string1 -> buffer, string -> length, string1 -> length);
    String_ptr new_string = concatenation(string, string1);
    if (!new_string)
    {
        delete_string(string);
        delete_string(string1);
        print_error(no_memory);
        return no_memory;
    }
    printf("After: %s | %s %s, lengths: %d | %d %d\n\n", new_string -> buffer, string -> buffer, string1 -> buffer, new_string -> length, string -> length, string1 -> length);

    printf("Comparison of %s and %s : %d\n\n", string -> buffer, string1 -> buffer, strings_cmp(string, string1));
    
    printf("Copy %s to %s\n", string -> buffer, string1 -> buffer);
    strings_copy(string, string1);
    
    printf("Result: %s %s\n\n", string -> buffer, string1 -> buffer);

    printf("Dublication of %s\n", string -> buffer);
    String_ptr string2 = dublicate(string);
    if (!string2)
    {
        delete_string(string);
        delete_string(string1);
        delete_string(string2);
        print_error(no_memory);
        return no_memory;
    }
    printf("Result: %s\n\n", string2 -> buffer);

    printf("Dynamic copy of %s\n", string -> buffer);
    String_ptr string3 = (String_ptr) malloc(sizeof(String));
    if (!string3)
    {
        delete_string(string);
        delete_string(string1);
        delete_string(new_string);
        delete_string(string2);
        print_error(no_memory);
        return no_memory;
    }
    int result = string_copy_dynamic(string3, string);
    if (result == no_memory)
    {
        delete_string(string);
        delete_string(string1);
        delete_string(string3);
        delete_string(new_string);
        delete_string(string2);
        print_error(no_memory);
        return no_memory;
    }
    printf("Result: %s\n\n", string3 -> buffer);

    delete_string(string);
    delete_string(string1);
    delete_string(string2);
    delete_string(string3);
    delete_string(new_string);

    printf("Program has finished correctly.\n");
    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
    }
}