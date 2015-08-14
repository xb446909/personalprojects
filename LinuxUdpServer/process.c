#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>

#include "logger.h"
#include "process.h"

extern char ip_addr[16];
extern int port;
int sockfd;
char recv_buf[CMD_BUF_LEN] = {0};
char proc_buf[CMD_BUF_LEN] = {0};
int child_pid;
pthread_t keeponline_thread;
struct sockaddr_in addr_ser = { 0 };

void    process_all(void);
int     create_process(const char* file, const char* argv);
void*   keeponline(void* arg);
void    send_msg(struct sockaddr_in remoteaddr, char* msg);

void send_msg(struct sockaddr_in remoteaddr, char* msg)
{
    sendto(sockfd, msg, strlen(msg), 0,
            (struct sockaddr*)&remoteaddr, sizeof(struct sockaddr));
}


void process_all(void)
{
    int res;
    struct sockaddr_in addr_ser = { 0 };
    int len;

    res = pthread_create(&keeponline_thread, NULL, keeponline, NULL);
    if(res != 0)
    {
        debug("Thread creation failed!");
        exit(0);
    }

    while(1)
    {
    }
}

void*   keeponline(void* arg)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int err;
    int read_len = 0;
    
    if(sockfd == -1)
    {
        debug("Create socket error, %d\n", errno);
        return;
    }

    addr_ser.sin_family = AF_INET;
    addr_ser.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_ser.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr*)&addr_ser, sizeof(addr_ser)) < 0)
    {
        debug("Bind port error!\n");
        perror("Bind port error!\n");
    }
    else
    {
        debug("Bind port %d successfully!\n", port);
    }

    struct sockaddr_in clientAddr;
    int len = sizeof(clientAddr);

    while(1)
    {
        memset(recv_buf, '\0', CMD_BUF_LEN);
        read_len = recvfrom(sockfd, recv_buf, CMD_BUF_LEN, 0, 
                (struct sockaddr*)&clientAddr, &len);
        printf("recvfrom %s:%d length: %d, recv: %s\n", 
                inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port,
                read_len, recv_buf);
        send_msg(clientAddr, "Received!\n");
    }
}

