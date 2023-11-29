#include <stdio.h>
#include <stdlib.h>
#include "stops.h"
#include <string.h>
#include <ctype.h>
#define __USE_XOPEN
#include <time.h>
#include <math.h>
#include <stdio_ext.h>

#define _GNU_SOURCE

/*
ЗАПУСКАТЬ ТОЛЬКО НА ЛИНУКСЕ
*/

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

int validation(int d1, int m1, int y1, int h1, int min1, int s1, int d2, int m2, int y2, int h2, int min2, int s2, char marker);
Transport_ptr get_information(int * result, int size, char * file_names[]);
Transport_ptr get_info_from_file(Transport_ptr transport, int * result, FILE * file);

void menu();
void switch_command(int command, Transport_ptr transports);
int get_command();

void print_error(int error);

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    int result;
    Transport_ptr transport = get_information(&result, argc - 1, argv + 1);
    if (!transport)
    {
        print_error(result);
        return result;
    }
    printf("All transports:\n\n");
    print_transports(transport);
    int command = -1;
    while (command != 0)
    {
        menu();
        command = get_command();
        if (command == 0) break;
        switch_command(command, transport);
    }
    delete_transports(transport);
    printf("Program has finished correctly.\n");
    return success;
}


int get_command()
{
    int command;
    int result = scanf("%d", &command);
    while (result != 1 && !(command >= 0 && command <= 9))
    {
        __fpurge(stdin);
        printf("\nIncorrect input. Try again: ");
        result = scanf("%d", &command);
    }
    printf("\n");
    return command;
}

void switch_command(int command, Transport_ptr transports)
{
    Transport_ptr transport;
    int result;
    switch (command)
    {
        case 1:
            transport = min_routes(&result, transports);
            printf("Transport id: %s. Number of routes: %d\n\n", transport -> id -> buffer, result);
            print_stops(transport -> stop);
            break;
        case 2:
            transport = max_routes(&result, transports);
            printf("Transport id: %s. Number of routes: %d\n\n", transport -> id -> buffer, result);
            print_stops(transport -> stop);
            break;
        case 3:
            transport = min_length(&result, transports);
            printf("Transport id: %s. Number of passed stops: %d\n\n", transport -> id -> buffer, result);
            print_stops(transport -> stop);
            break;
        case 4:
            transport = max_length(&result, transports);
            printf("Transport id: %s. Number of passed stops: %d\n\n", transport -> id -> buffer, result);
            print_stops(transport -> stop);
            break;
        case 5:
            transport = min_length_route(&result, transports);
            printf("Transport id: %s. Number of passed stops on the min length route: %d\n\n", transport -> id -> buffer, result);
            print_stops(transport -> stop);
            break;
        case 6:
            transport = max_length_route(&result, transports);
            printf("Transport id: %s. Number of passed stops on the max length route: %d\n\n", transport -> id -> buffer, result);
            print_stops(transport -> stop);
            break;
        case 7:
            transport = min_downtime_on_stop(transports);
            printf("Transport id: %s.\n\n", transport -> id -> buffer);
            print_stops(transport -> stop);
            break;
        case 8:
            transport = max_downtime_on_stop(transports);
            printf("Transport id: %s.\n\n", transport -> id -> buffer);
            print_stops(transport -> stop);
            break;
        case 9:
            transport = max_downtime(transports);
            printf("Transport id: %s.\n\n", transport -> id -> buffer);
            print_stops(transport -> stop);
            break;
        default: break;
    }
}

void menu()
{
    printf("\nMENU\n\n");
    printf("1. Find transport with min of routes quantity.\n");
    printf("2. Find transport with max of routes quantity.\n");
    printf("3. Find transport with min pass.\n");
    printf("4. Find transport with max pass.\n");
    printf("5. Find transport with min of routes length.\n");
    printf("6. Find transport with max of routes length.\n");
    printf("7. Find transport with min time on stop.\n");
    printf("8. Find transport with max time on stop.\n");
    printf("9. Find transport with max time on all stops.\n");
    printf("0. Exit.\n\n");
    printf("Input form: ");
}

Transport_ptr get_information(int * result, int size, char * file_names[])
{
    FILE * file;
    Transport_ptr transport = NULL;
    for (int i = 0; i < size; ++i)
    {
        file = fopen(file_names[i], "r");
        if (!file)
        {
            *result = file_open_error;
            if (transport) delete_transports(transport);
            return NULL;
        }

        transport = get_info_from_file(transport, result, file);
        if (!transport)
        {
            if (transport) delete_transports(transport);
            return NULL;
        }
        fclose(file);
    }
    return transport;
}

Transport_ptr get_info_from_file(Transport_ptr transport, int * result, FILE * file)
{
    char * line = NULL;
    size_t len = 0;
    int read;
    double coordinates[2];
    char id[20];
    int day, month, year, hours, minutes, seconds;
    int day1, month1, year1, hours1, minutes1, seconds1;
    char marker;
    char * time1, * time2;
    int res;
    res = fscanf(file, "%lf %lf\n", &(coordinates[0]), &(coordinates[1]));
    if (res != 2)
    {
        if (transport) delete_transports(transport);
        *result = incorrect_input;
        fclose(file);
        return NULL;
    }
    while ((read = getline(&line, &len, file)) != EOF)
    {
        res = sscanf(line, "%s %d.%d.%d %d:%d:%d %d.%d.%d %d:%d:%d %c", id, &day, &month, &year, &hours, &minutes, &seconds, &day1, &month1, &year1, &hours1, &minutes1, &seconds1, &marker);
        if (res != 14 || validation(day, month, year, hours, minutes, seconds, day1, month1, year1, hours1, minutes1, seconds1, marker) != success)
        {
            free(line);
            line = NULL;
            fclose(file);
            *result = incorrect_input;
            return NULL;
        }
        
        time1 = (char*)malloc(len * sizeof(char));
        if (!time1)
        {
            free(line);
            line = NULL;
            if (transport) delete_transports(transport);
            *result = no_memory;
            fclose(file);
            return NULL;
        }
        time2 = (char*)malloc(len * sizeof(char));
        if (!time2)
        {
            free(time1);
            time1 = NULL;
            free(line);
            line = NULL;
            if (transport) delete_transports(transport);
            *result = no_memory;
            fclose(file);
            return NULL;
        }
        sprintf(time1, "%d.%d.%d %d:%d:%d", day, month, year, hours, minutes, seconds);
        sprintf(time2, "%d.%d.%d %d:%d:%d", day1, month1, year1, hours1, minutes1, seconds1);
        transport = add_stop(transport, id, time1, time2, coordinates, marker);
        if (!(transport))
        {
            free(line);
            line = NULL;
            fclose(file);
            free(time1);
            time1 = NULL;
            free(time2);
            time2 = NULL;
            *result = no_memory;
            return NULL;
        }
        free(line);
        line = NULL;
        len = 0;
        free(time1);
        time1 = NULL;
        free(time2);
        time2 = NULL;
    }
    free(line);
    line = NULL;
    *result = success;
    return transport;
}

int validation(int d1, int m1, int y1, int h1, int min1, int s1, int d2, int m2, int y2, int h2, int min2, int s2, char marker)
{
    return success;
}

void print_error(int error)
{
    switch(error)
    {
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.");
            break;
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_open_error:
            printf("\nCannot open file!\n");
            break;
    }
}