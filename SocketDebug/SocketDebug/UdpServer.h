#pragma once


// CUdpServer 对话框

class CUdpServer : public CDialogEx
{
	DECLARE_DYNAMIC(CUdpServer)

public:
	CUdpServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdpServer();

// 对话框数据
	enum { IDD = IDD_UDP_Server };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
