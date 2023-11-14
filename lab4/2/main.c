#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_library.h"
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

enum COMMANDS
{
    load_com = 0,
    save_com = 1,
    rand_com = 2,
    concat_com = 3,
    free_com = 4,
    remove_com = 5,
    copy_com = 6,
    sort_com = 7,
    shuffle_com = 8,
    stats_com = 9,
    print_com = 10
};

void instructions();
int is_command(char * string);
int get_command_length(char * string);
void get_command(char * string, int length, char ** result);
int get_lexems_count(char * string);
int validate_command(char * string, int length);
int make_instructions(FILE * file);
int do_command(Arrays_ptr arrays, char * string, int command);
int is_all(char * string);

// файл  с командами подается в виде аргумента командной строки

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

    int result = make_instructions(file);
    if (result != success)
    {
        print_error(result);
        if (result == incorrect_command_input) instructions();
        return result;
    }

    printf("\nProgram has finished correctly.\n");
    return success;
}

int make_instructions(FILE * file)
{
    Arrays_ptr arrays = create_arrays();
    if (!arrays)
    {
        fclose(file);
        return no_memory;
    }
    char * line = NULL;
    int read;
    size_t length = 0;
    int len;
    int command;
    int result;
    while ((read = getline(&line, &length, file)) != EOF)
    {
        len = strlen(line);
        if (line[len - 1] == '\n')
        {
            line[len - 1] = 0;
            len--;
        }
        command = validate_command(line, len);
        if (command < 0)
        {
            fclose(file);
            delete_arrays(arrays);
            free(line);
            line = NULL;
            return command;
        }
        result = do_command(arrays, line, command);
        if (result != success)
        {
            fclose(file);
            free(line);
            line = NULL;
            return incorrect_command_input;
        }
    }
    fclose(file);
    delete_arrays(arrays);
    free(line);
    line = NULL;
    return success;
}

int do_command(Arrays_ptr arrays, char * string, int command)
{
    make_lower(&string);

    int first, second, third;
    char name, name_1;
    int size = strlen(string);
    string[size - 1] = 0;
    char file_name[size];
    int len;

    int arg_count;
    int res;
    switch (command)
    {
        case load_com:
            res = sscanf(string, "load %c, %s", &name, file_name);
            if (res != 2) return incorrect_command_input;
            res = load(&arrays, name, file_name);
            if (res != success) return res;
            break;
        case save_com:
            res = sscanf(string, "save %c, %s", &name, file_name);
            if (res != 2) return incorrect_command_input;
            res = save(arrays, name, file_name);
            if (res != success) return res;
            break;
        case rand_com:
            res = sscanf(string, "rand %c, %d, %d, %d", &name, &first, &second, &third);
            if (res != 4) return incorrect_command_input;
            res = randomize(&arrays, name, first, second, third);
            if (res != success) return res;
            break;
        case concat_com:
            res = sscanf(string, "concat %c, %c", &name, &name_1);
            if (res != 2) return incorrect_command_input;
            res = concatenation(arrays, name, name_1);
            if (res != success) return res;
            break;
        case free_com:
            res = sscanf(string, "free(%c)", &name);
            if (res != 1) return incorrect_command_input;
            res = free_array(arrays, name);
            if (res != success) return res;
            break;
        case remove_com:
            res = sscanf(string, "remove %c, %d, %d", &name, &first, &second);
            if (res != 3) return incorrect_command_input;
            res = remove_elements(arrays, name, first, second);
            if (res != success) return res;
            break;
        case copy_com:
            res = sscanf(string, "copy %c, %d, %d, %c", &name, &first, &second, &name_1);
            if (res != 4) return incorrect_command_input;
            res = copy_arrays(arrays, name, first, second, name_1);
            if (res != success) return res;
            break;
        case sort_com:
            size = strlen(string);
            if (string[size - 1] != '+' && string[size - 1] != '-') return incorrect_command_input;
            if (string[size - 1] == '+')
            {
                res = sscanf(string, "sort %c+", &name);
                if (res != 1) return incorrect_command_input;
                res = sort_array(arrays, name, '+');
            }
            else
            {
                res = sscanf(string, "sort %c-", &name);
                if (res != 1) return incorrect_command_input;
                res = sort_array(arrays, name, '-');
            }
            if (res != success) return res;
            break;
        case shuffle_com:
            res = sscanf(string, "shuffle %c", &name);
            if (res != 1) return incorrect_command_input;
            res = shuffle_array(arrays, name);
            if (res != success) return res;
            break;
        case stats_com:
            res = sscanf(string, "stats %c", &name);
            if (res != 1) return incorrect_command_input;
            res = print_states(arrays, name);
            if (res != success) return res;
            break;
        case print_com:
            arg_count = get_lexems_count(string);
            if (arg_count == 3)
            {
                res = sscanf(string, "print %c, %d, %d", &name, &first, &second);
                if (res != 3) return incorrect_command_input;
                res = print_diap(arrays, name, first, second);
            }
            else if (arg_count == 2)
            {
                res = is_all(string);
                if (res == success)
                {
                    res = sscanf(string, "print %c, all", &name);
                    if (res != 1) return incorrect_command_input;
                    res = print_all(arrays, name);
                }
                else
                {
                    res = sscanf(string, "print %c, %d", &name, &first);
                    if (res != 2) return incorrect_command_input;
                    res = print_element(arrays, name, first);
                }
            }
            if (res != success) return res;
            break;
    }
    return success;
}

int is_all(char * string)
{
    int i = strlen(string);
    if (string[i - 1] == 'l' && string[i - 2] == 'l' && string[i - 3] == 'a' && string[i - 4] == ' ') return success;
    return fail;
}

int validate_command(char * string, int length)
{
    if (string[length - 1] != ';') return incorrect_command_input;
    int count = 0;
    for (int i = 0; i < length; ++i) if (string[i] == ';') count++;
    if (count != 1) return incorrect_command_input;
    int arguments_count = get_lexems_count(string);
    if (!(arguments_count >= 0 && arguments_count <= 4)) return incorrect_command_input;
    int len = get_command_length(string);
    if (!(len >= 4 && len <= 7)) return incorrect_command_input;
    char * command = (char*)malloc((len + 1) * sizeof(char));
    if (!command) return no_memory;
    get_command(string, len, &command);
    int command_ind = is_command(command);
    free(command);
    command = NULL;
    if (command_ind < 0) return incorrect_command_input;
    if ((command_ind == 0 || command_ind == 1 || command_ind == 3) && arguments_count != 2) return incorrect_command_input;
    if (command_ind == 4 && arguments_count != 0) return incorrect_command_input;
    if ((command_ind >= 7 && command_ind <= 9) && arguments_count != 1) return incorrect_command_input;
    if ((command_ind == 2 || command_ind == 6) && arguments_count != 4) return incorrect_command_input;
    if (command_ind == 5 && arguments_count != 3) return incorrect_command_input;
    if (command_ind == 10 && (!(arguments_count == 2 || arguments_count == 3))) return incorrect_command_input;
    return command_ind;
}

int get_lexems_count(char * string)
{
    int i = 0;
    int count = 0;
    while (string[i] != '\0')
    {
        if (string[i] == ' ') count++;
        i++;
    }
    return count;
}

void get_command(char * string, int length, char ** result)
{
    for (int i = 0; i < length; ++i) (*result)[i] = string[i];
    (*result)[length] = '\0';
}

int get_command_length(char * string)
{
    int length = 0;
    while (string[length] != ' ' && string[length] != '(') length++;
    return length;
}

// при неудаче возвращает -1, иначе - индекс команды
int is_command(char * string)
{
    make_lower(&string);
    char * commands[] = {"load", "save", "rand", "concat", "free", "remove", "copy", "sort", "shuffle", "stats", "print", NULL};
    int i = 0;
    while (commands[i] != NULL)
    {
        if (strcmp(string, commands[i]) == success) return i;
        i++;
    }
    return fail;
}

void instructions()
{
    printf("\nRules for file with instructions:\n\n");
    printf("- every command should end with ;\n");
    printf("- do not forget about spaces after commas\n");
    printf("- common format: command array_name, arguments, ...;\n");
    printf("- do NOT add more than one spaces\n\n");
    printf("If you will forget about these rules, program will be finished automatically!\n");
}