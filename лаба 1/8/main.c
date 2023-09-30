#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 256

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

int get_decimal_number(char* number, int base)
{
    int result = 0, power = 1, length = strlen(number), temp;

    for (int i = length - 1; i >= 0; --i)
    {
        if (number[i] >= '0' && number[i] <= '9') temp = number[i] - '0';
        else temp = tolower(number[i]) - 'a' + 10;

        result += temp * power;
        power *= base;
    }
    return result;
}

int get_min_base(char* number)
{
    int maximum = 0, temp;
    int length = strlen(number);
    
    for (int i = 0; i < length; ++i)
    {
        if (number[i] >= '0' && number[i] <= '9') temp = number[i] - '0';
        else temp = tolower(number[i]) - 'a' + 10;

        if (temp > maximum) maximum = temp;
    }
    if (maximum < 1 || maximum > 35) return -1;
    return maximum + 1;
}

int print_result(FILE* input, FILE* output)
{
    char words[N][N] = {"\0"};
    int length = read_file(words, input);

    if (length > N) return 0;
    if (length == -1) return 0;

    int base, decimal_number;

    for (int i = 0; i < length; ++i)
    {
        base = get_min_base(words[i]);
        decimal_number = get_decimal_number(words[i], base);
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
    if (!result)
    {
        printf("Error! Too many symbols in file. Maximum is 256.");
        return 0;
    }
    printf("Program has finished! Check the file.");
    return 0;
}