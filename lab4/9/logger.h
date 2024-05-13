#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <stdio.h>
#include "errors.h"
#include <time.h>

typedef struct Logger
{
    FILE * file;
} Logger, *Logger_ptr;

status create_logger(Logger_ptr raw_space, char * file_name);

void delete_logger(Logger_ptr logger);

status new_request(Logger_ptr logger, const char * message, time_t time_log);

status request_handling_started(Logger_ptr logger, const char * message, time_t time_log);

status request_handling_finished(Logger_ptr logger, const char * message, time_t time_log);

status department_overload(Logger_ptr logger, const char * message, time_t time_log);

#endif