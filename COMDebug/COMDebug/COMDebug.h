
// COMDebug.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCOMDebugApp:
// �йش����ʵ�֣������ COMDebug.cpp
//

class CCOMDebugApp : public CWinAppEx
{
public:
	CCOMDebugApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCOMDebugApp theApp;