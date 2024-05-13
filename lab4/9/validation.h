#ifndef _VALIDATION_H_
#define _VALIDATION_H_

#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "string.h"

typedef enum
{
    BinaryHeap = 1,
    BinomialHeap = 2,
    FibonacciHeap = 3,
    SkewHeap = 4,
    LeftistHeap = 5,
    Treap = 6
} storage_of_applications;

typedef enum
{
    HashSet = 1,
    DinamicArray = 2,
    BinarySearchTree = 3,
    Trie = 4
} department_storage;

status unsigned_int_check(char * number);

status double_check(char * number);

status validate_storage_structure(char * string, int * structure);

status validate_departments_structure(char * string, int * structure);

status is_leap_year(const int year);

status validate_time(char * time);

status validate_application_time_and_departments_count(FILE * file, int * argument);

status validate_start_and_end(FILE * file, time_t * time);

status validate_structures(FILE * file, int * applications_storage, int * departments_storage);

#endif