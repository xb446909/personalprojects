#pragma once

#define VID 0x0483
#define PID 0x5750

#define WM_USB_STATUS		(WM_USER + 1)

class CUSBHID
{
public:
	CUSBHID(void);
	~CUSBHID(void);
	HWND m_hWnd;			//传入窗口句柄接收usb消息
	void OpenUSB(USHORT VenderID, USHORT ProductID);  //打开usb设备
	void CloseUSB();
	BOOL Send64Byte( BYTE* SendBuffer, int size);
	BOOL Receive64Byte(BYTE* RecvBuffer);
};

