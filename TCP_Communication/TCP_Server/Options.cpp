#include "Options.h"
#include <stdio.h>

COptions* COptions::m_options = NULL;

COptions::COptions()
{
}


COptions::~COptions()
{
}

void COptions::Init()
{
	m_options = new COptions();
}


COptions* COptions::Get()
{
	return m_options;
}


void COptions::AddClient(SOCKET fd, SOCKADDR_IN addr, const char* name)
{
	pClientList pTemp = head.next;
	pClientList pList = new ClientList;

	pList->fd = fd;
	pList->addr = addr;
	strcpy_s(pList->name, 512, name);
	pList->next = pTemp;

	head.next = pList;
	ClientCnt++;
}

void COptions::PrintClient()
{
	pClientList pTemp = head.next;
	printf_s("There are %d clients!\n", ClientCnt);
	while (pTemp != NULL)
	{
		printf_s("name: %s\n", pTemp->name);
		pTemp = pTemp->next;
	}
}

