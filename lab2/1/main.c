#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_error = -2,
    no_memory = -3,
    file_input_error = -4
};

int check_valid_flag(char *flag);

void print_error(int error);

int function_c(char **result, char *prev_string, char* arguments[], int size, unsigned int seed);

int is_number(char *number);

int reversed(char **new_string, char *string);

int function_u(char **new_string, char *string);

int function_n(char **new_string, char *string);

// замена strlen мда :)
int get_size(const char *string);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    int result = check_valid_flag(argv[1]);
    if (result == incorrect_input)
    {
        print_error(result);
        return result;
    }
    char flag = argv[1][1];
    char *string = argv[2];

    if (flag == 'c')
    {
        if (argc < 5)
        {
            print_error(incorrect_input);
            return incorrect_input;
        }
    }
    else
    {
        if (argc != 3)
        {
            print_error(incorrect_input);
            return incorrect_input;
        }
    }

    //////////////////////////
    char *seed_string;
    int seed;
    char *new_string;
    //////////////////////////

    switch(flag)
    {
        case 'l':
            result = get_size(string);
            printf("Result for flag -l: %d\n", result);
            break;
        case 'r':
            new_string = NULL;
            result = reversed(&new_string, string);
            if (result == no_memory)
            {
                print_error(result);
                return result;
            }
            printf("Result for flag -r: %s\n", new_string);
            free(new_string);
            new_string = NULL;
            break;
        case 'u':
            new_string = NULL;
            result = function_u(&new_string, string);
            if (result == no_memory)
            {
                print_error(result);
                return result;
            }
            printf("Result for flag -u: %s\n", new_string);
            free(new_string);
            new_string = NULL;
            break;
        case 'n':
            new_string = NULL;
            result = function_n(&new_string, string);
            if (result == no_memory)
            {
                print_error(result);
                return result;
            }
            printf("Result for flag -n: %s\n", new_string);
            free(new_string);
            new_string = NULL;
            break;
        case 'c':
            seed_string = argv[3];
            if (is_number(seed_string) == incorrect_input)
            {
                print_error(incorrect_input);
                return incorrect_input;
            }
            seed = atoi(seed_string);
            new_string = NULL;
            result = function_c(&new_string, string, argv + 4, argc - 4, seed);
            if (result == no_memory)
            {
                print_error(result);
                return result;
            }
            printf("\nResult for flag -c: %s\n", new_string);
            free(new_string);
            new_string = NULL;
            break;
    }

    printf("\nProgram has finished correctly.\n");
    return success;
}

void combine(char **string, char *comb_string, int index, int length)
{
    for (int i = index, j = 0; i < length + index, j < length; ++i, ++j)
    {
        (*string)[i] = comb_string[j];
    }
    return;
}

int function_n(char **new_string, char *string)
{
    int length = get_size(string);
    char *didgits = (char*)malloc((length + 1) * sizeof(char));
    int didgits_index = 0;
    if (didgits == NULL) return no_memory;
    char *alphas = (char*)malloc((length + 1) * sizeof(char));
    int alphas_index = 0;
    if (alphas == NULL)
    {
        free(didgits);
        didgits = NULL;
        return no_memory;
    }
    char *other = (char*)malloc((length + 1) * sizeof(char));
    int other_index = 0;
    if (other == NULL)
    {
        free(didgits);
        didgits = NULL;
        free(alphas);
        alphas = NULL;
        return no_memory;
    }
    *new_string = (char*)malloc((length + 1) * sizeof(char));
    if (*new_string == NULL)
    {
        free(didgits);
        didgits = NULL;
        free(alphas);
        alphas = NULL;
        free(other);
        other = NULL;
        return no_memory;
    }

    for (int i = 0; i < length; ++i)
    {
        if (string[i] >= '0' && string[i] <= '9')
        {
            didgits[didgits_index] = string[i];
            didgits_index++;
        }
        else if ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= 'A' && string[i] <= 'Z'))
        {
            alphas[alphas_index] = string[i];
            alphas_index++;
        }
        else
        {
            other[other_index] = string[i];
            other_index++;
        }
    }
    int index = 0;
    combine(new_string, didgits, index, didgits_index);
    index += didgits_index;
    combine(new_string, alphas, index, alphas_index);
    index += alphas_index;
    combine(new_string, other, index, other_index);
    index += other_index + 1;
    (*new_string)[index] = '\0';

    free(didgits);
    didgits = NULL;
    free(alphas);
    alphas = NULL;
    free(other);
    other = NULL;
    return success;
}

int function_u(char **new_string, char *string)
{
    int length = get_size(string);
    *new_string = (char*)malloc((length + 1) * sizeof(char));
    if (*new_string == NULL) return no_memory;

    for (int i = 0; i < length; ++i)
    {
        if (i % 2 != 0)
        {
            if (string[i] >= 'a' && string[i] <= 'z')
            {
                (*new_string)[i] = string[i] - 32;
            }
        }
        else (*new_string)[i] = string[i];
    }
    (*new_string)[length] = '\0';
    return success;
}

int reversed(char **new_string, char *string)
{
    int length = get_size(string);

    *new_string = (char*)malloc((length + 1) * sizeof(char));
    if (*new_string == NULL) return no_memory;
    (*new_string)[length] = '\0';

    for (int i = 0; i <= length / 2; ++i)
    {
        (*new_string)[i] = string[length - i - 1];
        (*new_string)[length - i - 1] = string[i];
    }
    return success;
}

int is_number(char *number)
{
    while (*number != '\0')
    {
        if (!(*number >= '0' && *number <= '9')) return incorrect_input;
        number++;
    }
    return success;
}

int get_size(const char *string)
{
    char const *start = string;
    while (*string != '\0')
    {
        string++;
    }
    return string - start;
}

//конкатенация - склеивание строк
int function_c(char **result, char *prev_string, char* prev_arguments[], int size, unsigned int seed)
{
    srand(seed);
    int length = 0;
    int new_length = 0;
    int string_length;
    *result = (char*)malloc(sizeof(char) * length);
    if (*result == NULL) return no_memory;

    char** arguments = (char**)malloc(size * sizeof(char*) + 1);
    if (arguments == NULL) return no_memory;

    arguments[0] = prev_string;
    for (int i = 1; i <= size; ++i)
    {
        arguments[i] = prev_arguments[i - 1];
    }
    size++;

    char *string;
    char *temp;
    int a = 0, b = size - 1;

    int index;
    // алгоритм Фишера-Йетса, чтобы числа не повторялись. 
    // Таким образом, перемешиваем псевдослучайным образом массив
    for (int i = 0; i < size - 1; ++i)
    {
        index = rand() % (size - i) + i;
        temp = arguments[index];
        arguments[index] = arguments[i];
        arguments[i] = temp;
    }
    for (int i = 0; i < size; ++i)
    {
        string_length = get_size(arguments[i]);
        new_length = length + string_length;
        temp = (char*)realloc(*result, sizeof(char) * (new_length + 1));
        if (temp == NULL)
        {
            free(*result);
            *result = NULL;
            return no_memory;
        }
        *result = temp;
        for (int j = length, k = 0; j < new_length, k < string_length; ++j, ++k)
        {
            (*result)[j] = arguments[i][k];
        }
        (*result)[new_length] = '\0';
        length = new_length;
    }
    free(arguments);
    return success;
}

int check_valid_flag(char *flag)
{
    if (get_size(flag) != 2) return 0;

    char flags[] = {'l', 'r', 'u', 'n', 'c', '\0'};

    if (flag[0] != '-') return 0;

    int length = get_size(flags);
    for (int i = 0; i < length; ++i)
    {
        if (flag[1] == flags[i]) return success;
    }

    return incorrect_input;
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