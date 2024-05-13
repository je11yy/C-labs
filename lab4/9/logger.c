#include "logger.h"

#define MAX_LENGTH 25

status create_logger(Logger_ptr raw_space, char * file_name)
{
    if (raw_space) return invalid_argument;
    Logger_ptr raw_space = (Logger_ptr)malloc(sizeof(Logger));
    if (!raw_space) return no_memory;
    raw_space->file = fopen(file_name, "w");
    if (!(raw_space->file))
    {
        free(raw_space);
        return file_open_error;
    }
    return success;
}

void delete_logger(Logger_ptr logger)
{
    fclose(logger->file);
    free(logger);
}

status new_request(Logger_ptr logger, const char * message, time_t time_log)
{
    if (!logger) return invalid_argument;
    if (!(logger->file)) return invalid_argument;
    char time_str[MAX_LENGTH];
    struct tm* time_tm = localtime(&time_log);
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", 
        time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, 
            time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);

    fprintf(logger->file, "[%s] [%s]: %s", "NEW_REQUEST", time_str, message);
    return success;
}

status request_handling_started(Logger_ptr logger, const char * message, time_t time_log)
{
    if (!logger) return invalid_argument;
    if (!(logger->file)) return invalid_argument;
    char time_str[MAX_LENGTH];
    struct tm* time_tm = localtime(&time_log);
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", 
        time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, 
            time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: %s", "NEW_REQUEST", time_str, message);
    return success;
}

status request_handling_finished(Logger_ptr logger, const char * message, time_t time_log)
{
    if (!logger) return invalid_argument;
    if (!(logger->file)) return invalid_argument;
    char time_str[MAX_LENGTH];
    struct tm* time_tm = localtime(&time_log);
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", 
        time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, 
            time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: %s", "NEW_REQUEST", time_str, message);
    return success;
}

status department_overload(Logger_ptr logger, const char * message, time_t time_log)
{
    if (!logger) return invalid_argument;
    if (!(logger->file)) return invalid_argument;
    char time_str[MAX_LENGTH];
    struct tm* time_tm = localtime(&time_log);
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", 
        time_tm->tm_year+1900, time_tm->tm_mon+1, time_tm->tm_mday, 
            time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
    fprintf(logger->file, "[%s] [%s]: %s", "NEW_REQUEST", time_str, message);
    return success;
}