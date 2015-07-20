
// CurrencyConverterDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CCurrencyConverterDlg �Ի���
class CCurrencyConverterDlg : public CDialogEx
{
// ����
public:
	CCurrencyConverterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CURRENCYCONVERTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	// ԭʼ������
	double db_src;
	// Ŀ�������
	double db_dst;
	CComboBox combobox_src;
	CComboBox combobox_dst;
	afx_msg void OnBnClickedBtnCalc();
	double g_usd;
	int g_words;
};
