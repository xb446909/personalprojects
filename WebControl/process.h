#ifndef __PROCESS_H
#define __PROCESS_H

#define DEFAULT_HOST    "localhost"
#define DEFAULT_PORT    10000
#define CMD_BUF_LEN     1024
#define MAX_FILEPATH    80

#define DEFAULT_DEV     "/dev/ttyS0"
#define DEFAULT_BURD    115200
#define DEFAULT_WIDTH   8
#define DEFAULT_EVENT   'N'
#define DEFAULT_STOP    1


struct ParamStruct {
    void* pParam;
    char SorI;
    char name[MAX_FILEPATH];
};


void process_all(void);

#endif
