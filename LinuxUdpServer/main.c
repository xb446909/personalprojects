#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "process.h"
#include "logger.h"

char ip_addr[16] = {0};
int port = 0;

int main(int argc, char** argv)
{
    int result;
    
    opterr = 0;
    port = DEFAULT_PORT;
    strcpy(ip_addr, DEFAULT_IP);

    while((result = getopt(argc, argv, "i:p:h")) != -1)
    {
        switch(result)
        {
            case 'i':
                if(strlen(optarg) > 15)
                {
                    fprintf(stderr, "Usage: %s [-i ip] [-p port]\n", argv[0]);
                    exit(0);
                }
                else
                    strcpy(ip_addr, optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                fprintf(stderr, "Usage: %s [-i ip] [-p port]\n", argv[0]);
                exit(0);
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-i ip] [-p port]\n", argv[0]);
                exit(0);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i ip] [-p port]\n", argv[0]);
                exit(0);
                break;
        }
    }
    process_all();
}



void debug(const char* format, ...)
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

