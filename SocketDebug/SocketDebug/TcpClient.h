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
};
