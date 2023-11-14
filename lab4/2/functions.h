#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "arrays.h"

int save(Arrays_ptr arrays, char name, char * file_name);
int load(Arrays_ptr * arrays, char array_name, char * file_name);
int concatenation(Arrays_ptr arrays, char name_a, char name_b);
int randomize(Arrays_ptr * arrays, char name, int count, int lb, int lr);
int free_array(Arrays_ptr arrays, char name);
int remove_elements(Arrays_ptr arrays, char name, int first_ind, int quantity);
int copy_arrays(Arrays_ptr arrays, char name_a, int first, int second, char name_b);
int sort_array(Arrays_ptr arrays, char name, char flag);
int shuffle_array(Arrays_ptr arrays, char name);
int print_states(Arrays_ptr arrays, char name);
int print_all(Arrays_ptr arrays, char name);
int print_diap(Arrays_ptr arrays, char name, int pos_l, int pos_r);
int print_element(Arrays_ptr arrays, char name, int position);

int validate_name(char name);
int comp_decrease(const int * first, const int * second);
int comp_increase(const int * first, const int * second);
int shuffle_compare(const int * first, const int * second);

void print_array(Array_ptr array);

#endif