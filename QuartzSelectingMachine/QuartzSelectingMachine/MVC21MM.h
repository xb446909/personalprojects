#ifndef _MVC1000APIUSER_H
#define _MVC1000APIUSER_H

#define MVC1000MEV	100
// Capture Information Structure
typedef struct CapInfoStruct
{
	unsigned char *Buffer;
	unsigned long Height;
	unsigned long Width;
	unsigned long HorizontalOffset;
	unsigned long VerticalOffset;
	unsigned long Exposure;
	unsigned char Gain[3];
	unsigned char Control;
	unsigned char Reserved[8];
}MVC21MMCapInfo, *pMVC21MMCapInfo;

struct MVRECT 
{
	LONG   left;
	LONG   top;
	LONG   width;
	LONG   height;
};


// Return Contanst
#define ResSuccess					0x0000
#define ResNullHandleErr			0x0001
#define ResNullPointerErr			0x0002
#define ResFileOpenErr				0x0003
#define ResNoDeviceErr				0x0004
#define ResInvalidParameterErr		0x0005
#define ResOutOfMemoryErr			0x0006
#define ResNoPreviewRunningErr		0x0007
#define ResOSVersionErr				0x0008
#define ResUsbNotAvailableErr		0x0009
#define ResNotSupportedErr			0x000a
#define ResNoSerialString			0x000b
#define ResVerificationErr			0x000c
//#define ResTimeoutErr	            0x000d
#define ResScaleModeErr             0x000e
#define ResPreviewRunningErr        0x000f
#define ResNotPauseErr              0x0010
#define ResPIDERR					0x0011
#define ResNoiseOverflow			0x0012
#define ResUnknownErr				0x00ff



#ifdef MVC1000API_EXPORTS
#define MV_API extern __declspec(dllexport) int WINAPI
#else
#define MV_API extern __declspec(dllimport) int WINAPI
#endif

#ifdef __cplusplus
extern "C"{
#else
#endif

typedef
VOID
(WINAPI * LPMV_CALLBACK)(LPVOID lpParam);

typedef
VOID
(WINAPI * LPMV_CALLBACK2)(LPVOID lpParam, LPVOID lpUser);


/*--------------------------------------------------------------------------
	Initialize device, should call before all function, the HANDLE returned.
  --------------------------------------------------------------------------*/
MV_API 
MV_Usb2Init(	IN  LPCTSTR      pszFilterName,
				OUT int          *pnIndex,
				IN CapInfoStruct *pCapInfo,
				OUT HANDLE       *hImager);	

/*--------------------------------------------------------------------------
	Uninitialize device, should call when initialize fail or exit application.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2Uninit(IN HANDLE* hImager);	


/*--------------------------------------------------------------------------
	start preview, should call after initial.
  --------------------------------------------------------------------------*/
MV_API 
MV_Usb2Start(   IN HANDLE   hImager,
				IN LPCTSTR  pszTitle,
				IN DWORD    dwStyle,
				IN DWORD    x,	
				IN DWORD    y,	
				IN DWORD    dwWidth,
				IN DWORD    dwHeight,
				IN HWND     hParentWnd,
				IN DWORD    nId,	
				IN int      ViewDataThreadPriority,
				IN int      ViewDrawThreadPriority);


/*--------------------------------------------------------------------------
	pause preview, call this before call MV_Usb2SavePausedFrameAsBmp.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2Pause(IN HANDLE hImager,	
				IN ULONG pause);

/*--------------------------------------------------------------------------
	stop preview, call before exit to application
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2Stop(IN HANDLE hImager);	

/*--------------------------------------------------------------------------
	set a gamma array to preview.
  --------------------------------------------------------------------------*/
MV_API 
MV_Usb2SetGamma( IN HANDLE hImager,
				 IN BYTE   *pGamma,
				 IN BOOL   bGamma );



/*--------------------------------------------------------------------------
	Set the capture information, note CapInfoStruct.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2SetCapInfo(IN HANDLE hImager, IN CapInfoStruct *pCapInfo);

/*--------------------------------------------------------------------------
	Set the capture information without changing preview window
	add v1.1.2.2
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2SetPartOfCapInfo(IN HANDLE hImager, IN CapInfoStruct *pCapInfo);
/*--------------------------------------------------------------------------
	get frame rate.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2GetFrameRate( IN HANDLE  hImager,
				     OUT float  *pfFrameRate);

/*--------------------------------------------------------------------------
	save paused frame to file (must in pause state).
  --------------------------------------------------------------------------*/
MV_API 
MV_Usb2SavePausedFrameAsBmp( IN HANDLE  hImager,
						     IN LPCTSTR szFileName);


/*--------------------------------------------------------------------------
	save one frame to file.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2SaveFrameAsBmp( IN  HANDLE         hImager,
					   IN  CapInfoStruct  *pCapInfo,
					   OUT BYTE			  *pDest,
					   IN  LPCTSTR        szFileName);	



/*--------------------------------------------------------------------------
	get frame data
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2GetRawData(IN HANDLE hImager,
			      IN struct CapInfoStruct *pCapInfo);


/*--------------------------------------------------------------------------
	set preview window.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2SetViewWin(IN HANDLE hImager,
				  IN MVRECT* pClientRect);


/*--------------------------------------------------------------------------
	get preview window
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2GetViewWin(IN HANDLE hImager,
				  OUT MVRECT *pSubWindowRect);

/*--------------------------------------------------------------------------
	reset preview window to capInfo.Width and capInfo.Height.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2ResetViewWin(IN HANDLE hImager);	// Imager object handle


/*--------------------------------------------------------------------------
	get device number connecting to pc.
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2GetNumberDevices(IN HANDLE  hImager,
						OUT PULONG pNumberDevices);	// Ptr to number


/*--------------------------------------------------------------------------
	set gamma value to preview. this should mutex to gamma table.
  --------------------------------------------------------------------------*/
MV_API 
MV_Usb2SetGammaValue(HANDLE hImager, float GammaValue,  BOOL bGammaOn);



/*--------------------------------------------------------------------------------
	same to MV_Usb2GetRawData
  --------------------------------------------------------------------------*/
MV_API
MV_Usb2GetGrayData( IN HANDLE hImager, 
				    IN CapInfoStruct *pCapInfo);


/*--------------------------------------------------------------------------------
	get paused frame data (must be in pause state )
  --------------------------------------------------------------------------------*/
MV_API  
MV_Usb2GetPausedGrayData( IN HANDLE hImager, 
								  OUT BYTE* pDest);



/*--------------------------------------------------------------------------
	get serial. don't call now please.
  --------------------------------------------------------------------------------*/
MV_API	
MV_Usb2GetSerial(IN HANDLE hImager, OUT LPTSTR szSerial);


/*--------------------------------------------------------------------------
  Reserved. Cannot be used.
  capture external input, and capture one frame if an rising edge occur on pin6
  --------------------------------------------------------------*/
MV_API
MV_Usb2GetOneFrameByExternal(IN HANDLE hImager, 
							 IN struct CapInfoStruct *pCapInfo,
							 IN DWORD  dwTimeout,			//millisecond
			                 OUT BOOL  *bTriggered);


/*--------------------------------------------------------------------------
	setup callback function for mask, if MaskCallBackFunction==NULL, will no mask.
  --------------------------------------------------------------*/
MV_API
MV_Usb2SetMaskCallBack(IN HANDLE hImager, LPMV_CALLBACK2 MaskCallBackFunction, LPVOID lpUser);


/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MV_API
MV_Usb2SharpPreview(IN HANDLE hImager, BOOL bSharp);



/*--------------------------------------------------------------------------
	get an nFrames Average Frame
  --------------------------------------------------------------*/
MV_API
MV_Usb2GetFramesAverageGrayData( IN HANDLE hImager, 
								 IN struct CapInfoStruct *pCapInfo, 
								 IN int    nFrames );



/*--------------------------------------------------------------------------
	auro exposure
  --------------------------------------------------------------*/
MV_API 
MV_Usb2AutoExposure(IN HANDLE  hImager, 
					IN BOOL bAdjustExp, 
					IN BYTE btTarget, 
					IN LPVOID lpFunc, 
					IN LONG *pParam);

/*--------------------------------------------------------------------------
	Reserved. Cannot be used.
	GPIO Read and write, only pin2,pin3,pin4,pin5
  --------------------------------------------------------------*/
MV_API
MV_Usb2BitOperation( IN  HANDLE hImager,
			         IN  int   bOut,
			         OUT int   *bIn ); 

/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MV_API
MV_Usb2MeanPreview( IN HANDLE hImager, BOOL bMean );

/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MV_API
MV_Usb2EmbossPreview(IN HANDLE hImager, BOOL bEmboss);


/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MV_API
MV_Usb2EdgeDetectPreview(IN HANDLE hImager, BOOL bEdgeDetect);

/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MV_API
MV_Usb2GaussBlurPreview(IN HANDLE hImager, BOOL bGaussBlur);

/*------------------------------------------------------------------------------
	setup callback function for image data arriving.
  ------------------------------------------------------------------------------*/
MV_API
MV_Usb2SetFrameCallBack( IN HANDLE hImager, 
						 LPMV_CALLBACK2 FrameCallBackFunction, 
						 LPVOID lpUser);

MV_API
MV_Usb2VerifyUserPwd( IN HANDLE hImager, BYTE WRRDFlag, BYTE CH, BYTE *ptr );

MV_API
MV_Usb2ModifyUserPwd( IN HANDLE hImager, BYTE WRRDFlag, BYTE CH, BYTE *OldPtr, BYTE *NewPtr );

MV_API
MV_Usb2WriteUserDate( IN HANDLE hImager, BYTE CH, BYTE StartAdd, BYTE Num, BYTE *Ptr );

MV_API
MV_Usb2ReadUserDate( IN HANDLE hImager, BYTE CH, BYTE StartAdd, BYTE Num, BYTE *Ptr );

MV_API
MV_Usb2SetMaskType( IN HANDLE hImager, int nMaskType, int nLength1, int nLength2 );

MV_API
MV_Usb2ReadKey( IN HANDLE hImager );

MV_API
MV_Usb2GetDeviceSubNum( IN HANDLE hImager, OUT USHORT *pSubNum );

MV_API
MV_Usb2SetImageProcess(IN HANDLE hImager, int nBrightness, int nContrast, BOOL bProcess);

#ifdef __cplusplus
}
#endif

#endif //_MVC1000APIUSER_H

