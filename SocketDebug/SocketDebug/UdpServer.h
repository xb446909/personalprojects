#pragma once


// CUdpServer �Ի���

class CUdpServer : public CDialogEx
{
	DECLARE_DYNAMIC(CUdpServer)

public:
	CUdpServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUdpServer();

// �Ի�������
	enum { IDD = IDD_UDP_Server };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
