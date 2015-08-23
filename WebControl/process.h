#ifndef __PROCESS_H
#define __PROCESS_H

#define DEFAULT_CONFIG  "config.ini"

#define DEFAULT_HOST    "localhost"
#define DEFAULT_PORT    10000
#define CMD_BUF_LEN     1024
#define MAX_FILEPATH    80

#define DEFAULT_DEV     "/dev/ttyS0"
#define DEFAULT_BURD    115200
#define DEFAULT_WIDTH   8
#define DEFAULT_EVENT   0
#define DEFAULT_STOP    1


struct ParamStruct {
    const char* name;
    const char SorI;
    void* pParam;
};


void process_all(void);

#endif
