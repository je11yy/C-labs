#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_error = -2,
    no_memory = -3,
    fail = -4
};

int summary_any_numbers(char **result, int base, ...);
int summary_two_numbers(char **first, char *second, int base);
void print_error(int error);
int is_number_in_base(char* number, int base);

int main()
{
    char *result = NULL;
    
    int res = summary_any_numbers(&result, 7, "005", "132222", NULL);

    if (res == incorrect_input)
    {
        print_error(res);
        return res;
    }
    printf("Result: %s\n", result);
    free(result);
    result = NULL;
    printf("\nProgram has finished correctly.\n");
    return success;
}

int is_number_in_base(char* number, int base)
{
    int temp;
    while (*number != '\0')
    {
        if (isalpha(*number))
        {
            temp = tolower(*number) - 'a' + 10;
            if (temp >= base) return fail;
        }
        else if (isdigit(*number))
        {
            temp = *number - '0';
            if (temp >= base) return fail;
        }
        else return fail;
        number++;
    }
    return success;
}

int summary_any_numbers(char **result, int base, ...)
{
    if (base < 2 || base > 36) return incorrect_input;
    va_list numbers;
    va_start(numbers, base);
    int length = 1;
    *result = (char*)malloc((length + 1) * sizeof(char));
    if (*result == NULL)
    {
        va_end(numbers);
        return no_memory;
    }
    (*result)[0] = '0';
    (*result)[length] = '\0';

    int res;

    char *number;
    char *new;
    while ((number = va_arg(numbers, char*)) != NULL)
    {
        if (is_number_in_base(number, base) == fail)
        {
            va_end(numbers);
            return incorrect_input;
        }
        while (*number == '0') ++number;
        res = strlen(number);
        if (res != 0)
        {
            res = summary_two_numbers(result, number, base);
            if (res == no_memory)
            {
                va_end(numbers);
                return no_memory;
            }
        }
    }
    va_end(numbers);
    return success;
}


int summary_two_numbers(char **first, char *second, int base)
{
    int len1 = strlen(*first);
    int len2 = strlen(second);
    int len = (len1 > len2) ? len1 : len2;
    // +2 для возможного разряда переноса и для терм нуля
    char result[len + 2];
    result[len + 1] = '\0';
    int summary, carry = 0;
    int digit1, digit2;
    int deliter;
    char *temp;
    int k_ind;
    for (int i = len1 - 1, j = len2 - 1, k = len; i >= 0 || j >= 0; --i, --j, --k)
    {
        k_ind = k;
        if (isalpha((*first)[i])) digit1 = (i >= 0) ? (tolower((*first)[i]) - 'a' + 10) : 0;
        else digit1 = (i >= 0) ? ((*first)[i] - '0') : 0;

        if (isalpha(second[j])) digit2 = (j >= 0) ? (tolower(second[j]) - 'a' + 10) : 0;
        else digit2 = (j >= 0) ? (second[j] - '0') : 0;

        summary = digit1 + digit2 + carry;
        deliter = summary % base;
        if (deliter > 9) result[k] = deliter - 10 + 'A';
        else result[k] = deliter + '0';
        carry = summary / base;
    }
    if (carry > 0)
    {
        k_ind--;
        result[k_ind] = carry + '0';
        temp = (char*)realloc(*first, (len + 2) * sizeof(char));
        if (temp == NULL)
        {
            free(*first);
            first = NULL;
            return no_memory;
        }
        *first = temp;
        for (int i = 0; i < len + 2; ++i)
        {
            (*first)[i] = result[i];
        }
    }
    else
    {
        temp = (char*)realloc(*first, (len + 1) * sizeof(char));
        if (temp == NULL)
        {
            free(*first);
            first = NULL;
            return no_memory;
        }
        *first = temp;
        for (int i = 0; i < len + 2; ++i)
        {
            (*first)[i] = result[i + 1];
        }
    }
    return success;
}


void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("Incorrect input.\n");
            break;
        case file_error:
            printf("Cannot open file.\n");
            break;
        case no_memory:
            printf("Computer doesnot have enough memory for this calculation.\n");
            break;
    }
}