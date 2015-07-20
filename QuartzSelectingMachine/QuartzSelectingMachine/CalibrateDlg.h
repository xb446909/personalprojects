#pragma once

#include "Opration.h"
// CCalirateDlg �Ի���

class CCalibrateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrateDlg)

public:
	CCalibrateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalibrateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALIBRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRotate();
	virtual BOOL OnInitDialog();
	LRESULT USBStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnSave();
	UINT pulseNum;
	double OrigX;
	double OrigY;
	double paramK;
	double paramB;
	double innerR;
	double outerR;
	UINT waferPxHigh;
	UINT waferPxLow;
};
