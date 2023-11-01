#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "string.h"
#include "post.h"

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2
};

void print_adress(Adress_ptr adress)
{
    printf("%s ", adress -> town -> buffer);
    printf("%s ", adress -> street -> buffer);
    printf("%d ", adress -> building);
    printf("%s ", adress -> housing -> buffer);
    printf("%d ", adress -> apartment);
    printf("%s\n", adress -> index -> buffer);
}

void print_delivered_mails(Post_ptr post, int length)
{
    if (post -> mails == NULL)
    {
        printf("NO DELIVERED MAILS\n");
        return;
    }
    int cur_index = -1;
    int count = 0;
    for (int i = 0; i < length; ++i)
    {
        cur_index = find_minimum_time_more_than(post -> mails, cur_index, length);
        if (check_time((post -> mails)[cur_index] -> time_take) == success)
        {
            printf("%d)\n", count + 1);
            print_mail_info((post -> mails)[cur_index]);
            count++;
        }
    }
    if (count == 0) printf("NO DELIVERED MAILS\n");
}

int find_with_post_index(Mail_array mails, char * post_index, int length)
{
    if (mails == NULL) return -1;
    for (int i = 0; i < length; ++i)
    {
        if (strcmp(post_index, mails[i] -> post_index -> buffer) == success) return i;
    }
    return -1;
}

// найти наименьше, которое больше второго аргумента
int find_minimum_time_more_than(Mail_array mails, int time_index, int length)
{
    int index, result;
    if (length == 1) return 0;
    String_ptr cur_time = mails[0] -> time_made;
    // найти самое минимальное
    for (int i = 0; i < length; ++i)
    {
        result = time_diff(cur_time, mails[i] -> time_made);
        if (result > 0)
        {
            cur_time = mails[i] -> time_made;
            index = i;
        }
    }
    // если надо найти просто минимальное
    if (time_index == -1) return index;
    
    // найти минимальное, но большее второго аргумента

    // найти любое, большее чем второй аргумент
    for (int i = 0; i < length; ++i)
    {
        result = time_diff(mails[time_index] -> time_made, mails[i] -> time_made);
        if ((result == 0 && i > time_index) || result < 0)
        {
            cur_time = mails[i] -> time_made;
            index = i;
            break;
        }
    }
    for (int i = 0; i < length; ++i)
    {
        result = time_diff(mails[time_index] -> time_made, mails[i] -> time_made);
        if (result < 0 && time_diff(cur_time, mails[i] -> time_made) > 0)
        {
            cur_time = cur_time = mails[i] -> time_made;
            index = i;
        }
    }
    return index;
}

int time_diff(String_ptr time_str1, String_ptr time_str2)
{
    time_t time1;
    int day, month, year, hours, minutes, seconds;
    sscanf(time_str1 -> buffer, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    struct tm ttime;
    ttime.tm_mday = day;
    ttime.tm_mon = month - 1;
    ttime.tm_year = year - 1900;
    ttime.tm_hour = hours;
    ttime.tm_min = minutes;
    ttime.tm_sec = seconds;
    ttime.tm_isdst = -1;
    time_t time2;
    sscanf(time_str2 -> buffer, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    struct tm ttime1;
    ttime1.tm_mday = day;
    ttime1.tm_mon = month - 1;
    ttime1.tm_year = year - 1900;
    ttime1.tm_hour = hours;
    ttime1.tm_min = minutes;
    ttime1.tm_sec = seconds;
    ttime1.tm_isdst = -1;

    time_t time_1 = mktime(&ttime);
    time_t time_2 = mktime(&ttime1);

    if (time_1 < time_2) return -1; 
    if (time_1 > time_2) return 1;
    return 0;
}

int time_diff_char(char * time_str1, char * time_str2)
{
    time_t time1;
    int day, month, year, hours, minutes, seconds;
    sscanf(time_str1, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    struct tm ttime;
    ttime.tm_mday = day;
    ttime.tm_mon = month - 1;
    ttime.tm_year = year - 1900;
    ttime.tm_hour = hours;
    ttime.tm_min = minutes;
    ttime.tm_sec = seconds;
    ttime.tm_isdst = -1;
    time_t time2;
    sscanf(time_str2, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    struct tm ttime1;
    ttime1.tm_mday = day;
    ttime1.tm_mon = month - 1;
    ttime1.tm_year = year - 1900;
    ttime1.tm_hour = hours;
    ttime1.tm_min = minutes;
    ttime1.tm_sec = seconds;
    ttime1.tm_isdst = -1;

    time_t time_1 = mktime(&ttime);
    time_t time_2 = mktime(&ttime1);

    if (time_1 < time_2) return -1; 
    if (time_1 > time_2) return 1;
    return 0;
}

// дошла ли посылка до получателя
int check_time(String_ptr time_input)
{
    int day, month, year, hours, minutes, seconds;
    sscanf(time_input -> buffer, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);
    struct tm ttime;
    ttime.tm_mday = day;
    ttime.tm_mon = month - 1;
    ttime.tm_year = year - 1900;
    ttime.tm_hour = hours;
    ttime.tm_min = minutes;
    ttime.tm_sec = seconds;
    ttime.tm_isdst = -1;

    time_t time_cur = time(NULL);

    time_t input_time = mktime(&ttime);

    if (input_time < time_cur) return success;
    return fail;
}

void print_mail_info(Mail_ptr mail)
{
    printf("Receiver's adress: ");
    print_adress(mail -> adress);
    printf("Weight: %f\n", mail -> weight);
    printf("Post index: %s\n", mail -> post_index -> buffer);
    printf("Time when mail was created: %s\n", mail -> time_made -> buffer);
    int result = check_time(mail -> time_take);
    if (result == success)
    {
        printf("The mail is delivered!\n");
        return;
    }
    else printf("The mail on its way to be delivered...\n");
}

void print_all_mails(Post_ptr post, int length)
{
    printf("Mails in post:\n\n");
    if (length == 0)
    {
        printf("No mails in this post!\n");
        return;
    }
    for (int i = 0; i < length; ++i)
    {
        printf("\n%d)\n", i + 1);
        print_mail_info((post -> mails)[i]);
    }
}

Post_ptr create_post(Adress_ptr adress) // создаем новое почтовое отделение
{
    Post_ptr post = (Post_ptr) malloc(sizeof(Post));
    if (!post) return NULL;
    post -> post_adress = adress;
    post -> mails = NULL;
    return post;
}

// город, улица
int validate_string_alphas(char * string)
{
    int length = strlen(string);
    if (length == 0) return fail;
    for (int i = 0; i < length; ++i)
    {
        if (!isalpha(string[i])) return fail;
    }
    return success;
}

int check_lowercase(char * string)
{
    int length = strlen(string);
    for (int i = 0; i < length; ++i)
    {
        if (!islower(string[i])) return fail;
    }
    return success;
}

void insert_mail(int length, Post_ptr post, Mail_ptr mail)
{
    int result;
    Mail_ptr temp, temp1;
    for (int i = 0; i < length - 1; ++i)
    {
        // индекс получателя
        // 0 - равны, 1 - в массиве больше
        result = strings_order_relation((post -> mails)[i] -> adress -> index, mail -> adress -> index);
        if (result == 0)
        {
            // почтовый индентификатор
            result = strings_order_relation((post -> mails)[i] -> post_index, mail -> post_index);
            if (result == 1)
            {
                temp = (post -> mails)[i];
                (post -> mails)[i] = mail;
                for (int j = i + 1; j < length; ++j)
                {
                    temp1 = (post -> mails)[j];
                    (post -> mails)[j] = temp;
                    temp = temp1;
                }
                return;
            }
        }
        if (result == 1)
        {
            temp = (post -> mails)[i];
            (post -> mails)[i] = mail;
            for (int j = i + 1; j < length; ++j)
            {
                temp1 = (post -> mails)[j];
                (post -> mails)[j] = temp;
                temp = temp1;
            }
            return;
        }
    }
    (post -> mails)[length - 1] = mail;
    return;
}

int append_mail(Mail_ptr mail, Post_ptr post, int length)
{
    Mail_array temp;
    temp = (Mail_array)realloc(post -> mails, (length + 1) * sizeof(Mail_ptr));
    if (!temp)
    {
        delete_post(post);
        delete_mail(mail);
        return no_memory;
    }
    post -> mails = temp;
    insert_mail(length + 1, post, mail);
    return success;
}

// удаление по индексу
int remove_mail(Post_ptr post, int index, int length)
{
    Mail_ptr temp_mail = (post -> mails)[index];
    print_mail_info(temp_mail);
    printf("\n\nindex: %d length: %d\n\n", index, length);
    for (int i = index; i < length - 1; ++i)
    {
        (post -> mails)[i] = (post -> mails)[i + 1];
    }
    delete_mail(temp_mail);
    length--;
    Mail_array temp = (Mail_array)realloc(post -> mails, length);
    if (!temp)
    {
        delete_post(post);
        return no_memory;
    }
    post -> mails = temp;
    return success;
}

void delete_adress(Adress_ptr adress)
{
    delete_string(adress -> town);
    delete_string(adress -> street);
    delete_string(adress -> housing);
    delete_string(adress -> index);
    free(adress);
    adress = NULL;
}

void delete_mail(Mail_ptr mail)
{
    delete_adress(mail -> adress);
    delete_string(mail -> post_index);
    delete_string(mail -> time_made);
    delete_string(mail -> time_take);
    free(mail);
    mail = NULL;
}

void delete_post(Post_ptr post)
{
    delete_adress(post -> post_adress);
    int length;
    if (post -> mails != NULL) length = sizeof(post -> mails) / sizeof((post -> mails)[0]);
    else length = 0;
    for (int i = 0; i < length; ++i)
    {
        delete_mail((post -> mails)[i]);
    }
    free(post);
    post = NULL;
}

// корпус
int validate_string_alphas_digits(char * string)
{
    if (strcmp(string, "-") == success) return success;
    int length = strlen(string);
    for (int i = 0; i < length; ++i)
    {
        if (!(isalpha(string[i]) || isdigit(string[i]))) return fail;
    }
    return success;
}

// индекс
int validate_string_digits(char * string)
{
    int length = strlen(string);
    if (length == 0) return fail;
    for (int i = 0; i < length; ++i)
    {
        if (!isdigit(string[i])) return fail;
    }
    return success;
}

int check_is_index(char * string)
{
    int result = validate_string_digits(string);
    if (result == fail) return fail;
    result = strlen(string);
    if (result != 14) return fail;
    return success;
}

int check_is_weight(double weight)
{
    double eps = 0.0000001;
    if (weight > eps) return success;
    return fail;
}

int check_building_and_apartment(int house)
{
    if (house <= 0) return fail;
    return success;
}

Adress_ptr create_adress(char * town, char * street, unsigned int building, char * housing, unsigned int apartment, char * index)
{
    String_ptr ttown = create_string(town);
    if (!ttown) return NULL;

    String_ptr sstreet = create_string(street);
    if (!sstreet)
    {
        delete_string(ttown);
        return NULL;
    }

    String_ptr house = create_string(housing);
    if (!house)
    {
        delete_string(ttown);
        delete_string(sstreet);
        return NULL;
    }

    String_ptr iindex = create_string(index);
    if (!iindex)
    {
        delete_string(ttown);
        delete_string(sstreet);
        delete_string(house);
        return NULL;
    }
    
    Adress_ptr adress = (Adress_ptr) malloc(sizeof(Adress));
    if (!adress)
    {
        delete_string(ttown);
        delete_string(sstreet);
        delete_string(house);
        delete_string(iindex);
        return NULL;
    }

    adress -> town = ttown;
    adress -> street = sstreet;
    adress -> building = building;
    adress -> housing = house;
    adress -> apartment = apartment;
    adress -> index = iindex;

    return adress;
}

// адрес получателя, вес посылки, почтовый индекс, время создания, время получения
Mail_ptr create_mail(Adress_ptr adress, double weight, char * post_index, char * create_time, char * time_take) // создание посылки
{

    String_ptr ppost_index = create_string(post_index);
    if (!ppost_index) return NULL;

    String_ptr time1 = create_string(create_time);
    if (!time1)
    {
        delete_string(ppost_index);
        return NULL;
    }
    String_ptr time2 = create_string(time_take);
    if (!time2)
    {
        delete_string(ppost_index);
        delete_string(time1);
        return NULL;
    }

    Mail_ptr mail = (Mail_ptr) malloc(sizeof(Mail));
    if (!mail)
    {
        delete_string(ppost_index);
        delete_string(time1);
        delete_string(time2);
        return NULL;
    }

    mail -> adress = adress;
    mail -> weight = weight;
    mail -> post_index = ppost_index;
    mail -> time_made = time1;
    mail -> time_take = time2;

    return mail;
}