#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 256

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    incorrect_console_input = -2,
    one_element_array = -3,
    no_memory = -4
};

void menu(int state);

void print_error(int error);

int is_digit(char *number);

int base_valid_check(char *base);

int number_check_valid(char *number, int base);

int get_decimal_number(char *number, int base);

int to_base(char **buf, int size, int number, int base);

int getlen(int number, int base);

int get_string(char **string);

/////////////////////////////////////////

int main()
{
    menu(1);
    char *result = NULL;
    int tmp;
    tmp = get_string(&result);
    if (tmp == no_memory)
    {
        print_error(tmp);
        free(result);
        result = NULL;
        return no_memory;
    }

    while (base_valid_check(result) == incorrect_console_input)
    {
        free(result);
        result = NULL;
        printf("\nBase should be a number between 2 and 36!\nTry again: ");
        tmp = get_string(&result);
        if (tmp == no_memory)
        {
            print_error(tmp);
            free(result);
            result = NULL;
            return no_memory;
        }
    }
    int base = atoi(result);
    free(result);
    result = NULL;

    menu(0);
    int i = 1;
    printf("%d. ", i);
    tmp = get_string(&result);
    if (tmp == no_memory)
    {
        print_error(tmp);
        free(result);
        result = NULL;
        return no_memory;
    }
    int maximum = 0;
    int curr_decimal;
    int res = strcmp(result, "Stop");
    while (res != 0)
    {
        while(number_check_valid(result, base) != success)
        {
            free(result);
            result = NULL;
            printf("Number should be in %d base. Try again.\n");
            printf("%d. ", i);
            tmp = get_string(&result);
            res = strcmp(result, "Stop");
            if (tmp == no_memory)
            {
                print_error(tmp);
                free(result);
                result = NULL;
                return no_memory;
            }
            if (res == 0) break;
        }
        if (res == 0) break;

        curr_decimal = get_decimal_number(result, base);
        if (abs(curr_decimal) > abs(maximum)) maximum = curr_decimal;

        i++;
        free(result);
        result = NULL;
        printf("%d. ", i);
        tmp = get_string(&result);
        if (tmp == no_memory)
        {
            print_error(tmp);
            free(result);
            result = NULL;
            return no_memory;
        }
    }
    if (i == 1)
    {
        printf("\nNO NUMBERS WERE INPUTTED. FINISHING HE PROGRAM\n");
        return incorrect_input;
    }
    printf("Decimal maximum: %d\n", maximum);
    int size = getlen(maximum, base);
    char *based_maximum = NULL;
    res = to_base(&based_maximum, size, maximum, base);

    if (res == no_memory)
    {
        free(based_maximum);
        based_maximum = NULL;
        print_error(no_memory);
        return no_memory;
    }
    else printf("Maximum in original base: %s\n", based_maximum);
    free(based_maximum);

    // сделать циклом
    int count_nine = 0;
    for (int i = 0; i < 4; ++i)
    {
        count_nine += 9;
        size = getlen(maximum, count_nine);
        based_maximum = NULL;
        res = to_base(&based_maximum, size, maximum, count_nine);
        if (res == no_memory) print_error(res);
        else printf("Maximum in 9th base: %s\n", based_maximum);
        free(based_maximum);

    }

    printf("\n____________________________________\n");
    printf("\nThe program has finished.\n");
    printf("\n____________________________________\n");
    return 0;
}

/////////////////////////////////////////

int getlen(int number, int base)
{
    int result = 0;
    int flag = 0;
    if (number < 0)
    {
        flag = 1;
        number *= -1;
    }

    while (number > 0)
    {
        result++;
        number /= base;
    }

    if (flag == 1) result += 1;

    return result;
}

int to_base(char **buf, int size, int number, int base)
{
    *buf = malloc(size * sizeof(char));
    if (*buf == NULL) return no_memory;

    char *ptr = *buf + size;
    *(ptr--) = '\0';
    int flag = 0;
    if (number < 0) flag = 1;

    int remainder;
    number = abs(number);
    while (number > 0)
    {
        remainder = number % base;
        *(ptr--) = (remainder > 9) ? remainder - 10 + 'A' : remainder + '0';
        number /= base;
    }
    if (flag == 1) *(ptr) = '-';
    return success;
}

int get_decimal_number(char* number, int base)
{
    int result = 0, length = strlen(number);
    char *ptr = number - 1;
    int flag = 0;
    if (*(++ptr) == '-') flag = 1;
    else --ptr;

    while (*(++ptr))
    {
        result = result * base + (isdigit(*ptr) ? *ptr - '0' : *ptr - 'A' + 10);
    }
    if (flag == 1) result *= -1;

    return result;
}

int number_check_valid(char *number, int base)
{
    int length = strlen(number);
    int temp;
    int i = 0;
    if (number[0] == '-') i = 1;

    for (i; i < length; ++i)
    {
        if (isalpha(number[i]))
        {
            temp = tolower(number[i]) - 'a' + 10;
            if (temp >= base) return incorrect_input;
        }
        else if (isdigit(number[i]))
        {
            temp = number[i] - '0';
            if (temp >= base) return incorrect_input;
        }
        else return incorrect_input;
    }

    return success;
}

int base_valid_check(char *base)
{
    if (is_digit(base) == 0) return incorrect_console_input;

    int base_int = atoi(base);
    if (!(base_int > 1 && base_int < 37)) return incorrect_console_input;

    return success;
}

void menu(int state)
{
    switch (state)
    {
        case 1:
            printf("\n____________________________________\n");
            printf("\t\tMENU\n");
            printf("\n____________________________________\n");
            printf("Please, write down the base (from 2 to 36): ");
            break;
        case 2:
            printf("Write down numbers in this base. To stop write down 'Stop'.\n");
    }
    return;
}

void print_error(int error)
{
    switch (error)
    {
        case incorrect_input:
            printf("\nIncorrect input! Please, use the format:\n");
            printf(".\a.exe A B\n");
            printf("A and B should be more than -2 147 483 648 and less than 2 147 483 647\n");
            printf("A > B\n");
            break;
        case incorrect_console_input:
            printf("\nIncorrect input! Please, input number that is more than -2 147 483 648 and less than 2 147 483 647\n");
            break;
        case one_element_array:
            printf("\nMinimum and maximum cannot be changed, because array has only one element\n");
            break;
        case no_memory:
            printf("\nComputer doesn't have enough memory for this calculation\n");
            break;
    }
    return;
}

int is_digit(char *number)
{
    if (number == "-") return 0;
    if (*number == '-') number++;

    while (*number != '\0')
    {
        if (!(*number >= '0' && *number <= '9')) return 0;
        number++;
    }
    return 1;
}

int get_string(char **new_string)
{
    int length = 2;
    char *string = malloc(length * sizeof(char));
    if (string == NULL) return no_memory;
    char temp;
    char *temp_string;
    temp = getc(stdin);
    int count = 0;

    while(temp != '\n')
    {
        if (length <= count)
        {
            length *= 2;
            temp_string = (char*)realloc(string, length * sizeof(char));
            if (temp_string == NULL)
            {
                free(string);
                return no_memory;
            }
            string = temp_string;
        }
        string[count] = temp;
        temp = fgetc(stdin);
        count++;
    }
    if (length <= count)
    {
        length += 1;
        temp_string = realloc(string, length * sizeof(char));
        if (temp_string == NULL)
        {
            free(string);
            return no_memory;
        }
        string = temp_string;
    }
    string[count] = '\0';
    *new_string = string;
    return count;
}