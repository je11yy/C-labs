#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <math.h>

void print_polynomial(List_ptr polynomial);
Node_ptr find_power(Node_ptr head, int power);
List_ptr increase_power(List_ptr list, int power);
List_ptr raise_in_power(List_ptr list, List_ptr prev, int power);

List_ptr add(List_ptr first, List_ptr second);
List_ptr sub(List_ptr first, List_ptr second);
List_ptr make_negative(List_ptr list);
List_ptr mult(List_ptr first, List_ptr second);
List_ptr division(int * error, List_ptr first, List_ptr second);
List_ptr mod(int * error, List_ptr first, List_ptr second);
List_ptr eval(List_ptr list, int value);
List_ptr diff(List_ptr list);
List_ptr cmps(List_ptr first, List_ptr second);

void quick_pow(int * result, int value, int power);

#endif