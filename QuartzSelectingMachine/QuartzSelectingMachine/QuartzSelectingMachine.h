
// QuartzSelectingMachine.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQuartzSelectingMachineApp:
// �йش����ʵ�֣������ QuartzSelectingMachine.cpp
//

class CQuartzSelectingMachineApp : public CWinApp
{
public:
	CQuartzSelectingMachineApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQuartzSelectingMachineApp theApp;