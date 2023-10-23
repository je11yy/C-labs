#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_error = -2,
    no_memory = -3,
    file_input_error = -4,
    can_not_calculate = -5,
    wrong = -6
};

void make_vector(double *new_coord, double *coord_first, double *coord_second);
int check_polygon(int quantity, ...);
void print_error(int error);
int find_polynomial(double *final_result, double x, int power, ...);

int main()
{
    // треугольник
    // double first[] = {-1.0, -1.0};
    // double second[] = {1.0, -1.0};
    // double third[] = {0.0, 2.0};
    // int result = check_polygon(3, first, second, third);

    // невыпуклый многоугольник
    double first[] = {1.0, 1.0};
    double second[] = {0.0, 1.0};
    double third[] = {0.0, 0.0};
    double forth[] = {1.0, 0.0};
    int result = check_polygon(4, first, second, third, forth);

    if (result == incorrect_input)
    {
        print_error(result);
        return result;
    }
    if (result == wrong)
    {
        printf("The polygon is not convex.\n");
    }
    else printf("The polygon is convex.\n");

    ////многочлен
    double x = 2;
    int power = 3;
    double double_result;
    result = find_polynomial(&double_result, x, power, 1.0, 1.0, 3.0, 3.0);
    if (result == incorrect_input) print_error(result);
    else printf("\nResult for polynomial: %f\n", double_result);

    printf("\nThe program has finished correctly.\n");
    return success;
}

// схема горнера
int find_polynomial(double *final_result, double x, int power, ...)
{
    va_list odds;
    va_start(odds, power);
    double odd = va_arg(odds, double);
    double result = odd;
    if (power >= 0)
    {
        for (int i = 0; i < power; ++i)
        {
            odd = va_arg(odds, double);
            result = result * x + odd;
        }
    }
    else
    {
        if (x == 0.0) return incorrect_input;
        for (int i = 0; i > power; --i)
        {
            odd = va_arg(odds, double);
            result = result / x + odd;
        }
    }
    va_end(odds);
    *final_result = result;
    return success;
}

void make_vector(double *new_coord, double *coord_first, double *coord_second)
{
    for (int i = 0; i < 2; ++i)
    {
        new_coord[i] = coord_second[i] - coord_first[i];
    }
}

// на вход подаются даблвские массивы
int check_polygon(int quantity, ...)
{
    if (quantity <= 2) return incorrect_input;
    
    va_list coordinats;
    va_start(coordinats, quantity);
    double *first_coords = va_arg(coordinats, double*);
    double *coords = first_coords;
    double *new_coords;

    double vector[] = {0, 0};
    double prev_vector[] = {0, 0};

    double value, prev_value;
    for (int i = 0; i < quantity - 1; ++i)
    {
        new_coords = va_arg(coordinats, double*);
        make_vector(vector, coords, new_coords);
        if (i == 1)
        {
            value = vector[0] * prev_vector[1] - vector[1] * prev_vector[0];
            prev_value = value;
        }
        else if (i > 1)
        {
            value = vector[0] * prev_vector[1] - vector[1] * prev_vector[0];
            if (value * prev_value < 0) return wrong;
            prev_value = value;
        }
        prev_vector[0] = vector[0];
        prev_vector[1] = vector[1];
        coords[0] = new_coords[0];
        coords[1] = new_coords[1];
    }
    make_vector(vector, coords, first_coords);
    value = vector[0] * prev_vector[1] - vector[1] * prev_vector[0];
    if (value * prev_value < 0) return wrong;
    prev_value = value;
    va_end(coordinats);
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