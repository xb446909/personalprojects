
// CurrencyConverterDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CCurrencyConverterDlg 对话框
class CCurrencyConverterDlg : public CDialogEx
{
// 构造
public:
	CCurrencyConverterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CURRENCYCONVERTER_DIALOG };

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
	afx_msg void OnBnClickedBtnExc();
	afx_msg void OnBnClickedBtnCvt();
	double gbp;
	double eur;
	double cny;
	double jpy;
	bool CCurrencyConverterDlg::GetWebSrc(const CString& strFileURLInServer,
		const CString & strFileLocalFullPath);
	bool GetCurrencyOverXml();
	void InitialUI();
	// 原始货币数
	double db_src;
	// 目标货币数
	double db_dst;
	CComboBox combobox_src;
	CComboBox combobox_dst;
	afx_msg void OnBnClickedBtnCalc();
	double g_usd;
	int g_words;
};
