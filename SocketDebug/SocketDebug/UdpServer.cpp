// UdpServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "UdpServer.h"
#include "afxdialogex.h"


// CUdpServer �Ի���

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


// CUdpServer ��Ϣ�������
