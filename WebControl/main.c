#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "process.h"
#include "logger.h"

char hostname[CMD_BUF_LEN] = { 0 };
int port = 0;
char dev_name[MAX_FILEPATH] = { 0 };
int nBurdrate = 0;
int nDatWidth = 0;
int nEvent = 0;
int nStop = 0;

/*struct ParamStruct param_list[] = {
    "RemoteHost", 'S', hostname,
    "RemotePort", 'I', port,
    "SerialDev" , 'S', dev_name,
    "Burdrate"  , 'I', nBurdrate,
    "DataWidth" , 'I', nDatWidth,
    "OddEven"   , 'I', nEvent,
    "StopBits"  , 'I', nStop};
*/

struct ParamStruct p = {"aaa", 'S', hostname};

void read_config(const char* config_file);
int str_analyz(const char* str, char* name, char* value);

int main(int argc, char** argv)
{
    int result;

    read_config("config.ini");
    return 0;
    
    opterr = 0;
    port = DEFAULT_PORT;
    strcpy(hostname, DEFAULT_HOST);

    while((result = getopt(argc, argv, "i:p:h")) != -1)
    {
        switch(result)
        {
            case 'i':
                strcpy(hostname, optarg);
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


void read_config(const char* config_file)
{
    FILE* fd = fopen(config_file, "r");
    char read_buf[CMD_BUF_LEN] = { 0 };
    char name[MAX_FILEPATH] = { 0 };
    char value[MAX_FILEPATH] = { 0 };
    int i;

    if(fd == NULL)
    {
        perror("Can not open config file, use default config");
        debug("Can not open config file: %s\n", config_file);
        return;
    }

    fseek(fd, 0, SEEK_SET);

    while(fgets(read_buf, CMD_BUF_LEN, fd))
    {
        if(read_buf[0] == '#') continue;
        read_buf[strlen(read_buf) - 1] = '\0';
        if(!str_analyz(read_buf, name, value)) continue;
        printf("name: %s, value: %s\n", name, value);
        if(!strcmp(read_buf, "RemoteHost"))
        {
            strcpy(hostname, value);
        }
        if(!strcmp(read_buf, "RemoteHost"))
        {
            strcpy(hostname, read_buf);
        }
        memset(read_buf, 0, CMD_BUF_LEN);
    }
    
    fclose(fd);
}

int str_analyz(const char* str, char* name, char* value)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int flag = 0;

    while(str[i] != '\0')
    {
        if(str[i] == ' ')
        {
            i++;
            continue;
        }
        if(flag == 0)
        {
            if(str[i] != '=')
            {
                name[j++] = str[i];
            }
            else
            {
                name[j] = '\0';
                flag = 1;
            }
        }
        else
        {
            value[k++] = str[i];
        }
        i++;
    }
    value[k] = '\0';
    return flag;
}
