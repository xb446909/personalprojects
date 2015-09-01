#ifndef __CCLIENT_H_
#define __CCLIENT_H_

#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <sqlite3.h>

typedef struct ClientList_Struct
{
	char name[17];
	struct sockaddr_in addr;
}ClientInfo;

#define	TABLE_NAME	" ClientInfo "
#define TIME_OUT_S	20

class CClientList
{
public:
	CClientList();
	~CClientList();
	static CClientList* Get();
	void Init();

private:
	static CClientList* m_clientList;
public:
	void test();
	void RegClient(ClientInfo info);
	void GetClients(ClientInfo info);
	void SendMsg(ClientInfo src, ClientInfo dst);
};


#endif
