#ifndef OPERATIONS_H
#define OPERATIONS_H

int is_alpha(char * string);
int is_number(char * string);
int is_operator(char symbol);
int do_operation(int * result, char operation, int left, int right);

#endif