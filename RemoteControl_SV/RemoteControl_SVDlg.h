
// RemoteControl_SVDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRemoteControl_SVDlg ��ȭ ����
class CRemoteControl_SVDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CRemoteControl_SVDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_REMOTECONTROL_SV_DIALOG };

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
	CString state;
	afx_msg void OnEnChangeEdit1();
	int port;
	CString password;
//	CIPAddressCtrl ip;
	afx_msg void OnBnClickedButton1();
	CString c_state;
	afx_msg void OnEnChangeEdit2();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnBnClickedButton2();
//	int aaa;
	CIPAddressCtrl in_ip;
//	CIPAddressCtrl ip_out;
//	CIPAddressCtrl ip_out;
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnBnClickedButton3();
//	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	CComboBox combo;
};
