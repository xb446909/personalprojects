// TcpClient.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "TcpClient.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CTcpClient 消息处理程序
