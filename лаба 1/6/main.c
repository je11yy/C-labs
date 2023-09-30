#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int is_double(char* number)
{
    int c = 0;
    if (*number == '-') return 0;
    while (*number != '\0')
    {
        if (*number == '.')
        {
            if (c == 0) c += 1;
            else return 0;
        }
        else if (!(*number >= '0' && *number <= '9')) return 0;
        number++;
    }
    return 1;
}

int is_epsilon_representation(char* eps)
{
    int length = strlen(eps);
    if (length < 4) return 0;
    if (eps[0] != '1' && eps[1] != 'e' && eps[2] != '-') return 0;
    for (int i = 3; i < length; ++i)
    {
        if(!(eps[i] >= '0' && eps[i] <= '9')) return 0;
    }
    return 1;
}

int check_is_epsilon(char* eps)
{
    int result = is_double(eps);
    if (result) return 1;
    result = is_epsilon_representation(eps);
    if (result) return 1;
    return 0;
}

double function_a(double x)
{
    return (log(1 + x) / x);
}

double function_b(double x)
{
    return exp(((-1.0) * x * x) / 2.0);
}

double function_c(double x)
{
    return log(1.0 / (1.0 - x));
}

double function_d(double x)
{
    return pow(x, x);
}

// формула парабол
double integral_a(double eps)
{
    double a = 0, b = 1.0, square = 0;
    double h = (b - a) * eps;
    double x = a + h;
    while (x - b < eps)
    {
        // нечетные индексы
        square += 4.0 * function_a(x);
        x += h;
        if (x - b >= eps) break;
        // четные индексы
        square += 2.0 * function_a(x);
        x += h;
    }
    //первое и последнее значение функции прибавляется без умножения
    square = (h / 3.0) * (square + function_a(b));
    return square;
}

double integral_b(double eps)
{
    double a = 0, b = 1.0, square = 0;
    double h = (b - a) * eps;
    double x = a + h;
    while (x - b < eps)
    {
        // нечетные индексы
        square += 4 * function_b(x);
        x += h;
        if (x - b >= eps) break;
        // четные индексы
        square += 2 * function_b(x);
        x += h;
    }
    //первое и последнее значение функции прибавляется без умножения
    square = (h / 3.0) * (square + 1.0 + exp(-0.5));
    return square;
}

double integral_c(double eps)
{
    double a = 0, b = 1.0;
    double square = 0;
    double h = (b - a) * eps;
    double x = a + h;
    while (x < b)
    {
        // нечетные индексы
        square += 4 * function_c(x);
        x += h;
        if (x >= b) break;
        // четные индексы
        square += 2 * function_c(x);
        x += h;
    }
    //первое и последнее значение функции прибавляется без умножения
    square = (h / 3.0) * square;
    return square;
}

double integral_d(double eps)
{
    double a = 0, b = 1.0, square = 0;
    double h = (b - a) * eps;
    double x = a + h;
    while (x - b < eps)
    {
        // нечетные индексы
        square += 4 * function_d(x);
        x += h;
        if (x - b >= eps) break;
        // четные индексы
        square += 2 * function_d(x);
        x += h;
    }
    //первое и последнее значение функции прибавляется без умножения
    square = (h / 3.0) * (square + 2.0);
    return square;
}

int main(int argc, char *argv[])
{
     if (argc != 2)
    {
        printf("Incorrect input! Please, use the format: epsilon");
        return 0;
    }
    int result = check_is_epsilon(argv[1]);
    if (!result)
    {
        printf("Incorrect input! Please, use the format: epsilon");
        printf("You can write epsilon as a double (example: 0.001) or in epsilon representation (example: 1e-3)");
        return 0;
    }
    double eps = atof(argv[1]);
    int l = 10;
    printf("Result for integral a: %.*f\n", l, integral_a(eps));
    printf("Result for integral b: %.*f\n", l, integral_b(eps));
    printf("Result for integral c: %.*f\n", l, integral_c(eps));
    printf("Result for integral d: %.*f\n", l, integral_d(eps));
}