
// SocketDebug.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSocketDebugApp: 
// �йش����ʵ�֣������ SocketDebug.cpp
//

class CSocketDebugApp : public CWinApp
{
public:
	CSocketDebugApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSocketDebugApp theApp;