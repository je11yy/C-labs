#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <ctype.h>
#include <math.h>
#include <string.h>

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

int infix_to_postfix(int * error, char ** postfix_result, const char * infix);
int calculate_postfix(int * error, int * expression_result, const char * postfix);
int do_operation(int * result, char operation, int left, int right);
int is_number(char * string);
int is_bracket(char symbol);
int is_operator(char symbol);
void print_error(int error);

int calculate_file(FILE * file, char * input_name);
void print_results(char * expression, char * postfix, int result);
void print_error_in_file(FILE * file, int error, int index, char * expression);

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    FILE * file;
    int result;
    for (int i = 1; i < argc; ++i)
    {
        file = fopen(argv[i], "r");
        if (!file)
        {
            print_error(file_open_error);
            return file_open_error;
        }
        printf("Results for file %s:\n\n", argv[i]);
        result = calculate_file(file, argv[i]);
        if (result < 0)
        {
            print_error(result);
            return result;
        }
        else if (result == 0)
        {
            printf("No valid expressions.\n\n");
        }
    }
    printf("Program has finished correctly.\n");
    return success;
}

// возвращает количество выполненных выражений
int calculate_file(FILE * file, char * input_name)
{
    int count = 0;
    int errors_count = 0;

    size_t len = 0;
    char * line = NULL;
    int res;
    int calculated_result;
    int error;
    int index = 0;
    char file_name[100];
    sprintf(file_name, "output_%s", input_name);
    FILE * errors_file = fopen(file_name, "w");

    char * postfix = NULL;
    int length = 0;

    while ((res = getline(&line, &len, file)) != EOF)
    {
        res = infix_to_postfix(&error, &postfix, line);
        if (res == runtime_error)
        {
            length = strlen(line);
            if (line[length - 1] == '\n') line[length - 1] = 0;
            print_error_in_file(errors_file, error, index, line);
            errors_count++;
        }
        else if (res != success)
        {
            free(line);
            line = NULL;
            fclose(file);
            fclose(errors_file);
            free(postfix);
            postfix = NULL;
            return res;
        }
        else
        {
            res = calculate_postfix(&error, &calculated_result, postfix);
            if (res == runtime_error)
            {
                length = strlen(line);
                if (line[length - 1] == '\n') line[length - 1] = 0;
                print_error_in_file(errors_file, error, index, line);
                errors_count++;
            }
            else if (res != success)
            {
                free(line);
                line = NULL;
                fclose(file);
                fclose(errors_file);
                free(postfix);
                postfix = NULL;
                return res;
            }
            else
            {
                length = strlen(line);
                if (line[length - 1] == '\n') line[length - 1] = 0;
                print_results(line, postfix, calculated_result);
                count++;
            }
        }
        free(line);
        line = NULL;
        len = 0;
        free(postfix);
        postfix = NULL;
        index++;
    }
    fclose(errors_file);
    if (errors_count == 0)
    {
        res = remove(file_name);
        if (res == EOF) return delete_file_error;
    }
    free(line);
    line = NULL;
    fclose(file);
    return count;
}

void print_results(char * expression, char * postfix, int result)
{
    printf("Expression: %s\n", expression);
    printf("Postfix expression: %s\n", postfix);
    printf("Result: %d\n\n", result);
}

void print_error_in_file(FILE * file, int error, int index, char * expression)
{
    fprintf(file, "Index: %d\n", index);
    fprintf(file, "Expression: %s\n", expression);
    fprintf(file, "Error: ");
    switch (error)
    {
        case incorrect_input:
            fprintf(file, "Invalid expression.\n");
            break;
        case invalid_brackets:
            fprintf(file, "Parentheses mismatch.\n");
            break;
        case invalid_arguments:
            fprintf(file, "Invalid arguments in expression.\n");
            break;
        default: break;
    }
    fprintf(file, "\n");
}

int priority(char operation)
{
    switch (operation)
    {
        case '+': return 1;
        case '-': return 1;
        case '/': return 2;
        case '*': return 2;
        case '~': return 2;
        case '%': return 2;
        case '^': return 3;
        default: return fail;
    }
}

int infix_to_postfix(int * error, char ** postfix_result, const char * infix)
{
    Node_ptr stack = NULL;
    int count = 0;
    char * temp;

    char * postfix = (char*)calloc((2 * strlen(infix) + 1), sizeof(char));
    if (!postfix) return no_memory;
    int index = 0;
    Node_ptr node;


    int res;
    int i = 0;
    int flag = 1;
    char symbol;
    while (infix[i] != '\n' && infix[i] != '\0')
    {
        symbol = infix[i];
        if (isdigit(symbol))
        {
            postfix[index++] = symbol;
            flag = 0;
        }
        else if (is_operator(symbol) == success)
        {   
            if (symbol == '-' && flag) symbol = '~';
            else if (flag)
            {
                free_list(stack);
                free(postfix);
                postfix = NULL;
                *error = incorrect_input;
                return runtime_error;
            }
            else flag = 1;
            postfix[index++] = ' ';
            node = get_last(stack);
            while (stack && priority(node -> data) >= priority(symbol))
            {
                postfix[index++] = node -> data;
                postfix[index++] = ' ';
                pop_back(&stack);
                node = get_last(stack);
            }
            res = push_back_char(&stack, symbol);
            node = get_last(stack);
            if (res != success)
            {
                free_list(stack);
                free(postfix);
                postfix = NULL;
                return no_memory;
            }
        }
        else if (is_bracket(symbol) == success)
        {
            if (symbol == '(')
            {
                res = push_back_char(&stack, symbol);
                node = get_last(stack);
                if (res != success)
                {
                    free_list(stack);
                    free(postfix);
                    postfix = NULL;
                    return no_memory;
                }
            }
            else if (symbol == ')')
            {
                node = get_last(stack);
                while (stack && node -> data != '(')
                {
                    postfix[index++] = node -> data;
                    postfix[index++] = ' ';
                    pop_back(&stack);
                    node = get_last(stack);
                }
                if (!stack)
                {
                    free_list(stack);
                    free(postfix);
                    postfix = NULL;
                    *error = invalid_brackets;
                    return runtime_error;
                }
                pop_back(&stack);
            }
        }
        else if (symbol != ' ' && symbol != '\n' && symbol != '\t')
        {
            free_list(stack);
            free(postfix);
            postfix = NULL;
            *error = invalid_arguments;
            return runtime_error;
        }
        i++;
    }
    while (stack)
    {
        if (stack -> data == '(')
        {
            free_list(stack);
            free(postfix);
            postfix = NULL;
            *error = invalid_brackets;
            return runtime_error;
        }
        node = get_last(stack);
        if (is_operator(node -> data) == success) postfix[index++] = ' ';
        postfix[index++] = node -> data;
        pop_back(&stack);
    }
    postfix[index] = 0;
    if (postfix[index - 1] == 0)
    {
        *error = incorrect_input;
        free_list(stack);
        free(postfix);
        postfix = NULL;
        return runtime_error;
    }
    free_list(stack);
    *postfix_result = postfix;
    return success;
}

// вычисление выражения, записанного в обратной польской записи
int calculate_postfix(int * error, int * expression_result, const char * postfix)
{
    Node_ptr stack = NULL;
    char tmp[strlen(postfix)];
    int result = sscanf(postfix, "%s", tmp);
    char symbol;
    Node_ptr last;
    Node_ptr prev_last;
    char *end;
    int value;
    int res;
    int length = strlen(postfix);
    int len;
    while (length > 0)
    {
        symbol = tmp[0];
        if (isdigit(symbol))
        {
            value = atoi(tmp);
            result = push_back(&stack, value);
            if (result != success) return result;
        }
        else
        {
            last = get_last(stack);
            if (symbol == '~')
            {
                prev_last = last;
            }
            else prev_last = get_prev_last(stack);
            if (!last || !prev_last)
            {
                *error = incorrect_input;
                free_list(stack);
                return runtime_error;
            }
            result = do_operation(&res, symbol, prev_last -> value, last -> value);
            if (result != success)
            {
                *error = result;
                free_list(stack);
                return runtime_error;
            }
            if (symbol == '~') pop_back(&stack);
            else
            {
                pop_back(&stack);
                pop_back(&stack);
            }
            result = push_back(&stack, res);
            if (result != success) return result;
        }
        len = strlen(tmp) + 1;
        postfix += len;
        length -= len;
        result = sscanf(postfix, "%s", tmp);
    }
    *expression_result = stack -> value;
    free_list(stack);
    return success;
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
            if (left < 0 || right < 0) return invalid_arguments;
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

int is_number(char * string)
{
    for (int i = 0; string[i] != 0; ++i)
    {
        if (!(isdigit(string))) return fail;
    }
    return success;
}

int is_bracket(char symbol)
{
    if (symbol == '(' || symbol == ')') return success;
    return fail;
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
        case invalid_brackets:
            printf("\nParentheses mismatch.\n");
            break;
        case delete_file_error:
            printf("\nCannot delete file with this name.\n");
            break;
        default: break;
    }
}