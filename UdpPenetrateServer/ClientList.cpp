#include <iostream>
#include <vector>
#include "ClientList.h"

using namespace std;

CClientList::CClientList()
{
}

CClientList::~CClientList()
{
}

CClientList* CClientList::m_clientList = 0;

CClientList * CClientList::Get()
{
	return m_clientList;
}
//http://www.sqlite.org/quickstart.html
void CClientList::Init()
{
	int rc;
	m_clientList = new CClientList();
	rc = sqlite3_open(":memory:", &db);
}

void CClientList::print()
{
	cout << "This is print from ClientList" << endl;
}