#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

double UNDEFINED = -1.111111111;

// проверка, является ли флагом
int in_flags(char *item)
{
    if (strlen(item) != 2) return 0;
    char flags[6][3] = {"-q", "/q", "-m", "/m", "-t", "/t"};
    if (item[0] != '-' || item[0] != '-') return 0;
    for (int i = 0; i < 12; ++i)
    {
        if (item[1] == flags[i][1]) return 1;
    }
    return 0;
}

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

int is_digit(char *number)
{
    if (!strcmp(number, "-")) return 0;
    if (*number == '-') number++;
    while (*number != '\0')
    {
        if (!(*number >= '0' && *number <= '9')) return 0;
        number++;
    }
    return 1;
}

int check_valid_double(int size, char *argv[])
{
    for (int i = 3; i < size; ++i)
    {
        if (!is_double(argv[i])) return 0;
    }
    return 1;
}

int check_valid_int(int size, char *argv[])
{
    for (int i = 2; i < size; ++i)
    {
        if (!is_digit(argv[i])) return 0;
    }
    return 1;
}

//кратность
int multiples(int number_1, int number_2)
{
    if (number_1 % number_2 == 0) return 1;
    return 0;
}


void function_solution(double* solutions, double eps, double *odds)
{
    double a = odds[0];
    double b = odds[1];
    double c = odds[2];
    double discriminant = b * b - 4 * a * c;
    if (discriminant > eps)
    {
        solutions[0] = ((-b) + sqrt(discriminant)) / (2 * a);
        solutions[1] = ((-b) - sqrt(discriminant)) / (2 * a);
        return;
    }
    if (discriminant == eps)
    {
        solutions[0] = (-b) / (2 * a);
        solutions[1] = UNDEFINED;
        return;
    }
    solutions[0] = UNDEFINED;
    solutions[1] = UNDEFINED;
    return;
}

//перестановка
int permutations(double *odds, int length, double eps)
{
    int i = length - 2;
    while (i != -1 && (odds[i] - odds[i + 1]) >= eps) i--;
    if (i == -1) return 0;
    int j = length - 1;
    while ((odds[i] - odds[j]) >= eps) j--;

    double temp = odds[i];
    odds[i] = odds[j];
    odds[j] = temp;
    
    int left = i + 1;
    int right = length - 1;
    while (left < right)
    {
        double temp = odds[left];
        odds[left] = odds[right];
        odds[right] = temp;
        left++;
        right--;
    }
    return 1;
}

double find_max(double a, double b, double c, double eps)
{
    if (a - b > eps)
    {
        if (c - a > eps) return c;
        return a;
    }
    if (b - a > eps)
    {
        if (c - b > eps) return c;
        return b;
    }
}

double find_min(double a, double b, double c, double eps)
{
    if (a - b > eps)
    {
        if (c - a < eps) return c;
        return a;
    }
    if (b - a < eps)
    {
        if (c - b < eps) return c;
        return b;
    }
}

int triangle(double eps, double a, double b, double c)
{
    if (!(a > eps && b > eps && c > eps))
    {
        return 0;
    }
    double maximum = find_max(a, b, c, eps);
    double minimum = find_min(a, b, c, eps);
    if (pow(maximum, 2) - (pow(minimum, 2) + pow(a + b + c - maximum - minimum, 2)) < eps)
    {
        return 1;
    }
    return 0;
}

// пузырек
void sort_array(double *array, int length)
{
    double temp;
    for (int i = 0; i < length - 1; ++i)
    {
        for (int j = 0; j < length - i - 1; ++j)
        {
            if (array[j] > array[j + 1])
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc == 1 || !in_flags(argv[1]))
    {
        printf("Incorrect input. Please, try again with format: '-flag' 'val1' ... ");
        return 0;
    }
    char state = argv[1][1];
    int size = argc - 3;
    switch(state)
    {
        case 'q':
            if (argc != 6 || (!check_valid_double(size, argv))|| (!check_is_epsilon(argv[2])))
            {
                printf("Incorrect input. Please, try again with format: '-flag' 'epsilon' 'val1' 'val2' 'val3' ");
                return 0;
            }
            double eps = atof(argv[2]);
            double a = atof(argv[3]);
            double b = atof(argv[4]);
            double c = atof(argv[5]);
            double odds[] = { a, b, c };
            int length = 3;
            double solutions[2] = {0};
            sort_array(odds, length);
            do
            {
                printf("Solutions for %.3fx^2 + %.3fx + %.3f: ", odds[0], odds[1], odds[2]);
                function_solution(solutions, eps, odds);
                if (solutions[0] != UNDEFINED) printf("x = %.3f ", solutions[0]);
                if (solutions[1] != UNDEFINED) printf("x = %.3f ", solutions[1]);
                else printf("no solutions");
                printf("\n\n");
            } 
            while (permutations(odds, length, eps));
            return 0;

        case 'm':
            if (argc != 4 || (!check_valid_int(size, argv)))
            {
                printf("Incorrect input. Please, try again with format: '-flag' 'val1' 'val2' ");
                return 0;
            }
            int a_1 = atoi(argv[2]);
            int b_1 = atoi(argv[3]);
            if (a_1 == 0 || b_1 == 0)
            {
                printf("Incorrect input: 'val1' and 'val2' must be not equal to 0. Please, try again.");
                return 0;
            }
            if (!multiples(a_1, b_1))
            {
                printf("%d is not multiple of %d\n", a_1, b_1);
                return 0;
            }
            printf("%d is a multiple of %d\n", a_1, b_1);
            return 0;

        case 't':
            if (argc != 6 || (!check_valid_double(size, argv)))
            {
                printf("Incorrect input. Please, try again with format: '-flag' 'epsilon' 'val1' 'val2' 'val3' ");

                return 0;
            }
            double eps_1 = atof(argv[2]);
            double a_2 = atof(argv[3]);
            double b_2 = atof(argv[4]);
            double c_2 = atof(argv[5]);
            if(!triangle(eps_1, a_2, b_2, c_2))
            {
                printf("%.1f, %.1f, %.1f can't be lengths of a right triangle.", a_2, b_2, c_2);
                return 0;
            }
            printf("%.1f, %.1f, %.1f can be lengths of a right triangle.", a_2, b_2, c_2);
            return 0;
    }
    return 0;
}