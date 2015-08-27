#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "ClientList.h"

using namespace std;

sqlite3* db;
int rc;
int nrow = 0;
int ncolumn = 0;
char* zErrMsg;
char **azResult;
stringstream sql;


CClientList::CClientList()
{
}

CClientList::~CClientList()
{
	sqlite3_close(db);
}

CClientList* CClientList::m_clientList = 0;

extern void send_msg(struct sockaddr_in remoteaddr,const char* msg);
extern void send_clientinfo(struct sockaddr_in remoteaddr, int num, ClientInfo* pInfo);

static int callback(void *nEvent, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;

}

CClientList * CClientList::Get()
{
	return m_clientList;
}


void CClientList::Init()
{
	m_clientList = new CClientList();
	rc = sqlite3_open("", &db);
	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(0);
	}

	sql.str("");
	sql << "create table " << TABLE_NAME << "(name text, ip text, port integer, lastTime long)";

	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}
}

void CClientList::test()
{
	sql.str("");
	sql << "insert into " << TABLE_NAME << "(name, ip, port, lastTime) values('111', '10.10.10.10', 1234, 1111111111)";

	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}

	sql.str("");
	sql << "insert into " << TABLE_NAME << "(name, ip, port, lastTime) values('222', '10.10.10.10', 2234, 2222222222)";

	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}


	sql.str("");
	sql << "select * from " << TABLE_NAME << " where ip != '10.10.10.10' or port != 1234";
	cout << sql.str() << endl;

	//rc = sqlite3_get_table(db, sql.str().c_str(), &azResult, &nrow, &ncolumn, &zErrMsg);
	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	sqlite3_free(zErrMsg);
	exit(0);
	}


	/*	cout << "Row:" << nrow << endl << "Column:" << ncolumn << endl;
	if (nrow * ncolumn)
	{
		cout << "Result:" << azResult[1] << endl;
	}
	sqlite3_free_table(azResult);
	
	sql.str("");
	sql << "select * from " << TABLE_NAME;
	cout << sql.str() << endl;

	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}
	*/
	cout << "This is print from ClientList" << endl;
}

void CClientList::RegClient(ClientInfo info)
{
	time_t t;
	sql.str("");
	sql << "select * from " << TABLE_NAME << "where ip='" << inet_ntoa(info.addr.sin_addr) << "' and port=" << info.addr.sin_port;

	rc = sqlite3_get_table(db, sql.str().c_str(), &azResult, &nrow, &ncolumn, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}

	time(&t);

	if (nrow == 0)
	{
		sql.str("");
		sql << "insert into " << TABLE_NAME << "(name, ip, port, lastTime) values(";
		sql << "'" << info.name << "', ";
		sql << "'" << inet_ntoa(info.addr.sin_addr) << "', ";
		sql << info.addr.sin_port << ", ";
		sql << t << ")";

		rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			exit(0);
		}
	}
	else
	{
		sql.str("");
		sql << "update" << TABLE_NAME << "set ";
		sql << "name='" << info.name << "', ";
		sql << "lastTime=" << t;
		sql << " where ip='" << inet_ntoa(info.addr.sin_addr) << "' and port=" << info.addr.sin_port;

		rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			exit(0);
		}
	}
	sqlite3_free_table(azResult);
	cout << info.name << " from " << inet_ntoa(info.addr.sin_addr) << ":" << ntohs(info.addr.sin_port) << endl;
}

void CClientList::GetClients(ClientInfo info)
{
	time_t t;
	time(&t);

	sql.str("");
	sql << "delete from " << TABLE_NAME << "where lastTime < " << t - TIME_OUT_S;

	rc = sqlite3_exec(db, sql.str().c_str(), callback, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}

	sql.str("");
	sql << "select * from " << TABLE_NAME << "where ip!='" << inet_ntoa(info.addr.sin_addr) << "' or port!=" << info.addr.sin_port;

	rc = sqlite3_get_table(db, sql.str().c_str(), &azResult, &nrow, &ncolumn, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}

	sql.str("");
	sql << nrow;

	send_msg(info.addr, sql.str().c_str());

	if (nrow > 0)
	{
		ClientInfo* pInfo = new ClientInfo[nrow];
		for (int i = 1; i <= nrow; i++)
		{
			pInfo[i - 1].addr.sin_family = AF_INET;
			pInfo[i - 1].addr.sin_addr.s_addr = inet_addr(azResult[i * 4 + 1]);
			pInfo[i - 1].addr.sin_port = atoi(azResult[i * 4 + 2]);
			memcpy(pInfo[i - 1].name, azResult[i * 4], 16);

			cout << "name:" << pInfo[i - 1].name << " addr: " << inet_ntoa(pInfo[i - 1].addr.sin_addr) << ":" << ntohs(pInfo[i - 1].addr.sin_port) << endl;
		}
		send_clientinfo(info.addr, nrow, pInfo);
		delete[] pInfo;
	}
}