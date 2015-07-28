
// SocketDebugDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "SocketDebugDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketDebugDlg 对话框



CSocketDebugDlg::CSocketDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketDebugDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_BASE, m_tablist);
}

BEGIN_MESSAGE_MAP(CSocketDebugDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BASE, &CSocketDebugDlg::OnTcnSelchangeTabBase)
END_MESSAGE_MAP()


// CSocketDebugDlg 消息处理程序

BOOL CSocketDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_tablist.InsertItem(0, L"TCP Server");
	m_tablist.InsertItem(1, L"TCP Client");
	m_tablist.InsertItem(2, L"UDP Server");
	m_tablist.InsertItem(3, L"UDP Client");

	CRect rect;

	m_tablist.GetClientRect(&rect);
	rect.top += 22;
	rect.left += 2;

	m_tcpserver.Create(IDD_TCP_Server, GetDlgItem(IDC_TAB_BASE));
	m_tcpserver.MoveWindow(&rect);
	m_tcpserver.ShowWindow(SW_SHOW);

	m_tcpclient.Create(IDD_TCP_Client, GetDlgItem(IDC_TAB_BASE));
	m_tcpclient.MoveWindow(&rect);
	m_tcpclient.ShowWindow(SW_HIDE);

	m_udpserver.Create(IDD_UDP_Server, GetDlgItem(IDC_TAB_BASE));
	m_udpserver.MoveWindow(&rect);
	m_udpserver.ShowWindow(SW_HIDE);

	m_udpclient.Create(IDD_UDP_Client, GetDlgItem(IDC_TAB_BASE));
	m_udpclient.MoveWindow(&rect);
	m_udpclient.ShowWindow(SW_HIDE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSocketDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocketDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketDebugDlg::OnTcnSelchangeTabBase(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int iSel = m_tablist.GetCurSel();
	switch (iSel)
	{
	case 0:
		m_tcpserver.ShowWindow(SW_SHOW);
		m_tcpclient.ShowWindow(SW_HIDE);
		m_udpserver.ShowWindow(SW_HIDE);
		m_udpclient.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_tcpserver.ShowWindow(SW_HIDE);
		m_tcpclient.ShowWindow(SW_SHOW);
		m_udpserver.ShowWindow(SW_HIDE);
		m_udpclient.ShowWindow(SW_HIDE);
		break;
	case 2:
		m_tcpserver.ShowWindow(SW_HIDE);
		m_tcpclient.ShowWindow(SW_HIDE);
		m_udpserver.ShowWindow(SW_SHOW);
		m_udpclient.ShowWindow(SW_HIDE);
		break;
	case 3:
		m_tcpserver.ShowWindow(SW_HIDE);
		m_tcpclient.ShowWindow(SW_HIDE);
		m_udpserver.ShowWindow(SW_HIDE);
		m_udpclient.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
}
