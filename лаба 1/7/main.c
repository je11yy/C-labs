#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int is_flag(char *flag)
{
    if (!(flag[0] == '/' || flag[0] == '-')) return 0;
    if (!(flag[1] == 'r' || flag[1] == 'a')) return 0;
    return 1;
}

void input_error()
{
    printf("Please, use the format:\n");
    printf("-r name1.txt name2.txt output_file_name.txt\n");
    printf("    or\n-a input_file_name.txt output_file_name.txt\n");
    return;
}

int is_delimiter(char symbol)
{
    if (symbol == ' ' || symbol == '\n' || symbol == '\t') return 1;
    return 0;
}

int read_file(FILE* file, char **words)
{
    char temp = fgetc(file);
    if (temp == EOF) return 0; // пустой файл

    int length = 2;
    char *temp_1 = (char*)malloc(length * sizeof(char));
    if (temp_1 == NULL) return 0;
    (*words) = temp_1;
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
            if (temp_1 == NULL) return 0;
            *words = temp_1;
        }
        (*words)[count] = temp;
        count++;
        temp = fgetc(file);
    }
    (*words)[count] = '\0';
    return word_count + 1;
}

void print_result(FILE* file, char *words_1, char *words_2, int words_count_1, int words_count_2)
{
    int max_count;
    if (words_count_1 >= words_count_2) max_count = words_count_1;
    else max_count = words_count_2;
    char *words_array_1[words_count_1];

    char *word_1 = strtok(words_1, " \n\t");
    for (int i = 0; i < words_count_1; ++i)
    {
        words_array_1[i] = word_1;
        word_1 = strtok(NULL, " \n\t");
    }

    char *words_array_2[words_count_2];
    char *word_2 = strtok(words_2, " \n\t");
    for (int i = 0; i <= words_count_2; ++i)
    {
        words_array_2[i] = word_2;
        word_2 = strtok(NULL, " \n\t");
    }

    for (int i = 0; i < max_count; ++i)
    {
        if (i < words_count_1)
        {
            fprintf(file, "%s ", words_array_1[i]);
        }
        if (i < words_count_2)
        {
            fprintf(file, "%s ", words_array_2[i]);
        }
    }
    return;
}

int flag_r(FILE* input_1, FILE* input_2, FILE* output)
{
    char *words_1 = NULL;
    char *words_2 = NULL;

    int result_1 = read_file(input_1, &words_1);
    if (!result_1)
    {
        free(words_1);
        return 0;
    }

    int result_2 = read_file(input_2, &words_2);
    if (!result_2)
    {
        free(words_2);
        return 0;
    }
    print_result(output, words_1, words_2, result_1, result_2);

    free(words_1);
    free(words_2);

    return 1;
}

void make_lower(char word[])
{
    while (*word != '\0')
    {
        if (isalpha(*word)) *word = tolower(*word);
        word++;
    }
    return;
}

int base_4(int number)
{
    int number_4 = 0;
    int mult = 1;

    while (number > 0)
    {
        number_4 += (number % 4) * mult;
        number /= 4;
        mult *= 10;
    }
    return number_4;
}

int flag_a(FILE* input, FILE* output)
{
    char *words = NULL;

    int length = read_file(input, &words);

    if (!length) return 0;

    char *words_array[length];
    char *word = strtok(words, " \n\t");
    for (int i = 0; i < length; ++i)
    {
        words_array[i] = word;
        word = strtok(NULL, " \n\t");
    }

    for (int i = 0; i < length; ++i)
    {
        if ((i + 1) % 10 == 0)
        {
            make_lower(words_array[i]);
            for (int j = 0; words_array[i][j] != '\0'; j++)
            {
                fprintf(output, "%d", base_4(words_array[i][j]));
            }
            fputc(' ', output);
        }
        else if ((i + 1) % 2 == 0)
        {
            make_lower(words_array[i]);
            fprintf(output, "%s ", words_array[i]);
        }
        else if ((i + 1) % 5 == 0)
        {
            for (int j = 0; words_array[i][j] != '\0'; j++)
            {
                fprintf(output, "%o", words_array[i][j]);
            }
            fputc(' ', output);
        }
        else fprintf(output, "%s ", words_array[i]);
    }
    free(words);
}

int main(int argc, char *argv[])
{

    if (!(argc == 5 || argc == 4))
    {
        printf("Too many or too low arguments! ");
        input_error();
        return 0;
    }
    int result;

    FILE *input_1;
    FILE *input_2;
    FILE *output;
    char *input_file_1;
    char *input_file_2;
    char *output_file;
    if (argc == 5)
    {
        result = is_flag(argv[1]);
        if (!result)
        {
            printf("Incorrect input! ");
            input_error();
            return 0;
        }
        input_file_1 = argv[2];
        input_file_2 = argv[3];
        output_file = argv[4];
    }
    else
    {
        result = is_flag(argv[1]);
        if (!result)
        {
            printf("Incorrect input! ");
            input_error();
            return 0;
        }
        input_file_1 = argv[2];
        output_file = argv[3];
    }
    char flag = argv[1][1];
    switch(flag)
    {
        case 'r':
            input_1 = fopen(input_file_1, "r");
            input_2 = fopen(input_file_2, "r");
            output = fopen(output_file, "w");
            if(!input_1 || !input_2 || !output)
            {
                printf("File Error!");
                return 0;
            }
            result = flag_r(input_1, input_2, output);
            if (!result)
            {
                printf("File Error!\n");
                fclose(input_1);
                fclose(input_2);
                fclose(output);
                return 0;
            }
            printf("The program has finished. Check the file.");
            fclose(input_1);
            fclose(input_2);
            fclose(output);
            return 0;

        case 'a':
            input_1 = fopen(input_file_1, "r");
            output = fopen(output_file, "w");
            if(!input_1 || !output)
            {
                printf("File Error!");
                return 0;
            }
            result = flag_a(input_1, output);
            if (!result)
            {
                printf("File Error!\n");
                fclose(input_1);
                fclose(output);
                return 0;
            }
            printf("The program has finished. Check the file.");
            fclose(input_1);
            fclose(output);
            return 0;
    }
}