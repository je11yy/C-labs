#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    incorrect_console_input = -2,
    one_element_array = -3,
    no_memory = -4
};

void print_error(int error);

int is_digit(char *number);

int valid_check(char* min, char* max);

int check_valid_size(char* size);

void print_menu();

void make_random_array_1(int array[], int size, int a, int b);

void print_array(int array[], int size);

int change_min_and_max(int array[], int size);

int make_arrays(int **a, int **b, int *length_1, int *length_2);

int make_c_array(int **c, int *a, int *b, int size, int size_b);

int search_for_element(int element, int *array, int size);

int get_string(char **new_string);

///////////////////////////////////////////////

int main(int argc, char *argv[])
{
    if (argc != 3) return 0;
    int result = valid_check(argv[1], argv[2]);
    if (result == incorrect_input)
    {
        print_error(result);
        return incorrect_input;
    }
    
    print_menu();
    char *size_string;
    result = get_string(&size_string);
    if (result == no_memory)
    {
        free(size_string);
        size_string = NULL;
        print_error(result);
        return no_memory;
    }
    result = check_valid_size(size_string);
    while (result == incorrect_console_input)
    {
        free(size_string);
        size_string = NULL;
        print_error(incorrect_console_input);
        print_menu();
        result = get_string(&size_string);
        if (result == no_memory)
        {
            free(size_string);
            size_string = NULL;
            print_error(result);
            return no_memory;
        }
        result = check_valid_size(size_string);
        if (!result) break;
    }
    printf("\n");

    srand(time(NULL));

    int a = atoi(argv[1]), b = atoi(argv[2]);
    int size = atoi(size_string);
    int array[size]; // заменить на маллок

    // первая часть
    printf("\n____________________________________\n");
    printf("\tTASK ONE\n");
    make_random_array_1(array, size, a, b);
    printf("Array with random numbers from %d to %d with size %d:\n", a, b, size);
    print_array(array, size);
    result = change_min_and_max(array, size);
    if (result == one_element_array) print_error(result);
    printf("Array after changing minimum and maximum:\n");
    print_array(array, size);

    // вторая часть
    printf("\n____________________________________\n");
    printf("\tTASK TWO\n");

    int *array_a = NULL;
    int *array_b = NULL;
    int *array_c = NULL;
    int size_a, size_b, size_c;
    result = make_arrays(&array_a, &array_b, &size_a, &size_b);
    if (result == no_memory) print_error(result);
    else
    {
        printf("\nRandom-sized with random number array A (FIRST 15 ELEMENTS):\n");
        print_array(array_a, 15);
        printf("\nRandom-sized with random number array B (FIRST 15 ELEMENTS):\n");
        print_array(array_b, 15);

        result = make_c_array(&array_c, array_a, array_b, size_a, size_b);

        if (result == no_memory) print_error(result);
        else
        {
            printf("\nArray C:\n");
            print_array(array_c, size_a);
        }
    }

    free(array_a);
    array_a = NULL;
    free(array_b);
    array_b = NULL;
    free(array_c);
    array_c = NULL;
    printf("\n____________________________________\n");
    printf("\nThe program has finished.\n");
    printf("\n____________________________________\n");
    return 0;
}

//////////////////////////////////////////////

int search_for_element(int element, int *array, int size)
{
    int closest = array[0];
    int difference = abs(closest - element);
    int current_difference;

    for (int i = 1; i < size; ++i)
    {
        current_difference = abs(array[i] - element);
        if (current_difference < difference)
        {
            closest = array[i];
            difference = current_difference;
        }
    }

    return closest;
}

int make_c_array(int **c, int *a, int *b, int size, int size_b)
{
    *c = malloc(size * sizeof(int));
    if (*c == NULL) return no_memory;

    for (int i = 0; i < size; ++i)
    {
        (*c)[i] = a[i] + search_for_element(a[i], b, size_b);
    }
    return success;
}

int make_arrays(int **a, int **b, int *size_a, int *size_b)
{
    int a_min = 10, b_min = 10000;
    int min = -1000, max = 1000;
    *size_a = (rand() % (b_min - a_min + 1)) + a_min;
    *size_b = (rand() % (b_min - a_min + 1)) + a_min;

    *a = malloc(*size_a * sizeof(int));
    if (*a == NULL) return no_memory;

    *b = malloc(*size_b * sizeof(int));
    if (*b == NULL)
    {
        free(*a);
        return no_memory;
    }

    for (int i = 0; i < *size_a; ++i)
    {
        (*a)[i] = (rand() % (max - min + 1)) + min;
    }

    for (int i = 0; i < *size_b; ++i)
    {
        (*b)[i] = (rand() % (max - min + 1)) + min;
    }
    return success;
}

int change_min_and_max(int array[], int size)
{
    if (size == 1) return one_element_array;
    int min = array[0];
    int min_index = 0;
    int max = array[0];
    int max_index = 0;

    for (int i = 1; i < size; ++i)
    {
        if (array[i] < min)
        {
            min = array[i];
            min_index = i;
        }
        if (array[i] > max)
        {
            max = array[i];
            max_index = i;
        }
    }
    array[min_index] = max;
    array[max_index] = min;
    return success;
}

void print_array(int array[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", array[i]);
        if ((i % 20 == 0) && i != 0) printf("\n");
    }
    printf("\n");
    return;
}

void make_random_array_1(int array[], int size, int a, int b)
{
    // формула получения псевдослучайного числа в диапазоне от a до b
    int temp = (rand() % (b - a + 1)) + a;

    for (int i = 0; i < size; ++i)
    {
        array[i] = temp;
        temp = (rand() % (b - a + 1)) + a;
    }
    return;
}

int check_valid_size(char* size)
{
    int length = strlen(size);
    int int_length = 11;
    if (length > int_length) return incorrect_console_input;
    if (length < 1) return incorrect_console_input;

    if (!is_digit(size)) return incorrect_console_input;

    if (atoi(size) <= 0) return incorrect_console_input;

    return success;
}

void print_menu()
{
    printf("____________________________________");
    printf("\n\tMENU\n");
    printf("Please, input the size of array: ");
}

int is_digit(char *number)
{
    if (!strcmp(number, "-")) return 0;
    if (*number == '-') number++;
    while (*number != '\0')
    {
        if (!(*number >= '0' && *number <= '9')) return 0;
        number++;
    }
    return 1;
}

int valid_check(char* min, char* max)
{
    int length_min = strlen(min), length_max = strlen(max);
    int int_length = 11;

    if (length_min > int_length || length_min < 1) return incorrect_input;
    if (length_max > int_length || length_max < 1) return incorrect_input;

    if (!is_digit(min)) return incorrect_input;
    if (!is_digit(max)) return incorrect_input;

    if (atoi(min) > atoi(max)) return incorrect_input;

    return success;
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


int get_string(char **new_string)
{
    int length = 2;
    char *string = (char *)malloc(length * sizeof(char));
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