#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "arrays.h"
#include "functions.h"

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

int print_all(Arrays_ptr arrays, char name)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if ((arrays -> arrays)[index] -> size == 0)
    {
        printf("Array %c is empty.\n\n", name);
        return success;
    }
    printf("Elements of array %c: ", name);
    for (int i = 0; i < (arrays -> arrays)[index] -> size; ++i)
    {
        printf("%d ", ((arrays -> arrays)[index] -> array)[i]);
    }
    printf("\n\n");
    return success;
}

int print_diap(Arrays_ptr arrays, char name, int pos_l, int pos_r)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if ((arrays -> arrays)[index] -> size == 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if (pos_l < 0) 
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (pos_r < pos_l)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if ((arrays -> arrays)[index] -> size < pos_r)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    printf("Elements of array %c from %d element to %d element: ", name, pos_l, pos_r);
    for (int i = pos_l - 1; i < pos_r; ++i)
    {
        printf("%d ", ((arrays -> arrays)[index] -> array)[i]);
    }
    printf("\n\n");
    return success;
}

int print_element(Arrays_ptr arrays, char name, int position)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if ((arrays -> arrays)[index] -> size < position)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    printf("Element on %d position in array %c: %d\n\n", position, name, ((arrays -> arrays)[index] -> array)[position - 1]);
    return success;
}

int print_states(Arrays_ptr arrays, char name)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }

    printf("Stats of array %c\n\n", name);
    if ((arrays -> arrays)[index] -> size == 0)
    {
        printf("Size: %d\n\n", 0);
        return success;
    }
    printf("Size: %d\n", (arrays -> arrays)[index] -> size);

    // минимальные и максимальные значения, сумма для среднего значения
    int min = ((arrays -> arrays)[index] -> array)[0], max = 0, summary = 0;

    for (int i = 0; i < (arrays -> arrays)[index] -> size; ++i)
    {
        if (((arrays -> arrays)[index] -> array)[i] < min) min = ((arrays -> arrays)[index] -> array)[i];
        if (((arrays -> arrays)[index] -> array)[i] > max) max = ((arrays -> arrays)[index] -> array)[i];
        summary += ((arrays -> arrays)[index] -> array)[i];
    }

    // среднее значение
    double average = summary / (double) (arrays -> arrays)[index] -> size;

    // наиболее часто встреющееся
    int max_count = 0, cur_count;
    int most_common;
    for (int i = 0; i < (arrays -> arrays)[index] -> size; ++i)
    {
        cur_count = 1;
        for (int j = i + 1; j < (arrays -> arrays)[index] -> size; ++j)
        {
            if (((arrays -> arrays)[index] -> array)[i] == ((arrays -> arrays)[index] -> array)[j]) cur_count++;
        }
        if (cur_count > max_count)
        {
            max_count = cur_count;
            most_common = ((arrays -> arrays)[index] -> array)[i];
        }
        else if (cur_count == max_count && most_common < ((arrays -> arrays)[index] -> array)[i])
        {
            most_common = ((arrays -> arrays)[index] -> array)[i];
        }
    }

    // отклонение от среднего
    double max_deviation = 0;
    double deviation;
    double eps = 0.00000000001;
    int element;
    for (int i = 0; i < (arrays -> arrays)[index] -> size; ++i)
    {
        element = ((arrays -> arrays)[index] -> array)[i];
        deviation = fabs(element - average);
        if (deviation - max_deviation > eps) max_deviation = deviation;
    }

    printf("Max element: %d\n", max);
    printf("Min element: %d\n", min);
    printf("Most common element: %d\n", most_common);
    printf("Average of elements: %lf\n", average);
    printf("Max deviation from average: %lf\n\n", max_deviation);
    return success;
}

int validate_name(char name)
{
    if (!(tolower(name) >= 'a' && tolower(name) <= 'b')) return fail;
    return success;
}

int shuffle_array(Arrays_ptr arrays, char name)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    
    srand(time(NULL));
    qsort((arrays -> arrays)[index] -> array, (arrays -> arrays)[index] -> size, sizeof(int), (int(*) (const void *, const void *)) shuffle_compare);
    return success;
}

int shuffle_compare(const int * first, const int * second)
{
    return rand() % 3 - 1;
}

int sort_array(Arrays_ptr arrays, char name, char flag)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (flag != '+' && flag != '-')
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }

    switch (flag)
    {
        case '+':
            qsort((arrays -> arrays)[index] -> array, (arrays -> arrays)[index] -> size, sizeof(int), (int(*) (const void *, const void *)) comp_increase);
            break;
        case '-':
            qsort((arrays -> arrays)[index] -> array, (arrays -> arrays)[index] -> size, sizeof(int), (int(*) (const void *, const void *)) comp_decrease);
            break;
        default: break;
    }
    return success;
}

void print_array(Array_ptr array)
{
    for (int i = 0; i < array -> size; ++i)
    {
        printf("%d ", (array -> array)[i]);
    }
    printf("\n");
}

int comp_decrease(const int * first, const int * second)
{
    if (*first > *second) return -1;
    if (*first < *second) return 1;
    return 0;
}

int comp_increase(const int * first, const int * second)
{
    if (*first > *second) return 1;
    if (*first < *second) return -1;
    return 0;
}

// копирования элементов с first элемента до second элемента (включительно) из А в Б
// индексы. первый: first - 1, второй: second - 1
// если массива Б не существует - создать
int copy_arrays(Arrays_ptr arrays, char name_a, int first, int second, char name_b)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name_a) != success || validate_name(name_b) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (first < 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (second < first)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index_a = array_exists(arrays, name_a);
    int index_b = array_exists(arrays, name_b);
    if (index_a < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if ((arrays -> arrays)[index_a] -> size < second)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if (index_b != -1) delete_array((arrays -> arrays)[index_b]);
    Array_ptr new_array = create_array(name_b);
    if (!new_array)
    {
        delete_arrays(arrays);
        return no_memory;
    }
    int res;
    for (int i = first - 1; i < second; ++i)
    {
        res = append_element(&new_array, ((arrays -> arrays)[index_a] -> array)[i]);
        if (res != success)
        {
            delete_arrays(arrays);
            return no_memory;
        }
    }
    (arrays -> arrays)[index_b] = new_array;
    return success;
}

// удаления quantity элементов, начиная с first_ind
int remove_elements(Arrays_ptr arrays, char name, int first_ind, int quantity)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    if ((arrays -> arrays)[index] -> size < (first_ind + quantity)) 
    {
        delete_arrays(arrays);
        return runtime_error;
    }

    if ((arrays -> arrays)[index] -> size == (first_ind + quantity))
    {
        free_array(arrays, name);
        return success;
    }

    for (int i = first_ind, j = first_ind + quantity; i < (arrays -> arrays)[index] -> size - quantity; ++i, ++j)
    {
        ((arrays -> arrays)[index] -> array)[i] = ((arrays -> arrays)[index] -> array)[j];
    }
    (arrays -> arrays)[index] -> size = (arrays -> arrays)[index] -> size - quantity;
    int * tmp = (int*)realloc((arrays -> arrays)[index] -> array, (arrays -> arrays)[index] -> size);
    if (!tmp)
    {
        delete_arrays(arrays);
        return no_memory;
    }
    (arrays -> arrays)[index] -> array = tmp;
    return success;
}

int free_array(Arrays_ptr arrays, char name)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index = array_exists(arrays, name);
    if (index < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    free((arrays -> arrays)[index] -> array);
    (arrays -> arrays)[index] -> array = NULL;
    (arrays -> arrays)[index] -> size = 0;
    return success;
}

// заполнение псевдослучайными числами в количестве count штук
// lb - lr <- диапазон псевдослучайных чисел
int randomize(Arrays_ptr * arrays, char name, int count, int lb, int lr)
{
    if (validate_name(name) != success)
    {
        delete_arrays(*arrays);
        return incorrect_command_input;
    }
    int index = array_exists(*arrays, name);
    if (index != -1) delete_array(((*arrays) -> arrays)[index]);

    Array_ptr new_array = create_array(name);
    int element;
    srand(time(NULL));
    int res;
    for (int i = 0; i < count; ++i)
    {
        element = rand() % (lr - lb + 1) + lb;
        res = append_element(&new_array, element);
        if (res != success)
        {
            delete_arrays(*arrays);
            return res;
        }
    }
    if (index < 0)
    {
        res = append_array(arrays, new_array);
        if (res != success) return res;
    }
    else ((*arrays) -> arrays)[index] = new_array;
    return success;
}

// распечатка массива в файл
int save(Arrays_ptr arrays, char name, char * file_name)
{
    if (arrays -> quantity == 0)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    FILE * file = fopen(file_name, "w");
    if (!file)
    {
        delete_arrays(arrays);
        return file_open_error;
    }

    int index = array_exists(arrays, name);
    if (index == -1)
    {
        fclose(file);
        delete_arrays(arrays);
        return runtime_error;
    }
    if ((arrays -> arrays)[index] -> size > 0)
    {
        for (int i = 0; i < (arrays -> arrays)[index] -> size; ++i)
        {
            fprintf(file, "%d\n", ((arrays -> arrays)[index] -> array)[i]);
        }
    }
    else fprintf(file, "Array is empty.\n");
    fclose(file);
    return success;
}

// выгрузка из файла в массив
/*
Если массива еще нет:
Создать массив array_name
По очереди добавить элементы из файла
Добавить массив в структуру для хранения массивов

Если массив есть:
Удалить прошлый массив
Аналогично для первого случая добавить элементы
Поставить массив на прежнее место
*/
int load(Arrays_ptr * arrays, char array_name, char * file_name)
{
    // валидация:
    if (validate_name(array_name) != success)
    {
        delete_arrays(*arrays);
        return incorrect_command_input;
    }
    FILE * file = fopen(file_name, "r");
    if (!file)
    {
        delete_arrays(*arrays);
        return file_open_error;
    }
    //проверка, существует ли массив. если да, возвращает индекс, если нет, то fail
    int index = array_exists(*arrays, array_name);
    if (index != fail)
    {
        delete_array(((*arrays) -> arrays)[index]);
    }
    Array_ptr new_array = create_array(array_name);
    if (!new_array)
    {
        delete_arrays(*arrays);
        return no_memory;
    }
    int element;
    int result, res;
    while ((result = fscanf(file, "%d", &element)) == 1)
    {
        res = append_element(&new_array, element);
        if (res != success)
        {
            delete_arrays(*arrays);
            return res;
        }
    }
    fclose(file);
    if (result != EOF)
    {
        delete_arrays(*arrays);
        delete_array(new_array);
        return incorrect_array_input;
    }
    switch (index)
    {
        case -1:
            res = append_array(arrays, new_array);
            if (res != success) return res;
            break;
    
        default:
            ((*arrays) -> arrays)[index] = new_array;
            break;
    }
    return success;
}

// конкатенация
/*
Результат сохраняется в первый массив
Если массивов не существует, то возвращается ошибка
*/
int concatenation(Arrays_ptr arrays, char name_a, char name_b)
{
    if (arrays -> quantity < 2)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    if (validate_name(name_a) != success || validate_name(name_b) != success)
    {
        delete_arrays(arrays);
        return incorrect_command_input;
    }
    int index_a = array_exists(arrays, name_a);
    if (index_a < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    int index_b = array_exists(arrays, name_b);
    if (index_b < 0)
    {
        delete_arrays(arrays);
        return runtime_error;
    }
    int size_a = (arrays -> arrays)[index_a] -> size;
    int size_b = (arrays -> arrays)[index_b] -> size;
    int result;
    for (int i = 0; i < size_b; ++i)
    {
        result = append_element(&((arrays -> arrays)[index_a]), ((arrays -> arrays)[index_b] -> array)[i]);
        if (result != success)
        {
            delete_arrays(arrays);
            return result;
        }
    }
    return success;
}