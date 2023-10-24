#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

enum RESULT
{
    success = 0,
    fail = -1,
    incorrect_input = -2,
    no_memory = -3
};

int final_representation(double eps, int base, double number);
int check_final_representation(char **answer, double eps, int base, ...);
int append_string(char **answer, int *length, char *string);
int add_to_answer(char **answer, int length, double number, int base, int result);
void print_error(int error);
int has_final_representation(int number, int base);
int get_denominator(int number);

int main()
{
    char *answer = NULL;
    double eps = 0.00001;
    // разложить на множители систему счисления и чисел
    int result = check_final_representation(&answer, eps, 8, 0.125, 0.17, 0.5, eps);
    if (result == incorrect_input)
    {
        print_error(result);
        free(answer);
        return result;
    }
    if (result == no_memory)
    {
        print_error(no_memory);
        free(answer);
        return result;
    }
    printf("%s\n", answer);
    printf("Program has finished correctly.\n");
    return success;
}

int check_final_representation(char **answer, double eps, int base, ...)
{
    if (base < 2 || base > 36) return incorrect_input;
    va_list numbers;
    va_start(numbers, base);
    double number;

    int result;
    int length = 0;
    while ((number = va_arg(numbers, double)) != eps)
    {
        if (number < eps || (number -1) > eps)
        {
            va_end(numbers);
            return incorrect_input;
        }
        result = final_representation(eps, base, number);
        length = add_to_answer(answer, length, number, base, result);
    }
    va_end(numbers);
    return success;
}

int append_string(char **answer, int *length, char *string)
{
    int len = strlen(string);
    char *temp = (char*)realloc(*answer, (*length + len + 1) * sizeof(char));
    if (temp == NULL)
    {
        free(*answer);
        *answer = NULL;
        return no_memory;
    }
    *answer = temp;

    for (int i = *length, j = 0; i < *length + len, j < len; ++i, ++j)
    {
        (*answer)[i] = string[j];
    }
    (*answer)[*length + len] = '\0';
    *length += len;
    return success;
}

int add_to_answer(char **answer, int length, double number, int base, int result)
{
    char number_string[16];
    sprintf(number_string, "%.20f", number);
    char base_string[3];
    itoa(base, base_string, 10);

    int res = append_string(answer, &length, "The number ");
    if (res == no_memory) return no_memory;
    res = append_string(answer, &length, number_string);
    if (res == no_memory) return no_memory;
    res = append_string(answer, &length, " in base ");
    if (res == no_memory) return no_memory;
    res = append_string(answer, &length, base_string);
    if (res == no_memory) return no_memory;
    if (result == success)
    {
        res = append_string(answer, &length, " has a final representation.\n\n");
        if (res == no_memory) return no_memory;
    }
    else
    {
        res = append_string(answer, &length, " doesnt have a final representation.\n\n");
        if (res == no_memory) return no_memory;
    }
    return length;
}

int get_denominator(int number)
{
    int size = 256;
    char buffer[size];
    sprintf(buffer, "%d", number);
    int length = strlen(buffer);
    int denominator = 1; // знаменатель
    for (int i = 0; i < length; ++i) denominator *= 10;
    //будем сокращать дробь, где number - числитель, denominator - знаменатель
    while (number % 2 == 0 && denominator % 2 == 0)
    {
        number /= 2;
        denominator /= 2;
    }
    for (int i = 3; i < sqrt(number); i += 2)
    {
        while (number % i == 0 && denominator % i == 0)
        {
            number /= i;
            denominator /= i;
        }
    }
    if (number > 2) if (denominator % number == 0)  denominator /= number;
    return denominator;
}

// если основание делится на каждое из простых делителей знаменателя, то число имеет конечное представление
int has_final_representation(int number, int base)
{
    while (number % 2 == 0)
    {
        if (base % 2 != 0) return fail;
        number /= 2;
    }
    for (int i = 3; i < sqrt(number); i += 2)
    {
        while (number % i == 0)
        {
            if (base % i != 0) return fail;
            number /= i;
        }
    }
    if (number > 2) if (base % number != 0) return fail;
    return success;
}

int final_representation(double eps, int base, double number)
{
    double whole;
    double fractional = modf(number, &whole);
    int max_iterations = 0;
    while (fractional > eps && max_iterations < 9)
    {
        number *= 10;
        fractional = modf(number, &whole);
        max_iterations++;
    }
    int full_fraction = (int) whole;
    printf("fraction %d\n", full_fraction);
    int denominator = get_denominator(full_fraction);
    printf("denominator %d\n", denominator);
    int result = has_final_representation(denominator, base);
    return result;
}

void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("Incorrect input.\n");
            break;
        case no_memory:
            printf("Computer doesnot have enough memory for this calculation.\n");
            break;
    }
}