
// CurrencyConverterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CurrencyConverter.h"
#include "CurrencyConverterDlg.h"
#include "afxdialogex.h"
#include "afxinet.h"
#include "msxml.h"

#import "msxml6.dll"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCurrencyConverterDlg �Ի���



CCurrencyConverterDlg::CCurrencyConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurrencyConverterDlg::IDD, pParent)
	, db_src(0)
	, db_dst(0)
	, g_usd(0)
	, g_words(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCurrencyConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC, db_src);
	DDX_Text(pDX, IDC_EDIT_DST, db_dst);
	DDX_Control(pDX, IDC_COMBO_SRC, combobox_src);
	DDX_Control(pDX, IDC_COMBO_DST, combobox_dst);
	DDX_Text(pDX, IDC_EDIT_USD, g_usd);
	DDX_Text(pDX, IDC_EDIT_WORDS, g_words);
}

BEGIN_MESSAGE_MAP(CCurrencyConverterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXC, &CCurrencyConverterDlg::OnBnClickedBtnExc)
	ON_BN_CLICKED(IDC_BTN_CVT, &CCurrencyConverterDlg::OnBnClickedBtnCvt)
	ON_BN_CLICKED(IDC_BTN_CALC, &CCurrencyConverterDlg::OnBnClickedBtnCalc)
END_MESSAGE_MAP()


// CCurrencyConverterDlg ��Ϣ�������

BOOL CCurrencyConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	if (GetWebSrc(L"https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.xchange%20where%20pair%20in%20(%22USDEUR%22%2C%22USDGBP%22%2C%22USDCNY%22%2C%22USDJPY%22)&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys", L"rate.xml"))
	{
		MessageBox(L"�Ѹ��������»��ʣ�");
	}
	else
	{
		MessageBox(L"��������ʧ�ܣ������߻��ʣ�");
	}
	GetCurrencyOverXml();
	InitialUI();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCurrencyConverterDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCurrencyConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCurrencyConverterDlg::InitialUI()
{
	combobox_src.AddString(L"��Ԫ");
	combobox_src.AddString(L"����");
	combobox_src.AddString(L"�����Ԫ");
	combobox_src.AddString(L"����ҽ�");
	combobox_src.AddString(L"����ҷ�");
	combobox_src.AddString(L"ŷԪ");
	combobox_src.AddString(L"ŷ��");
	combobox_src.AddString(L"Ӣ��");
	combobox_src.AddString(L"����");
	combobox_src.AddString(L"�±�ʿ");
	combobox_src.AddString(L"��Ԫ");
	combobox_src.SetCurSel(0);
	combobox_dst.AddString(L"��Ԫ");
	combobox_dst.AddString(L"����");
	combobox_dst.AddString(L"�����Ԫ");
	combobox_dst.AddString(L"����ҽ�");
	combobox_dst.AddString(L"����ҷ�");
	combobox_dst.AddString(L"ŷԪ");
	combobox_dst.AddString(L"ŷ��");
	combobox_dst.AddString(L"Ӣ��");
	combobox_dst.AddString(L"����");
	combobox_dst.AddString(L"�±�ʿ");
	combobox_dst.AddString(L"��Ԫ");
	combobox_dst.SetCurSel(0);
}

bool CCurrencyConverterDlg::GetWebSrc(const CString& strFileURLInServer, //�������ļ���URL
	const CString & strFileLocalFullPath)//��ŵ����ص�·��
{
	ASSERT(strFileURLInServer != "");
	ASSERT(strFileLocalFullPath != "");
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;
	DWORD dwType;
	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //����֮��ĵȴ���ʱ
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //���Դ���
	char* pszBuffer = NULL;

	try
	{
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer,INTERNET_FLAG_SECURE ,wPort);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_SECURE);
		if (pHttpFile->SendRequest() == FALSE)
			return false;
		DWORD dwStateCode;

		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if (dwStateCode == HTTP_STATUS_OK)
		{
			HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE,
				FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
				NULL);  //���������ļ�
			if (hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return false;
			}

			char szInfoBuffer[1000];  //������Ϣ
			DWORD dwFileSize = 0;   //�ļ�����
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
			BOOL bResult = FALSE;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
				(void*)szInfoBuffer, &dwInfoBufferSize, NULL);

			dwFileSize = atoi(szInfoBuffer);
			const int BUFFER_LENGTH = 1024 * 10;
			pszBuffer = new char[BUFFER_LENGTH];  //��ȡ�ļ��Ļ���
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //��ȡ������������

			while (nRead > 0)
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //д�������ļ�
				dwTotalWrite += dwWrite;
				nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH);
			}

			delete[]pszBuffer;
			pszBuffer = NULL;
			CloseHandle(hFile);
		}
		else
		{
			delete[]pszBuffer;
			pszBuffer = NULL;
			if (pHttpFile != NULL)
			{
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			if (pHttpConnection != NULL)
			{
				pHttpConnection->Close();
				delete pHttpConnection;
				pHttpConnection = NULL;
			}
			session.Close();
			return false;
		}
	}
	catch (...)
	{
		delete[]pszBuffer;
		pszBuffer = NULL;
		if (pHttpFile != NULL)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if (pHttpConnection != NULL)
		{
			pHttpConnection->Close();
			delete pHttpConnection;
			pHttpConnection = NULL;
		}
		session.Close();
		return false;
	}

	if (pHttpFile != NULL)
		pHttpFile->Close();
	if (pHttpConnection != NULL)
		pHttpConnection->Close();
	session.Close();
	return true;
}


void CCurrencyConverterDlg::OnBnClickedBtnExc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int sel_src = combobox_src.GetCurSel();
	int sel_dst = combobox_dst.GetCurSel();

	combobox_src.SetCurSel(sel_dst);
	combobox_dst.SetCurSel(sel_src);
}

inline void TESTHR(HRESULT _hr)
{
	if FAILED(_hr) throw(_hr);
}


bool CCurrencyConverterDlg::GetCurrencyOverXml()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	bool ret = false;
	try {
		MSXML2::IXMLDOMDocumentPtr docPtr;
		MSXML2::IXMLDOMNodeListPtr NodeListPtr;
		MSXML2::IXMLDOMNodePtr NodePtr;
		MSXML2::IXMLDOMNodeListPtr ChildNodeListPtr;
		MSXML2::IXMLDOMNodePtr ChildNodePtr;

		BSTR temp;
		int i;

		//init
		TESTHR(CoInitialize(NULL));
		TESTHR(docPtr.CreateInstance("Msxml2.DOMDocument.6.0"));

		// load a document
		_variant_t varXml("rate.xml");
		_variant_t varOut((bool)TRUE);
		varOut = docPtr->load(varXml);
		if ((bool)varOut == FALSE)
			throw(0);
		NodeListPtr = docPtr->getElementsByTagName("rate");
		for (i = 0; i < NodeListPtr->length; i++)
		{
			TESTHR(NodeListPtr->get_item(i, &NodePtr));
			TESTHR(NodePtr->get_childNodes(&ChildNodeListPtr));
			TESTHR(ChildNodeListPtr->get_item(0, &ChildNodePtr));
			TESTHR(ChildNodePtr->get_text(&temp));
			if (!strcmp("USD/GBP", _com_util::ConvertBSTRToString(temp)))
			{
				TESTHR(ChildNodeListPtr->get_item(1, &ChildNodePtr));
				TESTHR(ChildNodePtr->get_text(&temp));
				gbp = atof(_com_util::ConvertBSTRToString(temp));
			}
			if (!strcmp("USD/EUR", _com_util::ConvertBSTRToString(temp)))
			{
				TESTHR(ChildNodeListPtr->get_item(1, &ChildNodePtr));
				TESTHR(ChildNodePtr->get_text(&temp));
				eur = atof(_com_util::ConvertBSTRToString(temp));
			}
			if (!strcmp("USD/CNY", _com_util::ConvertBSTRToString(temp)))
			{
				TESTHR(ChildNodeListPtr->get_item(1, &ChildNodePtr));
				TESTHR(ChildNodePtr->get_text(&temp));
				cny = atof(_com_util::ConvertBSTRToString(temp));
			}
			if (!strcmp("USD/JPY", _com_util::ConvertBSTRToString(temp)))
			{
				TESTHR(ChildNodeListPtr->get_item(1, &ChildNodePtr));
				TESTHR(ChildNodePtr->get_text(&temp));
				jpy = atof(_com_util::ConvertBSTRToString(temp));
			}
		}
	}
	catch (...)
	{
		MessageBox(_T("Exception occurred"), _T("Error"), MB_OK);
	}

	CoUninitialize();
	return ret;
}


void CCurrencyConverterDlg::OnBnClickedBtnCvt()
{	
	double currency_arr[11] = { 0 };
	currency_arr[0] = 1;
	currency_arr[1] = 100;
	currency_arr[2] = cny;
	currency_arr[3] = cny * 10;
	currency_arr[4] = cny * 100;
	currency_arr[5] = eur;
	currency_arr[6] = eur * 100;
	currency_arr[7] = gbp;
	currency_arr[8] = gbp * 10;
	currency_arr[9] = gbp * 100;
	currency_arr[10] = jpy;

	int sel_src = combobox_src.GetCurSel();
	int sel_dst = combobox_dst.GetCurSel();
	UpdateData();

	db_dst = db_src * currency_arr[sel_dst] / currency_arr[sel_src];
	db_dst *= 1000000;
	db_dst = round(db_dst);
	db_dst /= 1000000;
	UpdateData(FALSE);
}


void CCurrencyConverterDlg::OnBnClickedBtnCalc()
{
	// TODO: Add your control notification handler code here
	CString str;
	double temp;

	UpdateData();

	temp = g_usd / (double)g_words;
	str.Format(L"%f", temp);
	SetDlgItemText(IDC_EDIT_USDPERWORD, str);

	temp = (double)g_words / g_usd;
	str.Format(L"%f", temp);
	SetDlgItemText(IDC_EDIT_WORDPERUSD, str);

}
