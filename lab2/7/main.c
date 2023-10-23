#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_error = -2,
    no_memory = -3,
    file_input_error = -4
};

int check_valid_double(char *number);

double function_cos(double x);

double function_sin(double x);

double function_ln(double x);

double random_function(double x);

int dichotomy(double *result, double a, double b, double eps, double (*function)(double));

int get_string(char **string);

void print_error(int error);

int main()
{
    printf("\nInput lower limit of the interval: ");
    char *a_string = NULL;
    int result = get_string(&a_string);
    if (result == no_memory)
    {
        free(a_string);
        a_string = NULL;
        print_error(result);
        return result;
    }
    result = check_valid_double(a_string);
    while (result != success)
    {
        printf("\nIncorrect input. Try again: ");
        free(a_string);
        a_string = NULL;
        result = get_string(&a_string);
        if (result == no_memory)
        {
            free(a_string);
            a_string = NULL;
            print_error(result);
            return result;
        }
        result = check_valid_double(a_string);
    }
    double a = atof(a_string);
    free(a_string);
    a_string = NULL;

    printf("\nInput upper limit of the interval: ");
    char *b_string = NULL;
    result = get_string(&b_string);
    if (result == no_memory)
    {
        free(b_string);
        b_string = NULL;
        print_error(result);
        return result;
    }
    result = check_valid_double(b_string);
    while (result != success)
    {
        printf("\nIncorrect input. Try again: ");
        free(b_string);
        b_string = NULL;
        result = get_string(&b_string);
        if (result == no_memory)
        {
            free(b_string);
            b_string = NULL;
            print_error(result);
            return result;
        }
        result = check_valid_double(b_string);
    }
    double b = atof(b_string);
    free(b_string);
    b_string = NULL;

    printf("\nInput epsilon: ");
    char *eps_string = NULL;
    result = get_string(&eps_string);
    if (result == no_memory)
    {
        free(eps_string);
        eps_string = NULL;
        print_error(result);
        return result;
    }
    result = check_valid_double(eps_string);
    while (result != success)
    {
        printf("\nIncorrect input. Try again: ");
        free(eps_string);
        eps_string = NULL;
        result = get_string(&eps_string);
        if (result == no_memory)
        {
            free(eps_string);
            eps_string = NULL;
            print_error(result);
            return result;
        }
        result = check_valid_double(eps_string);
    }
    double eps = atof(eps_string);
    free(eps_string);
    eps_string = NULL;

    ///// cos(x)
    double answer;
    result = dichotomy(&answer, a, b, eps, &function_cos);
    if (result == incorrect_input)
    {
        printf("\nCannot calculate cos-function with this arguments.\n");
    }
    else
    {
        printf("\nResult for cos(x): %f\n", answer);
    }

    ////// sin(x)
    answer;
    result = dichotomy(&answer, a, b, eps, &function_sin);
    if (result == incorrect_input)
    {
        printf("\nCannot calculate cos-function with this arguments.\n");
    }
    else
    {
        printf("\nResult for sin(x): %f\n", answer);
    }

    ///// ln(x)
    if (a > 0 && b > 0)
    {
        answer;
        result = dichotomy(&answer, a, b, eps, &function_ln);
        if (result == incorrect_input)
        {
            printf("\nCannot calculate ln-function with this arguments.\n");
        }
        else
        {
            printf("\nResult for ln(x): %f\n", answer);
        }
    }
    else printf("\nCannot calculate ln-function with this arguments.\n");

    // random function
    answer;
    result = dichotomy(&answer, a, b, eps, &random_function);
    if (result == incorrect_input)
    {
        printf("\nCannot calculate random-function with this arguments.\n");
    }
    else
    {
        printf("\nResult for random function: %f\n", answer);
    }

    printf("\nThe program has finished correctly.\n");
    return success;
}

int check_valid_double(char *number)
{
    if (*number == '-')
    {
        number++;
        if (*number == '\0') return incorrect_input;
    }
    if (*number == '.') return incorrect_input;

    int dot_count = 0;
    while (*number != '\0')
    {
        if (*number == '.')
        {
            if (!dot_count) dot_count++;
            else return incorrect_input;
        }
        else if (!isdigit(*number)) return incorrect_input;
        number++;
    }
    number--;
    if (*number == '.') return incorrect_input;

    return success;
}

double function_cos(double x)
{
    return cos(x);
}

double function_sin(double x)
{
    return sin(x);
}

double function_ln(double x)
{
    return log(x);
}

double random_function(double x)
{
    return x * x - 2;
}

// метод половинного деления
// валидация на уровне функции
int dichotomy(double *result, double a, double b, double eps, double (*function)(double))
{
    double a_result = function(a);
    double b_result = function(b);
    double x;
    if (a_result * b_result > 0) return incorrect_input;

    while (fabs(b - a) > eps)
    {
        x = (a + b) / 2.0;
        if (function(x) * a_result > 0) a = x;
        else b = x;
    }
    *result = (a + b) / 2.0;
}

int get_string(char **string)
{
    int length = 2;
    *string = malloc(length * sizeof(char));
    if (*string == NULL) return no_memory;
    char temp;
    char *temp_string;
    temp = getc(stdin);
    int count = 0;

    while(temp != '\n')
    {
        if (length <= count)
        {
            length *= 2;
            temp_string = (char*)realloc(*string, length * sizeof(char));
            if (temp_string == NULL)
            {
                free(*string);
                return no_memory;
            }
            *string = temp_string;
        }
        (*string)[count] = temp;
        temp = fgetc(stdin);
        count++;
    }
    if (length <= count)
    {
        length += 1;
        temp_string = realloc(*string, length * sizeof(char));
        if (temp_string == NULL)
        {
            free(*string);
            return no_memory;
        }
        *string = temp_string;
    }
    (*string)[count] = '\0';
    return count;
}

void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("\nIncorrect input.");
            break;
        case file_error:
            printf("\nCannot open file.");
            break;
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
        case file_input_error:
            printf("\nError with file contents.");
            break;
    }
}