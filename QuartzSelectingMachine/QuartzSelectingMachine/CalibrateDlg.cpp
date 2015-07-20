// CalirateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QuartzSelectingMachine.h"
#include "CalibrateDlg.h"
#include "afxdialogex.h"
#include "Opration.h"


// CCalirateDlg 对话框

IMPLEMENT_DYNAMIC(CCalibrateDlg, CDialogEx)

CCalibrateDlg::CCalibrateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibrateDlg::IDD, pParent)
	, pulseNum(0)
	, OrigX(0)
	, OrigY(0)
	, paramK(0)
	, paramB(0)
	, innerR(0)
	, outerR(0)
	, waferPxHigh(0)
	, waferPxLow(0)
{

}

CCalibrateDlg::~CCalibrateDlg()
{
}

void CCalibrateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PLS_NUM, pulseNum);
	DDX_Text(pDX, IDC_EDIT_ORG_X, OrigX);
	DDX_Text(pDX, IDC_EDIT_ORG_Y, OrigY);
	DDX_Text(pDX, IDC_EDIT_PARAM_K, paramK);
	DDX_Text(pDX, IDC_EDIT_PARAM_B, paramB);
	DDX_Text(pDX, IDC_EDIT_INNER_R, innerR);
	DDX_Text(pDX, IDC_EDIT_OUTER_R, outerR);
	DDX_Text(pDX, IDC_EDIT_PX_HIGH, waferPxHigh);
	DDX_Text(pDX, IDC_EDIT_PX_LOW, waferPxLow);
}


BEGIN_MESSAGE_MAP(CCalibrateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &CCalibrateDlg::OnBnClickedButtonRotate)
	ON_MESSAGE(WM_USB_STATUS, &CCalibrateDlg::USBStatus)
	ON_BN_CLICKED(ID_BTN_SAVE, &CCalibrateDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CCalirateDlg 消息处理程序


void CCalibrateDlg::OnBnClickedButtonRotate()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT pulseNum;
	pulseNum = GetDlgItemInt(IDC_EDIT_PLS_NUM);
	COpration::Get()->RotatePlatform(pulseNum);

}


BOOL CCalibrateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	COpration::Get()->InitMachine(m_hWnd);
	SetWindowText(L"USB连接断开");

	CFile file;
	CalibrateData m_CalibrateData;
	
	COpration::Get()->m_Tools.GetCalirateParam(&m_CalibrateData);
		
	pulseNum = m_CalibrateData.PlatPulseNum;
	OrigX = m_CalibrateData.OriginPoint.x;
	OrigY = m_CalibrateData.OriginPoint.y;
	innerR = m_CalibrateData.InnerRadius;
	outerR = m_CalibrateData.OuterRadius;
	paramK = m_CalibrateData.Param_k;
	paramB = m_CalibrateData.Param_b;
	waferPxHigh = m_CalibrateData.WaferPxHigh;
	waferPxLow = m_CalibrateData.WaferPxLow;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT CCalibrateDlg::USBStatus(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		SetWindowText(L"USB连接断开");
	} 
	else
	{
		SetWindowText(L"USB连接正常");
	}
	return 0;
}

void CCalibrateDlg::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile file;
	UpdateData(TRUE);
	CalibrateData m_CalibrateData;
	m_CalibrateData.PlatPulseNum = pulseNum;
	m_CalibrateData.OriginPoint.x = OrigX;
	m_CalibrateData.OriginPoint.y = OrigY;
	m_CalibrateData.Param_k = paramK;
	m_CalibrateData.Param_b = paramB;
	m_CalibrateData.InnerRadius = innerR;
	m_CalibrateData.OuterRadius = outerR;
	m_CalibrateData.WaferPxHigh = waferPxHigh;
	m_CalibrateData.WaferPxLow = waferPxLow;
	file.Open(L"Calibrate.dat", CFile::modeCreate | CFile::modeWrite);
	file.Write(&m_CalibrateData, sizeof(CalibrateData));
	file.Close();
	MessageBox(L"配置保存成功！");
}
