#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_error = -2,
    no_memory = -3,
    file_input_error = -4,
    can_not_calculate = -5
};

int geometric_mean(double *new_result, int quantity, ...);

void print_error(int error);

double quick_pow(double number, int power);

int main()
{
    // учесть ноль чисел
    int quantity = 0;

    //вывод функции среднего геометрического
    double result_double;
    int result = geometric_mean(&result_double, quantity, 3.3, 6.6, 1.1, 9.222);

    printf("\nFor geometric mean:");
    if (result == can_not_calculate)
    {
        print_error(result);
    }
    else printf("\nResult: %f\n\n", result_double);

    printf("\nFor quick pow:\n");
    result_double = quick_pow(-2.3, -9);
    double check_pow = pow(-2.3, -9);
    printf("Result: %f\n", result_double);
    printf("For comparison, normal pow: %f\n", check_pow);

    printf("\nProgram has finished correctly.\n");
    return success;
}

// бинарное возведение
double quick_pow(double number, int power)
{
    if (power == 0) return 1.0;
    if (number == 0.0) return number; // неопределенность

    double temp;
    if (power % 2 == 0)
    {
        temp = quick_pow(number, power / 2);
        return temp * temp;
    }
    else if (power > 0)
    {
        temp = quick_pow(number, power / 2);
        return temp * temp * number;
    }
    else
    {
        temp = quick_pow(number, (-power)/2);
        return 1.0 / (temp * temp * number);
    }
}

int geometric_mean(double *new_result, int quantity, ...)
{
    double result = 1.0;
    va_list arguments;
    double temp;
    va_start(arguments, quantity);
    for (int i = 0; i < quantity; ++i)
    {
        temp = va_arg(arguments, double);
        result *= temp;
    }
    va_end(arguments);

    if (result < 0 && quantity % 2 == 0) return can_not_calculate;
    
    *new_result = pow(fabs(result), 1.0 / quantity);
    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_error:
            printf("\nCannot open file.\n");
            break;
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.\n");
            break;
        case file_input_error:
            printf("\nError with file contents.\n");
            break;
        case can_not_calculate:
            printf("\nCannot calculate.\n");
            break;
    }
}