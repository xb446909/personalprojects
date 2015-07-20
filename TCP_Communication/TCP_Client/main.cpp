#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

#define REMOTE_SERVER

#ifdef REMOTE_SERVER

#define		REMOTE_PORT		37872
#define		REMOTE_NAME		"udptest.nat123.net"

#else

#define		REMOTE_PORT		12345
#define		REMOTE_NAME		"localhost"

#endif // REMOTE_SERVER

int __cdecl main() {

	//----------------------
	// Declare and initialize variables.
	WSADATA wsaData;
	int iResult;

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;

	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	struct hostent *remoteHost;
	DWORD dwError;

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed: %d\n", iResult);
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

	//----------------------
	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = *(u_long *)remoteHost->h_addr_list[0];
	clientService.sin_port = htons(REMOTE_PORT);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		printf("Unable to connect to server: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf) + 1, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			printf_s("Recv: %s\n", recvbuf);
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());

	} while (iResult > 0);

	while (true)
	{

	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

