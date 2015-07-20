#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include "Options.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT 12345

typedef struct tagPerHandleData
{
	SOCKET socket;
}PerHandleData, *pPerHandleData;

typedef struct tagPerIoData
{
	OVERLAPPED OverLapped;
	WSABUF wsaBuf;
	char Buffer[DEFAULT_BUFLEN];
	int OprationType;
}PerIoData, *pPerIoData;

BOOL InitSocket();
void CreateWorkThreads(HANDLE hCompletionPort);
DWORD WINAPI WorkThread(_In_  LPVOID lpParameter);

SOCKET ListenSocket;

int main(void)
{
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN] = { 0 };

	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateWorkThreads(hCompletionPort);
	
	if (!InitSocket())
	{
		return 1;
	}
	COptions::Init();
	

	//----------------------
	// Listen for incoming connection requests.
	// on the created socket
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	//----------------------
	// Create a SOCKET for accepting incoming requests.
	SOCKET AcceptSocket = { 0 };
	SOCKADDR_IN client_addr = { 0 };
	int addr_len = sizeof(SOCKADDR_IN);
	wprintf(L"Waiting for client to connect...\n");

	//COptions::Get()->AddClient(AcceptSocket, client_addr, "aaa");
	//COptions::Get()->AddClient(AcceptSocket, client_addr, "bbb");
	//COptions::Get()->AddClient(AcceptSocket, client_addr, "ccc");
	COptions::Get()->PrintClient();

	while (true)
	{
		//----------------------
		// Accept the connection.
		AcceptSocket = accept(ListenSocket, (SOCKADDR *)&client_addr, &addr_len);
		if (AcceptSocket == INVALID_SOCKET) 
		{
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		recv(AcceptSocket, recvbuf, recvbuflen, 0);
		printf_s("Addr: %s\n", recvbuf);

		pPerHandleData pHandleData = new PerHandleData;
		pHandleData->socket = AcceptSocket;
		CreateIoCompletionPort((HANDLE)AcceptSocket, hCompletionPort, (ULONG_PTR)pHandleData, 0);

		pPerIoData pIoData = new PerIoData;
		ZeroMemory((void*)(&pIoData->OverLapped), sizeof(pIoData->OverLapped));
		pIoData->wsaBuf.buf = pIoData->Buffer;
		pIoData->wsaBuf.len = DEFAULT_BUFLEN;
		pIoData->OprationType = 0;

		DWORD dwRecv = 0;
		DWORD dwFlag = 0;
		WSARecv(pHandleData->socket, &pIoData->wsaBuf, 1, &dwRecv, &dwFlag, &pIoData->OverLapped, NULL);
	}
	// No longer need server socket
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}

//http://blog.chinaunix.net/uid-26335251-id-3497970.html

DWORD WINAPI WorkThread(_In_  LPVOID lpParameter)
{
	HANDLE hCompletionPort = (HANDLE)lpParameter;
	DWORD dwBytesTransferred;
	pPerHandleData pHandleData = NULL;
	pPerIoData pIoData = NULL;
	LPOVERLAPPED pOverLapped = NULL;
	DWORD dwRecv = 0;
	DWORD dwFlag = 0;

	while (true)
	{
		if (!GetQueuedCompletionStatus(hCompletionPort, &dwBytesTransferred, (PULONG_PTR)&pHandleData, &pOverLapped, INFINITE))
		{
			printf_s("Get Completion Status Error: %ld\n", GetLastError());
		}
		pIoData = CONTAINING_RECORD(pOverLapped, PerIoData, OverLapped);

		if (dwBytesTransferred == 0)
		{
			closesocket(pHandleData->socket);
			delete pHandleData;
			delete pIoData;
			continue;
		}

		printf_s("Receive: %s\n", pIoData->Buffer);

		ZeroMemory((void*)(&pIoData->OverLapped), sizeof(pIoData->OverLapped));
		pIoData->wsaBuf.buf = pIoData->Buffer;
		pIoData->wsaBuf.len = DEFAULT_BUFLEN;
		pIoData->OprationType = 0;
		WSARecv(pHandleData->socket, &pIoData->wsaBuf, 1, &dwRecv, &dwFlag, &pIoData->OverLapped, NULL);
	}
	return 0;
}

void CreateWorkThreads(HANDLE hCompletionPort)
{
	HANDLE hThread;
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);
	for (size_t i = 0; i < (2 * sys_info.dwNumberOfProcessors) + 1; i++)
	{
		hThread = CreateThread(NULL, 0, WorkThread, (LPVOID)hCompletionPort, 0, NULL);
		if (!hThread)
		{
			printf_s("Create Thread failed!\n");
		}
		CloseHandle(hThread);
	}
}

BOOL InitSocket()
{
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return FALSE;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return FALSE;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(DEFAULT_PORT);

	if (bind(ListenSocket,
		(SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
		wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}