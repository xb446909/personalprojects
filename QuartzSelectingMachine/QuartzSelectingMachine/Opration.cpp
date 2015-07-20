#include "StdAfx.h"
#include "Opration.h"
#include "USBHID.h"
#include "MVC21MM.h"
#include "HidLib.h"

#pragma comment(lib, "MVC21MM.lib")
#pragma comment(lib, "HidLib.lib")

MVC21MMCapInfo CapInfo;

BYTE mRawData[(1280 * 1024) + 512];
static HidInfoStruct m_recv_hidinfo;
static MsgInfoStruct m_recv_msginfo;
static HidInfoStruct m_send_hidinfo;
static MsgInfoStruct m_send_msginfo;


COpration::COpration(void)
{
}


COpration::~COpration(void)
{
	m_USBHID.CloseUSB();	
}

COpration* COpration::m_theOpration = NULL;

COpration* COpration::Get() { return m_theOpration; }

void COpration::UninitCam()
{
	MV_Usb2Uninit(&hMVC21MM);
}

void COpration::InitCam()
{
	int nRet, index;

	memset(&CapInfo, 0, sizeof(MVC21MMCapInfo));
	CapInfo.Buffer = mRawData;
	CapInfo.Height = 1024;
	CapInfo.Width = 1280;
	CapInfo.HorizontalOffset = 0;
	CapInfo.VerticalOffset = 0;
	CapInfo.Exposure = 500;
	CapInfo.Gain[0] = 20;

	nRet = MV_Usb2Init(L"MVC1000", &index, &CapInfo, &hMVC21MM);
	if (nRet != 0)
	{
		AfxMessageBox(L"相机初始化失败!");
		return;
	}
}

void COpration::InitOpration()
{
	m_theOpration = new COpration();
}

void COpration::InitMachine(HWND hWnd)
{
	//m_USBHID.m_hWnd = hWnd;
	//m_USBHID.OpenUSB(VID, PID);
	dwThreadID = OpenHid(VID, PID);
	g_hWnd = hWnd;
}

void COpration::ShowToWindow(HWND hWnd, UINT32 CTRL_ID)
{
	int nRet;
	nRet = MV_Usb2Start(hMVC21MM,
		L"MVC1000 Preview",
		WS_CHILD|WS_VISIBLE,
		0,
		0,
		-1,	
		-1,	
		hWnd,
		CTRL_ID,
		THREAD_PRIORITY_NORMAL,
		THREAD_PRIORITY_NORMAL);

	/*nRet = MV_Usb2Start (hMVC21MM, L"My View",
	WS_OVERLAPPEDWINDOW|WS_VISIBLE,0,0,-1,-1,NULL,NULL,
	THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_NORMAL);*/

	if (nRet != 0)
	{
		AfxMessageBox(L"打开相机失败!");
	}

	CRect rect;
	GetClientRect(hWnd, &rect);
	MVRECT   rectPreview = { rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top };

	MV_Usb2SetViewWin(hMVC21MM, &rectPreview);
}

void COpration::SaveFrameAsBmp(char* FileName)
{
	MV_Usb2Pause(hMVC21MM, 1);
	MV_Usb2SavePausedFrameAsBmp(hMVC21MM, (LPCTSTR)FileName);
	MV_Usb2Pause(hMVC21MM, 0);
}

IplImage* COpration::GetFrame()
{
	MV_Usb2GetRawData(hMVC21MM, &CapInfo);	
	IplImage* frame = cvCreateImage(cvSize(1280, 1024), 8, 1);
	memcpy(frame->imageData, mRawData, 1280*1024);
	cvFlip(frame);
	return frame;
}

void COpration::MoveMachine(float pos)			//移动机械手到指定位置
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_SET_ROBOT_MOVE;
	cmd_struct.speed = 100;
	cmd_struct.location = (double)pos;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}


void COpration::CleanPlatform(bool bSwitch)	//开：上电极吹气	关：上电极停止吹气
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_CLEAN_PLAT;
	cmd_struct.IO_Status[0] = bSwitch;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}


void COpration::DisperseWafer(UINT32 uPulseNum)	//平台振动使料散开
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_DISPERSE;
	*(UINT32*)cmd_struct.IO_Status = uPulseNum;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}


void COpration::RotatePlatform(UINT32 uPulseNum)		//uPulseNum：发送脉冲数
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_ROTATE_PLAT;
	*(UINT32*)cmd_struct.IO_Status = uPulseNum;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}


void COpration::WaferBox(UINT32 uPulseNum, bool bDirection)
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_WAFER_BOX;
	cmd_struct.speed = bDirection;
	*(UINT32*)cmd_struct.IO_Status = uPulseNum;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}


void COpration::PoleDistance(bool bSwitch)	//电极距离，true为加大,false为减小
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_POLE_DIST;
	cmd_struct.IO_Status[0] = bSwitch;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}


void COpration::SystemScan(void)				//系统扫频
{

}


void COpration::MeasureWafer(UINT iTimes)		//测量晶片频率	iTimes为0 连续测量，iTimes不为0 测量指定次数
{

}


void COpration::SuckWafer(bool bSwitch)
{
	DWORD dwError;
	BYTE SendBuffer[64] = {0};
	CString str;

	COMMAND_STRUCT cmd_struct;
	cmd_struct.uCMD = CMD_SUCK;
	cmd_struct.IO_Status[0] = bSwitch;
	memcpy(SendBuffer, &cmd_struct, sizeof(COMMAND_STRUCT));

	if (!m_USBHID.Send64Byte(SendBuffer, 64))
	{
		dwError = GetLastError();
		str.Format(L"发送错误，错误代码:%d", dwError);
		AfxMessageBox(str);
	}
}

void COpration::SetPos(CvPoint pt)
{
	CalibrateData m_CalibrateData;
	double temp_x, x;
	m_Tools.GetCalirateParam(&m_CalibrateData);
	double x0 = m_CalibrateData.OriginPoint.x;
	double y0 = m_CalibrateData.OriginPoint.y;
	double x1 = pt.x;
	double y1 = pt.y;
	double r = Distance(y1 - y0, x1 - x0);
	double theta1 = asin((y1 - y0) / r);
	double theta2 = asin(13.0 / r);
	double theta = theta1 + theta2;
	int num = m_CalibrateData.PlatPulseNum * 0.5 * theta / CV_PI;
	RotatePlatform(num);
	temp_x = x0 - sqrt((r*r) - 169);
	x = temp_x * m_CalibrateData.Param_k + m_CalibrateData.Param_b;
	MoveMachine(x);
}

void COpration::SetDDSParam(dds_struct* pdds_param)
{
	Send_Data_Struct send_dat;
	send_dat.cmd_type = COMMAND_SET_DDS_PARAM;
	memcpy(&send_dat.cmd_data.m_dds_param, pdds_param, sizeof(dds_struct));
	BYTE* temp = (BYTE*)&send_dat;
	SendData(temp, sizeof(Send_Data_Struct));
}

void COpration::SetSWAndATC(sw_and_atc_struct* psw_and_atc)
{
	Send_Data_Struct send_dat;
	send_dat.cmd_type = COMMAND_SET_SW_AND_ATC;
	memcpy(&send_dat.cmd_data.m_sw_and_stc, psw_and_atc, sizeof(sw_and_atc_struct));
	BYTE* temp = (BYTE*)&send_dat;
	SendData(temp, sizeof(Send_Data_Struct));
}


void COpration::SendData(BYTE* dat, int size)
{
	WaitForSingleObject(hWrite, INFINITE);
	m_send_hidinfo.uVID = VID;
	m_send_hidinfo.uPID = PID;
	m_send_msginfo.size = (size > 64) ? 64 : size;
	memcpy(&m_send_msginfo.src, dat, size);
	PostThreadMessage(dwThreadID, WM_HID_WRITE, (WPARAM)&m_send_msginfo, (LPARAM)&m_send_hidinfo);
}

void COpration::ReceiveData(BYTE* dat, int size)
{
	WaitForSingleObject(hRead, INFINITE);
	m_recv_hidinfo.uVID = VID;
	m_recv_hidinfo.uPID = PID;
	m_recv_msginfo.size = (size > 64) ? 64 : size;
	PostThreadMessage(dwThreadID, WM_HID_READ, (WPARAM)&m_recv_msginfo, (LPARAM)&m_recv_hidinfo);
	memcpy(dat, m_recv_msginfo.src, m_recv_msginfo.size);
}
