#ifndef __LOGGER_H
#define __LOGGER_H

#include <time.h>

time_t __now;
struct tm* __nowtime;
FILE* __log_fd;

#define debug(format, ...)  __log_fd = fopen("log.txt", "a+"); \
                            time(&__now); \
                            __nowtime = localtime(&__now);  \
                            fprintf(__log_fd, "[DEBUG] %d/%d/%d %d:%d:%d ", __nowtime->tm_mon + 1, \
                                    __nowtime->tm_mday, \
                                    __nowtime->tm_year + 1900,  \
                                    __nowtime->tm_hour, \
                                    __nowtime->tm_min,  \
                                    __nowtime->tm_sec);  \
                            fprintf(__log_fd, format, __VA_ARGS__); \
                            fclose(__log_fd);

#endif
