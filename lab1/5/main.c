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

// необязательно, ибо указано, что вещественное
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
    if (strcmp(eps, ".") == 0) return 0;
    int result = is_double(eps);
    if (result) return 1;
    result = is_epsilon_representation(eps);
    if (result) return 1;
    return 0;
}

double row_a(double eps, double x)
{
    double n = 1.0;
    double sum = 1.0 + x, element = x;
    do
    {
        element *= x / (n + 1.0);
        sum += element;
        n++;
    } 
    while (fabs(element) > eps);
    return sum;
}

double row_b(double eps, double x)
{
    double n = 1.0;
    double sum = 1.0 - ((x * x) / 2.0), element = sum - 1.0;
    do
    {
        element *= ((-1.0) * x * x) / ((2.0 * n + 2.0) * (2.0 * n + 1.0));
        sum += element;
        n++;
    } 
    while (fabs(element) > eps);
    return sum;
}


double row_c(double eps, double x)
{
    double n = 1.0;
    double sum = 1.0 + ((9.0 * x * x) / 2.0), element = ((9.0 * x * x) / 2.0);
    do
    {
        element *= (9.0 * (n + 1.0) * (n + 1.0) * x * x) / ((3.0 * n + 2.0) * (3.0 * n + 1.0));
        sum += element;
        n++;
    } 
    while (fabs(element) > eps);
    return sum;
}

double row_d(double eps, double x)
{
    double n = 1.0;
    double sum = ((-1.0) * x * x) / 2.0, element = sum;
    do
    {
        element *= ((-1.0) * (2.0 * n + 1.0) * x * x) / (2.0 * n + 2.0);
        sum += element;
        n++;
    } 
    while (fabs(element) > eps);
    return sum;
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

int is_x(char *number)
{
    if (is_digit(number) || is_double(number)) return 1;
    return 0;
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
    char x[10];
    printf("Enter x to calculate rows: ");
    scanf("%s", &x);
    result = is_x(x);
    while (!result)
    {
        printf("Incorrect input! Try again.\nEnter x to calculate rows: ");
        scanf("%s", &x);
        result = is_x(x);
    }
    double res = atof(x);
    int l = 20;

    printf("Result for row a: %.*f\n", l, row_a(eps, res));
    printf("Result for row b: %.*f\n", l, row_b(eps, res));

    if (fabs(res) < 1)
    {
        printf("Result for row c: %.*f\n", l, row_c(eps, res));
        printf("Result for row d: %.*f\n", l, row_d(eps, res));
    }
    else printf("Can't solve row c and d with this x.");

    return 0;
}