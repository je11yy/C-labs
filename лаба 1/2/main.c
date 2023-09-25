#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define PI 3.14

int is_positive_digit(char *number)
{
    if (*number == '-') return 0;
    while (*number != '\0')
    {
        if (!(*number >= '0' && *number <= '9')) return 0;
        number++;
    }
    return 1;
}

// учитывать 1e-6 
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
}

int check_is_epsilon(char* eps)
{
    int result = is_double(eps);
    if (result) return 1;
}


// факториал
double factorial(double number)
{
    if (number == 0)
    {
        return 1;
    }
    double result = 1;
    for (int i = 1; i < number + 1; ++i)
    {
        result *= i;
    }
    return result;
}

double find_e_limit(double eps)
{
    double n = 1;
    double preres = pow(1 + 1 / n, n);
    double res = pow(1 + 1 / (n + 1), n + 1);
    while (eps < fabs(res - preres))
    {
        n += 1;
        preres = res;
        res = pow(1 + 1 / (n + 1), n + 1);
    }
    return preres;
}

double find_e_row(double eps)
{
    double res = 0;
    double n = 0;
    double preres = 1 / factorial(n);
    do
    {
        res += preres;
        n += 1;
        preres = 1 / factorial(n);
    } 
    while (eps < preres);
    return preres + res;
}

double find_e_function(double eps)
{
    double preres, res = 1;
    do
    {
        //делим функцию на производную
        preres = (-1) * (log(res) - 1) * res;
        res += preres;
    //preres - разность между двумя точками
    } 
    while (eps < fabs(preres));
    return res;
}

double find_pi_limit(double eps)
{
    double n = 1;
    double preres = 0;
    //из n+1 получить n
    double res = 4.0;
    do
    {
        preres = res;
        res *= (4 * (n + 1) * n) / pow(2 * n + 1, 2);
        n++;
    } 
    while (eps < fabs(res - preres));

    return preres;
}

double find_pi_row(double eps)
{
    double res = 0;
    double n = 1;
    double preres = 0;
    do
    {
        preres = (pow(-1, n - 1)) / (2 * n - 1);
        res += preres;
        n += 1;
    } 
    while (eps < fabs(preres));
    
    return 4 * res;
}

double find_pi_function(double eps)
{
    double preres, res = 1;
    do
    {
        preres = (cos(res) + 1) / sin(res);
        res += preres;
    } 
    while (eps < fabs(preres));
    return res;
}

double find_ln2_limit(double eps)
{
    double n = 1;
    double preres, res = 0;
    do
    {
        preres = res;
        res = n * (pow(2, 1 / n) - 1);
        n++;
    } while (eps < fabs(res - preres));
    return res;
}

double find_ln2_row(double eps)
{
    double n = 1;
    double preres, res = 0;
    do
    {
        preres = (pow(-1, n - 1)) / n;
        res += preres;
        n += 1;
    } 
    while (eps < fabs(preres));
    return res;
}

double find_ln2_function(double eps)
{
    double preres, res = 1;
    do
    {
        preres = (-1) * (exp(res) - 2) / exp(res);
        res += preres;
    } 
    while (eps < fabs(preres));
    return res;
}

double find_sqrt2_limit(double eps)
{
    int n = 0;
    double res = -0.5;
    double preres = 0;
    while (eps < fabs(res - preres))
    {
        n += 1;
        preres = res;
        res = preres - pow(preres, 2) / 2 + 1;
    }
    return res;
}

double find_sqrt2_row(double eps)
{
    double n = 0;
    double power = 0.25;
    double res = pow(2, power);
    double preres;
    do
    {
        preres = res;
        power *= 0.5;
        res *= pow(2, power);
        n += 1;
    } 
    while (eps < fabs(res - preres));
    return res;
}

double find_sqrt2_function(double eps)
{
    double preres, res = 1;
    do
    {
        preres = (-1) * (pow(res, 2) - 2) / (2 * res);
        res += preres;
    } 
    while (eps < fabs(preres));
    return res;
}

double find_c(double n, double k)
{
    double result = 1;
    // типо делим n! на k!
    for (double i = k + 1; i < n + 1; ++i)
    {
        result *= i;
    }
    return result / factorial(n - k);
}

double find_gamma_limit(double eps)
{
    eps = 0.01;
    int m = 2;
    double preres = 0;
    double res = 0;
    double sum;
    double fact;
    int c;
    do
    {
        preres = res;
        sum = 0;
        fact = 1;
        for (int i = 1; i < m + 1; ++i)
        {
            fact *= i;
            c = find_c(m, i);
            sum += (c * pow(-1, i) * log(fact)) / i;
        }
        res = sum;
        m += 1;
    } 
    while (eps < fabs(res - preres));
    
    return res;
}

double find_gamma_row(double eps)
{
    double preres = 0;
    double n = 3;
    eps = 0.0000000000001;
    // floor - отбрасывает дробную часть
    double res = 0.5;
    double elem = 0;
    do
    {
        preres = res;
        elem = ((1.0 / pow(floor(sqrt(n)), 2)) - (1.0 / n));
        res += elem;
        if (elem < eps) preres = 0;
        n++;
    } 
    while (eps < fabs(res - preres));
    return (res - (pow(PI, 2) / 6));
}

int check_prime(int number)
{
    if (number <= 1) return 0;

    for (int i = 2; i <= (int)sqrt(number); ++i)
    {
        if (number % i == 0) return 0;
    }

    return 1;
}

double p_for_gamma(int t)
{
    double res = 1;
    for (int i = 2; i < t + 1; ++i)
    {
        if (check_prime(i))
        {
            res *= (i - 1.0) / i;
        }
    }
    return res;
}

double limit_for_gamma(double eps)
{
    eps = 0.0001;
    double res, preres = 0;
    int t = 2;
    do
    {
        preres = res;
        res = log(t) * p_for_gamma(t);
        t += 1;
    } 
    while (fabs(res - preres) > eps);
    return res;
    
}

double find_gamma_function(double eps)
{
    double preres, res = 1;
    do
    {
        preres = (exp(-res) - limit_for_gamma(eps)) / (exp(-res));
        res += preres;
    } 
    while (eps < fabs(preres));
    return res;
}

void menu()
{
    printf("\nProgram's menu\n\n");
    printf("1. Find e\n2. Find pi\n3. Find ln2\n4. Find sqrt(2)\n5. Find y\n0. Exit\n\n");
    printf("Please, write the number that indicates the function: ");
}

int main(int argc, char *argv[])
{
    if (argc != 2 || !check_is_epsilon(argv[1]))
    {
        printf("The program input is incorrect.\nTry to run the program again using input format: 'epsilon'.");
        return 0;
    }
    double eps = atof(argv[1]);
    char l_temp[10];
    printf("Write the number of simbols after comma: ");
    
    scanf("%s", &l_temp);
    while(is_positive_digit(l_temp) == 0)
    {
        printf("Incorrect input. Try again.\n");
        printf("Write the number of simbols after comma: ");
        scanf("%s", &l_temp);
        printf("\n");
    }
    int l = atoi(l_temp);
    int state = 9;
    char *state_temp;
    while (state != 0)
    {
        menu();
        scanf("%s", state_temp);
        printf("\n");
        while(!is_positive_digit(state_temp))
        {
            printf("\nIncorrect input! Try again.\n");
            printf("Please, write the number that indicates the function: ");
            scanf("%s", state_temp);
            printf("\n");
        }
        state = atoi(state_temp);
        double result;
        switch(state)
        {
            case 1:
                result = find_e_limit(eps);
                printf("\nLimit calculation for e: %.*f\n", l, result);
                result = find_e_row(eps);
                printf("\nRow calculation for e: %.*f\n", l, result);
                result = find_e_function(eps);
                printf("\nFunction calculation for e: %.*f\n", l, result);
                break;
            case 2:
                result = find_pi_limit(eps);
                printf("\nLimit calculation for pi: %.*f\n", l, result);
                result = find_pi_row(eps);
                printf("\nRow calculation for pi: %.*f\n", l, result);
                result = find_pi_function(eps);
                printf("\nFunction calculation for pi: %.*f\n", l, result);
                break;
            case 3:
                result = find_ln2_limit(eps);
                printf("\nLimit calculation for ln2: %.*f\n", l, result);
                result = find_ln2_row(eps);
                printf("\nRow calculation for ln2: %.*f\n", l, result);
                result = find_ln2_function(eps);
                printf("\nFunction calculation for ln2: %.*f\n", l, result);
                break;
            case 4:
                result = find_sqrt2_limit(eps);
                printf("\nLimit calculation for sqrt(2): %.*f\n", l, result);
                result = find_sqrt2_row(eps);
                printf("\nRow calculation for sqrt(2): %.*f\n", l, result);
                result = find_sqrt2_function(eps);
                printf("\nFunction calculation for sqrt(2): %.*f\n", l, result);
                break;
            case 5:
                result = find_gamma_limit(eps);
                printf("\nLimit calculation for gamma: %.*f\n", l, result);
                result = find_gamma_row(eps);
                printf("\nRow calculation for gamma: %.*f\n", l, result);
                result = find_gamma_function(eps);
                printf("\nFunction calculation for gamma: %.*f\n", l, result);
                break;
            case 0:
                return 0;
        }
    }
}