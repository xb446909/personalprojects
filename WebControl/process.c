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
char recv_buf[CMD_BUF_LEN] = {0};
char proc_buf[CMD_BUF_LEN] = {0};
int child_pid;
pthread_t keeponline_thread;
struct sockaddr_in addr_ser = { 0 };

void    process_all(void);
void    process_command(char* cmd);
int     connect_server(void);
int     create_process(const char* file, const char* argv);
void*   keeponline(void* arg);
void    send_msg(char* msg);


void send_msg(char* msg)
{
    sendto(sockfd, msg, strlen(msg), 0,
            (struct sockaddr*)&addr_ser, sizeof(addr_ser));
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
        recvfrom(sockfd, recv_buf, CMD_BUF_LEN, 0, 
                (struct sockaddr*)&addr_ser, &len);
        process_command(recv_buf);
    }
}

void*   keeponline(void* arg)
{
    char buf[512] = {0};
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int err;

    debug("Connecting to %s:%d\n", ip_addr, port);

    if(sockfd == -1)
    {
        debug("Create socket error, %d\n", errno);
        return;
    }

    addr_ser.sin_family = AF_INET;
    addr_ser.sin_addr.s_addr = inet_addr(ip_addr);
    addr_ser.sin_port = htons(port);

    while(1)
    {
        sendto(sockfd, "Hello!", strlen("Hello!"), 0,
                (struct sockaddr*)&addr_ser, sizeof(addr_ser));
        sleep(1);
    }
}

void process_command(char* cmd)
{
    char file_cwd[MAX_FILEPATH] = { 0 };
    getcwd(file_cwd, MAX_FILEPATH);
    
    if(cmd[0] != '@')
    {
        debug("Unknown command (%s) received!\n", cmd);
        return;
    }
    switch(cmd[1])
    {
    case 'a':
        send_msg("Receive a");
        system("rm log.txt -f");
        break;
    case 'A':
        send_msg("Receive A");
        //sprintf(file_cwd, "%s/test.exe", file_cwd);
        child_pid = create_process("/usr/bin/omxplayer", "/usr/bin/omxplayer -b /home/pi/20110729005.mp4");
        break;
    case 'b':
        send_msg("Receive b");
        sprintf(proc_buf, "kill %d", child_pid);
        printf("b: %s\n", proc_buf);
        system(proc_buf);
        break;
    default:
        send_msg("Unknown command");
        debug("Unknown command (%s) received!\n", cmd);
        break;
    }
}


int   create_process(const char* file, const char* argv)
{
    int child;
    char* strargv[128] = { 0 };
    int i = 0;
    char* tempargv = (char*)malloc(strlen(argv) + 1);
    strcpy(tempargv, argv);

    strargv[i] = strtok(tempargv, " ");
    while(strargv[i] != NULL)
    {
        i++;
        strargv[i] = strtok(NULL, " ");
    }

    if((child = fork()) == -1)
    {
        perror("fork process error\n");
        exit(EXIT_FAILURE);
    }
    else if(child == 0)
    {
        if(execv(file, strargv) == -1)
        {
            perror("execv error");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        return child;
    }
}


