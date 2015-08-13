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
void    process_command(char* cmd);
int     connect_server(void);
int     create_process(const char* file, const char* argv);
void*   keeponline(void* arg);
void    send_msg(struct sockaddr_in remoteaddr, char* msg);
int     opencom(char* dev);
int     set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);


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
        //recvfrom(sockfd, recv_buf, CMD_BUF_LEN, 0, 
          //      (struct sockaddr*)&addr_ser, &len);
        //process_command(recv_buf);
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
        break;
    case 'A':
        break;
    case 'b':
        break;
    default:
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


int opencom(char* dev)
{
    int fd = 0;

    fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
    {
        debug("Can't open serial port %s\n", dev);
        return fd;
    }

    if(fcntl(fd, F_SETFL, 0) < 0)
    {
        debug("fcntl failed! port: %s\n", dev);
    }
    else
    {
        debug("fcntl = %d, port: %s\n", fcntl(fd, F_SETFL, 0), dev);
    }

    if(isatty(STDIN_FILENO) == 0)
    {
        debug("standard input is not a terminal device\n");
    }
    else
    {
        debug("isatty success!\n");
    }

    debug("open serial port %s successfully, fd: %d\n", dev, fd);

    return fd;
}


int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;

    if  ( tcgetattr( fd,&oldtio)  !=  0) 
    { 
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 

    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( nEvent )
    {
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}
