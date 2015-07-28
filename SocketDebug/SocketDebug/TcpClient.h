#pragma once


// CTcpClient 对话框

class CTcpClient : public CDialogEx
{
	DECLARE_DYNAMIC(CTcpClient)

public:
	CTcpClient(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTcpClient();

// 对话框数据
	enum { IDD = IDD_TCP_Client };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
