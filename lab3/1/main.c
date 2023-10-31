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
int get_size(int power);
int reduce_number(int number, int how_much);
char make_char_number(int number);

int main()
{
    int r = 2;
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

int get_size(int power)
{
    int result = 1;
    for (int i = 0; i < power; ++i) result <<= 1;
    return result;
}

int reduce_number(int number, int how_much)
{
    for (int i = 0; i < how_much; ++i) number = ~-number;
    return number;
}

char make_char_number(int number)
{
    if (number > 9)
    {
        number = reduce_number(number, 10);
        for (int i = 0; i < 'A'; ++i) number = -~number;
    }
    else for (int i = 0; i < '0'; ++i) number = -~number;
    return number;
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
        number = abs(number);
    }

    int length = 1;
    *result = (char*)malloc((length + 1) * sizeof(char)); // максимальный размер 2^5
    if (*result == NULL) return no_memory;
    char * temp;

    int bit_size = ~-get_size(r);
    int remainder;
    char symbol;
    int count = 0;

    while (number > 0)
    {
        if (length <= count)
        {
            length *= 2;
            temp = (char*)realloc(*result, length * sizeof(char));
            if (temp == NULL)
            {
                (*result)[count] = 0;
                free(*result);
                *result = NULL;
                return no_memory;
            }
        }
        remainder = number & bit_size; // остаток от деления

        symbol = make_char_number(remainder);
        (*result)[count] = symbol;
        number = number >> r; // делим число на r-ую степень двойки
        count++;
    }
    if (flag)
    {
        if (length <= count)
        {
            length *= 2;
            temp = (char*)realloc(*result, length * sizeof(char));
            if (temp == NULL)
            {
                free(*result);
                *result = NULL;
                return no_memory;
            }
        }
        (*result)[count] = '-';
        count++;
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