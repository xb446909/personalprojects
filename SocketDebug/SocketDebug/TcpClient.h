#pragma once


// CTcpClient �Ի���

class CTcpClient : public CDialogEx
{
	DECLARE_DYNAMIC(CTcpClient)

public:
	CTcpClient(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTcpClient();

// �Ի�������
	enum { IDD = IDD_TCP_Client };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTcpclientClr();
	afx_msg void OnBnClickedBtnTcpclientSend();
	afx_msg void OnBnClickedBtnTcpclientCon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	virtual BOOL OnInitDialog();


	bool bConnect;
};
