// TcpServer.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "TcpServer.h"
#include "afxdialogex.h"

static DWORD WINAPI ReceiveProc(LPVOID lpParameter);
void EditBoxAppendText(HWND hEditBox, LPCTSTR text);
int MultibyteToUnicode(LPCSTR strIn, LPWSTR strOut, int nLenOut);
// CTcpServer 对话框

IMPLEMENT_DYNAMIC(CTcpServer, CDialogEx)

CTcpServer::CTcpServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTcpServer::IDD, pParent)
{

}

CTcpServer::~CTcpServer()
{
}

void CTcpServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTcpServer, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_TCPSER_BIND, &CTcpServer::OnBnClickedBtnTcpserBind)
	ON_BN_CLICKED(IDC_BTN_TCPSER_CLR, &CTcpServer::OnBnClickedBtnTcpserClr)
	ON_BN_CLICKED(IDC_BTN_TCPSER_SEND, &CTcpServer::OnBnClickedBtnTcpserSend)
END_MESSAGE_MAP()


// CTcpServer 消息处理程序


BOOL CTcpServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	bConnect = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

HBRUSH CTcpServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
	return b;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

SOCKET TcpSerListenSocket = { 0 };
SOCKET AcceptSocket = { 0 };
HANDLE hReceive;

void EditBoxAppendText(HWND hEditBox, LPCTSTR text)
{
	int len = 0;
	len = GetWindowTextLength(hEditBox);
	SetFocus(hEditBox);
	SendMessage(hEditBox, EM_SETSEL, len, len);
	SendMessage(hEditBox, EM_REPLACESEL, 0, (LPARAM)text);
}

BOOL InitSocket(SOCKET& ListenSocket, int port)
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
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		WSACleanup();
		return FALSE;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(port);

	if (bind(ListenSocket,
		(SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
		closesocket(ListenSocket);
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

void CTcpServer::OnBnClickedBtnTcpserBind()
{
	// TODO: Add your control notification handler code here
	DWORD dwExitCode;
	if (bConnect)
	{
		GetExitCodeThread(hReceive, &dwExitCode);
		TerminateThread(hReceive, dwExitCode);
		CloseHandle(hReceive);
		closesocket(AcceptSocket);
		closesocket(TcpSerListenSocket);
		WSACleanup();
		SetDlgItemText(IDC_BTN_TCPSER_BIND, L"绑定");
		SetDlgItemText(IDC_EDIT_TCPSER_CLIENT, L"");
		((CEdit*)GetDlgItem(IDC_EDIT_TCPSER_PORT))->SetReadOnly(FALSE);
		bConnect = false;
	}
	else
	{
		hReceive = CreateThread(NULL, 0, ReceiveProc, (LPVOID)GetSafeHwnd(), 0, NULL);
		((CEdit*)GetDlgItem(IDC_EDIT_TCPSER_PORT))->SetReadOnly();
		SetDlgItemText(IDC_BTN_TCPSER_BIND, L"断开");
		bConnect = true;
	}	
}


DWORD WINAPI ReceiveProc(LPVOID lpParameter)
{
	char recv_buf[1024];
	wchar_t wrecv_buf[1024] = { 0 };
	char ip_strbuf[16] = { 0 };
	HWND hwnd = (HWND)lpParameter;
	HWND hEditReceive = GetDlgItem(hwnd, IDC_EDIT_TCPSER_RECV);
	CString str;
	int iResult;
	ConnectSt con_state = ST_INIT;

	SOCKADDR_IN client_addr = { 0 };
	int addr_len = sizeof(SOCKADDR_IN);

	if (!InitSocket(TcpSerListenSocket, GetDlgItemInt(hwnd, IDC_EDIT_TCPSER_PORT, NULL, FALSE)))
	{
		AfxMessageBox(L"Initialize socket error!");
		return 0;
	}

	while (true)
	{
		switch (con_state)
		{
		case ST_INIT:
			if (listen(TcpSerListenSocket, SOMAXCONN) == SOCKET_ERROR)
			{
				AfxMessageBox(L"Listen socket error!");
				closesocket(TcpSerListenSocket);
				WSACleanup();
				return 0;
			}
			SetDlgItemText(hwnd, IDC_EDIT_TCPSER_CLIENT, L"Waiting...");
			con_state = ST_LISTENING;
			break;
		case ST_LISTENING:
			AcceptSocket = accept(TcpSerListenSocket, (SOCKADDR *)&client_addr, &addr_len);
			if (AcceptSocket == INVALID_SOCKET)
			{
				AfxMessageBox(L"Accept socket error!");
				closesocket(TcpSerListenSocket);
				WSACleanup();
				return 0;
			}
			str.Format(L":%d", client_addr.sin_port);
			str = CA2W(inet_ntoa(client_addr.sin_addr)) + str;
			SetDlgItemText(hwnd, IDC_EDIT_TCPSER_CLIENT, str);
			con_state = ST_ACCEPTED;
			break;
		case ST_ACCEPTED:
			memset(recv_buf, 0, 1024 * sizeof(char));
			iResult = recv(AcceptSocket, recv_buf, 1024, 0);
			if (iResult == SOCKET_ERROR)
			{
				iResult = WSAGetLastError();
				if (iResult == WSAECONNRESET)
				{
					str.Format(L":%d 客户端已断开\r\n", client_addr.sin_port);
					str = CA2W(inet_ntoa(client_addr.sin_addr)) + str;
				}
				else
				{
					str.Format(L"Recv Error! Error code: %d\r\n", iResult);
				}
				EditBoxAppendText(hEditReceive, str);
				SetDlgItemText(hwnd, IDC_EDIT_TCPSER_CLIENT, L"Waiting...");
				closesocket(AcceptSocket);
				con_state = ST_LISTENING;
			}
			else if (iResult == 0)
			{
				str.Format(L":%d 客户器已断开\r\n", client_addr.sin_port);
				str = CA2W(inet_ntoa(client_addr.sin_addr)) + str;
				EditBoxAppendText(hEditReceive, str);
				SetDlgItemText(hwnd, IDC_EDIT_TCPSER_CLIENT, L"Waiting...");
				closesocket(AcceptSocket);
				con_state = ST_LISTENING;
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

void CTcpServer::OnBnClickedBtnTcpserClr()
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_TCPSER_RECV, L"");
}


void CTcpServer::OnBnClickedBtnTcpserSend()
{
	// TODO: Add your control notification handler code here
	CString str;

	char send_buf[1024] = { 0 };
	GetDlgItemTextA(GetSafeHwnd(), IDC_EDIT_TCPSER_SEND, send_buf, 1023);
	if (send(AcceptSocket, send_buf, strlen(send_buf) + 1, 0) == SOCKET_ERROR)
	{
		str.Format(L"Send error, code = %d", WSAGetLastError());
		MessageBox(str);
		return;
	}
	SetDlgItemText(IDC_EDIT_TCPSER_SEND, L"");
}

int MultibyteToUnicode(LPCSTR strIn, LPWSTR strOut, int nLenOut)
{
	if (strOut == NULL)
	{
		return 0;
	}

	int nLen = MultiByteToWideChar(CP_ACP, 0, strIn, -1, NULL, 0);
	nLen = min(nLen, nLenOut);
	MultiByteToWideChar(CP_ACP, 0, strIn, -1, strOut, nLen);

	if (nLen < nLenOut)
	{
		strOut[nLen] = 0;
	}

	return nLen;
}