
// �����Dlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// C�����Dlg ��ȭ ����
class C�����Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	C�����Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl IP_ADR;
	afx_msg void OnBnClickedButton1();
	CListCtrl LIST_CRT;
	CButton RADIO_FIND;
	afx_msg void OnBnClickedButton2();
	int StudentNum;
	CString StudentName;
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit SELECT_;
	int SET_SCORE;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CButton RADIO_A;
	int RADIO_ABC;
	afx_msg void OnBnClickedButton6();
	int ABC_NUM;
	afx_msg void OnMcnSelchangeMonthcalendar2(NMHDR *pNMHDR, LRESULT *pResult);
	CMonthCalCtrl CAL_CTR;
	CListBox LIST_BOX;
	CString MEMO_TEXT;
	afx_msg void OnBnClickedButton7();
	CString LOGIN_NAME;
	afx_msg void OnBnClickedButton8();
};
