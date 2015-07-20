#pragma once

#define VID 0x0483
#define PID 0x5750

#define WM_USB_STATUS		(WM_USER + 1)

class CUSBHID
{
public:
	CUSBHID(void);
	~CUSBHID(void);
	HWND m_hWnd;			//���봰�ھ������usb��Ϣ
	void OpenUSB(USHORT VenderID, USHORT ProductID);  //��usb�豸
	void CloseUSB();
	BOOL Send64Byte( BYTE* SendBuffer, int size);
	BOOL Receive64Byte(BYTE* RecvBuffer);
};

