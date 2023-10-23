#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_error = -2,
    no_memory = -3,
    fail = -4
};

int find_substring_in_files(char *substring, char **answer, int quantity, ...);
int append_to_answer(char **answer, int length, int file_count, int **result, int count);
int append_string(char **answer, int *length, char *string);
int find_substring(int ***result, char *substring, FILE* file);
int check_string(char *substring, char *string);
int change_buffer(char buffer[], FILE *file, int length);
void print_error(int error);

int main()
{
    char *substring = "111";
    char *answer = NULL;
    int result = find_substring_in_files(substring, &answer, 3, "test.txt", "test2.txt", "test3.txt");
    if (result == no_memory)
    {
        print_error(result);
        return result;
    }
    if (result == file_error)
    {
        print_error(result);
        return result;
    }

    printf("%s\n", answer);
    free(answer);
    answer = NULL;

    printf("\nThe program has finished correctly.\n");
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
        case fail:
            printf("Can't find any equal string in this file.\n");
            break;
    }
}

// quantity - количество файлов
int find_substring_in_files(char *substring, char **answer, int quantity, ...)
{
    va_list files;
    va_start(files, quantity);

    int length = 0;

    FILE* file;
    int **result = NULL;
    int find;

    char *name;
    for (int i = 0; i < quantity; ++i)
    {
        name = va_arg(files, char*);
        file = fopen(name, "r");
        if (!file)
        {
            fclose(file);
            return file_error;
        }
        find = find_substring(&result, substring, file);
        if (find == no_memory) return no_memory;
        length = append_to_answer(answer, length, i + 1, result, find);

        fclose(file);
        for (int i = 0; i < find; ++i)
        {
            free(result[i]);
            result[i] = NULL;
        }
        free(result);
        result = NULL;
    }
    va_end(files);
    return success;
}

int append_to_answer(char **answer, int length, int file_count, int **result, int count)
{
    int max_size = 100;
    int res = append_string(answer, &length, "\nAnswer for file ");
    char file_number[max_size];
    char result_1[max_size];
    char result_2[max_size];
    itoa(file_count, file_number, 10);

    res = append_string(answer, &length, file_number);
    if (res == no_memory) return no_memory;
    res = append_string(answer, &length, ": ");
    if (res == no_memory) return no_memory;\
    if (count == fail)
    {
        res = append_string(answer, &length, "Can't find substring in this file.\n");
        if (res == no_memory) return no_memory;
    }
    for (int i = 0; i < count; ++i)
    {
        itoa(result[i][0], result_1, 10);
        itoa(result[i][1], result_2, 10);

        res = append_string(answer, &length, "[ ");
        if (res == no_memory) return no_memory;
        res = append_string(answer, &length, result_1);
        if (res == no_memory) return no_memory;
        res = append_string(answer, &length, ", ");
        if (res == no_memory) return no_memory;
        res = append_string(answer, &length, result_2);
        if (res == no_memory) return no_memory;
        res = append_string(answer, &length, " ] ");
        if (res == no_memory) return no_memory;
    }
    return length;
}

int append_string(char **answer, int *length, char *string)
{
    int len = strlen(string);
    char *temp = (char*)realloc(*answer, (*length + len + 1) * sizeof(char));
    if (temp == NULL)
    {
        free(*answer);
        *answer = NULL;
        return no_memory;
    }
    *answer = temp;

    for (int i = *length, j = 0; i < *length + len, j < len; ++i, ++j)
    {
        (*answer)[i] = string[j];
    }
    (*answer)[*length + len] = '\0';
    *length += len;
    return success;
}

int find_substring(int ***new_result, char *substring, FILE* file)
{
    int len = 1, size = 2;
    int **result = (int**)malloc(sizeof(int**) * len);
    if (result == NULL) return no_memory;
    result[0] = (int*)calloc(size, sizeof(int));
    if (result[0] == NULL)
    {
        free(result);
        return no_memory;
    }

    int length = strlen(substring);
    char buffer[length];
    char symbol;
    int line_count = 1;
    int symbol_count = 1;
    for (int i = 0; i < length; ++i)
    {
        if ((symbol = fgetc(file)) == EOF)
        {
            free(result[0]);
            result[0] = NULL;
            free(result);
            result = NULL;
            return fail;
        }
        buffer[i] = symbol;
    }
    int count = 0;
    int check = check_string(substring, buffer);
    if (check == success)
    {
        result[count][0] = line_count;
        result[count][1] = symbol_count;
        count++;
    }
    int buf_check = change_buffer(buffer, file, length);
    symbol_count++;
    int **temp;
    while (buf_check != fail)
    {
        if (buffer[0] == '\n')
        {
            line_count++;
            symbol_count = 0;
        }
        if (len <= count)
        {
            len++;
            temp = (int**)realloc(result, len * sizeof(int**));
            if (temp == NULL)
            {
                for (int i = 0; i < len; ++i)
                {
                    free(result[i]);
                    result[i] = NULL;
                }
                free(result);
                result = NULL;
                return no_memory;
            }
            result = temp;
            result[count] = (int*)calloc(size, sizeof(int));
        }
        check = check_string(substring, buffer);
        if (check == success)
        {
            result[count][0] = line_count;
            result[count][1] = symbol_count;
            count++;
        }
        buf_check = change_buffer(buffer, file, length);
        symbol_count++;
    }
    *new_result = result;
    return count;
}

int check_string(char *substring, char *string)
{
    while (*substring != '\0')
    {
        if (*substring != *string) return fail;
        substring++;
        string++;
    }
    return success;
}

int change_buffer(char buffer[], FILE *file, int length)
{
    for (int i = 0; i < length - 1; ++i)
    {
        buffer[i] = buffer[i + 1];
    }
    char symbol = fgetc(file);
    if (symbol == EOF) return fail;
    buffer[length - 1] = symbol;
    return success;
}