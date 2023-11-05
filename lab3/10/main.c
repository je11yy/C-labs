#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

void delete_trees(Node_array trees, int length);
void print_trees(FILE * output, Node_array trees, int length);
int get_trees(FILE * input, Node_array * trees, int * length);
int get_expression(FILE * file, char ** expression);

void print_error(int error);

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    char * file_name1 = argv[1];
    char * file_name2 = argv[2];
    FILE * input = fopen(file_name1, "r");
    if (!input)
    {
        print_error(file_open_error);
        return file_open_error;
    }
    FILE * output = fopen(file_name2, "w");
    if (!output)
    {
        fclose(input);
        print_error(file_open_error);
        return file_open_error;
    }
    Node_array trees = NULL;
    int length = 0;
    int result = get_trees(input, &trees, &length);
    if (result != success)
    {
        fclose(input);
        fclose(output);
        print_error(result);
        return result;
    }
    fclose(input);

    print_trees(output, trees, length);
    fclose(output);

    delete_trees(trees, length);

    printf("Program has finished correctly.\n");
    return success;
}

void delete_trees(Node_array trees, int length)
{
    for (int i = 0; i < length; ++i)
    {
        delete_tree(trees[i]);
    }
    free(trees);
    trees = NULL;
}

void print_trees(FILE * output, Node_array trees, int length)
{
    for (int i = 0; i < length; ++i)
    {
        fprintf(output, "\n%d TREE\n\n", i + 1);
        print_tree(output, trees[i], 0);
    }
}

int get_expression(FILE * file, char ** expression)
{
    char symbol = fgetc(file);
    int length = 1;
    *expression = (char*)malloc(length * sizeof(char));
    int count = 0;
    char * tmp;
    while (symbol != '\n' && symbol != EOF)
    {
        if (length <= count)
        {
            length *= 2;
            tmp = (char*)realloc(*expression, length * sizeof(char));
            if (!tmp)
            {
                free(*expression);
                *expression = NULL;
                return no_memory;
            }
            *expression = tmp;
        }
        if (symbol != ' ')
        {
            (*expression)[count] = symbol;
            count++;
        }
        symbol = fgetc(file);
    }
    if (length <= count)
    {
        length *= 2;
        tmp = (char*)realloc(*expression, length * sizeof(char));
        if (!tmp)
        {
            free(*expression);
            *expression = NULL;
            return no_memory;
        }
        *expression = tmp;
    }
    (*expression)[count] = 0;
    return success;
}

int get_trees(FILE * input, Node_array * trees, int * length)
{
    Node_ptr root;
    Node_array tmp;
    char * expression = NULL;
    int result = 0;
    while (!feof(input))
    {
        result = get_expression(input, &expression);
        if (result != success)
        {
            delete_trees(*trees, *length);
            return no_memory;
        }
        root = create_expression_tree(expression);
        if (!root)
        {
            delete_trees(*trees, *length);
            free(expression);
            expression = NULL;
            return no_memory;
        }
        (*length)++;
        tmp = (Node_array)realloc(*trees, (*length) * sizeof(Node_ptr));
        if (!tmp)
        {
            delete_trees(*trees, *length);
            free(expression);
            expression = NULL;
            return no_memory;
        }
        *trees = tmp;
        (*trees)[(*length) - 1] = root;
        free(expression);
        expression = NULL;
    }
    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_open_error:
            printf("\nCannot open file!\n");
            break;
    }
}