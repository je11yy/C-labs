#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 256

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

int read_file(char words[][N], FILE* file)
{
    int count = 0;
    while(!feof(file))
    {
        fscanf(file, "%s", words[count]);
        count += 1;
    }
    return count;
}

int flag_r(FILE* input_1, FILE* input_2, FILE* output)
{
    char words_1[N][N] = {"\0"};
    char words_2[N][N] = {"\0"};
    int length_1 = read_file(words_1, input_1);
    int length_2 = read_file(words_2, input_2);
    if (length_1 > N || length_2 > N) return 0;
    if (length_1 == -1 || length_2 == -1) return 0;

    int max_length;
    if (length_1 >= length_2) max_length = length_1;
    else max_length = length_2;

    for (int i = 0; i < max_length; ++i)
    {
        if (i < length_1) fprintf(output, "%s ", words_1[i]);
        if (i < length_2) fprintf(output, "%s ", words_2[i]);
    }
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
    char words[N][N] = {"\0"};
    int length = read_file(words, input);

    if (length > N) return 0;
    if (length == -1) return 0;

    for (int i = 0; i < length; ++i)
    {
        if ((i + 1) % 10 == 0)
        {
            make_lower(words[i]);
            for (int j = 0; words[i][j] != '\0'; j++)
            {
                fprintf(output, "%d", base_4(words[i][j]));
            }
            fputc(' ', output);
        }
        else if ((i + 1) % 2 == 0)
        {
            make_lower(words[i]);
            fprintf(output, "%s ", words[i]);
        }
        else if ((i + 1) % 5 == 0)
        {
            for (int j = 0; words[i][j] != '\0'; j++)
            {
                fprintf(output, "%o", words[i][j]);
            }
            fputc(' ', output);
        }
        else fprintf(output, "%s ", words[i]);
    }
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
                printf("Too many words in file! Max quantity is 256 words.\n");
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