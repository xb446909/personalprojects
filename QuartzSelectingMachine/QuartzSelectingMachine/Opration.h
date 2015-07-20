#pragma once

#include "USBHID.h"
#include "cv.h"
#include "highgui.h"
#include "Tools.h"
#include "common_def.h"

#define VID 0x0483
#define PID 0x5750

#define		BOX1_POS		114.4
#define		BOX2_POS		109.7
#define		BOX3_POS		105.0
#define		BOX4_POS		100.3
#define		BOX5_POS		95.6
#define		BOX6_POS		90.9		
#define		BOX7_POS		86.2
#define		BOX8_POS		81.5
#define		BOX9_POS		76.8
#define		BOX10_POS		72.1
#define		BOX11_POS		67.4
#define		BOX12_POS		62.7
#define		BOX13_POS		58.0
#define		BOX14_POS		53.3
#define		BOX15_POS		48.6
#define		BOX16_POS		43.9
#define		BOX17_POS		39.2
#define		BOX18_POS		34.5
#define		BOX19_POS		29.8
#define		BOX20_POS		25.1	
#define		BOX21_POS		20.4
#define		BOX22_POS		15.7
#define		BOX23_POS		11.0
#define		BOX24_POS		6.3
#define		MEASURE_POS		121.0

#define CMD_SET_ROBOT_ORG	0x01
#define CMD_SET_ROBOT_MOVE	0x02
#define CMD_SET_IO			0x03
#define CMD_CLEAN_PLAT		0x04
#define CMD_DISPERSE		0x05
#define CMD_ROTATE_PLAT		0x06
#define CMD_WAFER_BOX		0x07
#define CMD_POLE_DIST		0x08
#define CMD_SUCK			0x09

#ifndef Distance
#define Distance(a, b)	sqrt((a) * (a) + (b) * (b))
#endif

typedef struct command_struct
{
	unsigned char uCMD;
	unsigned char speed;
	double location;
	unsigned char IO_Status[4];
}COMMAND_STRUCT, *pCOMMAND_STRUCT;


class COpration
{
	static COpration* m_theOpration;

public:
	COpration(void);
	~COpration(void);
	static COpration* Get();
	void InitOpration();
	void InitMachine(HWND hWnd);
	void MoveMachine(float pos);		//移动机械手到指定位置
	void CleanPlatform(bool bSwitch);	//开：上电极吹气	关：上电极停止吹气
	void DisperseWafer(UINT32 uPulseNum);		//uPulseNum：发送脉冲数
	void RotatePlatform(UINT32 uPulseNum);		//uPulseNum：发送脉冲数
	void SystemScan(void);				//系统扫频
	void MeasureWafer(UINT iTimes);		//测量晶片频率	iTimes为0 连续测量，iTimes不为0 测量指定次数
	void WaferBox(UINT32 uPulseNum, bool bDirection);	//true为出料 false为入料  uPulseNum：发送脉冲数	
				
	void PoleDistance(bool bSwitch);	//电极距离，true为加大,false为减小
	void SuckWafer(bool bSwitch);		//吸取晶片，true为吸取，false为放下

	void InitCam(void);
	void UninitCam(void);
	void ShowToWindow(HWND hWnd, UINT32 CTRL_ID);
	void SaveFrameAsBmp(char* FileName);
	IplImage* GetFrame();

	void SetPos(CvPoint pt);

	void SetDDSParam(dds_struct* pdds_param);
	void SetSWAndATC(sw_and_atc_struct* psw_and_atc);
	void SendData(BYTE* dat, int size);
	void ReceiveData(BYTE* dat, int size);

	CTools m_Tools;

	HANDLE hMVC21MM;
	CUSBHID m_USBHID;

	DWORD dwThreadID;
	HWND g_hWnd;

};

