#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    file_error = -3
};

void print_error(int error);

int number_to_base(char ** result, int number, int r);
char make_char_number(int number);
int summary (int first, int second);
void reverse(char * string);
int decrease(int first, int second);
int negative(int number);

int main()
{
    int r = 5;
    FILE * file = fopen("tests.txt", "r");
    int result;
    char * string = NULL;
    int res;
    while(fscanf(file, "%d", &result) > 0)
    {
        res = number_to_base(&string, result, r);
        if (res == success)
        {
            printf("%d in base 2^%d: %s\n", result, r, string);
            free(string);
            string = NULL;
        }
        else if (res == fail)
        {
            printf("Invalid r!\n");
            fclose(file);
            return res;
        }
        else
        {
            print_error(res);
            fclose(file);
            return res;
        }
        free(string);
        string = NULL;
    }
    fclose(file);
    return success;
}

char make_char_number(int number)
{
    if (number < 10) return summary(number, '0');
    else return decrease(summary(number, 'A'), 10);
}

void reverse(char * string)
{
    int length = strlen(string);
    char temp;
    for (int i = 0, j = length - 1; i < j; ++i, --j)
    {
        temp = string[i];
        string[i] = string[j];
        string[j] = temp;
    }
}

int negative(int number)
{
    return summary(~number, 1);
}

int decrease(int first, int second)
{
    return summary(first, negative(second));
}

int summary(int first, int second)
{
    int carry;
    while (second)
    {
        carry = first & second;
        first ^= second;
        second = carry << 1;
    }
    return first;
}

int number_to_base(char ** result, int number, int r)
{
    if (number == 0)
    {
        *result = (char*)malloc(2 * sizeof(char));
        (*result)[0] = '0';
        (*result)[1] = 0;
        return success;
    }
    if (!(r >= 1 && r <= 5)) return fail;

    int flag = 0;
    if (number < 0)
    {
        flag = 1;
        number = negative(number);
    }

    int length = 2;
    *result = (char*)malloc(length * sizeof(char));
    if (*result == NULL) return no_memory;
    char * temp;

    int bit_size = decrease(1 << r, 1);
    int remainder;
    char symbol;
    int count = 0;

    while (number)
    {
        if (length <= count)
        {
            length = summary(length, 1);
            temp = (char*)realloc(*result, length * sizeof(char));
            if (!temp)
            {
                free(*result);
                result = NULL;
                return no_memory;
            }
            *result = temp;
        }
        
        remainder = number & bit_size; // остаток от деления

        symbol = make_char_number(remainder);
        (*result)[count] = symbol;

        number >>= r; // делим число на r-ую степень двойки
        count = summary(count, 1);
    }
    if (flag)
    {
        if (length <= count)
        {
            length = summary(length, 1);
            temp = (char*)realloc(*result, length * sizeof(char));
            if (!temp)
            {
                free(*result);
                result = NULL;
                return no_memory;
            }
            *result = temp;
        }
        (*result)[count] = '-';
        count = summary(count, 1);
    }
    (*result)[count] = 0;
    reverse(*result);
    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.\n");
            break;
        case file_error:
            printf("\nCannot open the file,\n");
            break;
    }
}