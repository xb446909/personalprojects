#ifndef __PROCESS_H
#define __PROCESS_H

#define DEFAULT_IP  "127.0.0.1"
#define DEFAULT_PORT    10000
#define CMD_BUF_LEN     1024
#define MAX_FILEPATH    80


enum
{
    ST_DISCONNECT,
    ST_CONNECT
}connect_state;


void process_all(void);

#endif
