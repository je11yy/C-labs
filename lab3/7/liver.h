#ifndef LIVER_H
#define LIVER_H

#include <stdio.h>
#include "string.h"

typedef struct
{
    unsigned int day;
    unsigned int month;
    unsigned int year;
} Birthday, *Birthday_ptr;

typedef struct
{
    String_ptr surname;
    String_ptr name;
    String_ptr father_name;
    Birthday_ptr birthday;
    char gender;
    double income;
} Liver, *Liver_ptr, **Liver_array;

typedef struct
{
    Liver_ptr previous;
    Liver_ptr new;
    int command;
} Undo, *Undo_ptr, **Undo_stack;

typedef struct Node
{
    Liver_ptr liver;
    struct Node * next;
} Node, *Node_ptr;

Undo_ptr create_undo(int command, Liver_ptr previous, Liver_ptr new);

void delete_stack(Undo_stack undos, int size);
void delete_undo(Undo_ptr undo);

Birthday_ptr create_birthday(int day, int month, int year);
Liver_ptr create_liver(char * surname, char * name, char * father_name, int day, int month, int year, char gender, double income);

void delete_birthday(Birthday_ptr birthday);
void delete_liver(Liver_ptr liver);

int insert(Node_ptr * head, Liver_ptr liver);
void delete_list(Node_ptr head);
void delete_node(Node_ptr node);
void remove_node(Node_ptr head, int index);

int compare_livers(Liver_ptr first, Liver_ptr second);
int is_equal(Node_ptr node, Node_ptr node1);
int is_equal_bday(Birthday_ptr first, Birthday_ptr second);
int is_equal_livers(Liver_ptr first, Liver_ptr liver);

void print_liver(FILE * file, Liver_ptr liver);
void print_livers(FILE * file, Node_ptr head);

int edit_liver(Node_ptr head, int index, Liver_ptr new_liver);
Node_ptr find_via_index(Node_ptr head, int index);
int get_index(Node_ptr head, Node_ptr liv);
int get_liver_index(Node_ptr head, Liver_ptr liver);

#endif