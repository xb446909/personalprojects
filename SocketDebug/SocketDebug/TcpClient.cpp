// TcpClient.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "TcpClient.h"
#include "afxdialogex.h"


static DWORD WINAPI TcpClientReceiveProc(LPVOID lpParameter);
extern void EditBoxAppendText(HWND hEditBox, LPCTSTR text);
extern int MultibyteToUnicode(LPCSTR strIn, LPWSTR strOut, int nLenOut);
BOOL TCPClientInitSocket();

SOCKET TcpClientConnectSocket = { 0 };
HANDLE hTcpClientReceive;

// CTcpClient 对话框
IMPLEMENT_DYNAMIC(CTcpClient, CDialogEx)

CTcpClient::CTcpClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTcpClient::IDD, pParent)
{

}

CTcpClient::~CTcpClient()
{
}

void CTcpClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTcpClient, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_TCPCLIENT_CLR, &CTcpClient::OnBnClickedBtnTcpclientClr)
	ON_BN_CLICKED(IDC_BTN_TCPCLIENT_SEND, &CTcpClient::OnBnClickedBtnTcpclientSend)
	ON_BN_CLICKED(IDC_BTN_TCPCLIENT_CON, &CTcpClient::OnBnClickedBtnTcpclientCon)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTcpClient 消息处理程序


void CTcpClient::OnBnClickedBtnTcpclientClr()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TCPCLIENT_RECV, L"");
}


void CTcpClient::OnBnClickedBtnTcpclientSend()
{
	// TODO: Add your control notification handler code here
	CString str;

	char send_buf[1024] = { 0 };
	GetDlgItemTextA(GetSafeHwnd(), IDC_EDIT_TCPCLIENT_SEND, send_buf, 1023);
	if (send(TcpClientConnectSocket, send_buf, strlen(send_buf) + 1, 0) == SOCKET_ERROR)
	{
		str.Format(L"Send error, code = %d", WSAGetLastError());
		MessageBox(str);
		return;
	}
	SetDlgItemText(IDC_EDIT_TCPCLIENT_SEND, L"");
}

BOOL TCPClientInitSocket()
{
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		return FALSE;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	TcpClientConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TcpClientConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

void CTcpClient::OnBnClickedBtnTcpclientCon()
{
	// TODO: Add your control notification handler code here
	DWORD dwExitCode;
	if (bConnect)
	{
		GetExitCodeThread(hTcpClientReceive, &dwExitCode);
		TerminateThread(hTcpClientReceive, dwExitCode);
		CloseHandle(hTcpClientReceive);
		closesocket(TcpClientConnectSocket);
		WSACleanup();
		SetDlgItemText(IDC_BTN_TCPCLIENT_CON, L"连接");
		SetDlgItemText(IDC_EDIT_TCPCLIENT_STATE, L"已断开");
		bConnect = false;
	}
	else
	{
		hTcpClientReceive = CreateThread(NULL, 0, TcpClientReceiveProc, (LPVOID)GetSafeHwnd(), 0, NULL);
		SetDlgItemText(IDC_BTN_TCPCLIENT_CON, L"断开");
		bConnect = true;
	}
}


HBRUSH CTcpClient::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
	return b;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CTcpClient::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialogEx::OnOK();
}


BOOL CTcpClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	bConnect = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


DWORD WINAPI TcpClientReceiveProc(LPVOID lpParameter)
{
	char recv_buf[1024];
	wchar_t wrecv_buf[1024] = { 0 };
	HWND hwnd = (HWND)lpParameter;
	HWND hEditReceive = GetDlgItem(hwnd, IDC_EDIT_TCPCLIENT_RECV);
	CString str;
	int iResult;
	ConnectSt con_state = ST_INIT;
	int port = 0;
	char ip_addr[20] = {0};

	SOCKADDR_IN server_addr = { 0 };
	int addr_len = sizeof(SOCKADDR_IN);

	while (true)
	{
		switch (con_state)
		{
		case ST_INIT:
			if (!TCPClientInitSocket())
			{
				WSACleanup();
				Sleep(1000);
				break;
			}
			SetDlgItemText(hwnd, IDC_EDIT_TCPCLIENT_STATE, L"正在连接");
			GetDlgItemTextA(hwnd, IDC_IP_TCPCLIENT, ip_addr, 20);
			port = GetDlgItemInt(hwnd, IDC_EDIT_TCPCLIENT_PORT, FALSE, FALSE);
			server_addr.sin_port = htons(port);
			server_addr.sin_addr.S_un.S_addr = inet_addr(ip_addr);
			server_addr.sin_family = AF_INET;
			iResult = connect(TcpClientConnectSocket, (sockaddr*)&server_addr, addr_len);
			if (iResult == SOCKET_ERROR)
			{
				Sleep(1000);
				break;
			}
			con_state = ST_CONNECTED;
			break;
		case ST_CONNECTED:
			SetDlgItemText(hwnd, IDC_EDIT_TCPCLIENT_STATE, L"已连接");
			iResult = recv(TcpClientConnectSocket, recv_buf, 1024, 0);
			if (iResult == SOCKET_ERROR)
			{
				iResult = WSAGetLastError();
				if (iResult == WSAECONNRESET)
				{
					str.Format(L"服务器已断开！\r\n");
				}
				else
				{
					str.Format(L"Recv Error! Error code: %d\r\n", iResult);
				}
				EditBoxAppendText(hEditReceive, str);
				closesocket(TcpClientConnectSocket);
				WSACleanup();
				con_state = ST_INIT;
			}
			else if (iResult == 0)
			{
				str.Format(L"服务器已断开！\r\n");
				EditBoxAppendText(hEditReceive, str);
				closesocket(TcpClientConnectSocket);
				WSACleanup();
				con_state = ST_INIT;
			}
			else
			{
				MultibyteToUnicode(recv_buf, wrecv_buf, 1024);
				str.Format(L"%s\r\n", wrecv_buf);
				EditBoxAppendText(hEditReceive, str);
			}
			break;
		default:
			break;
		}
	}

	return 0;
}