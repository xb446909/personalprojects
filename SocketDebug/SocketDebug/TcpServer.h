#pragma once


// CTcpServer �Ի���

class CTcpServer : public CDialogEx
{
	DECLARE_DYNAMIC(CTcpServer)

public:
	CTcpServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTcpServer();

// �Ի�������
	enum { IDD = IDD_TCP_Server };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
