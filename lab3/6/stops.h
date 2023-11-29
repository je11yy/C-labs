/* 
Основной односвязный список содержит в своем теле список остановок, которые прошло ОДНО транспортное средство

Вложенный односвязный список содержит все пройденные остановки от начальной до конечной и отсортирован по времени, когда
транспортное средство там остановилось.

В одном файле - информация по ОДНОМУ остановочному пункту
*/

#ifndef STOPS_H
#define STOPS_H

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef struct Stop
{
    struct Stop * next;
    String_ptr stop_time;
    String_ptr departure_time;
    double coordinates[2]; // координаты остановочного пункта
    char marker; // M <- промежуточная (middle), S <- начальная (start), F <- конечная (final)
    
} Stop, *Stop_ptr;

typedef struct Transport
{
    String_ptr id; // номер транспортного средства
    struct Transport * next; // указатель на следующее транспортное средство
    Stop_ptr stop; // указатель на ПЕРВЫЙ элемент списка остановок
} Transport, *Transport_ptr;

void print_transports(Transport_ptr transport);
void print_stop(Stop_ptr stop, int i);
void print_stops(Stop_ptr stop);

Transport_ptr create_transport(char * id);
Stop_ptr create_stop(char * stop_time, char * departure_time, double coordinates[2], char marker);

void delete_transports(Transport_ptr transports);
void delete_transport(Transport_ptr transport);
void delete_stops(Stop_ptr stop);
void delete_stop(Stop_ptr stop);

Transport_ptr add_stop(Transport_ptr transports, char * transport_id, char * stop_time, char * departure_time, 
double coordinates[2], char marker);
Transport_ptr add_transports(Transport_ptr transports, char * transport_id);

int compare_stops(Stop_ptr first, Stop_ptr second);
Transport_ptr search_transport(Transport_ptr transports, char * transport_id);

Transport_ptr max_length_route(int * result, Transport_ptr transport);
Transport_ptr min_length_route(int * result, Transport_ptr transport);
Transport_ptr min_routes(int * result, Transport_ptr transport);
Transport_ptr max_routes(int * result, Transport_ptr transport);
Transport_ptr min_downtime_on_stop(Transport_ptr transport);
Transport_ptr max_downtime_on_stop(Transport_ptr transport);
Transport_ptr min_downtime(Transport_ptr transport);
Transport_ptr max_downtime(Transport_ptr transport);
Transport_ptr min_length(int * result, Transport_ptr transport);
Transport_ptr max_length(int * result, Transport_ptr transport);

double get_downtime(Stop_ptr stop);
#endif