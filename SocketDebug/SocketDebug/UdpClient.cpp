// UdpClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "UdpClient.h"
#include "afxdialogex.h"


// CUdpClient �Ի���

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


// CUdpClient ��Ϣ�������
