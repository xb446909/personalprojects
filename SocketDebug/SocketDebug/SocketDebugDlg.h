
// SocketDebugDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "TcpServer.h"
#include "TcpClient.h"
#include "UdpServer.h"
#include "UdpClient.h"


// CSocketDebugDlg 对话框
class CSocketDebugDlg : public CDialogEx
{
// 构造
public:
	CSocketDebugDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOCKETDEBUG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
