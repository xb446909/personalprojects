#pragma once
#include <winsock2.h>

typedef struct _client_list
{
	SOCKET fd;
	SOCKADDR_IN addr;
	char name[512];
	struct _client_list* next;
}ClientList, *pClientList;

class COptions
{
private:
	static COptions* m_options;
	ClientList head;
	int ClientCnt = 0;

public:
	COptions();
	~COptions();
	static void Init();
	static COptions* Get();
	void AddClient(SOCKET fd, SOCKADDR_IN addr, const char* name);
	void PrintClient();
};

