// UdpServer.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "UdpServer.h"
#include "afxdialogex.h"


// CUdpServer 对话框

IMPLEMENT_DYNAMIC(CUdpServer, CDialogEx)

CUdpServer::CUdpServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUdpServer::IDD, pParent)
{

}

CUdpServer::~CUdpServer()
{
}

void CUdpServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUdpServer, CDialogEx)
END_MESSAGE_MAP()


// CUdpServer 消息处理程序
