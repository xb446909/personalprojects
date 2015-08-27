#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/md5.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>

#include "logger.h"
#include "process.h"

extern char hostname[CMD_BUF_LEN];
extern int port;
extern char dev_name[MAX_FILEPATH];
extern int nBurdrate;
extern int nDatWidth;
extern int nEvent;
extern int nStop;


int sockfd;
int fd;
char server_recv_buf[CMD_BUF_LEN] = { 0 };
char short_recv_buf[CMD_BUF_LEN] = { 0 };
char proc_buf[CMD_BUF_LEN] = { 0 };
int child_pid;
pthread_t keeponline_thread;
pthread_t shortmessage_thread;
pthread_t servermessage_thread;
struct sockaddr_in addr_ser = { 0 };
struct hostent* phost;

void    process_all(void);
void    process_command(char* cmd);
int     connect_server(void);
int     create_process(const char* file, const char* argv);
void*   keeponline(void* arg);
void*   recv_shortmessage(void* arg);
void*   recv_servermessage(void* arg);
void    send_msg(char* msg);
void	com_write(char* msg);
int     opencom(char* dev);
int     set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
void    splitstr(char* src, char* dst);
void	base64_decode(char *src, char *dst, int *decode_len);
void	calc_md5(char* src, char* dst);
void	crypto(char* src, int encode, char* key, char* dst, int expiry);


void send_msg(char* msg)
{
	sendto(sockfd, msg, strlen(msg), 0,
		(struct sockaddr*)&addr_ser, sizeof(addr_ser));
}


void com_write(char* msg)
{
	if (write(fd, msg, strlen(msg)) < 0)
	{
		debug("Write COM error!, msg: %s\n", msg);
	}
}


void process_all(void)
{
	int res;

	phost = gethostbyname(hostname);

	char* dst[513] = { 0 };
	char* src[513] = { 0 };

	if (phost == NULL)
	{
		perror("Get host error");
		debug("Can't get host by %s\n", hostname);
	}
	else
	{

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);

		if (sockfd == -1)
		{
			debug("Create socket error, %d\n", errno);
			return;
		}

		addr_ser.sin_family = AF_INET;
		memcpy(&addr_ser.sin_addr.s_addr, phost->h_addr_list[0], phost->h_length);
		addr_ser.sin_port = htons(port);

		debug("Connecting to %s(%s):%d\n", hostname, inet_ntoa(addr_ser.sin_addr), port);

		sendto(sockfd, "Hello!", strlen("Hello!"), 0,
			(struct sockaddr*)&addr_ser, sizeof(addr_ser));

		res = pthread_create(&keeponline_thread, NULL, keeponline, NULL);
		if (res != 0)
		{
			debug("Keep Online thread creation failed!");
			exit(0);
		}

		res = pthread_create(&servermessage_thread, NULL, recv_servermessage, NULL);
		if (res != 0)
		{
			debug("Receive server thread creation failed!");
			exit(0);
		}
	}

	fd = opencom(dev_name);

	if (fd < 0)
	{
		perror("open error\n");
		return;
	}

	int i = set_opt(fd, nBurdrate, nDatWidth, nEvent, nStop);
	if (i < 0)
	{
		perror("set opt error");
		return;
	}

	res = pthread_create(&shortmessage_thread, NULL, recv_shortmessage, NULL);
	if (res != 0)
	{
		debug("Receive short message thread creation failed!");
		exit(0);
	}

	char line[50];
	int len;

	while (1)
	{
		memset(line, 0, 50);
		if (gets(line))
		{
			len = strlen(line);
			line[len] = '\r';
			line[len + 1] = '\n';
			line[len + 2] = '\0';
			com_write(line);
		}
	}


}


void*   recv_shortmessage(void* arg)
{
	char cmd[MAX_FILEPATH];
	while (1)
	{
		memset(short_recv_buf, 0, CMD_BUF_LEN);
		int n = read(fd, short_recv_buf, CMD_BUF_LEN);
		if (n > 0)
		{
			memset(cmd, 0, MAX_FILEPATH);
			printf("recv: %s", short_recv_buf);
			splitstr(short_recv_buf, cmd);
			if (cmd[0] != 0)
			{
				printf("cmd: %s", cmd);
			}
		}
	}

}


void    splitstr(char* src, char* dst)
{
	int i = 2;
	int j = 0;
	int flag = 0;
	while (src[i] != '\0')
	{
		if (flag == 1)
		{
			dst[j++] = src[i];
		}
		if (src[i] == '\n')
		{
			flag = 1;
		}
		i++;
	}
}


void*   recv_servermessage(void* arg)
{
	struct sockaddr_in clientAddr = { 0 };
	int len = sizeof(clientAddr);
	int n;

	while (1)
	{
		memset(server_recv_buf, 0, CMD_BUF_LEN);
		n = recvfrom(sockfd, server_recv_buf, CMD_BUF_LEN, 0,
			(struct sockaddr*)&clientAddr, &len);
		printf("recvfrom %s:%d length: %d, recv: %s\n",
			inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port,
			n, server_recv_buf);
		process_command(server_recv_buf);
	}
}


void*   keeponline(void* arg)
{
	while (1)
	{
		sendto(sockfd, "Hello!", strlen("Hello!"), 0,
			(struct sockaddr*)&addr_ser, sizeof(addr_ser));
		sleep(20);
	}
}

/*
(1)配置指令：#cfg
1)配置更新 #cfg#upconfig
2)固件更新 #cfg#upfirmware
(2)文件指令：#fls
1)资源更新 #fls#upsource
2)资源删除 #fls#delsource
3)播放列表更新 #fls#uplist
4)子播放列表添加 #fls#addchildlist
5)子播放列表更新 #fls#upchildlist
6)子播放列表删除 #fls#delchildlist
(3)播放指令 #mdi
1)暂停 #mdi#pause
2)播放 #mdi#play
3)上一个 #mdi#next
4)下一个 #mdi#prev
5)播放路况（电信号）
6)播放倒计时（电信号）
7)播放广告（电信号）
(4)系统指令 #sys
1)开户屏幕 #sys#openled
2)关闭屏幕 #sys#closeled
3)重启系统 #sys#rebootsys
4)重启程序 #sys#rebootpro
*/

void process_command(char* cmd)
{
	char file_cwd[MAX_FILEPATH] = { 0 };
	getcwd(file_cwd, MAX_FILEPATH);

	if (cmd[0] != '#')
	{
		debug("Unknown command (%s) received!\n", cmd);
		return;
	}

	switch (cmd[1])
	{
	case 'c':
		if (strcmp(&cmd[5], "upconfig") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "upfirmware") == 0)
		{
			break;
		}
		send_msg("Receive a");
		system("rm log.txt -f");
		break;
	case 'f':
		if (strcmp(&cmd[5], "upsource") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "delsource") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "uplist") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "addchildlist") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "upchildlist") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "delchildlist") == 0)
		{
			break;
		}
		send_msg("Receive A");
		//sprintf(file_cwd, "%s/test.exe", file_cwd);
		break;
	case 'm':
		if (strcmp(&cmd[5], "pause") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "play") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "next") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "prev") == 0)
		{
			break;
		}
		child_pid = create_process("/usr/bin/omxplayer", "/usr/bin/omxplayer -b /home/pi/20110729005.mp4");
		send_msg("Receive b");
		sprintf(proc_buf, "kill %d", child_pid);
		printf("b: %s\n", proc_buf);
		system(proc_buf);
		break;
	case 's':
		if (strcmp(&cmd[5], "openled") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "closeled") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "rebootsys") == 0)
		{
			break;
		}
		if (strcmp(&cmd[5], "rebootpro") == 0)
		{
			break;
		}
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
	while (strargv[i] != NULL)
	{
		i++;
		strargv[i] = strtok(NULL, " ");
	}

	if ((child = fork()) == -1)
	{
		perror("fork process error\n");
		exit(EXIT_FAILURE);
	}
	else if (child == 0)
	{
		if (execv(file, strargv) == -1)
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
	if (fd == -1)
	{
		debug("Can't open serial port %s\n", dev);
		return fd;
	}

	if (fcntl(fd, F_SETFL, 0) < 0)
	{
		debug("fcntl failed! port: %s\n", dev);
	}
	else
	{
		debug("fcntl = %d, port: %s\n", fcntl(fd, F_SETFL, 0), dev);
	}

	if (isatty(STDIN_FILENO) == 0)
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
	struct termios newtio, oldtio;

	if (tcgetattr(fd, &oldtio) != 0)
	{
		perror("SetupSerial 1");
		return -1;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	switch (nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch (nEvent)
	{
	case 1:                     //奇校验
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 2:                     //偶校验
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 0:                    //无校验
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch (nSpeed)
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
	if (nStop == 1)
	{
		newtio.c_cflag &= ~CSTOPB;
	}
	else if (nStop == 2)
	{
		newtio.c_cflag |= CSTOPB;
	}
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);
	if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
	{
		perror("com set error");
		return -1;
	}
	return 0;
}

void base64_decode(char *src, char *dst, int *decode_len)
{
	char *q = malloc(strlen(src) + 1);
	char *p = dst;
	char *temp = q;
	char *s = src;
	int len = strlen(src), i;
	memset(q, 0, strlen(src) + 1);
	while (*s)
	{
		if (*s >= 'A'&&*s <= 'Z') *temp = *s - 'A';
		else if (*s >= 'a'&&*s <= 'z') *temp = *s - 'a' + 26;
		else if (*s >= '0'&&*s <= '9') *temp = *s - '0' + 52;
		else if (*s == '+') *temp = 62;
		else if (*s == '/') *temp = 63;
		else if (*s == '=') *temp = -1;
		else
		{
			printf("\n%c:Not a valid base64 string\n", *s);
			exit(0);
		}
		++s;
		++temp;
	}
	for (i = 0; i<len - 4; i += 4)
	{
		*p++ = (*(q + i) << 2) + (*(q + i + 1) >> 4);
		*p++ = (*(q + i + 1) << 4) + (*(q + i + 2) >> 2);
		*p++ = (*(q + i + 2) << 6) + (*(q + i + 3));
	}
	if (*(q + i + 3) != -1)
	{
		*p++ = (*(q + i) << 2) + (*(q + i + 1) >> 4);
		*p++ = (*(q + i + 1) << 4) + (*(q + i + 2) >> 2);
		*p++ = (*(q + i + 2) << 6) + *(q + i + 3);
	}
	else if (*(q + i + 2) != -1)
	{
		*p++ = (*(q + i) << 2) + (*(q + i + 1) >> 4);
		*p++ = (*(q + i + 1) << 4) + (*(q + i + 2) >> 2);
		*p++ = (*(q + i + 2) << 6);
	}
	else if (*(q + i + 1) != -1)
	{
		*p++ = (*(q + i) << 2) + (*(q + i + 1) >> 4);
		*p++ = (*(q + i + 1) << 4);
	}
	else
	{
		printf("Not a valid base64 string\n");
		exit(0);
	}
	*p = 0;
	*decode_len = len * 6 / 8;
	free(q);
}

/**
* 加密解密函数
* @param char* src 待加密解密字符串
* @param int encode 操作方法 非0加密 0解密
* @param char* key 加密解密效验码
* @param char* dst 处理后的字符串
*/
void	crypto(char* src, int encode, char* key, char* dst, int expiry)
{
	char key1[33] = { '\0' };
	char keya[33] = { '\0' };
	char keyb[33] = { '\0' };
	char key1a[17] = { '\0' };
	char key1b[17] = { '\0' };

	char tempstr[65] = { '\0' };
	time_t t;
	int i, j, k, temp;

	int ckey_len = 4;
	char* keyc = (char*)malloc(ckey_len + 1);
	memset(keyc, '\0', ckey_len + 1);

	calc_md5(key, key1);
	memcpy(key1a, key1, 16);
	memcpy(key1b, &key1[16], 16);
	calc_md5(key1a, keya);
	calc_md5(key1b, keyb);
	if (ckey_len)
	{
		if (encode)
		{
			time(&t);
			sprintf(tempstr, "%032ld", t);
			memcpy(keyc, &tempstr[strlen(tempstr) - ckey_len], ckey_len);
		}
		else
		{
			memcpy(keyc, src, ckey_len);
		}
	}

	char cryptkey[65] = { '\0' };
	strcpy(tempstr, keya);
	strcat(tempstr, keyc);
	calc_md5(tempstr, cryptkey);
	strcpy(tempstr, keya);
	strcat(tempstr, cryptkey);
	strcpy(cryptkey, tempstr);

	int key_len = strlen(cryptkey);
	int str_len;

	char str[257] = { '\0' };
	char* decode_str;
	char* encode_str;
	int decode_size;
	if (encode)
	{
		time(&t);
		sprintf(str, "%010d", expiry ? expiry + t : 0);
		strcpy(tempstr, src);
		strcat(tempstr, keyb);
		calc_md5(tempstr, keya);
		memset(tempstr, 0, 65);
		memcpy(tempstr, keya, 16);
		strcat(str, tempstr);
		strcat(str, src);
		str_len = strlen(str);
	}
	else
	{
		memset(tempstr, 0, 65);
		strcpy(tempstr, &src[ckey_len]);
		base64_decode(tempstr, str, &str_len);
	}

	int box[256] = { 0 };
	int rndkey[256] = { 0 };
	for (i = 0; i < 256; i++)
	{
		box[i] = i;
		rndkey[i] = (int)cryptkey[i % key_len];
	}
	for (i = 0, j = 0; i < 256; i++)
	{
		j = (j + box[i] + rndkey[i]) % 256;
		k = box[i];
		box[i] = box[j];
		box[j] = k;
	}
	unsigned char* result = (unsigned char*)malloc(strlen + 1);
	memset(result, '\0', strlen + 1);
	for (i = 0, j = 0, k = 0; i < str_len; i++)
	{
		k = (k + 1) % 256;
		j = (j + box[k]) % 256;
		temp = box[k];
		box[k] = box[j];
		box[j] = temp;
		result[i] = str[i] ^ (box[(box[j] + box[k]) % 256]);
	}

	time_t t_tmp;
	char* encode_tmp;
	int encode_size;
	if (encode)
	{
		encode_str = g_base64_encode(result, str_len);
		encode_size = strlen(encode_str);
		encode_tmp = (char*)malloc(encode_size + 1);
		memset(encode_tmp, '\0', encode_size + 1);
		for (i = 0, j = 0; i < encode_size; i++)
		{
			if (encode_str[i] != '=')
			{
				encode_tmp[j] = encode_str[i];
				j++;
			}
		}
		g_free(encode_str);
		strcpy(dst, keyc);
		strcat(dst, encode_tmp);
		free(encode_tmp);
	}
	else
	{
		memset(tempstr, 0, 65);
		memcpy(tempstr, result, 10);
		t_tmp = atol(tempstr);
		time(&t);

		memset(tempstr, 0, 65);
		strcpy(tempstr, &result[26]);
		strcat(tempstr, keyb);
		calc_md5(tempstr, str);
		memset(tempstr, 0, 65);
		memcpy(tempstr, str, 16);
		memset(str, 0, 257);
		memcpy(str, &result[10], 16);
		if (((t_tmp == 0) || (t_tmp - t) > 0) && (strcmp(str, tempstr) == 0))
		{
			strcpy(dst, &result[26]);
		}
	}
	free(result);
	free(keyc);
}

void	calc_md5(char* src, char* dst)
{
	int i;
	MD5_CTX ctx;
	unsigned char md[16] = { 0 };
	char temp[3] = { 0 };
	memset(dst, '\0', 33);
	MD5_Init(&ctx);
	MD5_Update(&ctx, src, strlen(src));
	MD5_Final(md, &ctx);
	for (i = 0; i < 16; i++)
	{
		sprintf(temp, "%2.2x", md[i]);
		strcat(dst, temp);
	}
}