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

int get_string(FILE * stream, char ** string);
int in_flags(char * flag);
void print_error(int error);

int overfscanf(FILE * stream, const char * format, ...);
int scan_my_flags(FILE * stream, char * temp, va_list * args);
int get_value_roman(char symbol);
int get_roman(char * string);
unsigned int get_zeckendorf(char * string);
int get_decimal_number(char* number, int base);

int main()
{
    FILE * file = fopen("file.txt", "r");
    int number;
    int number1;
    int number2;
    int base = 16;
    unsigned int number_unsigned;
    double number_double;
    char string[10];
    int result = overfscanf(file, "%d %lf %s", &number, &number_double, &string);
    if (result == no_memory)
    {
        print_error(result);
        fclose(file);
        return result;
    }
    printf("\n  test 1\nresult: %d\nnumber: %d\ndouble: %lf\nstring: %s\n", result, number, number_double, string);
    result = overfscanf(file, "%Ro %Zr %CV %Cv", &number, &number_unsigned, &number1, base, &number2, base);
    if (result == no_memory)
    {
        print_error(result);
        fclose(file);
        return result;
    }
    printf("\n  test2\nresult: %d\nroman: %d\nzeckendorf: %d\n", result, number, number_unsigned);
    printf("to decimal high: %d\nto decimal low: %d\n", number1, number2);
    printf("\nThe program has finished correctly.\n");
    fclose(file);
    return success;
}

int overfscanf(FILE * stream, const char * format, ...)
{
    int number_of_scanfed = 0;
    va_list arguments;
    va_start(arguments, format);
    int length = strlen(format);
    char * string = (char*)format;
    char * temp = (char*)malloc((length + 1) * sizeof(char));
    if (temp == NULL)
    {
        va_end(arguments);
        return no_memory;
    }
    for (int i = 0; i <= length; ++i) temp[i] = 0;
    char flag[] = {'1', '1', '\0'};
    int count = 0, temp_count = 0;
    int result;
    void * nomatter_flag;
    for (int i = 0; i < length; ++i)
    {
        if (format[i] == '%')
        {
            flag[0] = format[i + 1];
            flag[1] = format[i + 2];
            if (in_flags(flag) == success)
            {
                if (temp_count != 0)
                {
                    temp[temp_count] = 0;
                    result = vfscanf(stream, temp, arguments);
                    if (result == EOF)
                    {
                        free(temp);
                        temp = NULL;
                        return fail;
                    }
                    count += result;
                    for (int j = 0; j < result; ++j) nomatter_flag = va_arg(arguments, void*);
                }
                result = scan_my_flags(stream, flag, &arguments);
                count++;
                if (result == no_memory)
                {
                    free(temp);
                    temp = NULL;
                    return result;
                }

                for (int j = 0; j <= length; ++j) temp[j] = 0;
                temp_count = 0;
                i += 3;
            }
        }
        temp[temp_count] = format[i];
        temp_count++;
    }
    if (temp_count != 0)
    {
        temp[temp_count] = 0;
        result = vfscanf(stream, temp, arguments);
        if (result < 0)
        {
            free(temp);
            temp = NULL;
            return fail;
        }
        count += result;
    }
    free(temp);
    temp = NULL;

    va_end(arguments);
    return count;
}

int scan_my_flags(FILE * stream, char * flag, va_list * args)
{
    int number, base;
    unsigned int number2;
    int * result;
    unsigned int * result2;
    unsigned int zeckendorf;
    char * string = NULL;
    number = get_string(stream, &string);
    if (number == no_memory) return number;
    switch(flag[0])
    {
        case 'R':
            result = va_arg(*args, int*);
            number = get_roman(string);
            *result = number;
            break;
        case 'Z':
            result2 = va_arg(*args, unsigned int*);
            number2 = get_zeckendorf(string);
            *result2 = number2;
            break;
        case 'C':
            result = va_arg(*args, int*);
            base = va_arg(*args, int);
            if (!(base >= 2 && base <= 36)) base = 10;
            number = get_decimal_number(string, base);
            *result = number;
            break;
        default: break;
    }
    free(string);
    string = NULL;
    return success;
}

int get_value_roman(char symbol)
{
    switch(symbol)
    {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
    }
    return 0;
}

int get_roman(char * string)
{
    int result = 0;
    int temp;

    for (int i = 0; string[i] != '\0'; ++i)
    {
        temp = get_value_roman(string[i]);
        if (temp < get_value_roman(string[i + 1])) result -= temp;
        else result += temp;
    }

    return result;
}

unsigned int get_zeckendorf(char * string)
{
    int length = strlen(string);
    unsigned int result = 0;
    unsigned int cur_fib = 1, prev_fib = 1, next_fib = 1;
    for (int i = 0; i < length - 1; ++i)
    {
        cur_fib = next_fib;
        if (string[i] == '1') result += cur_fib;
        if (i != 0)
        {
            next_fib = cur_fib + prev_fib;
            prev_fib = cur_fib;
        }
    }
    return result;
}

int get_decimal_number(char* number, int base)
{
    int result = 0;
    int flag = 0;
    char * ptr = number - 1;
    if (*(++ptr) == '-') flag = 1;
    else --ptr;

    while (*(++ptr))
    {
        result = result * base + (isdigit(*ptr) ? *ptr - '0' : tolower(*ptr) - 'a' + 10);
    }
    if (flag == 1) result *= -1;
    return result;
}

int in_flags(char * flag)
{
    char * flags[] = {"Ro", "Zr", "Cv", "CV"};
    int size = sizeof(flags) / sizeof(flags[0]);

    for (int i = 0; i < size; ++i)
    {
        if (!strcmp(flag, flags[i])) return success;
    }
    return fail;
}

int get_string(FILE * stream, char ** string)
{
    int length = 2;
    *string = (char*)malloc(length * sizeof(char));
    if (*string == NULL) return no_memory;
    char * temp;
    int count = 0;
    char symbol = fgetc(stream);
    while (symbol == ' ' || symbol == '\n' || symbol == ',' || symbol == '\t') symbol = fgetc(stream);
    while (symbol != ' ' && symbol != '\n' && symbol != ',' && symbol != '\t' && symbol != EOF)
    {
        if (length <= count)
        {
            length *= 2;
            temp = (char*)realloc(*string, length * sizeof(char));
            if (temp == NULL)
            {
                free(string);
                string = NULL;
                return no_memory;
            }
            *string = temp;
        }
        (*string)[count] = symbol;
        count++;
        symbol = fgetc(stream);
    }
    (*string)[count] = '\0';
    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_error:
            printf("\nCannot open file.\n");
            break;
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.\n");
            break;
        case fail:
            printf("\nCannot run the function.\n");
            break;
    }
}