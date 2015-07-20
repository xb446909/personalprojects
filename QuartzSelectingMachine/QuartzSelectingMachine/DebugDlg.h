#pragma once

#include "Opration.h"
// CDebugDlg 对话框

class CDebugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugDlg)

public:
	CDebugDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebugDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBox1();
	afx_msg void OnBnClickedBox2();
	afx_msg void OnBnClickedBox3();
	afx_msg void OnBnClickedBox4();
	afx_msg void OnBnClickedBox5();
	afx_msg void OnBnClickedBox6();
	afx_msg void OnBnClickedBox7();
	afx_msg void OnBnClickedBox8();
	afx_msg void OnBnClickedBox9();
	afx_msg void OnBnClickedBox10();
	afx_msg void OnBnClickedBox11();
	afx_msg void OnBnClickedBox12();
	afx_msg void OnBnClickedBox13();
	afx_msg void OnBnClickedBox14();
	afx_msg void OnBnClickedBox15();
	afx_msg void OnBnClickedBox16();
	afx_msg void OnBnClickedBox17();
	afx_msg void OnBnClickedBox18();
	afx_msg void OnBnClickedBox19();
	afx_msg void OnBnClickedBox20();
	afx_msg void OnBnClickedButtonTrashPos();
	afx_msg void OnBnClickedButtonReadyPos();
	afx_msg void OnBnClickedButtonMeasurePos();
	afx_msg void OnBnClickedButtonSuck();
	afx_msg void OnBnClickedButtonPut();
	afx_msg void OnBnClickedButtonCleanOn();
	afx_msg void OnBnClickedButtonCleanOff();
	afx_msg void OnBnClickedButtonReady();
	afx_msg void OnBnClickedButtonDisperse();
	afx_msg void OnBnClickedButtonRorateOn();
	afx_msg void OnBnClickedButtonRorateOff();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonConMeasure();
	afx_msg void OnBnClickedButtonMeasure();

	virtual BOOL OnInitDialog();
	afx_msg LRESULT USBStatus(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonPoleInc();
	afx_msg void OnBnClickedButtonPoleDec();
	afx_msg void OnBnClickedButtonOutWafer();
	afx_msg void OnBnClickedButtonInWafer();
	afx_msg void OnBnClickedBox21();
	afx_msg void OnBnClickedBox22();
	afx_msg void OnBnClickedBox23();
	afx_msg void OnBnClickedBox24();
	double db_out_freq;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
