
// QuartzSelectingMachineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QuartzSelectingMachine.h"
#include "QuartzSelectingMachineDlg.h"
#include "afxdialogex.h"
#include "DebugDlg.h"
#include "USBHID.h"
#include "CalibrateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT indicators[] =
{
	IDS_STATUSBAR_ON_TIME,
	IDS_STATUSBAR_POLE,
	IDS_STATUSBAR_CONNECT
};

// CQuartzSelectingMachineDlg �Ի���

void Foo(int event, int x, int y, int flags, void* param);


CQuartzSelectingMachineDlg::CQuartzSelectingMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQuartzSelectingMachineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuartzSelectingMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQuartzSelectingMachineDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_DEBUG, &CQuartzSelectingMachineDlg::OnDebug)
	ON_MESSAGE(WM_USB_STATUS, &CQuartzSelectingMachineDlg::SetUSBStatus)
	ON_COMMAND(ID_CALIBRATE, &CQuartzSelectingMachineDlg::OnCalibrate)
	ON_BN_CLICKED(IDC_BTN_AUTO, &CQuartzSelectingMachineDlg::OnBnClickedBtnAuto)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, &CQuartzSelectingMachineDlg::OnBnClickedBtnStop)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CQuartzSelectingMachineDlg ��Ϣ�������

BOOL CQuartzSelectingMachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	mStatusBar.Create(this);
	mStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	CRect rect;
	GetClientRect(&rect);
	mStatusBar.SetPaneInfo(0, indicators[0], 0, rect.Width()/3);
	mStatusBar.SetPaneInfo(1, indicators[1], 0, rect.Width()/3);
	mStatusBar.SetPaneInfo(2, indicators[2], 0, rect.Width()/3);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 
		IDS_STATUSBAR_CONNECT);

	COpration::Get()->InitOpration();

	COpration::Get()->InitMachine(GetSafeHwnd());

	COpration::Get()->InitCam();

	COpration::Get()->ShowToWindow(GetDlgItem(IDC_PIC_PRE)->GetSafeHwnd(), IDC_PIC_PRE);

	m_font.CreateFont(35, 
		0,
		0,
		0,
		FW_MEDIUM,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		L"Arial");

	GetDlgItem(IDC_EDIT_FREQ)->SetFont(&m_font, FALSE);
	m_BlackBrush = CreateSolidBrush(RGB(0, 0, 0));

	SetDlgItemText(IDC_EDIT_FREQ, L"12.3456");	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQuartzSelectingMachineDlg::OnPaint()
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
HCURSOR CQuartzSelectingMachineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CQuartzSelectingMachineDlg::OnDebug()
{
	// TODO: �ڴ���������������
	CDebugDlg mDbgDlg;
	mDbgDlg.DoModal();
}


LRESULT CQuartzSelectingMachineDlg::SetUSBStatus(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		mStatusBar.SetPaneText(2, L"����״̬��USB�ѶϿ�");
	} 
	else
	{
		mStatusBar.SetPaneText(2, L"����״̬��USB������");
	}
	return 0;
}

void CQuartzSelectingMachineDlg::OnCalibrate()
{
	// TODO: �ڴ���������������
	CCalibrateDlg m_calibrate;
	m_calibrate.DoModal();
}


void CQuartzSelectingMachineDlg::OnBnClickedBtnAuto()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	IplImage* frame = COpration::Get()->GetFrame();
	IplImage* procimg = cvCreateImage(cvGetSize(frame), 8, 3);
	//cvNamedWindow("Image");
	IplImage* temp = cvCreateImage(cvSize(640, 512), 8, 1);
	cvResize(frame, temp);
	cvLine(temp, cvPoint(0, 168), cvPoint(640, 168), cvScalar(255));
	cvLine(temp, cvPoint(0, 163), cvPoint(640, 163), cvScalar(255));
	//cvShowImage("Image", temp);
	//cvSetMouseCallback("Image", Foo, (void*)temp);
	
	SetTimer(1000, 2000, NULL);
}

int num = 0;
CvPoint pt[2];

CvPoint center;
double k1, k2, b1, b2, temp_x, temp_y, r;

void Foo(int event, int x, int y, int flags, void* param)
{
	CString str;
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		cvCircle(param, cvPoint(x, y), 5, cvScalar(255), -1);
		cvShowImage("Image", param);
		pt[num] = cvPoint(x, y);
		num++;
		if (num == 3)
		{
			k1 = (double)(x - pt[0].x)/(double)(pt[0].y - y);
			k2 = (double)(x - pt[1].x)/(double)(pt[1].y - y);
			b1 = ((double)(y + pt[0].y)/2) - k1*((double)(x + pt[0].x)/2);
			b2 = ((double)(y + pt[1].y)/2) - k2*((double)(x + pt[1].x)/2);
			temp_x = (b2 - b1) / (k1 - k2);
			temp_y = k1 * temp_x + b1;
			r = sqrt((temp_x - x) * (temp_x - x) + (temp_y - y) * (temp_y - y));
			center.x = (int)temp_x;
			center.y = (int)temp_y;
			cvCircle(param, center, (int)r, cvScalar(255), 2);
			cvShowImage("Image", param);
			num = 0;
			str.Format(L"center(%d, %d), r = %d", center.x, center.y, (int)r);
			AfxMessageBox(str);
		}
	}

	if (event == CV_EVENT_RBUTTONDOWN)
	{
		COpration m_Ops;
		m_Ops.InitMachine(NULL);
		str.Format(L"(%d, %d)", x, y);
		AfxMessageBox(str);
		m_Ops.SetPos(cvPoint(x, y));
	}
}

void CQuartzSelectingMachineDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	COpration::Get()->UninitCam();
}


void CQuartzSelectingMachineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IplImage* frame = COpration::Get()->GetFrame();
	IplImage* procimg = cvCreateImage(cvGetSize(frame), 8, 3);
	CvPoint pt = COpration::Get()->m_Tools.QuartzPostion(frame, procimg);
	COpration::Get()->m_Tools.DrawToCDC(procimg, GetDlgItem(IDC_PIC_PROC)->GetDC());
	pt.x = pt.x / 2;
	pt.y = pt.y / 2;
	COpration::Get()->SetPos(pt);
	cvReleaseImage(&frame);
	cvReleaseImage(&procimg);
	COpration::Get()->MoveMachine(MEASURE_POS);
	CDialogEx::OnTimer(nIDEvent);
}


void CQuartzSelectingMachineDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1000);
}


HBRUSH CQuartzSelectingMachineDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_FREQ)
	{
		pDC->SetTextColor(RGB(0, 190, 0));
		pDC->SetBkMode(TRANSPARENT);
		return m_BlackBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
