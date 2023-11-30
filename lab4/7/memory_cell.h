#ifndef MEMORY_CELL_H
#define MEMORY_CELL_H

typedef struct MemoryCell
{
    char * name;
    int value;
} MemoryCell, *MemoryCell_ptr, **MemoryCell_array;

int print_variable(MemoryCell_array array, int length, char * name);
void print_array(MemoryCell_array array, int length);

char* make_expression(int * error, char * expression, MemoryCell_array cell_array, int length, int size);
int get_size(int value);

int update_array(MemoryCell_array * cell_array, int * size, int * length, const char * name, const int value);
int append_cell(MemoryCell_array * cell_array, int * size, int * length, const char * name, const int value);

MemoryCell_ptr dichotomy_search(MemoryCell_array array, int length, const char * name);
int memory_cell_comparison(const MemoryCell_ptr * first, const MemoryCell_ptr * second);

MemoryCell_ptr create_memory_cell(const char * name, const int value);
int realloc_memory_cell_array(MemoryCell_array * array, int * size, int * length);

void free_memory_cell_array(MemoryCell_array array, const int length);
void free_memory_cell(MemoryCell_ptr cell);

#endif