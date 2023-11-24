#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int flag_check(char *flag){
    char flags [12][3] = {"-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f"};
    
    if (strlen(flag) != 2){
        return 0;
    }

    for (int i = 0; i < 12; ++i){
        if (strcmp(flag, flags[i]) == 0){
            return 1;
        }
    }

    return 0;
}
///**************///
int number_check(char *number){
    if (strlen(number) == 1){
        if (*number == '-'){
            return 0;
    }
    }
    if(sscanf (number, "%d", &number)){
        return 1;
    }
    else{return 0;}
}
///**************///
int flag_h(int number){
    int status = 0;
    if (number <= 0 || number > 100) {
        printf ("����� ������ ���� ������ 0 � ������ ��� ����� 100.\n");
    }
    else{
        for (int i = number; i <= 100; ++i){
            if (i % number == 0){
                printf("%d ", i);
                status = 1;
            }
        }
        if (status == 0){ printf("� �������� 100 ��� �����, ������� ����������.");} 
    }
    
}
///**************///
int flag_p(int number){
    if (number >= 2){
        for (int i = 2; i < number; ++i ){
            if (number % i == 0){
                return 0;
            }
        }
        return 1;
    }
    else if (number == 1){
        return 0;
    }
    else { return -1; }
}
///**************///
int flag_s(char *number){
    int position = 0;
    if (number[0] == '-'){
        position += 1;
    }
    if (number[position] == '0'){
        while (number[position] == '0'){
            position += 1;
        }
    }
    for (int i = position; i < strlen(number); ++i){
        printf("%c ", number[i]);
    }
}
///**************///
int calculations_for_e(int num, int pow){
    long long int total = num;
    for (int i = 1; i < pow; ++i){
        total *= num;
    }
    return total;
}

int flag_e(int number){
    if (number <= 0 || number > 10) {
        printf ("����� ������ ���� �� 1 �� 10");
    }
    else {
        printf("|  �������  |    ��������   |\n");
        printf("|-----------|---------------|\n");
        for (int i = 1; i <= number; ++i){
            printf ("|%6d     |", i);
            for (int j = 1; j <= 10; ++j){
                printf("%11lli    |\n", calculations_for_e(j, i));
                if (j != 10){
                    printf("|           |");
                }
            }
            printf("|___________|_______________|\n");
        }
    }
    
}
///**************///
int flag_a(int number){
    if (number <= 0){
        return -1;
    }
    printf("%d\n", number);
    int result = (1 + number) * number / 2;
    printf("%d\n", number);
    return result;
}
///**************///
int flag_f(unsigned long long number){
    unsigned long long total = 1;
    for (int i = 1; i <= number; ++i){
        total *= i;
    }
    printf("%llu", total);
}
///**************///
int main(int argc, char *argv[]){

    if (argc != 3){
        printf ("���������� ������ ��� ���������.");
        return 0;
    }

    if (!number_check(argv[1])){
        printf ("������� ������������ �����. ����� �� ������ ��������� ������ �������� � ����� ������.\n");
        return 0;
    }

    if (!flag_check(argv[2])){
        printf ("������ ������������ ����.\n");
        return 0;
    }

    int number = atoi(argv[1]);
    char flag = argv[2][1];
    int res;
    switch (flag){
        case 'h':
            flag_h(number);
            break;
        case 'p':
            if(flag_p(number) == 1){
                printf("����� �������");
            }
            else if (!flag_p(number)){
                printf("����� ���������");
            }
            else {printf ("����� ������ ���� ������ 0.\n");}
            break;
        case 's':
            flag_s(argv[1]);
            break;
        case 'e':
            flag_e(number);
            break;
        case 'a':
            printf("%d\n", number);
            res = flag_a(number);
            printf("%d\n", number);
            printf("%d %d", res, number);
            printf("%d\n", number);
        case 'f':
            if (number < 0){
                printf("����� ������ ���� ������ ��� ����� 0");
            }
            else if (number == 0){
                printf("%d", 1);
            }
            else {flag_f(number);}
    }

    return 0;
}