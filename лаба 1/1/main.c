#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

char flags[12][3] = {"-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f"};

// проверка, является ли флагом
int in_flags(char *item)
{
    if (strlen(item) != 2)
    {
        return 0;
    }
    for (int i = 0; i < 12; ++i)
    {
        if (strcmp(item, flags[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

//является ли числом
int is_digit(char *number)
{
    int i;
    if (number[0] == '-')
    {
        i = 1;
    }
    else i = 0;
    int length = strlen(number);
    for (i; i < length; ++i)
    {
        if (!(number[i] >= '0' && number[i] <= '9'))
        {
            return 0;
        }
    }
    return 1;
}

// проверка на валидность
int valid_check(char *number, char *flag)
{
    if (in_flags(flag) != 1)
    {
        return 0;
    }
    // проверка, является ли первый аргумент  числом
    if (is_digit(number) == 0)
    {
        return 0;
    }
    return 1;
}

//вывод кратных чисел в пределах 100 включительно
// нужно ли выводить само число???
void print_multiples(int number)
{
    if (number <= 0) 
    {
        printf("Impossible to calculate multiples because the number is negative or equal to 0.\n");
        return;
    }
    int c = 0;
    printf("Multiples of %d: ", number);
    for (int i = number + 1; i < 101; ++i)
    {
        if (i % number == 0)
        {
            printf("%d ", i);
            c += 1;
        }
    }
    if (c == 0)
    {
        printf("this number has no multiples in range 100.");
    }
    printf("\n");
    return;
}

//является ли число простым или составным
int check_prime(int number)
{
    if (number <= 1) return 0;

    for (int i = 2; i <= (int)sqrt(number); ++i)
    {
        if (number % i == 0) return 0;
    }

    return 1;
}

// число в строковом представлении
void print_number_to_figure(char *number)
{
    int i;
    int if_null = 0;
    if (number[0] == '-') i = 1;
    else i = 0;
    while (number[i] == '0') i += 1;
    printf("Number %d in string representation: ", atoi(number));
    for (i; i < strlen(number); ++i)
    {
        printf("%c ", number[i]);
    }
    printf("\n");
    return;
}

//таблица степеней
void print_power_table(int number)
{
    if (number <= 0) 
    {
        printf("Impossible to make power table because the number is negative or equal to 0.\n");
        return;
    }
    if (number > 10)
    {
        printf("Can't make the table: the number is greater than 10.\n");
        return;
    }
    printf("_______________________\n");
    printf("|base|power|  number  \n");
    printf("-----------------------\n");
    long long int preresult;
    for (int i = 1; i < 11; ++i)
    {
        preresult = 1;
        for (int j = 1; j < number + 1; ++j)
        {
            preresult *= i;
            if (i < 10 && j < 10)
            {
                printf("| %d  |  %d  |%lli\n", i, j, preresult);
            }
            else if (i < 10 && j == 10)
            {
                printf("| %d  | %d  |%lli\n", i, j, preresult);
            }
            else if (i == 10 && j < 10)
            {
                printf("| %d |  %d  |%lli\n", i, j, preresult);
            }
            else
            {
                printf("| %d | %d  |%lli\n", i, j, preresult);
            }
        }
        printf("_______________________\n");
    }
    return;
}

//сумма до числа
int sum(int number)
{
    int result = (1 + number) * number / 2;
    return result;
}

// факториал
long long int factorial(long long int number)
{
    if (number == 0) return 1;
    return number * factorial(number - 1);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("The program input is incorrect. Too low or too many arguments.\nTry to run the program again using input format: 'positive number' '-flag'");
        return 0;
    }
    if (valid_check(argv[1], argv[2]) == 0)
    {
        printf("The program input is incorrect.\nTry to run the program again using input format: 'positive number' '-flag'");
        return 0;
    }
    long long int number = atoi(argv[1]);
    int length = strlen(argv[1]);
    char state = argv[2][1];
    long long int result;
    switch(state)
    {
        case 'h':
            print_multiples(number);
            break;
        case 'p':
            result = check_prime(number);
            if (result == 1)
            {
                printf("%d is a prime number.\n", number);
                break;
            }
            if (number > 3)
            {
                printf("%d is a compound number.\n", number);
                break;
            }
            printf("%d is not a prime or a compound number.\n", number);
            break;
        case 's':
            print_number_to_figure(argv[1]);
            break;
        case 'e':
            print_power_table(number);
            break;
        case 'a':
            if (number <= 0)
            {
                printf("Impossible to calculate sum because the number is negative or equal to 0.\n");
            }
            result = sum(number);
            printf("Sum of number up to number %d: %d\n", number, result);
            break;
        case 'f':
            printf("Factorial of %lli: ", number);
            result = factorial(number);
            if (result == -1)
            {
                printf("impossible to calculate factorial because number is negative.");
                break;
            }
            printf("%lli\n", result);
            break;
    }

    return 0; 
}
