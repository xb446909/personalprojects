
// QuartzSelectingMachineDlg.h : ͷ�ļ�
//

#pragma once

#include "Opration.h"
// CQuartzSelectingMachineDlg �Ի���
class CQuartzSelectingMachineDlg : public CDialogEx
{
// ����
public:
	CQuartzSelectingMachineDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QUARTZSELECTINGMACHINE_DIALOG };

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
