#ifndef UNICODE
#define UNICODE
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

//#define		REMOTE_PORT		11109
#define		REMOTE_PORT		10000
//#define		REMOTE_NAME		"www.sinlody.com"
#define		REMOTE_NAME		"localhost"

int main()
{

	int iResult;
	WSADATA wsaData;

	SOCKET SendSocket = INVALID_SOCKET;
	sockaddr_in RecvAddr;
	struct hostent *remoteHost;
	DWORD dwError;

	unsigned short Port = REMOTE_PORT;

	char SendBuf[1024];
	char RecvBuf[1024];

	sprintf(SendBuf, "HelloWorld");

	int BufLen = 1024;


	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	remoteHost = gethostbyname(REMOTE_NAME);

	if (remoteHost == NULL) {
		dwError = WSAGetLastError();
		if (dwError != 0) {
			if (dwError == WSAHOST_NOT_FOUND) {
				printf("Host not found\n");
				return 1;
			}
			else if (dwError == WSANO_DATA) {
				printf("No data record found\n");
				return 1;
			}
			else {
				printf("Function failed with error: %ld\n", dwError);
				return 1;
			}
		}
	}

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.1.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];

	int recv_len = sizeof(RecvAddr);

	//---------------------------------------------
	// Send a datagram to the receiver
	wprintf(L"Sending a datagram to the receiver...\n");
	BufLen = strlen(SendBuf);
	iResult = sendto(SendSocket,
		SendBuf, BufLen, 0, (SOCKADDR *) & RecvAddr, sizeof (RecvAddr));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return 1;
	}

	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	wprintf(L"Receiving datagrams...\n");
	iResult = recvfrom(SendSocket,
		RecvBuf, BufLen, 0, (SOCKADDR *)& RecvAddr, &recv_len);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
	}
	printf_s("Recv: %s\n", RecvBuf);

	//---------------------------------------------
	// When the application is finished sending, close the socket.
	wprintf(L"Finished sending. Closing socket.\n");
	iResult = closesocket(SendSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n");
	WSACleanup();
	return 0;
}



