#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"
#include "post.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2
};

void print_error(int error);

int validation(char * town, char * street, int building, char * housing, int apartment, char * index);
void starting();
void print_adress_rules();
int do_command(int command, Post_ptr post, int *length);
int mail(Post_ptr post, int * length);
int validate_time(int day, int month, int year, int hours, int minutes, int seconds);
int menu();
void delete_mail_with_ind(Post_ptr post, int *length);

int main()
{
    starting();
    int size = 20;
    char town[size], street[size], housing[size], index[size];
    int building, apartment;
    int result = -1;

    while (result != success)
    {
        result = scanf("%s %s %d %s %d %s", town, street, &building, housing, &apartment, index);
        if (result == 6)
        {
            result = validation(town, street, building, housing, apartment, index);
            if (result == success) break;
        }
        else if (result == 0) result = -1;
        printf("\n\nSomething went wrong! Try again.\n");
        fflush(stdin);
        printf("Input form: ");
    }

    // post adress creation
    Adress_ptr post_adress = create_adress(town, street, building, housing, apartment, index);

    // post creation
    int length = 0;
    Post_ptr post = create_post(post_adress);

    printf("\nWonderful! You created the post with adress:\n");
    print_adress(post -> post_adress);

    int command = -1;
    while (command != success)
    {
        command = menu();
        if (command == success) break;
        result = do_command(command, post, &length);
        if (result != success)
        {
            print_error(result);
            return result;
        }
    }

    delete_post(post, length);

    printf("\n\nProgram has finished correctly.\n");
}

int do_command(int command, Post_ptr post, int * length)
{
    int result = 0;
    switch (command)
    {
        case 1:
            result = mail(post, length);
            if (result == no_memory) return no_memory;
            break;
        case 2:
            print_all_mails(post, *length);
            result = success;
            break;
        case 3:
            print_delivered_mails(post, *length);
            result = success;
            break;
        case 4:
            delete_mail_with_ind(post, length);
            result = success;
        default: break;
    }
    return success;
}

void delete_mail_with_ind(Post_ptr post, int *length)
{
    int result;
    printf("\nInput the identifier of the mail: ");
    char indef[20];
    result = -1;
    while (result != success)
    {
        result = scanf("%s", indef);
        if (result <= 0)
        {
            printf("\nTry again: ");
        }
        else
        {
            if (strlen(indef) == 14) result = validate_string_digits(indef);
            if (result == success) 
            {
                result = find_with_post_index(post -> mails, indef, *length);
                if (result != -1) break;
                printf("\nTry again: ");
            }
        }
    }
    remove_mail(&post, result, *length);
    (*length)--;
}

int mail(Post_ptr post, int * length)
{
    int result = -1;
    int size = 40;
    char town[size], street[size], housing[size], index[size], post_index[size];
    int building, apartment;
    printf("\nLet's create a mail!\n");
    printf("At first let's write adress of receiver.\n\n");
    print_adress_rules();
    printf("Input form: ");
    while (result != success)
    {
        result = scanf("%s %s %d %s %d %s", town, street, &building, housing, &apartment, index);
        if (result == 6)
        {
            result = validation(town, street, building, housing, apartment, index);
            if (result == success) break;
        }
        else if (result == 0) result = -1;
        fflush(stdin);
        printf("\n\nSomething went wrong! Try again.\n");
        printf("Input form: ");
    }
    printf("\n\n");

    printf("Input weight of the mail.\n");
    printf("Input form: ");
    double weight;
    result = -1;
    while(result != success)
    {
        result = scanf("%lf", &weight);
        if (result > 0)
        {
            result = check_is_weight(weight);
            if (result == success) break;
        }
        else if (result == 0) result = -1;
        fflush(stdin);
        printf("Something went wrong! Try again.\n");
        printf("Input form: ");
    }

    printf("\n\nNow write down the post identifier. It should contain only digits and be 14 symbols length.\n");
    printf("Input form: ");
    result = scanf("%s", post_index);
    if (result)
    {
        result = validate_string_digits(post_index);
        if (find_with_post_index(post -> mails, post_index, *length) != -1)
        {
            printf("\nMail with this identifier already exist.\n\n");
            result = fail;
        }
        if (result == success && strlen(post_index) != 14) result = fail;
    }
    while (result != success)
    {
        fflush(stdin);
        printf("\n\nSomething went wrong! Try again.\n");
        printf("Input form: ");
        result = scanf("%s", post_index);
        if (result)
        {
            result = validate_string_digits(post_index);
            if (find_with_post_index(post -> mails, post_index, *length) != -1)
            {
                printf("\nMail with this identifier already exist.\n\n");
                result = fail;
            }
            if (result == success && strlen(post_index) != 14) result = fail;
        }
        else if (result == 0) result = -1;
    }

    printf("\n\nFew steps left... Write down the time when the mail was created.\n");
    printf("Be careful! Use the format: dd:mm:yyyy hh:mm:ss\n");
    printf("Input form: ");
    char time[size];
    int day = 0, month = 0, year = 0, hours = 0, minutes = 0, seconds = 0;
    
    // не факт что пользователь введет числа с 0 впереди, но это будет исправлено при создании строки
    result = scanf("%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    if (result) result = validate_time(day, month, year, hours, minutes, seconds);
    while (result != success)
    {
        printf("\n\nSomething went wrong! Try again.\n");
        fflush(stdin);
        printf("Input form: ");
        result = scanf("%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
        if (result)
        {
            result = validate_time(day, month, year, hours, minutes, seconds);
            if (result == success) break;
        }
        else if (result == 0) result = -1;
    }
    // создание строки времени
    sprintf(time, "%02d:%02d:%04d %02d:%02d:%02d", day, month, year, hours, minutes, seconds);

    printf("\nAnd the last one! Same rules as for the previous. Write down time when mail will be delivered.\n");
    printf("Input form: ");
    char time1[size];
    result = scanf("%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    if (result)
    {
        result = validate_time(day, month, year, hours, minutes, seconds);
        if (result == success)
        {
            // создание строки времени
            sprintf(time1, "%02d:%02d:%04d %02d:%02d:%02d", day, month, year, hours, minutes, seconds);
            result = time_diff_char(time1, time);
            if (result > 0) result = success;
            else printf("DELIVER TIME MUST BE MORE THAN MAIL-MADE TIME.\n");
        }
    }
    while (result != success)
    {
        printf("\n\nSomething went wrong! Try again.\n");
        fflush(stdin);
        printf("Input form: ");
        result = scanf("%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
        if (result == 0) result = -1;
        else if (result == 6)
        {
            result = validate_time(day, month, year, hours, minutes, seconds);
            if (result == success)
            {
                // создание строки времени
                sprintf(time1, "%02d:%02d:%04d %02d:%02d:%02d", day, month, year, hours, minutes, seconds);
                result = time_diff_char(time1, time);
                if (result >= 0) result = success;
                else printf("DELIVER TIME MUST BE MORE THAN MAIL-MADE TIME.\n");
            }
        }
    }
    Adress_ptr adress = create_adress(town, street, building, housing, apartment, index);
    if (adress == NULL)
    {
        delete_post(post, *length);
        return no_memory;
    }

    Mail_ptr mail = create_mail(adress, weight, post_index, time, time1);
    if (mail == NULL)
    {
        delete_adress(adress);
        delete_post(post, *length);
        return no_memory;
    }
    ///////
    result = append_mail(mail, post, *length);
    (*length)++;
    if (result == no_memory)
    {
        delete_mail(mail);
        delete_post(post, *length);
        return no_memory;
    }
    printf("\n\nThe mail was created!\n");
    return success;
}

int validate_time(int day, int month, int year, int hours, int minutes, int seconds)
{
    // day
    if (day <= 0 || day > 31) return fail;
    if (month <= 0 || month > 12) return fail;
    if (year < 1900 || year > 9999) return fail;
    if (hours < 0 || hours > 24) return fail;
    if (minutes < 0 || minutes > 59) return fail;
    if (seconds < 0 || seconds > 59) return fail; 
    return success;
}

int menu()
{
    printf("\nCommands menu\n\n");
    printf("1. Create new mail and insert in post's array.\n");
    printf("2. Print all created mails.\n");
    printf("3. Print delivered mails.\n");
    printf("4. Delete mail with inputted post index.\n");
    printf("0. Finish the program.\n");

    int command;
    printf("Input form: ");
    while (!scanf("%d", &command))
    {
        fflush(stdin);
        printf("\n\nTry again!\n");
        printf("Input form: ");
    }
    printf("\n");
    return command;
}

int validation(char * town, char * street, int building, char * housing, int apartment, char * index)
{
    int result;
    // town
    result = validate_string_alphas(town);
    if (result != success) return fail;
    result = check_lowercase(town);
    if (result != success) return fail;

    // street
    result = validate_string_alphas(street);
    if (result != success) return fail;
    result = check_lowercase(street);
    if (result != success) return fail;

    // building
    result = check_building_and_apartment(building);
    if (result != success) return fail;

    // housing
    result = validate_string_alphas_digits(housing);
    if (result != success) return fail;

    // apartment
    result = check_building_and_apartment(apartment);
    if (result != success) return fail;

    // index
    result = validate_string_digits(index);
    if (result != success) return fail;
    if (strlen(index) != 6) return fail;

    return success;
}

void starting()
{
    printf("Welcome to the program! Let's create the post where you will make mails.\n\n");
    printf("Please, write adress of the post in special input form.\n");
    printf("If the adress do not have housing, write -. Other points should be written normally.\n");
    print_adress_rules();
    printf("Input form: ");
}

void print_adress_rules()
{
    printf("Rules:\n");
    printf("Town and street should have letters ONLY, lowercase.\n");
    printf("Building should be more than 0.\n");
    printf("Housing should be digits + letters or -, lowercase\n");
    printf("Apartment should more than 0.");
    printf("Index should have digits ONLY\n\n");
    printf("Input adress in format: town street building housing apartment index\n");
    printf("Example: moskva velikaya 12 - 182 125475\n\n");
}

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
    }
}