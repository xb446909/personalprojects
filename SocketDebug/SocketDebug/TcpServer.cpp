// TcpServer.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "TcpServer.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CTcpServer 消息处理程序


BOOL CTcpServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
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
