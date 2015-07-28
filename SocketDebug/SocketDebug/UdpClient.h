#pragma once


// CUdpClient 对话框

class CUdpClient : public CDialogEx
{
	DECLARE_DYNAMIC(CUdpClient)

public:
	CUdpClient(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUdpClient();

// 对话框数据
	enum { IDD = IDD_UDP_Client };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
