#ifndef POST_H
#define POST_H

#include "string.h"

typedef struct
{
    String_ptr town; // непустая
    String_ptr street; // непустая
    unsigned int building; 
    String_ptr housing; // может быть пустая
    unsigned int apartment;
    String_ptr index; // 6 символов!!

} Adress, *Adress_ptr;

typedef struct
{
    Adress_ptr adress;
    double weight; // неотрицательное
    String_ptr post_index; // 14 символов
    String_ptr time_made; // формат dd:MM:yyyy hh:mm:ss
    String_ptr time_take; // формат dd:MM:yyyy hh:mm:ss

} Mail, *Mail_ptr, **Mail_array;

typedef struct
{
    Adress_ptr post_adress;
    Mail_array mails;

} Post, *Post_ptr;

// creation
Post_ptr create_post(Adress_ptr adress);
Adress_ptr create_adress(char * town, char * street, unsigned int building, char * housing, unsigned int apartment, char * index);
Mail_ptr create_mail(Adress_ptr adress, double weight, char * post_index, char * create_time, char * time_take);

//delete
void delete_post(Post_ptr post);
void delete_mail(Mail_ptr mail);
void delete_adress(Adress_ptr adress);

// validation
int validate_string_alphas(char * string);
int validate_string_alphas_digits(char * string);
int validate_string_digits(char * string);
int check_is_index(char * string);
int check_is_weight(double weight);
int check_building_and_apartment(int house);
int check_lowercase(char * string);

// appending and deleting mails in post
int append_mail(Mail_ptr mail, Post_ptr post, int length);
void insert_mail(int length, Post_ptr post, Mail_ptr mail);
int remove_mail(Post_ptr post, int index, int length);

// time functions
int check_time(String_ptr time_input);
int time_diff(String_ptr time_str1, String_ptr time_str2);
int time_diff_char(char * time_str1, char * time_str2);

// searching functions
int find_minimum_time_more_than(Mail_array mails, int time_index, int length);
int find_with_post_index(Mail_array mails, char * post_index, int length);

// print-functions
void print_adress(Adress_ptr adress);
void print_delivered_mails(Post_ptr post, int length);
void print_mail_info(Mail_ptr mail);
void print_all_mails(Post_ptr post, int length);

#endif