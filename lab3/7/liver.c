#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "string.h"
#include "liver.h"
#include <math.h>

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

int is_equal(Node_ptr node, Node_ptr node1)
{
    Liver_ptr first = node -> liver;
    Liver_ptr liver = node1 -> liver;
    if (strings_cmp(first -> surname, liver -> surname) != success) return fail;
    if (strings_cmp(first -> name, liver -> name) != success) return fail;
    if (!(first -> father_name) && liver -> father_name) return fail;
    if ((first -> father_name) && !(liver -> father_name)) return fail;
    if (first -> father_name)
    {
        if (strings_cmp(first -> father_name, liver -> father_name) != success) return fail;
    }
    if (is_equal_bday(first -> birthday, liver -> birthday) != success) return fail;
    if (first -> gender != liver -> gender) return fail;
    double eps = 0.0000001;
    if (fabs(first -> income - liver -> income) >= eps) return fail;
    return success;
}

int is_equal_livers(Liver_ptr first, Liver_ptr liver)
{
    if (strings_cmp(first -> surname, liver -> surname) != success) return fail;
    if (strings_cmp(first -> name, liver -> name) != success) return fail;
    if (!(first -> father_name) && liver -> father_name) return fail;
    if ((first -> father_name) && !(liver -> father_name)) return fail;
    if (first -> father_name)
    {
        if (strings_cmp(first -> father_name, liver -> father_name) != success) return fail;
    }
    if (is_equal_bday(first -> birthday, liver -> birthday) != success) return fail;
    if (first -> gender != liver -> gender) return fail;
    double eps = 0.0000001;
    if (fabs(first -> income - liver -> income) >= eps) return fail;
    return success;
}

int is_equal_bday(Birthday_ptr first, Birthday_ptr second)
{
    if (first -> day != second -> day) return fail;
    if (first -> month != second -> month) return fail;
    if (first -> year != second -> year) return fail;
    return success;
}

Undo_ptr create_undo(int command, Liver_ptr previous, Liver_ptr new)
{
    Undo_ptr undo = (Undo_ptr)malloc(sizeof(Undo));
    if (!undo) return NULL;

    undo -> command = command;
    undo -> previous = previous;
    undo -> new = new;
    return undo;
}

void delete_full_stack(Undo_stack undos, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete_full_undo(undos[i]);
    }
    free(undos);
    undos = NULL;
}

void delete_stack(Undo_stack undos, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete_undo(undos[i]);
    }
    free(undos);
    undos = NULL;
}

void delete_undo(Undo_ptr undo)
{
    switch (undo -> command)
    {
        case 1:
            delete_liver(undo -> previous);
            break;
        case 2:
            delete_liver(undo -> previous);
            undo -> new = NULL;
            break;
        case 3:
            undo -> new = NULL;
            break;
    }
    free(undo);
    undo = NULL;
}

void delete_full_undo(Undo_ptr undo)
{
    if (undo -> previous)
    {
        delete_liver(undo -> previous);
    }
    if (undo -> new)
    {
        delete_liver(undo -> new);
    }
    free(undo);
    undo = NULL;
}

// возвращает указатель на старого жителя
Liver_ptr edit_liver(Node_ptr head, int index, Liver_ptr new_liver)
{
    Node_ptr node = head;
    int i = 0;
    while ((i++) != index) node = node -> next;
    remove_node(head, index);
    int result = insert(&head, new_liver);
    if (result != success)
    {
        delete_node(node);
        return NULL;
    }
    return node -> liver;
}

Node_ptr find_via_index(Node_ptr head, int index)
{
    int i = 0;
    Node_ptr node = head;
    while (i != index)
    {
        node = node -> next;
        i++;
    }
    return node;
}

int get_liver_index(Node_ptr head, Liver_ptr liver)
{
    Node_ptr node = head;
    int index = 0;
    while (node -> next != NULL)
    {
        if (is_equal_livers(node -> liver, liver) == success) return index;
        node = node -> next;
        index++;
    }
    if (is_equal_livers(node -> liver, liver) != success) return -1;
    return index;
}

int get_index(Node_ptr head, Node_ptr liv)
{
    Node_ptr node = head;
    int index = 0;
    while (node -> next != NULL)
    {
        if (is_equal(node, liv) == success) return index;
        node = node -> next;
        index++;
    }
    if (is_equal(node, liv) != success) return -1;
    return index;
}

void print_livers(FILE * file, Node_ptr head)
{
    int count = 1;
    if (head == NULL)
    {
        printf("NO LIVERS\n");
        return;
    }
    while (head)
    {
        fprintf(file, "%d)\n", count);
        count++;
        print_liver(file, head -> liver);
        head = head -> next;
    }
}

void print_liver(FILE * file, Liver_ptr liver)
{
    fprintf(file, "Surname: %s\n", liver -> surname -> buffer);
    fprintf(file, "Name: %s\n", liver -> name -> buffer);
    if (liver -> father_name) fprintf(file, "Father name: %s\n", liver -> father_name -> buffer);
    fprintf(file, "Birthday: %d %d %d\n", liver -> birthday -> day, liver -> birthday -> month, liver -> birthday -> year);
    fprintf(file, "Gender: %c\n", liver -> gender);
    fprintf(file, "Average income: %f\n\n", liver -> income);
}

// добавление в список
int insert(Node_ptr * head, Liver_ptr liver)
{
    Node_ptr tmp = *head;
    if (tmp == NULL)
    {
        Node_ptr node = (Node_ptr)malloc(sizeof(Node));
        if (!node) return no_memory;
        node -> liver = liver;
        *head = node;
        (*head) -> next = NULL;
        return success;
    }
    // пока не дойдем до конца или не найдем нужное место
    while (tmp -> next && compare_livers(tmp -> liver, liver) > 0)
    {
        tmp = tmp -> next;
    }

    Node_ptr node = (Node_ptr)malloc(sizeof(Node));
    if (!node) return no_memory;
    node -> liver = liver;

    if (tmp -> next) node -> next = tmp -> next;
    else node -> next = NULL;
    tmp -> next = node;
    return success;
}

void remove_node(Node_ptr head, int index)
{
    Node_ptr current = head, prev;
    int i = 0;
    while (i != index)
    {
        prev = current;
        current = current -> next;
        i++;
    }
    current -> liver = NULL;
    prev -> next = current -> next;
    current -> next = NULL;
}

void delete_list(Node_ptr head)
{
    if (head == NULL) return;
    Node_ptr tmp = head -> next;
    while (tmp)
    {
        delete_node(head);
        head = tmp;
        tmp = head -> next;
    }
    delete_node(head);
}

void delete_node(Node_ptr node)
{
    delete_liver(node -> liver);
    free(node);
    node = NULL;
}

int compare_livers(Liver_ptr first, Liver_ptr second)
{
    if (first -> birthday -> year > second -> birthday -> year) return 1;
    if (first -> birthday -> year < second -> birthday -> year) return -1;

    if (first -> birthday -> month > second -> birthday -> month) return 1;
    if (first -> birthday -> month < second -> birthday -> month) return -1;

    if (first -> birthday -> day > second -> birthday -> day) return 1;
    if (first -> birthday -> day < second -> birthday -> day) return -1;

    return 0;
}

Liver_ptr create_liver(char * surname, char * name, char * father_name, int day, int month, int year, char gender, double income)
{
    String_ptr surname_string = create_string(surname);
    if (!surname_string) return NULL;

    String_ptr name_string = create_string(name);
    if (!name_string)
    {
        delete_string(surname_string);
        return NULL;
    }

    String_ptr father_string;
    if (strlen(father_name) == 1 && father_name[0] == '-')
    {
        father_string = NULL;
    }
    else
    {
        father_string = create_string(father_name);
        if (!father_string)
        {
            delete_string(surname_string);
            delete_string(name_string);
            return NULL;
        }
    }

    Birthday_ptr birthday = create_birthday(day, month, year);
    if (!birthday)
    {
        delete_string(surname_string);
        delete_string(name_string);
        delete_string(father_string);
        return NULL;
    }

    Liver_ptr liver = (Liver_ptr)malloc(sizeof(Liver));
    if (!liver)
    {
        delete_string(surname_string);
        delete_string(name_string);
        delete_string(father_string);
        delete_birthday(birthday);
        return NULL;
    }

    liver -> surname = surname_string;
    liver -> name = name_string;
    liver -> father_name = father_string;
    liver -> birthday = birthday;
    liver -> gender = gender;
    liver -> income = income;

    return liver;
}

void delete_liver(Liver_ptr liver)
{
    delete_string(liver -> surname);
    delete_string(liver -> name);
    delete_string(liver -> father_name);
    delete_birthday(liver -> birthday);
    free(liver);
    liver = NULL;
}

void delete_birthday(Birthday_ptr birthday)
{
    free(birthday);
    birthday = NULL;
}

Birthday_ptr create_birthday(int day, int month, int year)
{
    Birthday_ptr birthday = (Birthday_ptr)malloc(sizeof(Birthday));
    if (!birthday) return NULL;
    birthday -> day = (unsigned int)day;
    birthday -> month = (unsigned int)month;
    birthday -> year = (unsigned int)year;
    return birthday;
}