#include <iostream>
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

void CClientList::Init()
{
	m_clientList = new CClientList();
}

void CClientList::print()
{
	cout << "This is print from ClientList" << endl;
}