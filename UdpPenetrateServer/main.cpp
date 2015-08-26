#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include "ClientList.h"
#include "logger.h"

#define DEFAULT_PORT 10000
#define CMD_BUF_LEN  1024

void ProcessMsg();
void send_msg(struct sockaddr_in remoteaddr, char* msg);
void* udp_penetrate(void* arg);

int port = DEFAULT_PORT;
char recv_buf[CMD_BUF_LEN] = { 0 };
int sockfd = 0;
struct sockaddr_in addr_ser = { 0 };
pthread_t penetrate_thread;

int main (int argc, char** argv)
{
	int result = 0;

	CClientList::Get()->Init();

	CClientList::Get()->test();


	while ((result = getopt(argc, argv, "p:h")) != -1)
	{
		switch (result)
		{
		case 'p':
			port = atoi(optarg);
			break;
		case 'h':
			fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
			exit(0);
			break;
		case '?':
			fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
			exit(0);
			break;
		default:
			fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
			exit(0);
			break;
		}
	}

	result = pthread_create(&penetrate_thread, NULL, udp_penetrate, NULL);
	if (result != 0)
	{
		debug("Thread creation failed!");
		exit(0);
	}

	while (1)
	{
		sleep(10);
	}
}

void* udp_penetrate(void* arg)
{
	ProcessMsg();
}

void ProcessMsg()
{
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	int err;
	int read_len = 0;
	char tmp[5] = { 0 };

	ClientInfo info;

	if (sockfd == -1)
	{
		debug("Create socket error, %d\n", errno);
		return;
	}

	addr_ser.sin_family = AF_INET;
	addr_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_ser.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr*)&addr_ser, sizeof(addr_ser)) < 0)
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

	while (1)
	{
		memset(recv_buf, '\0', CMD_BUF_LEN);
		memset(tmp, '\0', 5);
		read_len = recvfrom(sockfd, recv_buf, CMD_BUF_LEN, 0,
			(struct sockaddr*)&clientAddr, &len);
		printf("recvfrom %s:%d length: %d, recv: %s\n",
			inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port,
			read_len, recv_buf);

		memcpy(tmp, recv_buf, 4);

		if (strcmp(tmp, "#REG") == 0)
		{
			strcpy(info.name, &recv_buf[4]);
			memcpy(&info.addr, &clientAddr, len);
			CClientList::Get()->RegClient(info);
		}
		send_msg(clientAddr, (char*)"Received!\n");
	}
}

void send_msg(struct sockaddr_in remoteaddr, char* msg)
{
	sendto(sockfd, msg, strlen(msg), 0,
		(struct sockaddr*)&remoteaddr, sizeof(struct sockaddr));
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

	va_start(arg, format);
	vfprintf(__stream, format, arg);
	va_end(arg);

	fclose(__stream);

}