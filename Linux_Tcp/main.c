#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "process.h"

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

