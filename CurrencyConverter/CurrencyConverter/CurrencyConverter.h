
// CurrencyConverter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCurrencyConverterApp: 
// �йش����ʵ�֣������ CurrencyConverter.cpp
//

class CCurrencyConverterApp : public CWinApp
{
public:
	CCurrencyConverterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCurrencyConverterApp theApp;