
// QuartzSelectingMachineDlg.h : 头文件
//

#pragma once

#include "Opration.h"
// CQuartzSelectingMachineDlg 对话框
class CQuartzSelectingMachineDlg : public CDialogEx
{
// 构造
public:
	CQuartzSelectingMachineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QUARTZSELECTINGMACHINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFont m_font;
	HBRUSH m_BlackBrush;
	afx_msg void OnDebug();
	CStatusBar mStatusBar;
	afx_msg LRESULT SetUSBStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCalibrate();
	afx_msg void OnBnClickedBtnAuto();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStop();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
