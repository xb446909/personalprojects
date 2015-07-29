#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "logger.h"
#include "process.h"

extern char ip_addr[16];
extern int port;
int sockfd;
char recv_buf[1024] = {0};

void    process_all(void);
int     connect_server(void);


void process_all(void)
{
    connect_state = ST_DISCONNECT;

    while(1)
    {
        switch(connect_state)
        {
        case ST_DISCONNECT:
            if(connect_server() == 0)
                connect_state = ST_CONNECT;
            break;
        case ST_CONNECT:
            recv(sockfd, recv_buf, 512, 0);
            printf("recv: %s\n", recv_buf);
            send(sockfd, "Hello!\r\n", strlen("Hello!\r\n") + 1, 0);
            break;
        }/*switch(connect_state)*/
        sleep(5);
    }/*while(1)*/
}


int connect_server(void)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr_ser = {0};
    int err;

    debug("Connecting to %s:%d\n", ip_addr, port);

    if(sockfd == -1)
    {
        debug("Create socket error, %d\n", errno);
        return -1;
    }

    addr_ser.sin_family = AF_INET;
    addr_ser.sin_addr.s_addr = inet_addr(ip_addr);
    addr_ser.sin_port = htons(port);
    
    err = connect(sockfd, (struct sockaddr*)&addr_ser, sizeof(addr_ser));
    if(err == -1)
    {
        debug("Connect server error, %d\n", errno);
        return errno;
    }
    return 0;
}
