// TcpServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "TcpServer.h"
#include "afxdialogex.h"


// CTcpServer �Ի���

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
END_MESSAGE_MAP()


// CTcpServer ��Ϣ�������


BOOL CTcpServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

HBRUSH CTcpServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
	return b;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
