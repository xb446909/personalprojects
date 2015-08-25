#ifndef __CCLIENT_H_
#define __CCLIENT_H_

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
	void print();
};


#endif
