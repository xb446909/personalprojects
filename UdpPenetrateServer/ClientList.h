#ifndef __CCLIENT_H_
#define __CCLIENT_H_

#include <vector>
#include <arpa/inet.h>
#include <sqlite3.h>

typedef struct ClientList_Struct
{
	char name[16];
	struct sockaddr_in addr;
	time_t lastOnline;
}ClientInfo;

class CClientList
{
public:
	CClientList();
	~CClientList();
	static CClientList* Get();
	void Init();

private:
	static CClientList* m_clientList;
	sqlite3* db;
public:
	void print();
	void RegClient(ClientInfo info);


};


#endif
