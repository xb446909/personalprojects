#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

inline void debug(const char* format, ...)
{
    va_list arg;
    time_t __now;
    struct tm* __nowtime;
    FILE* __stream = fopen("log.txt", "a+");

    time(&__now);
    __nowtime = localtime(&__now);
    fprintf(__stream, "[DEBUG] %d/%d/%d %d:%d:%d ", 
            __nowtime->tm_mon + 1,
            __nowtime->tm_mday,
            __nowtime->tm_year + 1900,
            __nowtime->tm_hour,
            __nowtime->tm_min,
            __nowtime->tm_sec);

    va_start (arg, format);
    vfprintf (__stream, format, arg);
    va_end (arg);

    fclose(__stream);
  
}

#endif
