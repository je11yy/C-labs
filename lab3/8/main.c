#define _GNU_SOURCE
#include "list.h"
#include "polynomial.h"
#include <string.h>
#include <ctype.h>

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4,
    incorrect_command_input = -5,
    incorrect_argument = -6
};

enum COMMANDS
{
    cm_add = 0,
    cm_sub = 1,
    cm_mult = 2,
    cm_div = 3,
    cm_mod = 4,
    cm_eval = 5,
    cm_diff = 6,
    cm_cmps = 7
};

List_ptr make_polynomial(int * error, char * string);
int get_odd_power(char * string, int * odd, int * power);
int is_digit(char * string);
int command_handler(FILE * input);
int find_comment_end(char * string);
int read_command(int * comment_flag, char * string, List_ptr * first, List_ptr * second);
List_ptr do_command(int * error, int command, List_ptr first, List_ptr second);
int validation(int * comment_flag, char * string);
int validate_comment(char * string);
int validate_command(int command, char * string);
int is_command(char * string);
void print_command(int command);


void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_open_error:
            printf("\nCannot open file!\n");
            break;
        case incorrect_command_input:
            printf("\nInvalid command.\n");
            break;
        case incorrect_argument:
            printf("\nInvalid argument in command.\n");
            break;
        default: break;
    }
}

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    FILE * file = fopen(argv[1], "r");
    if (!file)
    {
        print_error(file_open_error);
        return file_open_error;
    }
    int result = command_handler(file);
    if (result != success)
    {
        print_error(result);
        return result;
    }
    printf("Program has finished correctly.\n");
    return success;
}

List_ptr make_polynomial(int * error, char * string)
{
    int length = strlen(string);
    // записываем в массив, является ли коэффициент отрицательным или нет для каждого из элементов
    int arg_count = 0;
    for (int i = 0; i < length; ++i)
    {
        if (string[i] == '+' || string[i] == '-') arg_count++;
    }
    if (string[0] == '-') arg_count--;
    arg_count++;

    int * odd_negatives = (int*)calloc((arg_count), sizeof(int));
    if (!odd_negatives)
    {
        *error = no_memory;
        return NULL;
    }
    for (int i = 0, k = 0; i < length; ++i)
    {
        if (string[i] == '-')
        {
            if (i == 0)
            {
                odd_negatives[k] = 1;
                k++;
            }
            else
            {
                k++;
                odd_negatives[k] = 1;
            }
        }
        else if (string[i] == '+') k++;
    }
    List_ptr polynomial = create_list();
    if (!polynomial)
    {
        free(odd_negatives);
        odd_negatives = NULL;
        *error = no_memory;
        return NULL;
    }
    char * string_copy = (char*)malloc((length + 1) * sizeof(char));
    if (!string_copy)
    {
        free(odd_negatives);
        odd_negatives = NULL;
        free_list(polynomial);
        *error = no_memory;
        return NULL;
    }
    strcpy(string_copy, string);
    char * tmp_arg = strtok(string_copy, "+-"); // разбиваем на элементы многочлена
    int res;
    int odd, power;
    int index = 0;
    while (tmp_arg != NULL)
    {
        res = get_odd_power(tmp_arg, &odd, &power);
        if (res != success)
        {
            free(odd_negatives);
            odd_negatives = NULL;
            free(string_copy);
            string_copy = NULL;
            free_list(polynomial);
            *error = res;
            return NULL;
        }
        if (odd_negatives[index] == 1) odd = -odd;
        res = push(&polynomial, odd, power);
        if (res != success)
        {
            free(odd_negatives);
            odd_negatives = NULL;
            free(string_copy);
            string_copy = NULL;
            free_list(polynomial);
            *error = res;
            return NULL;
        }
        index++;
        tmp_arg = strtok(NULL, "+-");
    }
    free(odd_negatives);
    odd_negatives = NULL;
    free(string_copy);
    string_copy = NULL;
    *error = success;
    return polynomial;
}

int get_odd_power(char * string, int * odd, int * power)
{
    int i = 0;
    int odd_tmp = -1, pow = 0;
    while (isdigit(string[i]))
    {
        if (odd_tmp == -1) odd_tmp = 0;
        odd_tmp *= 10;
        odd_tmp += string[i] - '0';
        i++;
    }
    if (string[i] == 0)
    {
        if (odd_tmp != -1) *odd = odd_tmp;
        else *odd = 1;
        *power = 0;
        return success;
    }
    if (string[i] != 'x') return incorrect_input;
    i++;
    if (string[i] == 0)
    {
        if (odd_tmp != -1) *odd = odd_tmp;
        else *odd = 1;
        *power = 1;
        return success;
    }
    if (string[i] != '^') return incorrect_input;
    i++;
    while (isdigit(string[i]))
    {
        pow *= 10;
        pow += string[i] - '0';
        i++;
    }
    if (string[i] != 0) return incorrect_input;
    if (odd_tmp != -1) *odd = odd_tmp;
    else *odd = 1;
    *power = pow;
    return success;
}

int is_digit(char * string)
{
    int i = 0;
    while (string[i] != 0)
    {
        if (!isdigit(string[i])) return fail;
        i++;
    }
    return success;
}

int command_handler(FILE * input)
{
    List_ptr summator = create_list();
    if (!summator)
    {
        fclose(input);
        return no_memory;
    }
    int res = push(&summator, 0, 0); // изначальное значение сумматора - ноль
    if (res != success)
    {
        free_list(summator);
        fclose(input);
        return res;
    }

    char * line = NULL;
    size_t len = 0;
    int command;
    // аргументы команд
    List_ptr first = NULL;
    List_ptr second = NULL;
    int comment_flag = 0;
    int length;
    int count = 0;
    while ((res = getline(&line, &len, input)) != EOF)
    {
        length = strlen(line);
        if (line[length - 1] == '\n') line[length - 1] = 0;
        if (comment_flag)
        {
            res = find_comment_end(line);
            if (res == success) comment_flag = 0;
            else if (res == incorrect_input)
            {
                free_list(summator);
                fclose(input);
                free(line);
                line = NULL;
                return res;
            }
            continue;
        }
        command = read_command(&comment_flag, line, &first, &second);
        if (command < 0)
        {
            free_list(summator);
            fclose(input);
            free(line);
            line = NULL;
            return command;
        }
        if (!second && command != cm_diff)
        {
            second = first;
            first = summator;
        }
        else free_list(summator);
        print_command(command); // распечатка названия команды
        printf("Polynom 1: ");
        print_polynomial(first);
        if (second)
        {
            printf("Polynom 2: ");
            print_polynomial(second);
        }
        summator = do_command(&res, command, first, second);
        if (res != success)
        {
            free_list(summator);
            fclose(input);
            free(line);
            line = NULL;
            return command;
        }
        printf("Result: ");
        print_polynomial(summator);
        printf("\n");
        count++;
    }
    free(line);
    line = NULL;
    fclose(input);
    free_list(summator);
    if (!count) return incorrect_input;
    return success;
}

int find_comment_end(char * string)
{
    int i = 0;
    while (string[i] != 0 && string[i] != ']') i++;
    if (string[i] == 0) return fail;
    i++;
    while (string[i] != 0)
    {
        if (string[i] != ' ') return incorrect_input;
        i++;
    }
    return success;
}

// возвращает комманду, записывает многочлены
int read_command(int * comment_flag, char * string, List_ptr * first, List_ptr * second)
{
    int length = strlen(string);
    if (string[length - 1] == '\n') string[length - 1] = 0;
    int command = validation(comment_flag, string);
    if (command < 0) return incorrect_input;
    int i = 0;
    int result;
    char * copy_string = (char*)malloc((length + 1) * sizeof(char));
    if (!copy_string) return no_memory;
    strcpy(copy_string, string);
    if (comment_flag)
    {
        while (copy_string[i] != '%' && copy_string[i] != '[' && copy_string[i] != 0) i++;
        if (copy_string[i] != 0)
        {
            while (copy_string[i] != 0)
            {
                copy_string[i] = 0;
                i++;
            }
        }
    }
    char * tmp = strtok(copy_string, "()"); // команда
    tmp = strtok(NULL, "()"); // аргументы
    i = 0;
    while (tmp[i] != ',' && tmp[i] != 0) i++;
    length = strlen(tmp);
    char * tmp2 = (char*)malloc((length + 1) * sizeof(char));
    if (!tmp2)
    {
        free(copy_string);
        copy_string = NULL;
        return no_memory;
    }
    strcpy(tmp2, tmp + i + 1);
    tmp = strtok(tmp, ","); // каждый аргумент по отдельности
    *first = make_polynomial(&result, tmp);
    if (result != success)
    {
        free(copy_string);
        copy_string = NULL;
        return result;
    }
    if (i == length)
    {
        free(copy_string);
        copy_string = NULL;
        free(tmp2);
        tmp2 = NULL;
        *second = NULL;
        return command;
    }
    else
    {
        *second = make_polynomial(&result, tmp2);
        if (result != success)
        {
            free(copy_string);
            copy_string = NULL;
            return result;
        }
    }
    free(tmp2);
    tmp2 = NULL;
    free(copy_string);
    copy_string = NULL;
    return command;
}

// выполняет команду, чистит многочлены
List_ptr do_command(int * error, int command, List_ptr first, List_ptr second)
{
    List_ptr new_list;
    switch (command)
    {
        case cm_add:
            new_list = add(first, second);
            if (!new_list)
            {
                *error = no_memory;
                return NULL;
            }
            break;
        case cm_sub:
            new_list = sub(first, second);
            if (!new_list)
            {
                *error = no_memory;
                return NULL;
            }
            break;
        case cm_mult:
            new_list = mult(first, second);
            if (!new_list)
            {
                *error = no_memory;
                return NULL;
            }
            break;
        case cm_div:
            new_list = division(error, first, second);
            if (!new_list) return NULL;
            break;
        case cm_mod:
            new_list = mod(error,first, second);
            if (!new_list) return NULL;
            break;
        case cm_eval:
            if (second -> size != 1)
            {
                free_list(new_list);
                free_list(first);
                free_list(second);
                *error = incorrect_argument;
                return NULL;
            }
            new_list = eval(first, second -> head -> odd);
            if (!new_list)
            {
                *error = no_memory;
                return NULL;
            }
            break;
        case cm_cmps:
            new_list = cmps(first, second);
            if (!new_list)
            {
                *error = no_memory;
                return NULL;
            }
            break;
        case cm_diff:
            new_list = diff(first);
            if (!new_list)
            {
                *error = no_memory;
                return NULL;
            }
            break;
        default: break;
    }
    *error = success;
    return new_list;
}

int validation(int * comment_flag, char * string)
{
    char * tmp = NULL;
    char * tmp1 = NULL;
    int length = 0;
    int count = 0;
    int i = 0;
    while (string[i] != '(' && string[i] != 0)
    {
        if (length <= count)
        {
            length++;
            tmp1 = (char*)realloc(tmp, length * sizeof(char));
            if (!tmp1)
            {
                free(tmp);
                tmp = NULL;
                return no_memory;
            }
            tmp = tmp1;
        }
        tmp[count] = string[i];
        count++;
        i++;
    }
    tmp[count] = 0;
    if (count != 4 && count != 3)
    {
        free(tmp);
        tmp = NULL;
        return incorrect_input;
    }
    int command = is_command(tmp);
    if (command < 0)
    {
        free(tmp);
        tmp = NULL;
        return incorrect_input;
    }
    free(tmp);
    tmp = NULL;
    int res = validate_command(command, string);
    if (res != success) return res;
    res = validate_comment(string);
    if (res < 0) return res;
    if (res == 1) *comment_flag = 1;
    else *comment_flag = 0;
    return command;
}

int validate_comment(char * string)
{
    int i = 0;
    while (string[i] != ')' && string[i] != 0) i++;
    i++;
    if (string[i] != ';') return incorrect_input;
    i++;
    if (string[i] == 0) return success; // комментария нет
    while (string[i] != 0 && string[i] != '%' && string[i] != '[') // есть комментарий
    {
        if (string[i] != ' ') return incorrect_input;
        i++;
    }
    if (string[i] == 0) return success; // все же нет(
    int flag = 0;
    if (string[i] == '%') return success;
    else if (string[i] == '[') flag = 1; // открыт многострочный комментари
    while (string[i] != ']' && string[i] != 0) i++;
    if (string[i] == ']') flag = 0;
    if (flag && string[i] == 0) return 1; // не закрыт
    i++; // закрыт!!!
    while (string[i] != 0)
    {
        if (string[i] != ' ') return incorrect_input;
        i++;
    }
    return success;
}

int validate_command(int command, char * string)
{
    int arguments_count = 0;
    int i = 0;
    while(string[i] != '(' && string[i] != 0) i++;
    if (string[i] == 0) return incorrect_input;
    while (string[i] != 0 && string[i] != ')')
    {
        if (string[i] == ',') arguments_count++;
        i++;
    }
    if (isdigit(string[i - 1]) || string[i - 1] == 'x') arguments_count++;
    if (string[i] == 0) return incorrect_input;
    i++;
    if (string[i] != ';') return incorrect_input;
    i++;
    if (arguments_count != 1 && arguments_count != 2) return incorrect_input;
    if (command == cm_diff && arguments_count == 2) return incorrect_argument;
    return success;
}

void print_command(int command)
{
    switch (command)
    {
        case cm_add:
            printf("Add command\n");
            break;
        case cm_sub:
            printf("Sub command\n");
            break;
        case cm_mult:
            printf("Mult command\n");
            break;
        case cm_div:
            printf("Div command\n");
            break;
        case cm_mod:
            printf("Mod command\n");
            break;
            break;
        case cm_eval:
            printf("Eval command\n");
            break;
        case cm_cmps:
            printf("Cmps command\n");
            break;
        case cm_diff:
            printf("Diff command\n");
            break;
        default: break;
    }
}

int is_command(char * string)
{
    char * command[] = {"Add", "Sub", "Mult", "Div", "Mod", "Eval", "Diff", "Cmps", NULL};
    int i = 0;
    while (command[i] != NULL)
    {
        if (strcmp(command[i], string) == success) return i;
        i++;
    }
    return fail;
}