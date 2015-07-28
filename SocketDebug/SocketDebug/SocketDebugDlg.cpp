
// SocketDebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketDebug.h"
#include "SocketDebugDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSocketDebugDlg �Ի���



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


// CSocketDebugDlg ��Ϣ�������

BOOL CSocketDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSocketDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSocketDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketDebugDlg::OnTcnSelchangeTabBase(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
