
// SocketDebugDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "TcpServer.h"
#include "TcpClient.h"
#include "UdpServer.h"
#include "UdpClient.h"


// CSocketDebugDlg �Ի���
class CSocketDebugDlg : public CDialogEx
{
// ����
public:
	CSocketDebugDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SOCKETDEBUG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tablist;
	CTcpServer m_tcpserver;
	CTcpClient m_tcpclient;
	CUdpServer m_udpserver;
	CUdpClient m_udpclient;
	afx_msg void OnTcnSelchangeTabBase(NMHDR *pNMHDR, LRESULT *pResult);
};
