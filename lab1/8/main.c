#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 10

void input_error()
{
    printf("Please, use the format:\n");
    printf("-a input_file_name.txt output_file_name.txt\n");
    return;
}

int is_delimiter(char symbol)
{
    if (symbol == ' ' || symbol == '\n' || symbol == '\t') return 1;
    return 0;
}

int read_file(char **words, FILE* file)
{
    char temp = fgetc(file);
    if (temp == EOF) return 0; // пустой файл

    int length = 2;
    (*words)  = (char*)malloc(length * sizeof(char));
    if ((*words) == NULL) return -1;

    char *temp_1;
    int count = 0;
    int flag = 0;
    int word_count = 0;
    while (temp != EOF)
    {
        if (is_delimiter(temp) && flag == 1)
        {
            flag = 0;
            word_count++;
        }
        else if (!is_delimiter(temp)) flag = 1;
        if (length <= count)
        {
            length *= 2;
            temp_1 = (char*)realloc((*words), length * sizeof(char));
            if (temp_1 == NULL) return -1;
            *words = temp_1;
        }
        (*words)[count] = temp;
        count++;
        temp = fgetc(file);
    }
    (*words)[count] = '\0';
    if (word_count == 0) return 0;
    return word_count + 1;
}

int get_decimal_number(char* number, int base, int flag)
{
    int result = 0, power = 1, length = strlen(number), temp;
    int end = 0;
    if (flag == 1) end = 1;
    for (int i = length - 1; i >= end; --i)
    {
        if (number[i] >= '0' && number[i] <= '9') temp = number[i] - '0';
        else temp = tolower(number[i]) - 'a' + 10;

        result += temp * power;
        power *= base;
    }
    if (flag == 1) result *= -1;
    return result;
}

int get_min_base(char* number, int flag)
{
    int maximum = 0, temp;
    int length = strlen(number);
    int i = 0;
    if (flag == 1) i = 1;
    for (i; i < length; ++i)
    {
        if (number[i] >= '0' && number[i] <= '9') temp = number[i] - '0';
        else temp = tolower(number[i]) - 'a' + 10;

        if (temp > maximum) maximum = temp;
    }
    if (maximum < 1 || maximum > 35) return -1;
    return maximum + 1;
}

int check_word(char *word)
{
    if (*word == '-') word++;
    while(*word != '\0')
    {
        if (!isdigit(*word) && !isalpha(*word)) return 0;
        word++;
    }
    return 1;
}

int print_result(FILE* input, FILE* output)
{
    char *words_string = NULL;
    int length = read_file(&words_string, input);
    int flag = 0;
    if (length == 0)
    {
        free(words_string);
        printf("Incorrect file-input\n");
        fclose(input);
        fclose(output);
        return 0;
    }
    else if (length == -1)
    {
        free(words_string);
        fclose(input);
        fclose(output);
        return -1;
    }
    char *words[length];
    char *word = strtok(words_string, " \n\t");
    for (int i = 0; i < length; ++i)
    {
        words[i] = word;
        word = strtok(NULL, " \n\t");
    }

    int base, decimal_number;

    for (int i = 0; i < length; ++i)
    {
        flag = 0;
        if (!check_word(words[i])) return 0;
        if (words[i][0] == '-') flag = 1;
        base = get_min_base(words[i], flag);
        decimal_number = get_decimal_number(words[i], base, flag);
        fprintf(output, "%s %d %d\n", words[i], base, decimal_number);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Input error! Too many or too low arguments.\n");
        input_error();
        return 0;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (!input || !output)
    {
        printf("File Error!\n");
        return 0;
    }
    int result = print_result(input, output);
    fclose(input);
    fclose(output);
    if (result == 0)
    {
        printf("Error! File-input is incorrect.");
        return 0;
    }
    else if (result == -1)
    {
        printf("Memory error!\n");
        return 0;
    }
    printf("Program has finished! Check the file.");
    return 0;
}