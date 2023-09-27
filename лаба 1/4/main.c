#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int in_flags(char item)
{
    char flags[] = {'d', 'i', 's', 'a'};
    int length = strlen(flags);
    for (int i = 0; i < length; ++i)
    {
        if (item == flags[i]) return 1;
    }
    return 0;
}

int check_file_name(char *name)
{
    int length = strlen(name);
    if (length < 6) return 0;

    // количество точек
    int count = 0;
    for (int i = 0; i < length; ++i)
    {
        if (name[i] == '.')
        {
            if (!count) count++;
            else return 0;
        }
    }
    // есть ли на конце txt
    char *temp = ".txt";
    length = strlen(temp);
    if (strcmp(name + strlen(name) - length, temp)) return 0;
    return 1;
}

// возвращает -3, если ошибка с длиной
// возвращает ноль, если ошибка с флагом, возвращает -1, если ошибка с названием входного файла, -2 - ошибка с названием выходного файла
int check_validation(int length, char *arguments[])
{
    if (length < 3 || length > 5) return -3;
    int result;
    if (length == 3)
    {
        // валидация флага
        char *flag = arguments[1];
        if (!(flag[0] == '-' || flag[0] == '/')) return 0;
        result = in_flags(flag[1]);
        if (!result) return 0;
        // входной файл
        char *file_name = arguments[2];
        result = check_file_name(file_name);
        if (!result) return -1;
        return 1;
    }
    if (length == 4)
    {
        // валидация флага
        char *flag = arguments[1];
        if (!((flag[0] == '-' || flag[0] == '/') && flag[1] == 'n')) return 0;
        result = in_flags(flag[2]);
        if (!result) return 0;
        // входной файл
        char *file_name = arguments[2];
        result = check_file_name(file_name);
        if (!result) return -1;
        // выходной файл
        file_name = arguments[3];
        result = check_file_name(file_name);
        if (!result) return -2;
        if (!strcmp(arguments[2], arguments[3])) return -2;
        return 1;
    }
}

void generate_name(char* new_name, char* name, int temp_length, int length, int new_length)
{
    char* temp = "out_";
    for (int i = 0; i < temp_length; ++i)
    {
        *new_name = temp[i];
        new_name++;
    }
    for (int i = 0; i < length; ++i)
    {
        *new_name = name[i];
        new_name++;
    }
    *new_name = '\0';
    return;
}

int is_letter(char temp)
{
    if ((temp >= 'a' && temp <= 'z') || (temp >= 'A' && temp <= 'Z')) return 1;
    return 0;
}

int is_digit(char temp)
{
    if (temp >= '0' && temp <= '9') return 1;
    return 0;
}

int flag_d(FILE* input, FILE* output)
{
    char temp;
    while(!feof(input))
    {
        if (ferror(input)) return 0;
        temp = fgetc(input);
        if (!(is_digit(temp)) && temp != EOF)
        {
            if (ferror(output)) return 0;
            fputc(temp, output);
        }
    }
    return 1;
}

int flag_i(FILE* input, FILE* output)
{
    char temp;
    char *temp_2;
    int count = 0;
    while(!feof(input))
    {
        if (ferror(input)) return 0;
        temp = fgetc(input);
        if (temp == '\n')
        {
            if (ferror(output)) return 0;
            fputs("Number of letters on this string: ", output);
            itoa(count, temp_2, 10);
            fputs(temp_2, output);
            fputc('\n', output);
            count = 0;
        }
        else if (is_letter(temp)) count++;
    }
    return 1;
}

int flag_s(FILE* input, FILE* output)
{
    int count = 0;
    char temp;
    char *temp_2;
    while(!feof(input))
    {
        if (ferror(input)) return 0;
        temp = fgetc(input);
        if (temp == '\n')
        {
            if (ferror(output)) return 0;
            fputs("Number of non-letters, non-digits and non-space symbols on this string: ", output);
            itoa(count, temp_2, 10);
            fputs(temp_2, output);
            fputc('\n', output);
            count = 0;
        }
        else if (temp != ' ' && !is_digit(temp) && !is_letter(temp)) count++;
    }
    return 1;
}

int flag_a(FILE* input, FILE* output)
{
    //%X - вывод шестнадцатиричного числа
    char temp;
    while(!feof(input))
    {
        if (ferror(input)) return 0;
        temp = fgetc(input);

        if (ferror(output)) return 0;

        if ((is_digit(temp) || temp == '\n') && temp != EOF) fputc(temp, output);
        else if(temp != EOF) fprintf(output, "%02X", temp);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int result;
    result = check_validation(argc, argv);
    if (result == -3)
    {
        printf("Incorrect input! Too many or too low arguments\nPlease, use the right format:\n-flag 'file.txt'\n      or\n-'n'flag 'file.txt' 'out_file.txt'");
        return 0;
    }
    if (result == 0)
    {
        printf("Incorrect flag input!\nPlease, use the right format:\n-flag 'file.txt'\n      or\n-'n'flag 'file.txt' 'out_file.txt'");
        return 0;
    }
    if (result == -1)
    {
        printf("Incorrect file input!\nPlease, use the right format:\n-flag 'file.txt'\n      or\n-'n'flag 'file.txt' 'out_file.txt'");
        return 0;
    }
    if (result == -2)
    {
        printf("Incorrect output-file input!\nPlease, use the right format:\n-flag 'file.txt'\n      or\n-'n'flag 'file.txt' 'out_file.txt'");
        return 0;
    }

    char *input_file = argv[2];
    char state;
    char *output_file;
    if (argc == 3)
    {
        char* temp = "out_";
        int length = strlen(input_file);
        int temp_length = strlen(temp);
        int new_length = length + temp_length;

        generate_name(output_file, input_file, temp_length, length, new_length);
        state = argv[1][1];
    }
    else {
        output_file = argv[3];
        state = argv[1][2];
    }
    FILE *input = fopen(input_file, "r");
    if (!input)
    {
        printf("ERROR: input-file can not be opened.\n");
        return 0;
    }

    FILE *output = fopen(output_file, "w");
    if (!output)
    {
        printf("ERROR: output-file can not be opened.\n");
        return 0;
    }

    switch(state)
    {
        //исключить арабские цифры
        case 'd':
            result = flag_d(input, output);
            if (!result) printf("File Error\n");

            fclose(input);
            fclose(output);
            printf("The program was finished. Check the file.");
            return 0;

        //сколько раз встречаются буквы
        case 'i':
            result = flag_i(input, output);
            if (!result) printf("File Error\n");

            fclose(input);
            fclose(output);
            printf("The program was finished. Check the file.");
            return 0;

        //сколько раз символы, которые не буквы, не цифры и не пробел
        case 's':
            result = flag_s(input, output);
            if (!result) printf("File Error\n");

            fclose(input);
            fclose(output);
            printf("The program was finished. Check the file.");
            return 0;

        //не цифры заменить аски кодом в 16ричной системе счисления
        case 'a':
            result = flag_a(input, output);
            if (!result) printf("File Error\n");

            fclose(input);
            fclose(output);
            printf("The program was finished. Check the file.");
            return 0;
    }
    fclose(input);
    fclose(output);
}