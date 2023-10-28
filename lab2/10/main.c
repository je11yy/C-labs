#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

enum RESULT
{
    success = 0,
    fail = -1,
    incorrect_input = -2,
    no_memory = -3
};
int polynomial_decomposition(double ** result, double epsilon, double a, int power, ...);
double deritative(double (*function)(double, int, double*), double x, double eps, int power, double * odds);
double function(double x, int power, double * odds);
void print_error(int error);

int main()
{
    int power = 3;
    double eps = 0.0001;
    double a = 2;
    double * odds = NULL;
    
    int result = polynomial_decomposition(&odds, eps, a, power, 0.33, -1.1, 1.025, -5.4);
    if (result == no_memory)
    {
        print_error(result);
        return result;
    }

    printf("Result: ");
    int cur_pow;
    for (int i = 0; i <= abs(power); ++i)
    {
        if (power < 0) cur_pow = -i;
        else cur_pow = i;
        if (odds[i] > -eps && odds[i] < eps) continue;
        if (i != 0)
        {
            if (odds[i] < eps) printf(" - ");
            else printf(" + ");
        }
        if (i == 0) printf("%.3f", odds[i]);
        else printf("%.3f", fabs(odds[i]));
        if (i == 0) continue;
        if (a > -eps && a < eps && i == 1) printf(" * x");
        else if (a > -eps && a < eps) printf(" * x^%d", cur_pow);
        else if (a < eps) printf(" * (x + %.1f)", fabs(a));
        else if (a < eps) printf(" * (x + %.1f)^%d", fabs(a), cur_pow);
        else if (i == 1) printf(" * (x - %.1f)", a);
        else printf(" * (x - %.1f)^%d", a, cur_pow);
    }
    printf("\n");
    printf("\nProgram has finished correctly.\n");
    return success;
}

double function(double x, int power, double * odds)
{
    double x_new = 1;
    double result = 0;
    for (int i = 0; i <= power; ++i)
    {
        result += x_new * odds[i];
        x_new *= x;
    }
    return result;
}

double deritative(double (*function)(double, int, double*), double x, double eps, int power, double * odds)
{
    for (int i = 0; i < power; ++i)
    {
        odds[i] = (i + 1) * odds[i + 1];
    }
    return function(x, power - 1, odds);
}

// формула Тейлора
int polynomial_decomposition(double ** result, double epsilon, double a, int power, ...)
{
    va_list odds;
    va_start(odds, power);
    double odd;
    if (power < 0) power = abs(power);

    double * odds_list = (double*)malloc((power + 1) * sizeof(double));
    if (odds_list == NULL)
    {
        va_end(odds);
        return no_memory;
    }
    double * temp;
    double function_result;
    double * result_odds_list = (double*)malloc((power + 1) * sizeof(double));
    if (result_odds_list == NULL)
    {
        free(odds_list);
        odds_list = NULL;
        va_end(odds);
        return no_memory;
    }

    for (int i = 0; i <= power; ++i)
    {
        odd = va_arg(odds, double);
        odds_list[i] = odd;
    }
    // проверка, что а - ноль?
    if (a > -epsilon && a < epsilon)
    {
        *result = odds_list;
        va_end(odds);
        free(result_odds_list);
        result_odds_list = NULL;
        return success;
    } 

    function_result = function(a, power, odds_list);
    int factorial = 1;
    int power_for_deritative = power;
    for (int i = 0; i <= power; ++i)
    {
        result_odds_list[i] = function_result / factorial;
        factorial *= (i + 1);
        function_result = deritative(function, a, epsilon, power_for_deritative, odds_list);
        power_for_deritative--;
    }
    *result = result_odds_list; 
    va_end(odds);

    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("Incorrect input.\n");
            break;
        case no_memory:
            printf("Computer doesnot have enough memory for this calculation.\n");
            break;
    }
}