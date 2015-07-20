#pragma once

#include "Opration.h"
// CCalirateDlg 对话框

class CCalibrateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrateDlg)

public:
	CCalibrateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibrateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALIBRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
