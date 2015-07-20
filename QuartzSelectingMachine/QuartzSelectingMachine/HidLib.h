// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HIDLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HIDLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef HIDLIB_EXPORTS
#define HIDLIB_API __declspec(dllexport)
#else
#define HIDLIB_API __declspec(dllimport)
#endif

#define WRITE_FEATURE_ID	0x01
#define READ_FEATURE_ID		0x02

#define	STATUS_SUCCESS		0x0
#define STATUS_ERROR		0x1

#define	WM_HID_READ		(WM_USER + 1)
#define WM_HID_WRITE	(WM_USER + 2)
#define WM_HID_STATUS	(WM_USER + 3)

typedef struct _hid_info_struct
{
	USHORT uVID;
	USHORT uPID;
}HidInfoStruct, *pHidInfoStruct;

typedef struct _msg_info_struct
{
	int size;
	BYTE src[64];
}MsgInfoStruct, *pMsgInfoStruct;

HIDLIB_API HANDLE hRead;
HIDLIB_API HANDLE hWrite;

HIDLIB_API DWORD OpenHid(USHORT uVID, USHORT uPID);	
HIDLIB_API void CloseHid(USHORT uVID, USHORT uPID);
