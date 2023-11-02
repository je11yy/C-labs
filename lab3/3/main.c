#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    unsigned int id;
    char * name;
    char * surname;
    double salary; // зарплата
    
} Employee, *Employee_ptr, **Employee_array;

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

void print_error(int error);

void print_employers(Employee_array employers, FILE * file, int length, char flag);
void delete_employers(Employee_array employers, int length);
int compare_increase(const Employee_ptr * first, const Employee_ptr * second);
int compare_decrease(const Employee_ptr * first, const Employee_ptr * second);
int get_employers(FILE * file, Employee_array * employers, int length);
void print_employee_info(Employee_ptr employee, FILE * file);
void delete_employee(Employee_ptr employee);
Employee_ptr create_employee(int id, const char * name, const char * surname, double salary);

int main(int argc, char * argv[])
{
    if (argc != 4)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    char * input_name = argv[1];
    char * output_name = argv[3];
    char * flag = argv[2];
    if (!(flag[0] == '-' || flag[0] == '/') || !(flag[1] == 'a' || flag[1] == 'd'))
    {
        print_error(incorrect_input);
        return incorrect_input;
    }

    FILE * input = fopen(input_name, "r");
    if (!input)
    {
        print_error(file_open_error);
        return file_open_error;
    }

    FILE * output = fopen(output_name, "w");
    if (!output)
    {
        fclose(input);
        print_error(file_open_error);
        return file_open_error;
    }

    int length = 1;
    Employee_array employers = (Employee_array) malloc(length * sizeof(Employee_ptr));
    if (!employers)
    {
        fclose(input);
        fclose(output);
        return no_memory;
    }
    int result = get_employers(input, &employers, length);
    if (result < 0)
    {
        print_error(result);
        fclose(input);
        fclose(output);
        return result;
    }
    length = result;
    print_employers(employers, output, length, flag[1]);

    fclose(input);
    fclose(output);

    delete_employers(employers, length);

    printf("The program has finished correctly.\n");
    return success;
}

void delete_employers(Employee_array employers, int length)
{
    for (int i = 0; i < length; ++i)
    {
        delete_employee(employers[i]);
    }
    free(employers);
    employers = NULL;
}

void delete_employee(Employee_ptr employee)
{
    free(employee -> surname);
    employee -> surname = NULL;
    free(employee -> name);
    employee -> name = NULL;
    free(employee);
    employee = NULL;
}

Employee_ptr create_employee(int id, const char * name, const char * surname, double salary)
{
    Employee_ptr employee = (Employee_ptr) malloc(sizeof(Employee));
    if (!employee) return NULL;

    employee -> id = id;
    int len = strlen(name);
    employee -> name = (char*)malloc((len + 1) * sizeof(char));
    if (!(employee -> name))
    {
        free(employee);
        employee = NULL;
        return NULL;
    }
    strcpy(employee -> name, name);

    len = strlen(surname);
    employee -> surname = (char*)malloc((len + 1) * sizeof(char));
    if (!(employee -> surname))
    {
        free(employee);
        employee = NULL;
        free(employee -> name);
        employee -> name = NULL;
        return NULL;
    }
    strcpy(employee -> surname, surname);

    employee -> salary = salary;

    return employee;
}

void print_employers(Employee_array employers, FILE * file, int length, char flag)
{
    switch (flag)
    {
        case 'a':
            qsort(employers, length, sizeof(Employee_ptr), (int (*) (const void *, const void *)) compare_increase);
            break;
        case 'd':
            qsort(employers, length, sizeof(Employee_ptr), (int (*) (const void *, const void *)) compare_decrease);
            break;
        default: break;
    }
    for (int i = 0; i < length; ++i)
    {
        fprintf(file, "%d) ", i + 1);
        print_employee_info(employers[i], file);
        fprintf(file, "\n");
    }
}

void print_employee_info(Employee_ptr employee, FILE * file)
{
    fprintf(file, "id: %d | ", employee -> id);
    fprintf(file, "name: %s | ", employee -> name);
    fprintf(file, "surname: %s | ", employee -> surname);
    fprintf(file, "salary: %f", employee -> salary);
}

int compare_increase(const Employee_ptr * first, const Employee_ptr * second)
{
    if ((*first) -> salary > (*second) -> salary) return 1;
    if ((*first) -> salary < (*second) -> salary) return -1;

    int surname = strcmp((*first) -> surname, (*second) -> surname);
    if (surname > 0) return 1;
    if (surname < 0) return -1;

    int name = strcmp((*first) -> name, (*second) -> name);
    if (name > 0) return 1;
    if (name < 0) return -1;

    if ((*first) -> id > (*second) -> id) return 1;
    if ((*first) -> id < (*second) -> id) return -1;

    return 0;
}

int compare_decrease(const Employee_ptr * first, const Employee_ptr * second)
{
    if ((*first) -> salary > (*second) -> salary) return -1;
    if ((*first) -> salary < (*second) -> salary) return 1;

    int surname = strcmp((*first) -> surname, (*second) -> surname);
    if (surname > 0) return -1;
    if (surname < 0) return 1;

    int name = strcmp((*first) -> name, (*second) -> name);
    if (name > 0) return -1;
    if (name < 0) return 1;

    if ((*first) -> id > (*second) -> id) return -1;
    if ((*first) -> id < (*second) -> id) return 1;

    return 0;
}

int get_employers(FILE * file, Employee_array * employers, int length)
{
    int len = 30;
    int id;
    char name[len], surname[len];
    double salary;

    int result;
    int count = 0;
    Employee_array temp;
    Employee_ptr employee;
    while (!feof(file))
    {
        result = fscanf(file, "%d %s %s %lf", &id, name, surname, &salary);
        if (result == EOF) break;
        if (result != 4)
        {
            delete_employers(*employers, count);
            return incorrect_input;
        }
        if (length <= count)
        {
            length *= 2;
            temp = (Employee_array) realloc(*employers, length * sizeof(Employee_ptr));
            if (!temp)
            {
                delete_employers(*employers, count);
                return no_memory;
            }
            *employers = temp;
        }
        employee = create_employee(id, name, surname, salary);
        if (!employee)
        {
            delete_employers(*employers, count);
            return no_memory;
        }
        
        (*employers)[count] = employee;
        count++;
    }
    return count;
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