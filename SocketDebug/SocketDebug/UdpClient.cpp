// UdpClient.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "UdpClient.h"
#include "afxdialogex.h"


// CUdpClient 对话框

IMPLEMENT_DYNAMIC(CUdpClient, CDialogEx)

CUdpClient::CUdpClient(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUdpClient::IDD, pParent)
{

}

CUdpClient::~CUdpClient()
{
}

void CUdpClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUdpClient, CDialogEx)
END_MESSAGE_MAP()


// CUdpClient 消息处理程序
