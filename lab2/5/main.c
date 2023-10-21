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

enum ROMAN
{
    I = 1,
    IV = 4,
    V = 5,
    IX = 9,
    X = 10,
    XL = 40,
    L = 50,
    XC = 90,
    C = 100,
    CD = 400,
    D = 500,
    CM = 900,
    M = 1000
};

int check_is_flag(char a, char b);
int overfprintf(FILE *stream, const char *format, ...);
int print_roman(char **answer, int number);
int find_max_fibonacci(int *fib, int number);
int print_zeckendorf(char ** answer, unsigned int number);
int number_to_base_low(char ** answer, int number, int base);
int number_to_base_high(char ** answer, int number, int base);
int getlen(int number, int base);
int print_bytes(unsigned char ** answer, const void * number, size_t size);
void print_error(int error);
int print_my_flags(FILE * stream, va_list *arguments, char flag1, char flag2);
int get_decimal_number(char* number, int base);
int oversprintf(char buf[], const char * format, ...);
int buf_my_flags(char ** buf, va_list *arguments, char flag1, char flag2);
int append_string(char answer[], int *length, char *string);
int append_string2(char answer[], int *length, char string[], int len);

int main()
{
    unsigned int for_zr = 51;
    signed int mi = +16;
    signed int mi1 = -16;
    unsigned int mu = 16;
    double md = 16.5;
    float mf = 16.5;

    int result = overfprintf(stdout, "\n%d %.3f\nroman: %Ro\nzeckendorf: %Zr\nto low base: %Cv\nto high base: %CV\nto decimal low: %to\n", 3, 0.5, 172, for_zr, 1839, 16, 1839, 16, "ef", 16);
    if (result == no_memory)
    {
        print_error(result);
        return result;
    }
    if (result == incorrect_input)
    {
        print_error(result);
        return result;
    }
    if (result == fail)
    {
        print_error(result);
        return result;
    }

    result = overfprintf(stdout, "to decimal high: %TO\nbytes signed: %mi\nbytes signed: %mi\nbytes unsigned: %mu\nbytes double: %md\nbytes float: %mf\n", "EF", 16, mi, mi1, mu, md, mf);
    if (result == no_memory)
    {
        print_error(result);
        return result;
    }
    if (result == incorrect_input)
    {
        print_error(result);
        return result;
    }
    if (result == fail)
    {
        print_error(result);
        return result;
    }
    

    //////////////////////////////////////////
    char answer[256] = {0};
    result = oversprintf(answer, "%d %f %Ro %CV", 3, 0.5, 173, 1839, 16);
    if (result == no_memory)
    {
        print_error(result);
        return result;
    }
    if (result == incorrect_input)
    {
        print_error(result);
        return result;
    }
    if (result == fail)
    {
        print_error(result);
        return result;
    }

    printf("\nfor sprintf: %s\n", answer);
    printf("\n\nThe program has finished correctly.\n\n");
    return success;
}

int oversprintf(char buf[], const char * format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    int length = strlen(format);
    char *temp = (char*)calloc(length, sizeof(char));
    if (temp == NULL)
    {
        va_end(arguments);
        return no_memory;
    }

    char * answer = NULL;
    char answer1[256] = {0};

    int result;
    int count = 0;
    int len = 0;
    int j = 0;
    for (int i = 0; i < length; ++i)
    {
        if (format[i] == '%')
        {
            if (check_is_flag(format[i + 1], format[i + 2]) == success && format[i + 1] != 'm')
            {
                if (i != 0)
                {
                    result = vsprintf(answer1, temp, arguments);
                    if (result < 0)
                    {
                        free(temp);
                        temp = NULL;
                        free(answer);
                        answer = NULL;
                        va_end(arguments);
                        return fail;
                    }
                    result = append_string2(buf, &len, answer1, result);

                    j = 0;
                    for (int k = 0; k < count; ++k)
                    {
                        while (temp[j] != '%') j++;
                        j++;
                        while (temp[j] != ' ' && temp[j] != '%' && temp[j] != '\0' && temp[j] != '\n' && temp[j] != '\t') j++;

                        if (temp[j - 1] == 'f') va_arg(arguments, double);
                        else va_arg(arguments, void*);
                    }
                }
                count = 0;
                // распечатать штуку по флагу
                result = buf_my_flags(&answer, &arguments, format[i + 1], format[i + 2]);
                if (result == no_memory)
                {
                    free(temp);
                    temp = NULL;
                    free(answer);
                    answer = NULL;
                    va_end(arguments);
                    return result;
                }
                if (result == incorrect_input)
                {
                    free(temp);
                    temp = NULL;
                    free(answer);
                    answer = NULL;
                    va_end(arguments);
                    return result;
                }
                result = append_string(buf, &len, answer);
                free(answer);
                answer = NULL;

                i += 3;
                j = 0;
                // очистить временную строку
                free(temp);
                temp = NULL;
                temp = (char*)calloc(length, sizeof(char));
                if (temp == NULL)
                {
                    free(answer);
                    answer = NULL;
                    va_end(arguments);
                    return result;
                }
            }
            else count++;
        }
        // добавление в строку
        temp[j] = format[i];
        j++;
    }
    if (j != 0)
    {
        result = vsprintf(answer1, temp, arguments);
        if (result < 0)
        {
            free(answer);
            answer = NULL;
            free(temp);
            temp = NULL;
            va_end(arguments);
            return fail;
        }
        result = append_string2(buf, &len, answer1, result);
    }
    free(temp);
    temp = NULL;
    free(answer);
    answer = NULL;

    va_end(arguments);
    return success;
}

int append_string(char answer[], int *length, char * string)
{
    int len = strlen(string);

    for (int i = *length, j = 0; i < *length + len, j < len; ++i, ++j)
    {
        answer[i] = string[j];
    }
    answer[*length + len] = '\0';

    *length += len;
    return success;
}

int append_string2(char answer[], int *length, char string[], int len)
{
    for (int i = *length, j = 0; i < *length + len, j < len; ++i, ++j)
    {
        answer[i] = string[j];
    }
    answer[*length + len] = '\0';

    *length += len;
    return success;
}

int buf_my_flags(char ** buf, va_list *arguments, char flag1, char flag2)
{
    char *answer = NULL;
    unsigned char *answer1 = NULL;
    int result, result1;
    signed int number1;
    unsigned int number2;
    double number3;
    float number4;
    char *string;
    switch (flag1)
    {
        case 'R':
            result = va_arg(*arguments, int);
            result = print_roman(&answer, result);
            if (result == no_memory)
            {
                free(answer);
                answer = NULL;
                return no_memory;
            }
            *buf = answer;
            break;
        case 'Z':
            result = va_arg(*arguments, unsigned int);
            result = print_zeckendorf(&answer, result);
            if (result == no_memory)
            {
                free(answer);
                answer = NULL;
                return no_memory;
            }
            *buf = answer;
            break;
        case 'C':
            switch (flag2)
            {
                case 'v':
                    result = va_arg(*arguments, int);
                    result1 = va_arg(*arguments, int);
                    result = number_to_base_low(&answer, result, result1);
                    if (result == no_memory)
                    {
                        free(answer);
                        answer = NULL;
                        return no_memory;
                    }
                    *buf = answer;
                    break;
                case 'V':
                    result = va_arg(*arguments, int);
                    result1 = va_arg(*arguments, int);
                    result = number_to_base_high(&answer, result, result1);
                    if (result == no_memory)
                    {
                        free(answer);
                        answer = NULL;
                        return no_memory;
                    }
                    *buf = answer;
                    break;
            }
            break;
        case 't':
            string = va_arg(*arguments, char*);
            result = va_arg(*arguments, int);
            result = get_decimal_number(string, result);
            sprintf(*buf, "%d", result);
            result = success;
            break;
        case 'T':
            string = va_arg(*arguments, char*);
            result = va_arg(*arguments, int);
            result = get_decimal_number(string, result);
            sprintf(*buf, "%d", result);
            result = success;
            break;
        case 'm':
            switch (flag2)
            {
                case 'i':
                    number1 = va_arg(*arguments, signed int);
                    result = print_bytes(&answer1, &number1, sizeof(signed int));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    *buf = answer1;
                    break;
                case 'u':
                    number2 = va_arg(*arguments, unsigned int);
                    result = print_bytes(&answer1, &number2, sizeof(unsigned int));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    *buf = answer1;
                    break;
                case 'd':
                    number3 = va_arg(*arguments, double);
                    result = print_bytes(&answer1, &number3, sizeof(double));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    *buf = answer1;
                    break;
                case 'f':
                    number4 = (float) va_arg(*arguments, double);
                    result = print_bytes(&answer1, &number4, sizeof(float));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    *buf = answer1;
                    break;
            }
            break;
    }
    return result;
}

// возвращает число выведенных символов или отрицательное значение в случае ошибки
int overfprintf(FILE *stream, const char *format, ...)
{
    va_list arguments;
    va_start(arguments, format);

    int length = strlen(format);
    char *temp = (char*)malloc((length + 1) * sizeof(char));
    if (temp == NULL)
    {
        printf("\nHERE\n");
        va_end(arguments);
        return no_memory;
    }

    int result;
    int count = 0;
    int j = 0;
    for (int i = 0; i < length; ++i)
    {
        if (format[i] == '%')
        {
            if (check_is_flag(format[i + 1], format[i + 2]) == success)
            {
                if (i != 0)
                {
                    result = vfprintf(stream, temp, arguments);
                    if (result < 0)
                    {
                        free(temp);
                        temp = NULL;
                        va_end(arguments);
                        return fail;
                    }
                    j = 0;
                    for (int k = 0; k < count; ++k)
                    {
                        while (temp[j] != '%') j++;
                        j++;
                        while (temp[j] != ' ' && temp[j] != '%' && temp[j] != '\0' && temp[j] != '\n' && temp[j] != '\t') j++;

                        if (temp[j - 1] == 'f') va_arg(arguments, double);
                        else va_arg(arguments, void*);
                    }
                }
                count = 0;
                // распечатать штуку по флагу
                result = print_my_flags(stream, &arguments, format[i + 1], format[i + 2]);
                if (result == no_memory)
                {
                    free(temp);
                    temp = NULL;
                    va_end(arguments);
                    return result;
                }
                if (result == incorrect_input)
                {
                    free(temp);
                    temp = NULL;
                    va_end(arguments);
                    return result;
                }

                i += 3;
                j = 0;

                // очистить временную строку

                for (int i = 0; i <= length; ++i)
                {
                    temp[i] = 0;
                }
                // free(temp);
                // temp = NULL;
                // temp = (char*)malloc(length * sizeof(char));
                // if (temp == NULL)
                // {
                //     ///////// СЮДА ЗАХОДИТ????
                //     printf("\n%d\n", strlen(format));
                //     free(temp);
                //     temp = NULL;
                //     va_end(arguments);
                //     return no_memory;
                // }
            }
            else count++;
        }
        // добавление в строку
        temp[j] = format[i];
        j++;
    }
    if (j != 0)
    {
        result = vfprintf(stream, temp, arguments);
        if (result < 0)
        {
            free(temp);
            temp = NULL;
            va_end(arguments);
            return fail;
        }
    }
    free(temp);
    temp = NULL;

    va_end(arguments);
    return success;
}

int print_my_flags(FILE * stream, va_list *arguments, char flag1, char flag2)
{
    char *answer = NULL;
    unsigned char *answer1 = NULL;
    int result, result1;
    signed int number1;
    unsigned int number2;
    double number3;
    float number4;
    char *string;
    switch (flag1)
    {
        case 'R':
            result = print_roman(&answer, va_arg(*arguments, int));
            if (result == no_memory)
            {
                free(answer);
                answer = NULL;
                return no_memory;
            }
            fprintf(stream, "%s", answer);
            free(answer);
            answer = NULL;
            break;
        case 'Z':
            result = print_zeckendorf(&answer, va_arg(*arguments, unsigned int));
            if (result == no_memory)
            {
                free(answer);
                answer = NULL;
                return no_memory;
            }
            fprintf(stream, "%s", answer);
            free(answer);
            answer = NULL;
            break;
        case 'C':
            switch (flag2)
            {
                case 'v':
                    result = va_arg(*arguments, int);
                    result1 = va_arg(*arguments, int);
                    result = number_to_base_low(&answer, result, result1);
                    if (result == no_memory)
                    {
                        free(answer);
                        answer = NULL;
                        return no_memory;
                    }
                    fprintf(stream, "%s", answer);
                    free(answer);
                    answer = NULL;
                    break;
                case 'V':
                    result = va_arg(*arguments, int);
                    result1 = va_arg(*arguments, int);
                    result = number_to_base_high(&answer, result, result1);
                    if (result == no_memory)
                    {
                        free(answer);
                        answer = NULL;
                        return no_memory;
                    }
                    fprintf(stream, "%s", answer);
                    free(answer);
                    answer = NULL;
                    break;
            }
            break;
        case 't':
            string = va_arg(*arguments, char*);
            result = va_arg(*arguments, int);
            result = get_decimal_number(string, result);
            fprintf(stream, "%d", result);
            result = success;
            break;
        case 'T':
            string = va_arg(*arguments, char*);
            result = va_arg(*arguments, int);
            result = get_decimal_number(string, result);
            fprintf(stream, "%d", result);
            result = success;
            break;
        case 'm':
            switch (flag2)
            {
                case 'i':
                    number1 = va_arg(*arguments, signed int);
                    result = print_bytes(&answer1, &number1, sizeof(signed int));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    for (int i = 0; i < 8 * sizeof(signed int); ++i)
                    {
                        if (i % 8 == 0) fprintf(stream, " ");
                        fprintf(stream, "%u", answer1[i]);
                    }
                    free(answer1);
                    answer1 = NULL;
                    break;
                case 'u':
                    number2 = va_arg(*arguments, unsigned int);
                    result = print_bytes(&answer1, &number2, sizeof(unsigned int));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    for (int i = 0; i < 8 * sizeof(unsigned int); ++i)
                    {
                        if (i % 8 == 0) fprintf(stream, " ");
                        fprintf(stream, "%u", answer1[i]);
                    }
                    free(answer1);
                    answer1 = NULL;
                    break;
                case 'd':
                    number3 = va_arg(*arguments, double);
                    result = print_bytes(&answer1, &number3, sizeof(double));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    for (int i = 0; i < 8 * sizeof(double); ++i)
                    {
                        if (i % 8 == 0) fprintf(stream, " ");
                        fprintf(stream, "%u", answer1[i]);
                    }
                    free(answer1);
                    answer1 = NULL;
                    break;
                case 'f':
                    number4 = (float) va_arg(*arguments, double);
                    result = print_bytes(&answer1, &number4, sizeof(float));
                    if (result == no_memory)
                    {
                        free(answer1);
                        answer1 = NULL;
                        return no_memory;
                    }
                    for (int i = 0; i < 8 * sizeof(float); ++i)
                    {
                        if (i % 8 == 0) fprintf(stream, " ");
                        fprintf(stream, "%u", answer1[i]);
                    }
                    free(answer1);
                    answer1 = NULL;
                    break;
            }
            break;
    }
    return result;
}

// максимальное число, которое может быть записано в виде римских цифр, - 3999
int print_roman(char **answer, int number)
{
    int length = 1;
    int len;
    char *buffer = (char*)malloc(length * sizeof(char));
    char *temp;
    if (buffer == NULL) return no_memory;
    length = 1;

    int numerals[] = {M, CM, D, CD, C, XC, L, XL, X, IX, V, IV, I};
    char *numerals_string[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    if (number <= 0 || number >= 4000) return incorrect_input;
    for (int i = 0; i < sizeof(numerals) / sizeof(int); ++i)
    {
        while (number >= numerals[i])
        {
            if (i % 2 == 0) len = 1;
            else len = 2;

            length += len;
            temp = (char*) realloc(buffer, length * sizeof(char));
            if (temp == NULL)
            {
                free(buffer);
                buffer = NULL;
                return no_memory;
            }
            buffer = temp;
            for (int j = length - len - 1, k = 0; j < length - 1, k < len; ++k, ++j)
            {
                buffer[j] = numerals_string[i][k];
            }
            number -= numerals[i];
        }
    }
    buffer[length - 1] = '\0';

    *answer = buffer;
    return success;
}

int print_zeckendorf(char ** answer, unsigned int number)
{
    if (number == 0) return incorrect_input;
    int fib = 0;
    int length = find_max_fibonacci(&fib, number);
    char *array = (char*) malloc(length * sizeof(char));
    if (array == NULL) return no_memory;
    for (int i = 0; i < length; ++i) array[i] = '0';
    array[length + 2] = '\0'; // конец строки
    array[length + 1] = '1'; // доп единица в записи
    array[length] = '1';

    number -= fib;
    int index = find_max_fibonacci(&fib, number);
    while (number > 0)
    {
        array[index] = '1';
        number -= fib;
        index = find_max_fibonacci(&fib, number);
    }
    *answer = array;
    return success;
}

int find_max_fibonacci(int *fib, int number)
{
    int fib_cur = 1, fib_prev = 1, fib_next, count = 0;
    while (number >= fib_cur)
    {
        fib_next = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur = fib_next;
        count++;
    }
    if (number == fib_cur)
    {
        *fib = fib_cur;
        return count;
    }
    count--;
    *fib = fib_prev;
    return count;
}

int number_to_base_low(char ** answer, int number, int base)
{
    if (base < 2 || base > 36) base = 10;

    int size = getlen(number, base);
    char *buf = malloc(size * sizeof(char));
    if (buf == NULL) return no_memory;

    char *ptr = buf + size;
    *(ptr--) = '\0';
    int flag = 0;
    if (number < 0) flag = 1;

    int remainder;
    number = abs(number);
    while (number > 0)
    {
        remainder = number % base;
        *(ptr--) = (remainder > 9) ? remainder - 10 + 'a' : remainder + '0';
        number /= base;
    }
    if (flag == 1) *(ptr) = '-';
    *answer = buf;

    return success;
}

int number_to_base_high(char ** answer, int number, int base)
{
    if (base < 2 || base > 36) base = 10;

    int size = getlen(number, base);
    char *buf = malloc(size * sizeof(char));
    if (buf == NULL) return no_memory;

    char *ptr = buf + size;
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
    *answer = buf;

    return success;
}

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

int get_decimal_number(char* number, int base)
{
    int result = 0;
    char *ptr = number - 1;
    int flag = 0;
    if (*(++ptr) == '-') flag = 1;
    else --ptr;

    while (*(++ptr))
    {
        result = result * base + (isdigit(*ptr) ? *ptr - '0' : tolower(*ptr) - 'a' + 10);
    }
    if (flag == 1) result *= -1;
    return result;
}

int print_bytes(unsigned char ** answer, const void * number, size_t size)
{
    unsigned char *byte_ptr = (unsigned char*) number;
    unsigned char byte, bit;
    unsigned char * buffer = (unsigned char*) malloc(size * sizeof(unsigned char));
    buffer[size - 1] = '\0';
    if (buffer == NULL) return no_memory;
    int k = 0;
    for (size_t i = 0; i < size; ++i)
    {
        byte = byte_ptr[i];
        for (int j = 7; j >= 0; --j)
        {
            bit = (byte >> j) & 1;
            buffer[k] = bit;
            k++;
        }
    }
    *answer = buffer;
    return success;
}

int check_is_flag(char a, char b)
{
    a = tolower(a);
    b = tolower(b);
    char *flags[] = {"ro", "zr", "cv", "to", "mi", "mu", "md", "mf"};
    int length = sizeof(flags) / sizeof(flags[0]);

    for (int i = 0; i < length; ++i)
    {
        if (flags[i][0] == a && flags[i][1] == b) return success;
    }
    
    return fail;
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