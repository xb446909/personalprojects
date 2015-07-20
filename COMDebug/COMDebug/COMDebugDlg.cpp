
// COMDebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "COMDebug.h"
#include "COMDebugDlg.h"

static DWORD WINAPI ReciveProc(LPVOID lpParameter);
static CString pRecvData;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct RECVPARM
{
	HANDLE hFile;
	HWND hwnd;
};

char RecvChar[1024];

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCOMDebugDlg �Ի���




CCOMDebugDlg::CCOMDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCOMDebugDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCOMDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCOMDebugDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, &CCOMDebugDlg::OnBnClickedBtnOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CLR, &CCOMDebugDlg::OnBnClickedBtnClr)
	ON_BN_CLICKED(IDC_BTN_SEND, &CCOMDebugDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CCOMDebugDlg ��Ϣ�������

BOOL CCOMDebugDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	OnInitPage();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCOMDebugDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCOMDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCOMDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCOMDebugDlg::OnInitPage()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM1");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM2");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM3");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM4");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM5");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM6");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM7");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM8");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->AddString(L"COM9");
	((CComboBox*)GetDlgItem(IDC_COMBO_COM))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"1200");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"2400");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"4800");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"9600");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"14400");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"19200");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"38400");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"57600");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->AddString(L"115200");
	((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->SetCurSel(3);

	((CComboBox*)GetDlgItem(IDC_COMBO_DATA))->AddString(L"8");
	((CComboBox*)GetDlgItem(IDC_COMBO_DATA))->AddString(L"7");
	((CComboBox*)GetDlgItem(IDC_COMBO_DATA))->AddString(L"6");
	((CComboBox*)GetDlgItem(IDC_COMBO_DATA))->AddString(L"5");
	((CComboBox*)GetDlgItem(IDC_COMBO_DATA))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_COMBO_PARITY))->AddString(L"��У��");
	((CComboBox*)GetDlgItem(IDC_COMBO_PARITY))->AddString(L"ODDУ��");
	((CComboBox*)GetDlgItem(IDC_COMBO_PARITY))->AddString(L"EVENУ��");
	((CComboBox*)GetDlgItem(IDC_COMBO_PARITY))->SetCurSel(1);

	((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->AddString(L"1λ");
	((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->AddString(L"1.5λ");
	((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->AddString(L"2λ");
	((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->SetCurSel(0);

	bCOMFlag = false;
}

void CCOMDebugDlg::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int m_COMName, m_BaudRate, m_ByteSize, m_Parity, m_Stop;
	const char* COMName[9] = { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9" };
	DWORD BaudRate[9] = { 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200 };
	unsigned char ByteSize[4] = { 8, 7, 6, 5 };
	unsigned char Parity[3] = { NOPARITY, ODDPARITY, EVENPARITY };
	unsigned char StopBits[3] = { ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS };

	m_COMName = ((CComboBox*)GetDlgItem(IDC_COMBO_COM))->GetCurSel();
	m_BaudRate = ((CComboBox*)GetDlgItem(IDC_COMBO_BTL))->GetCurSel();
	m_ByteSize = ((CComboBox*)GetDlgItem(IDC_COMBO_DATA))->GetCurSel();
	m_Parity = ((CComboBox*)GetDlgItem(IDC_COMBO_PARITY))->GetCurSel();
	m_Stop = ((CComboBox*)GetDlgItem(IDC_COMBO_STOP))->GetCurSel();
	if (bCOMFlag == false)
	{
		hCOM = CreateFileA(COMName[m_COMName], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCOM == INVALID_HANDLE_VALUE)
		{
			MessageBox(L"��COMʧ��!");
			return;
		}
		bCOMFlag = true;
		SetupComm(hCOM, 1024, 1024);
		DCB dcb;
		RECVPARM *pRecvParm = new RECVPARM;
		GetCommState(hCOM, &dcb);
		dcb.DCBlength = sizeof(DCB);
		dcb.BaudRate = BaudRate[m_BaudRate];
		dcb.ByteSize = ByteSize[m_ByteSize];
		dcb.Parity = Parity[m_Parity];
		dcb.StopBits = StopBits[m_Stop];

		COMMTIMEOUTS TimeOuts;
		TimeOuts.ReadIntervalTimeout = MAXDWORD;
		TimeOuts.ReadTotalTimeoutMultiplier = 0;
		TimeOuts.ReadTotalTimeoutConstant = 0;

		SetCommTimeouts(hCOM, &TimeOuts);
		SetCommState(hCOM, &dcb);
		PurgeComm(hCOM, PURGE_TXCLEAR | PURGE_RXCLEAR);

		pRecvParm->hFile = hCOM;
		pRecvParm->hwnd = m_hWnd;
		hRecive = CreateThread(NULL, 0, ReciveProc, (LPVOID)pRecvParm, 0, NULL);

		SetDlgItemText(IDC_BTN_OPEN, L"�رմ���");
	}
	else
	{

		DWORD ExitCode;
		GetExitCodeThread(hRecive, &ExitCode);
		TerminateThread(hRecive, ExitCode);
		CloseHandle(hRecive);
		CloseHandle(hCOM);
		SetDlgItemText(IDC_BTN_OPEN, L"�򿪴���");
		bCOMFlag = false;
	}


}

void EditBoxAppendText(HWND hEditBox, LPCTSTR text)
{
	int len = 0;
	len = GetWindowTextLength(hEditBox);
	SetFocus(hEditBox);
	SendMessage(hEditBox, EM_SETSEL, len, len);
	SendMessage(hEditBox, EM_REPLACESEL, 0, (LPARAM)text);
}

DWORD WINAPI ReciveProc(LPVOID lpParameter)
{
	char recv_buf[1024];
	HANDLE hFile = ((RECVPARM*)lpParameter)->hFile;
	HWND hwnd = ((RECVPARM*)lpParameter)->hwnd;
	HWND hEditBox = GetDlgItem(hwnd, IDC_EDIT_RECV);
	//SetWindowText(hEditBox, L"No Data");
	//EditBoxAppendText(hEditBox, L"\r\nAddLine");

	while (1)
	{
		memset(recv_buf, 0, 1024 * sizeof(char));
		DWORD dwReadSize = 0;
		int bReadStatus;
		while (!dwReadSize)
		{
			bReadStatus = ReadFile(hFile, recv_buf, 1024, &dwReadSize, NULL);
		}
		if (bReadStatus)
		{
			EditBoxAppendText(hEditBox, CA2W(recv_buf));
			bReadStatus = FALSE;
		}

	}
	return 0;
}


void CCOMDebugDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	DWORD ExitCode;
	GetExitCodeThread(hRecive, &ExitCode);
	TerminateThread(hRecive, ExitCode);
	CloseHandle(hRecive);
	CloseHandle(hCOM);

}

void CCOMDebugDlg::OnBnClickedBtnClr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	pRecvData = L"";
	SetDlgItemText(IDC_EDIT_RECV, pRecvData);
}

void CCOMDebugDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (bCOMFlag == false)
	{
		MessageBox(L"���ȴ򿪴��ڣ�");
		return;
	}
	CString lpSend;
	DWORD SendByte;
	int bWrireStatus;
	LPBOOL lpUsedDefaultChar = new BOOL far;
	GetDlgItemText(IDC_EDIT_SEND, lpSend);
	SendByte = lpSend.GetLength();
	int nlen;
	char pResult[1024];
	memset(pResult, 0, 1024 * sizeof(char));
	nlen = WideCharToMultiByte(CP_ACP, 0, lpSend, -1, 0, 0, NULL, lpUsedDefaultChar);
	WideCharToMultiByte(CP_ACP, 0, lpSend, -1, pResult, nlen, NULL, lpUsedDefaultChar);
	if (IsDlgButtonChecked(IDC_CHECK_SEND))
	{
		pResult[nlen - 1] = 0x0d;
		pResult[nlen] = 0x0a;
		pResult[nlen + 1] = 0;
	}
	bWrireStatus = WriteFile(hCOM, pResult, nlen, &SendByte, 0);
	if (!bWrireStatus)
	{
		MessageBox(L"д��ʧ�ܣ������ԣ�");
		return;
	}
}
