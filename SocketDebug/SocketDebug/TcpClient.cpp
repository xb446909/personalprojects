// TcpClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "TcpClient.h"
#include "afxdialogex.h"


// CTcpClient �Ի���

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


// CTcpClient ��Ϣ�������
