#pragma once


// CTcpServer 对话框

class CTcpServer : public CDialogEx
{
	DECLARE_DYNAMIC(CTcpServer)

public:
	CTcpServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTcpServer();

// 对话框数据
	enum { IDD = IDD_TCP_Server };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
