#pragma once


// CUdpClient �Ի���

class CUdpClient : public CDialogEx
{
	DECLARE_DYNAMIC(CUdpClient)

public:
	CUdpClient(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdpClient();

// �Ի�������
	enum { IDD = IDD_UDP_Client };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
