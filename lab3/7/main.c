#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string.h"
#include "liver.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

int menu();
int switch_commands(int * modifications, int command, Node_ptr * head, Undo_stack * stack, int * size);
void print_livers_file(Node_ptr head);
int append_liver_file(int modifications, Node_ptr head, Undo_stack * stack, int * size);
int append_liver(int modifications, Node_ptr head, Undo_stack * stack, int * size);
int find_liver(int * modifications, Node_ptr * head, Undo_stack * stack, int * size);
int liver_edit(int modification, Node_ptr head, Undo_stack * stack, int * size, int index);
int get_command();
int get_liver(Liver_ptr * liv, FILE * file);
int append_undo(int modifications, Undo_stack * stack, int * size, int operation, Liver_ptr prev, Liver_ptr new);
int undo(Node_ptr * head, Undo_stack stack, int * size);
int get_livers_from_file(FILE * file, Node_ptr * head);
int validation(char * name, char * surname, char * father_name, int day, int month, int year, char gender, double income);
int validate_date(int day, int month, int year);
int only_letters(char * string);

void print_error(int error);

int main()
{
    int modifications = 0;
    Undo_stack stack = NULL;
    int size = 0;

    Node_ptr head = NULL;
    FILE * file = fopen("livers.txt", "r");
    int result = get_livers_from_file(file, &head);
    fclose(file);
    if (result != success)
    {
        print_error(result);
        return result;
    }
    int command = -1;
    int flag = 0;
    while (command != 0)
    {
        menu();
        command = get_command();
        if (command != 6 && flag == 1)
        {
            modifications = 0;
            delete_stack(stack, size);
            flag = 0;
        }
        else if (command == 6) flag = 1;
        if (command == 0) break;
        result = switch_commands(&modifications, command, &head, &stack, &size);
        if (result != success)
        {
            print_error(result);
            return result;
        }
    }

    delete_list(head);
    delete_full_stack(stack, size);
}

int only_letters(char * string)
{
    int i = 0;
    while (string[i++] != '\0') if (!isalpha(string[i])) return fail;
    return success;
}

int validate_date(int day, int month, int year)
{
    if (month < 1 || month > 12) return fail;
    if (day < 1 || day > 31) return fail;
    if (year < 1900 || year > 2030) return fail;
    return success;
}

int validation(char * name, char * surname, char * father_name, int day, int month, int year, char gender, double income)
{
    if (only_letters(name) != success) return fail;
    if (only_letters(surname) != success) return fail;
    if (strcmp(father_name, "_") != success)
    {
        if (only_letters(father_name) != success) return fail;
    }
    if (validate_date(day, month, year) != success) return fail;
    if (!(gender == 'M' || gender == 'W')) return fail;
    double eps = 0.000000000000001;
    if (income < eps) return fail;
    return success;
}

int get_livers_from_file(FILE * file, Node_ptr * head)
{
    Liver_ptr liver;
    int result;
    while (!feof(file))
    {
        result = get_liver(&liver, file);
        if (result != success)
        {
            delete_list(*head);
            return result;
        }
        result = insert(head, liver);
        if (result != success)
        {
            delete_list(*head);
            return result;
        }
    }
    return success;
}

int menu()
{
    printf("\nCommand menu.\n");
    printf("1. Print livers.\n");
    printf("2. Print livers in file.\n");
    printf("3. Find liver.\n"); // ТУТ И УДАЛЕНИЕ, И РЕДАКТИРОВАНИЕ, И РАСПЕЧАТКА
    printf("4. Append liver via console.\n");
    printf("5. Append liver via file.\n");
    printf("6. Cancel last modification.\n");
    printf("0. Stop the program.\n");
    printf("\nInput form: ");
}

int switch_commands(int * modifications, int command, Node_ptr * head, Undo_stack * stack, int * size)
{
    int result;
    switch (command)
    {
        case 1:
            print_livers(stdout, *head);
            break;
        case 2:
            print_livers_file(*head);
            break;
        case 3:
            result = find_liver(modifications, head, stack, size);
            if (result != success) return result;
            break;
        case 4:
            result = append_liver(*modifications, *head, stack, size);
            (*modifications)++;
            if (result != success) return result;
            break;
        case 5:
            result = append_liver_file(*modifications, *head, stack, size);
            if (result == fail)
            {
                printf("Incorrect liver input in file!\n");
                return success;
            }
            (*modifications)++;
            if (result != success) return result;
            break;
        case 6:
            result = undo(head, *stack, size);
            if (result != success) return result;
            break;
    }
    return success;
}

void print_livers_file(Node_ptr head)
{
    printf("\nWrite down the name of the file. Maximum length is 50 letters.\n");
    printf("To cancel write 0\n");
    printf("Input form: ");
    int length = 51;
    char file_name[length];
    FILE *file;
    int result = scanf("%s", file_name);
    if (strcmp(file_name, "0") == success) return;
    file = fopen(file_name, "w");
    while (result != 1 || !file)
    {
        printf("Incorrect_input. Try again: ");
        result = scanf("%s", file_name);
        if (strcmp(file_name, "0") == success) return;
        file = fopen(file_name, "r");
    }
    print_livers(file, head);
    fclose(file);
}

int append_liver_file(int modifications, Node_ptr head, Undo_stack * stack, int * size)
{
    printf("\nWrite down file name. Maximum is 50 letters!\n");
    printf("Input form: ");
    int length = 51;
    char file_name[length];
    int result = scanf("%s", file_name);
    FILE * file = fopen(file_name, "r");
    while (result != 1 || !file)
    {
        printf("\nIncorrect input. Try again: ");
        fflush(stdin);
        result = scanf("%s", file_name);
        file = fopen(file_name, "r");
    }

    Liver_ptr new_liver;
    result = get_liver(&new_liver, file);
    if (result != success)
    {
        fclose(file);
        return result;
    }
    fclose(file);
    result = insert(&head, new_liver);
    if (result != success)
    {
        delete_stack(*stack, *size);
        return result;
    }
    result = append_undo(modifications, stack, size, 3, NULL, new_liver);
    if (result != success) return result;
    return success;
}

int append_liver(int modifications, Node_ptr head, Undo_stack * stack, int * size)
{
    printf("\nWrite down information about new liver.\n");
    printf("Max length of name, surname and father name is 50 letters.\n");
    printf("Format: name surname father_name birthday(day month year) gender (M or W) average_income\n\n");
    printf("Input form: ");
    Liver_ptr new_liver;
    int result = get_liver(&new_liver, stdin);
    while (result != success)
    {
        fflush(stdin);
        printf("\nIncorrect input. Try again: ");
        result = get_liver(&new_liver, stdin);
    }
    result = insert(&head, new_liver);
    if (!result)
    {
        delete_stack(*stack, *size);
        return result;
    }
    result = append_undo(modifications, stack, size, 3, NULL, new_liver);
    if (result != success) return result;
    return success;
}

int find_liver(int * modifications, Node_ptr * head, Undo_stack * stack, int * size)
{
    printf("Write down information about user.\n");
    printf("Max length of name, surname and father name is 50 letters.\n");
    printf("Format: name surname father_name birthday(day month year) gender (M or W) average_income\n\n");
    printf("Input form: ");
    Liver_ptr liver;
    Node_ptr node = NULL;
    int result = get_liver(&liver, stdin);
    int index;
    index = get_liver_index(*head, liver);
    if (index != -1) node = find_via_index(*head, index);
    while (result != success || !node)
    {
        fflush(stdin);
        printf("\nIncorrect input or no such liver. Try again: ");
        result = get_liver(&liver, stdin);
        index = get_liver_index(*head, liver);
        if (index != -1) node = find_via_index(*head, index);
    }
    print_liver(stdout, node -> liver);
    printf("\nLiver was found! Choose option.\n");
    printf("1. Delete user.\n");
    printf("2. Edit user.\n");
    printf("3. Return to main menu.\n\n");
    printf("Input form: ");
    int command = get_command();
    while (command < 1 || command > 3)
    {
        printf("\nIncorrect input. Try again: ");
        command = get_command();
    }
    index = get_index(*head, node);
    switch (command)
    {
        case 3: break;
        case 1: 
            result = append_undo(*modifications, stack, size, 1, liver, NULL);
            if (result != success) return result;
            remove_node(*head, index);
            printf("Liver has been deleted!\n");
            (*modifications)++;
            break;
        case 2:
            result = liver_edit(*modifications, *head, stack, size, index);
            (*modifications)++;
            if (result != success) return result;
            break;
    }
    return success;
}

int liver_edit(int modification, Node_ptr head, Undo_stack * stack, int * size, int index)
{
    printf("Input new information about liver.\n");
    printf("Max length of name, surname and father name is 50 letters.\n");
    printf("Format: name surname father_name birthday(day month year) gender (M or W) average_income\n\n");
    printf("Input form: ");
    Liver_ptr new_liver;
    int result = get_liver(&new_liver, stdin);
    while (result != success)
    {
        fflush(stdin);
        printf("\nIncorrect input. Try again: ");
        result = get_liver(&new_liver, stdin);
    }
    Liver_ptr prev = edit_liver(head, index, new_liver);
    if (!prev)
    {
        delete_list(head);
        delete_stack(*stack, *size);
        return no_memory;
    }
    result = append_undo(modification, stack, size, 2, prev, new_liver);
    if (result != success) return result;
    return success;
}

int get_command()
{
    int command;
    int result = scanf("%d", &command);
    while (result != 1)
    {
        fflush(stdin);
        result = scanf("%d", &command);
    }
    return command;
}

int get_liver(Liver_ptr * liv, FILE * file)
{
    int length = 51;
    char name[length], surname[length], father_name[length];
    int day, month, year;
    char gender;
    double income;
    int result = fscanf(file, "%s %s %s %d %d %d %c %lf", surname, name, father_name, &day, &month, &year, &gender, &income);
    if (result != 8 || !(validation(surname, name, father_name, day, month, year, gender, income))) return fail;

    Liver_ptr liver = create_liver(surname, name, father_name, day, month, year, gender, income);
    if (!liver) return no_memory;
    *liv = liver;
    return success;
}

int append_undo(int modifications, Undo_stack * stack, int * size, int operation, Liver_ptr prev, Liver_ptr new)
{
    Undo_ptr undo = create_undo(operation, prev, new);
    if (modifications != 0)
    {
        if ((*size) >= ((modifications + 1) / 2))
        {
            delete_undo((*stack)[0]);
            for (int i = 0; i < (*size) - 1; ++i)
            {
                (*stack)[i] = (*stack)[i + 1];
            }
            (*stack)[*size - 1] = undo;
            return success;
        }
    }

    (*size)++;
    Undo_stack tmp = (Undo_stack)realloc(*stack, (*size) * sizeof(Undo_ptr));
    if (!tmp)
    {
        delete_full_stack(*stack, *size);
        return no_memory;
    }
    *stack = tmp;

    (*stack)[*size - 1] = undo;
    return success;
}

int undo(Node_ptr * head, Undo_stack stack, int * size)
{
    if (*size == 0)
    {
        printf("You cannot cancel anymore!\n");
        return success;
    }
    Undo_ptr undo = stack[*size - 1];
    int operation = undo -> command;
    Liver_ptr prev = undo -> previous;
    Liver_ptr new = undo -> new;
    int result;
    Node_ptr node;
    int index;

    switch (operation)
    {
        case 1: // удаление
            result = insert(head, prev);
            if (result != success)
            {
                delete_stack(stack, *size);
                return result;
            }
            break;
        case 2: // изменение
            index = get_liver_index(*head, new);
            prev = edit_liver(*head, index, prev);

            if (!prev)
            {
                delete_stack(stack, *size);
                return result;
            }
            break;
        case 3: // добавление
            index = get_liver_index(*head, new);
            remove_node(*head, index);
            break;
    }
    delete_undo(undo);
    (*size)--;
    if (*size == 0)
    {
        free(stack);
        stack = NULL;
        return success;
    }
    Undo_stack tmp = (Undo_stack)realloc(stack, (*size) * sizeof(Undo_ptr));
    if (!tmp)
    {
        delete_stack(stack, *size);
        delete_list(*head);
        return no_memory;
    }
    stack = tmp;
    return success;
}

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
    }
}