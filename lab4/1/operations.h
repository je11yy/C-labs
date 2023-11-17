#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int multiply_two_numbers(char ** res, char * first, char second);
int multiply_numbers(char ** first, char * second);
int summary_two_numbers(char **first, char *second, int base);
int get_length(int number);
int get_remainder(char ** result, char * number, int divisor);
int get_first_digit(int number);
int comparison(const char * first, const char * second);
char * difference(const char * num1, const char * num2);
int is_prime(int number);
int multiply_numbers_with_save(char ** multiply, char * first, char * second);

#endif