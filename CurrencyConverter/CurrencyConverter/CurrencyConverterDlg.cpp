
// CurrencyConverterDlg.cpp : 实现文件
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


// CCurrencyConverterDlg 对话框



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


// CCurrencyConverterDlg 消息处理程序

BOOL CCurrencyConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	if (GetWebSrc(L"https://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.xchange%20where%20pair%20in%20(%22USDEUR%22%2C%22USDGBP%22%2C%22USDCNY%22%2C%22USDJPY%22)&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys", L"rate.xml"))
	{
		MessageBox(L"已更新至最新汇率！");
	}
	else
	{
		MessageBox(L"联网更新失败，将离线汇率！");
	}
	GetCurrencyOverXml();
	InitialUI();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCurrencyConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCurrencyConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCurrencyConverterDlg::InitialUI()
{
	combobox_src.AddString(L"美元");
	combobox_src.AddString(L"美分");
	combobox_src.AddString(L"人民币元");
	combobox_src.AddString(L"人民币角");
	combobox_src.AddString(L"人民币分");
	combobox_src.AddString(L"欧元");
	combobox_src.AddString(L"欧分");
	combobox_src.AddString(L"英镑");
	combobox_src.AddString(L"先令");
	combobox_src.AddString(L"新便士");
	combobox_src.AddString(L"日元");
	combobox_src.SetCurSel(0);
	combobox_dst.AddString(L"美元");
	combobox_dst.AddString(L"美分");
	combobox_dst.AddString(L"人民币元");
	combobox_dst.AddString(L"人民币角");
	combobox_dst.AddString(L"人民币分");
	combobox_dst.AddString(L"欧元");
	combobox_dst.AddString(L"欧分");
	combobox_dst.AddString(L"英镑");
	combobox_dst.AddString(L"先令");
	combobox_dst.AddString(L"新便士");
	combobox_dst.AddString(L"日元");
	combobox_dst.SetCurSel(0);
}

bool CCurrencyConverterDlg::GetWebSrc(const CString& strFileURLInServer, //待下载文件的URL
	const CString & strFileLocalFullPath)//存放到本地的路径
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
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
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
				NULL);  //创建本地文件
			if (hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return false;
			}

			char szInfoBuffer[1000];  //返回消息
			DWORD dwFileSize = 0;   //文件长度
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
			BOOL bResult = FALSE;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
				(void*)szInfoBuffer, &dwInfoBufferSize, NULL);

			dwFileSize = atoi(szInfoBuffer);
			const int BUFFER_LENGTH = 1024 * 10;
			pszBuffer = new char[BUFFER_LENGTH];  //读取文件的缓冲
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据

			while (nRead > 0)
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //写到本地文件
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
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此添加控件通知处理程序代码
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
