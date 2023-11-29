#include <stdio.h>
#include <stdlib.h>
#include "stops.h"
#include <string.h>
#include <ctype.h>
#define __USE_XOPEN
#include <time.h>
#include <math.h>

#define _GNU_SOURCE

enum RESULTS
{
    success = 0,
    fail = -1,
    no_memory = -2,
    incorrect_input = -3,
    file_open_error = -4
};

void print_transports(Transport_ptr transport)
{
    while (transport)
    {
        printf("Transport number: %s\n", transport -> id -> buffer);
        print_stops(transport -> stop);
        transport = transport -> next;
    } 
}

void print_stops(Stop_ptr stop)
{
    int i = 1;
    while (stop)
    {
        print_stop(stop, i);
        i++;
        stop = stop -> next;
    }
}

void print_stop(Stop_ptr stop, int i)
{
    printf("\tStop №%d\n", i);
    printf("\t\tCoordinates: %f %f\n", (stop -> coordinates)[0], (stop -> coordinates)[1]);
    printf("\t\tStop time: %s\n", stop -> stop_time -> buffer);
    printf("\t\tDeparture time: %s\n", stop -> departure_time -> buffer);
    printf("\t\tMarker: %c\n\n", stop -> marker);
}

Transport_ptr max_length(int * result, Transport_ptr transport)
{
    Transport_ptr tmp = transport;
    Transport_ptr answer = NULL;
    int max_count = 0, count = 0;
    Stop_ptr tmp_stop;

    while (tmp)
    {
        count = 0;
        tmp_stop = tmp -> stop;
        while (tmp_stop)
        {
            count++;
            tmp_stop = tmp_stop -> next;
        }
        if (count > max_count)
        {
            answer = tmp;
            max_count = count;
        }
        tmp = tmp -> next;
    }
    *result = max_count;
    return answer;
}

Transport_ptr min_length(int * result, Transport_ptr transport)
{
    Transport_ptr tmp = transport;
    Transport_ptr answer = NULL;
    int min_count = -1, count = 0;
    Stop_ptr tmp_stop;

    while (tmp)
    {
        count = 0;
        tmp_stop = tmp -> stop;
        while (tmp_stop)
        {
            count++;
            tmp_stop = tmp_stop -> next;
        }
        if (count < min_count || min_count == -1)
        {
            answer = tmp;
            min_count = count;
        }
        tmp = tmp -> next;
    }
    *result = min_count;
    return answer;
}

Transport_ptr max_downtime(Transport_ptr transport)
{
    if (!transport) return NULL;
    double eps = 0.000000001;
    double downtime = 0;
    double max_downtime = 0;
    Transport_ptr tmp = transport;
    Transport_ptr answer;
    Stop_ptr stop;
    while (tmp)
    {
        stop = tmp -> stop;
        while (stop)
        {
            downtime += get_downtime(stop);
            stop = stop -> next;
        }
        if (downtime - max_downtime < eps)
        {
            max_downtime = downtime;
            answer = tmp;
        }
        downtime = 0;
        tmp = tmp -> next;
    }
    return answer;
}

Transport_ptr max_downtime_on_stop(Transport_ptr transport)
{
    if (!transport) return NULL;
    double eps = 0.000000001;
    double downtime = 0;
    double max_downtime = 0;
    Transport_ptr tmp = transport;
    Transport_ptr answer;
    Stop_ptr stop;
    while (tmp)
    {
        stop = tmp -> stop;
        while (stop)
        {
            downtime = get_downtime(stop);
            if (max_downtime - downtime > eps)
            {
                max_downtime = downtime;
                answer = tmp;
            }
            downtime = 0;
            stop = stop -> next;
        }
        tmp = tmp -> next;
    }
    return answer;
}

Transport_ptr min_downtime_on_stop(Transport_ptr transport)
{
    if (!transport) return NULL;
    double eps = 0.000000001;
    double downtime = 0;
    double min_downtime = -1;
    Transport_ptr tmp = transport;
    Transport_ptr answer;
    Stop_ptr stop;
    while (tmp)
    {
        stop = tmp -> stop;
        while (stop)
        {
            downtime = get_downtime(stop);
            if (downtime - min_downtime > eps || min_downtime < eps)
            {
                min_downtime = downtime;
                answer = tmp;
            }
            downtime = 0;
            stop = stop -> next;
        }
        tmp = tmp -> next;
    }
    return answer;
}

double get_downtime(Stop_ptr stop)
{
    struct tm tm1 = {0};
    struct tm tm2 = {0};
    strptime(stop -> stop_time -> buffer, "%d.%m.%Y %H:%M:%S", &tm1);
    strptime(stop -> departure_time -> buffer, "%d.%m.%Y %H:%M:%S", &tm2);

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    return difftime(t1, t2) / 3600.0; // время в секундах
}

Transport_ptr max_routes(int * result, Transport_ptr transport)
{
    if (!transport) return NULL;
    int count = 0;
    int max_count = 0;
    Transport_ptr tmp = transport;
    Transport_ptr answer;
    Stop_ptr stop;
    while (tmp)
    {
        count = 0;
        stop = tmp -> stop;
        while (stop)
        {
            if (stop -> marker == 'F')
            {
                count++;
            }
            stop = stop -> next;
        }
        if (count > max_count)
        {
            max_count = count;
            answer = tmp;
        }
        tmp = tmp -> next;
    }
    *result = max_count;
    return answer;
}

Transport_ptr min_routes(int * result, Transport_ptr transport)
{
    if (!transport) return NULL;
    int count = 1;
    int min_count = -1;
    Transport_ptr tmp = transport;
    Transport_ptr answer;
    Stop_ptr stop;
    while (tmp)
    {   
        count = 0;
        stop = tmp -> stop;
        while (stop)
        {
            if (stop -> marker == 'F')
            {
                count++;
            }
            stop = stop -> next;
        }
        if (count < min_count || min_count == -1)
        {
            min_count = count;
            answer = tmp;
        }
        tmp = tmp -> next;
    }
    *result = min_count;
    return answer;
}

Transport_ptr min_length_route(int * result, Transport_ptr transport)
{
    int min_length = -1, length = 0;
    Transport_ptr answer;
    Transport_ptr tmp = transport;
    Stop_ptr stop;
    while (tmp)
    {
        stop = tmp -> stop;
        length = 0;
        while (stop -> next)
        {
            length++;
            if (stop -> marker == 'F')
            {
                if (length < min_length || min_length == -1)
                {
                    min_length = length;
                    answer = tmp;
                }
                length = 0;
            }
            stop = stop -> next;
        }
        tmp = tmp -> next;
    }
    *result = min_length;
    return answer;
}

Transport_ptr max_length_route(int * result, Transport_ptr transport)
{
    int max_length = 0, length = 0;
    Transport_ptr answer;
    Transport_ptr tmp = transport;
    Stop_ptr stop;
    while (tmp)
    {
        stop = tmp -> stop;
        length = 0;
        while (stop -> next)
        {
            length++;
            if (stop -> marker == 'F')
            {
                if (length > max_length)
                {
                    max_length = length;
                    answer = tmp;
                }
                length = 0;
            }
            stop = stop -> next;
        }
        tmp = tmp -> next;
    }
    *result = max_length;
    return answer;
}

Transport_ptr create_transport(char * id) 
{
    Transport_ptr transport = (Transport_ptr)malloc(sizeof(Transport));
    if (!transport) return NULL;

    transport -> id = create_string(id);
    if (!(transport -> id))
    {
        free(transport);
        transport = NULL;
        return NULL;
    }
    transport -> next = NULL;
    transport -> stop = NULL;
    return transport;
}

Stop_ptr create_stop(char * stop_time, char * departure_time, double coordinates[2], char marker)
{
    String_ptr time_stop = create_string(stop_time);
    if (!time_stop) return NULL;
    String_ptr time_dep = create_string(departure_time);
    if (!time_dep)
    {
        delete_string(time_stop);
        return NULL;
    }
    Stop_ptr stop = (Stop_ptr)malloc(sizeof(Stop));
    if (!stop)
    {
        delete_string(time_stop);
        delete_string(time_dep);
        return NULL;
    }
    stop -> stop_time = time_stop;
    stop -> departure_time = time_dep;
    (stop -> coordinates)[0] = coordinates[0];
    (stop -> coordinates)[1] = coordinates[1];
    stop -> marker = marker;
    stop -> next = NULL;
    return stop;
}

void delete_stop(Stop_ptr stop)
{
    if (!stop) return;
    if (stop -> stop_time) delete_string(stop -> stop_time);
    if (stop -> departure_time) delete_string(stop -> departure_time);
    free(stop);
    stop = NULL;
}

void delete_stops(Stop_ptr stop)
{
    Stop_ptr tmp;
    while (stop -> next)
    {
        tmp = stop -> next;
        delete_stop(stop);
        stop = tmp;
    }
    delete_stop(stop);
}

Transport_ptr add_stop(Transport_ptr transports, char * transport_id, char * stop_time, char * departure_time, 
double coordinates[2], char marker)
{
    Stop_ptr stop = create_stop(stop_time, departure_time, coordinates, marker);
    if (!stop) return NULL;
    Transport_ptr tmp = transports;
    Transport_ptr new_transport;
    if (!transports) // если нет ни одного транспорта
    {
        new_transport = create_transport(transport_id);
        if (!new_transport)
        {
            delete_stop(stop);
            return NULL;
        }
        transports = new_transport;
        tmp = transports;
    }
    else
    {
        while (tmp -> next && strcmp(tmp -> id -> buffer, transport_id) != success) tmp = tmp -> next;
        if (strcmp(tmp -> id -> buffer, transport_id) != success)
        {
            transports = add_transports(transports, transport_id);
            if (!transports)
            {
                delete_stop(stop);
                return NULL;
            }
            tmp = transports;
        }
        while (tmp -> next && strcmp(tmp -> id -> buffer, transport_id) != success) tmp = tmp -> next;
    }
    Stop_ptr current_stop = tmp -> stop;
    if (!current_stop) // если еще нет остановок
    {
        tmp -> stop = stop;
        return transports;
    }
    // если остановки есть, нужно найти место
    Stop_ptr prev = NULL;
    while (current_stop)
    {
        if (compare_stops(current_stop, stop) > 0)
        {
            stop -> next = current_stop;
            if (!prev) tmp -> stop = stop;
            else prev -> next = stop;
            return transports;
        }
        prev = current_stop;
        current_stop = current_stop -> next;
    }
    // закончились остановки
    prev -> next = stop;
    return transports;
}

int compare_stops(Stop_ptr first, Stop_ptr second)
{
    // по времени остановки
    struct tm tm1 = {0};
    struct tm tm2 = {0};
    strptime(first -> stop_time -> buffer, "%d.%m.%Y %H:%M:%S", &tm1);
    strptime(second -> stop_time -> buffer, "%d.%m.%Y %H:%M:%S", &tm2);

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 < t2) return -1; // первое меньше
    else if (t1 == t2) return 0; // одинаковые
    else return 1; // первое больше
}

Transport_ptr add_transports(Transport_ptr transports, char * transport_id) // вставка в конец
{
    Transport_ptr transport = create_transport(transport_id);
    if (!transport)
    {
        delete_transports(transports);
        delete_stops(transport -> stop);
        return NULL;
    }
    Transport_ptr tmp = transports;
    while (tmp -> next) tmp = tmp -> next;
    tmp -> next = transport;
    return transports;
}

void delete_transports(Transport_ptr transports)
{
    Transport_ptr tmp;
    while (transports -> next)
    {
        tmp = transports -> next;
        delete_transport(transports);
        transports = tmp;
    }
    delete_transport(transports);
}

void delete_transport(Transport_ptr transport)
{
    delete_string(transport -> id);
    delete_stops(transport -> stop);
    free(transport);
    transport = NULL;
}

Transport_ptr search_transport(Transport_ptr transports, char * transport_id)
{
    Transport_ptr tmp = transports;
    while (tmp)
    {
        if (strcmp(tmp -> id -> buffer, transport_id) == success) return tmp;
        tmp = tmp -> next;
    }
    return NULL;
}