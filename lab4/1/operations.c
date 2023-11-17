#include "operations.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    runtime_error = -5,
    incorrect_command_input = -6,
    incorrect_array_input = -7
};

int is_prime(int number)
{
    for (int i = 2; i <= number / 2; ++i)
    {
        if (number % i == 0) return fail;
    }
    return success;
}

int get_remainder(char ** result, char * number, int divisor)
{
    int length = strlen(number);
    int size = get_length(divisor);
    char * res = (char*) calloc((size + 1), sizeof(char));
    if (!res) return no_memory;
    char * preres = (char*) calloc((size + 1), sizeof(char));
    if (!preres)
    {
        free(res);
        res = NULL;
        return no_memory;
    }
    char * div = (char*) calloc((size + 1), sizeof(char));
    if (!div)
    {
        free(res);
        res = NULL;
        free(preres);
        preres = NULL;
        return no_memory;
    }
    size--;
    int prev;
    while (divisor != 0)
    {
        prev = divisor;
        divisor /= 10;
        res[size] = (prev - divisor * 10) + '0';
        preres[size] = res[size];
        div[size] = res[size];
        size--;
    }
    if (comparison(number, div) < 0)
    {
        free(res);
        res = NULL;
        free(preres);
        preres = NULL;
        free(div);
        div = NULL;
        return atoi(number);
    }
    else if (comparison(number, div) == 0)
    {
        free(res);
        res = NULL;
        free(preres);
        preres = NULL;
        free(div);
        div = NULL;
        return 0;
    }
    int tmp;
    char * quotient = (char*)calloc(2, sizeof(char)); // частное
    quotient[0] = '1';
    while (comparison(number, res) >= 0)
    {
        strcpy(preres, res);
        tmp = summary_two_numbers(&quotient, "1", 10);
        if (tmp != success)
        {
            free(res);
            res = NULL;
            free(preres);
            preres = NULL;
            free(div);
            div = NULL;
            return no_memory;
        }
        tmp = multiply_numbers_with_save(&res, div, quotient);
        if (tmp != success)
        {
            free(quotient);
            quotient = NULL;
            free(preres);
            preres = NULL;
            free(div);
            div = NULL;
            return no_memory;
        }
    }
    // вычесть из number preres
    *result = difference(number, preres);
    free(res);
    res = NULL;
    free(preres);
    preres = NULL;
    free(div);
    div = NULL;
    free(quotient);
    quotient = NULL;
    if (!(*result)) return no_memory;
    return success;
}

char * difference(const char * num1, const char * num2)
{
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_length = len1 > len2 ? len1 : len2;
    int carry = 0;
    int diff_index = 0;

    char* difference = (char*)malloc((max_length + 1) * sizeof(char));
    if (!difference) return NULL;
    difference[max_length] = '\0';

    // Вычисление разности чисел в столбик
    for (int i = 0; i < max_length; ++i) 
    {
        int digit1 = (i < len1) ? num1[len1 - 1 - i] - '0' : 0;
        int digit2 = (i < len2) ? num2[len2 - 1 - i] - '0' : 0;

        int diff = digit1 - digit2 - carry;
        if (diff < 0) 
        {
            diff += 10;
            carry = 1;
        } else carry = 0;

        difference[max_length - 1 - diff_index] = diff + '0';
        diff_index++;
    }

    // Удаление ведущих нулей
    int leading_zeros = 0;
    while (difference[leading_zeros] == '0' && leading_zeros < max_length - 1) leading_zeros++;

    if (leading_zeros > 0) 
    {
        for (int i = leading_zeros; i <= max_length; i++) 
        {
            difference[i - leading_zeros] = difference[i];
        }
    }

    return difference;
}

int comparison(const char * first, const char * second)
{
    int size = strlen(second);
    int len = strlen(first);
    if (len > size) return 1;
    if (len < size) return -1;
    
    int res = strcmp(first, second);
    return res;
}

int get_first_digit(int number)
{
    int result = number;
    while (number != 0)
    {
        result = number;
        number /= 10;
    }
    return result;
}

int get_length(int number)
{
    int length = 0;
    while (number != 0)
    {
        number /= 10;
        length++;
    }
    return length;
}

int multiply_two_numbers(char ** res, char * first, char second)
{
    int length = strlen(first) + 1;
    char * result = (char*)malloc((length + 1) * sizeof(char));
    if (!result)
    {
        free(first);
        first = NULL;
        return no_memory;
    }
    for (int i = 0; i < length + 1; ++i) result[i] = '0';
    int tmp, tmp1;
    result[length] = 0;
    for (int i = length - 2, j = length - 1; i >= 0; --i, --j)
    {
        tmp = (result[j] - '0') + ((first)[i] - '0') * (second - '0');
        if (tmp > 9)
        {
            tmp1 = tmp / 10;
            result[j - 1] = tmp1 + '0';
            result[j] = (tmp - tmp1 * 10) + '0';
        } 
        else result[j] = tmp + '0';
    }
    if (result[0] == '0')
    {
        for (int i = 0; i < length; ++i) result[i] = result[i + 1];
    }
    if (*res)
    {
        free(*res);
        *res = NULL;
    }
    *res = result;
    return success;
}

// умножение в столбик 
int multiply_numbers(char ** first, char * second)
{
    if (strcmp(*first, "0") == success) return success;
    if (strcmp(second, "0") == success)
    {
        free(*first);
        *first = NULL;
        *first = (char*)calloc(2, sizeof(char));
        if (!(*first)) return no_memory;
        (*first)[0] = '0';
        return success;
    }
    char * result = (char*)malloc(2 * sizeof(char));
    if (!result)
    {
        free(*first);
        *first = NULL;
        return no_memory;
    }
    result[0] = '0';
    result[1] = 0;

    int len = strlen(second);
    int res;
    char * res1 = NULL;
    int length_res1;
    char * temp;
    int power = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        res = multiply_two_numbers(&res1, *first, second[i]);
        if (res != success)
        {
            free(result);
            result = NULL;
            return res;
        }
        for (int i = 0; i < power; ++i)
        {
            length_res1 = strlen(res1) + 1;
            temp = (char*)realloc(res1, (length_res1 + 1) * sizeof(char));
            if (!temp)
            {
                free(res1);
                res1 = NULL;
                free(result);
                result = NULL;
                free(*first);
                *first = NULL;
                return no_memory;
            }
            res1 = temp;
            res1[length_res1 - 1] = '0';
            res1[length_res1] = 0;
        }
        power++;
        res = summary_two_numbers(&result, res1, 10);
    }
    int length = strlen(result);
    while (result[0] == '0')
    {
        for (int i = 0; i < length; ++i) result[i] = result[i + 1];
    }
    free(res1);
    res1 = NULL;
    free(*first);
    *first = NULL;
    *first = result;
    return success;
}

int multiply_numbers_with_save(char ** multiply, char * first, char * second)
{
    if (strcmp(first, "0") == success) return success;
    if (strcmp(second, "0") == success)
    {
        *multiply = (char*)calloc(2, sizeof(char));
        if (!(*multiply)) return no_memory;
        (*multiply)[0] = '0';
        return success;
    }
    char * result = (char*)malloc(2 * sizeof(char));
    if (!result) return no_memory;
    result[0] = '0';
    result[1] = 0;

    int len = strlen(second);
    int res;
    char * res1 = NULL;
    int length_res1;
    char * temp;
    int power = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        res = multiply_two_numbers(&res1, first, second[i]);
        if (res != success)
        {
            free(result);
            result = NULL;
            return res;
        }
        for (int i = 0; i < power; ++i)
        {
            length_res1 = strlen(res1) + 1;
            temp = (char*)realloc(res1, (length_res1 + 1) * sizeof(char));
            if (!temp)
            {
                free(res1);
                res1 = NULL;
                free(result);
                result = NULL;
                return no_memory;
            }
            res1 = temp;
            res1[length_res1 - 1] = '0';
            res1[length_res1] = 0;
        }
        power++;
        res = summary_two_numbers(&result, res1, 10);
    }
    int length = strlen(result);
    while (result[0] == '0')
    {
        for (int i = 0; i < length; ++i) result[i] = result[i + 1];
    }
    free(res1);
    res1 = NULL;
    *multiply = result;
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