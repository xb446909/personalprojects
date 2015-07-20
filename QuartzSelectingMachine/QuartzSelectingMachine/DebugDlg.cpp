// DebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QuartzSelectingMachine.h"
#include "DebugDlg.h"
#include "afxdialogex.h"
#include "USBHID.h"


// CDebugDlg �Ի���

IMPLEMENT_DYNAMIC(CDebugDlg, CDialogEx)

CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugDlg::IDD, pParent)
	, db_out_freq(0)
{

}

CDebugDlg::~CDebugDlg()
{
}

void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OUT_FREQ, db_out_freq);
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BOX1, &CDebugDlg::OnBnClickedBox1)
	ON_BN_CLICKED(IDC_BOX2, &CDebugDlg::OnBnClickedBox2)
	ON_BN_CLICKED(IDC_BOX3, &CDebugDlg::OnBnClickedBox3)
	ON_BN_CLICKED(IDC_BOX4, &CDebugDlg::OnBnClickedBox4)
	ON_BN_CLICKED(IDC_BOX5, &CDebugDlg::OnBnClickedBox5)
	ON_BN_CLICKED(IDC_BOX6, &CDebugDlg::OnBnClickedBox6)
	ON_BN_CLICKED(IDC_BOX7, &CDebugDlg::OnBnClickedBox7)
	ON_BN_CLICKED(IDC_BOX8, &CDebugDlg::OnBnClickedBox8)
	ON_BN_CLICKED(IDC_BOX9, &CDebugDlg::OnBnClickedBox9)
	ON_BN_CLICKED(IDC_BOX10, &CDebugDlg::OnBnClickedBox10)
	ON_BN_CLICKED(IDC_BOX11, &CDebugDlg::OnBnClickedBox11)
	ON_BN_CLICKED(IDC_BOX12, &CDebugDlg::OnBnClickedBox12)
	ON_BN_CLICKED(IDC_BOX13, &CDebugDlg::OnBnClickedBox13)
	ON_BN_CLICKED(IDC_BOX14, &CDebugDlg::OnBnClickedBox14)
	ON_BN_CLICKED(IDC_BOX15, &CDebugDlg::OnBnClickedBox15)
	ON_BN_CLICKED(IDC_BOX16, &CDebugDlg::OnBnClickedBox16)
	ON_BN_CLICKED(IDC_BOX17, &CDebugDlg::OnBnClickedBox17)
	ON_BN_CLICKED(IDC_BOX18, &CDebugDlg::OnBnClickedBox18)
	ON_BN_CLICKED(IDC_BOX19, &CDebugDlg::OnBnClickedBox19)
	ON_BN_CLICKED(IDC_BOX20, &CDebugDlg::OnBnClickedBox20)
	ON_BN_CLICKED(IDC_BUTTON_TRASH_POS, &CDebugDlg::OnBnClickedButtonTrashPos)
	ON_BN_CLICKED(IDC_BUTTON_READY_POS, &CDebugDlg::OnBnClickedButtonReadyPos)
	ON_BN_CLICKED(IDC_BUTTON_MEASURE_POS, &CDebugDlg::OnBnClickedButtonMeasurePos)
	ON_BN_CLICKED(IDC_BUTTON_SUCK, &CDebugDlg::OnBnClickedButtonSuck)
	ON_BN_CLICKED(IDC_BUTTON_PUT, &CDebugDlg::OnBnClickedButtonPut)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_ON, &CDebugDlg::OnBnClickedButtonCleanOn)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_OFF, &CDebugDlg::OnBnClickedButtonCleanOff)
	ON_BN_CLICKED(IDC_BUTTON_READY, &CDebugDlg::OnBnClickedButtonReady)
	ON_BN_CLICKED(IDC_BUTTON_DISPERSE, &CDebugDlg::OnBnClickedButtonDisperse)
	ON_BN_CLICKED(IDC_BUTTON_RORATE_ON, &CDebugDlg::OnBnClickedButtonRorateOn)
	ON_BN_CLICKED(IDC_BUTTON_RORATE_OFF, &CDebugDlg::OnBnClickedButtonRorateOff)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CDebugDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_CON_MEASURE, &CDebugDlg::OnBnClickedButtonConMeasure)
	ON_BN_CLICKED(IDC_BUTTON_MEASURE, &CDebugDlg::OnBnClickedButtonMeasure)
	ON_MESSAGE(WM_USB_STATUS, &CDebugDlg::USBStatus)
	ON_BN_CLICKED(IDC_BUTTON_POLE_INC, &CDebugDlg::OnBnClickedButtonPoleInc)
	ON_BN_CLICKED(IDC_BUTTON_POLE_DEC, &CDebugDlg::OnBnClickedButtonPoleDec)
	ON_BN_CLICKED(IDC_BUTTON_OUT_WAFER, &CDebugDlg::OnBnClickedButtonOutWafer)
	ON_BN_CLICKED(IDC_BUTTON_IN_WAFER, &CDebugDlg::OnBnClickedButtonInWafer)
	ON_BN_CLICKED(IDC_BOX21, &CDebugDlg::OnBnClickedBox21)
	ON_BN_CLICKED(IDC_BOX22, &CDebugDlg::OnBnClickedBox22)
	ON_BN_CLICKED(IDC_BOX23, &CDebugDlg::OnBnClickedBox23)
	ON_BN_CLICKED(IDC_BOX24, &CDebugDlg::OnBnClickedBox24)
	ON_BN_CLICKED(IDC_BTN_SEND, &CDebugDlg::OnBnClickedBtnSend)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDebugDlg ��Ϣ�������


void CDebugDlg::OnBnClickedBox1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	COpration::Get()->MoveMachine(BOX1_POS);
}


void CDebugDlg::OnBnClickedBox2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX2_POS);
}


void CDebugDlg::OnBnClickedBox3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX3_POS);
}


void CDebugDlg::OnBnClickedBox4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX4_POS);
}


void CDebugDlg::OnBnClickedBox5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX5_POS);
}


void CDebugDlg::OnBnClickedBox6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX6_POS);
}


void CDebugDlg::OnBnClickedBox7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX7_POS);
}


void CDebugDlg::OnBnClickedBox8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX8_POS);
}


void CDebugDlg::OnBnClickedBox9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX9_POS);
}


void CDebugDlg::OnBnClickedBox10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX10_POS);
}


void CDebugDlg::OnBnClickedBox11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX11_POS);
}


void CDebugDlg::OnBnClickedBox12()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX12_POS);
}


void CDebugDlg::OnBnClickedBox13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX13_POS);
}


void CDebugDlg::OnBnClickedBox14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX14_POS);
}


void CDebugDlg::OnBnClickedBox15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX15_POS);
}


void CDebugDlg::OnBnClickedBox16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX16_POS);
}


void CDebugDlg::OnBnClickedBox17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX17_POS);
}


void CDebugDlg::OnBnClickedBox18()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX18_POS);
}


void CDebugDlg::OnBnClickedBox19()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX19_POS);
}


void CDebugDlg::OnBnClickedBox20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX20_POS);
}


void CDebugDlg::OnBnClickedBox21()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX21_POS);
}


void CDebugDlg::OnBnClickedBox22()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX22_POS);
}


void CDebugDlg::OnBnClickedBox23()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX23_POS);
}


void CDebugDlg::OnBnClickedBox24()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX24_POS);
}



void CDebugDlg::OnBnClickedButtonTrashPos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(BOX1_POS);
}


void CDebugDlg::OnBnClickedButtonReadyPos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*IplImage* frame = COpration::Get()->GetFrame();
	IplImage* procimg = cvCreateImage(cvGetSize(frame), 8, 3);
	CvPoint pt = COpration::Get()->m_Tools.QuartzPostion(frame, procimg);
	COpration::Get()->SetPos(pt);*/
}


void CDebugDlg::OnBnClickedButtonMeasurePos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->MoveMachine(MEASURE_POS);
}


void CDebugDlg::OnBnClickedButtonSuck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->SuckWafer(true);
}


void CDebugDlg::OnBnClickedButtonPut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->SuckWafer(false);
}


void CDebugDlg::OnBnClickedButtonCleanOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->CleanPlatform(true);
}


void CDebugDlg::OnBnClickedButtonCleanOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->CleanPlatform(false);
}


void CDebugDlg::OnBnClickedButtonReady()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDebugDlg::OnBnClickedButtonDisperse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->DisperseWafer(20000);
}


void CDebugDlg::OnBnClickedButtonRorateOn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	COpration::Get()->RotatePlatform(INFINITE);

}


void CDebugDlg::OnBnClickedButtonRorateOff()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->DisperseWafer(2);
	COpration::Get()->RotatePlatform(2);
	COpration::Get()->WaferBox(2, true);
}


void CDebugDlg::OnBnClickedButtonScan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDebugDlg::OnBnClickedButtonConMeasure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDebugDlg::OnBnClickedButtonMeasure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

LRESULT CDebugDlg::USBStatus(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0)
	{
		SetWindowText(L"USB���ӶϿ�");
	} 
	else
	{
		SetWindowText(L"USB��������");
	}
	return 0;
}


void CDebugDlg::OnBnClickedButtonPoleInc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->PoleDistance(true);
}


void CDebugDlg::OnBnClickedButtonPoleDec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->PoleDistance(false);
}


void CDebugDlg::OnBnClickedButtonOutWafer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->WaferBox(INFINITE, true);
}


void CDebugDlg::OnBnClickedButtonInWafer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	COpration::Get()->WaferBox(INFINITE, false);
}



void CDebugDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	sw_and_atc_struct sw_atc_struct = { 0 };
	dds_struct dds_struct = { 0 };

	dds_struct.OutputFreq = db_out_freq;
	
	COpration::Get()->SetDDSParam(&dds_struct);


	if (IsDlgButtonChecked(IDC_CHECK_SW1A) == BST_CHECKED)
	{
		sw_atc_struct.sw1A_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW1B) == BST_CHECKED)
	{
		sw_atc_struct.sw1B_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW2A) == BST_CHECKED)
	{
		sw_atc_struct.sw2A_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW2B) == BST_CHECKED)
	{
		sw_atc_struct.sw2B_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW3A) == BST_CHECKED)
	{
		sw_atc_struct.sw3A_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW3B) == BST_CHECKED)
	{
		sw_atc_struct.sw3B_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW5A) == BST_CHECKED)
	{
		sw_atc_struct.sw5A_val = 1;
	}
	if (IsDlgButtonChecked(IDC_CHECK_SW5B) == BST_CHECKED)
	{
		sw_atc_struct.sw5B_val = 1;
	}

	CString str;
	UINT32 uATC = 0;
	GetDlgItemText(IDC_EDIT_ATC, str);
	if (str != L"")
	{
		sscanf_s(CW2A(str), "%x", &uATC);
	}
	
	sw_atc_struct.atc_val = uATC;

	COpration::Get()->SetSWAndATC(&sw_atc_struct);
}

double AIN1 = 0;
double AIN2 = 0;
double AIN3 = 0;
int times = 0;
#define AVERAGE_TIMES	40

BOOL CDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//COpration::Get()->InitMachine(m_hWnd);
	//SetWindowText(L"USB���ӶϿ�");
	SetTimer(1, 30, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Recv_Data_Struct recv_dat;
	double temp;
	CString str;
	COpration::Get()->ReceiveData((BYTE*)&recv_dat, sizeof(Recv_Data_Struct));
	if (times++ < AVERAGE_TIMES)
	{
		temp = 2.5 * (recv_dat.ADC_Value[0] & 0xfff) / (double)0xfff;
		AIN1 += temp;
		temp = 2.5 * (recv_dat.ADC_Value[1] & 0xfff) / (double)0xfff;	
		AIN2 += temp;
		temp = 2.5 * (recv_dat.ADC_Value[2] & 0xfff) / (double)0xfff;
		AIN3 += temp;
		//str.Format(L"%.3f", temp);
		//AfxMessageBox(str);
	}
	else
	{
		times = 0;
		str.Format(L"%.3f V", AIN1 / AVERAGE_TIMES);
		SetDlgItemText(IDC_EDIT_AIN1, str);
		AIN1 = 0;
		str.Format(L"%.3f V", AIN2 / AVERAGE_TIMES);
		SetDlgItemText(IDC_EDIT_AIN2, str);
		AIN2 = 0;
		str.Format(L"%.3f V", AIN3 / AVERAGE_TIMES);
		SetDlgItemText(IDC_EDIT_AIN3, str);
		AIN3 = 0;
	}
	CDialogEx::OnTimer(nIDEvent);
}
