
// RemoteControl_CLDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CRemoteControl_CLDlg ��ȭ ����
class CRemoteControl_CLDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CRemoteControl_CLDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_REMOTECONTROL_CL_DIALOG };

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
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	CString password;
	int port;
	afx_msg void OnEnChangeEdit1();
	CIPAddressCtrl ip;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	BOOL rad;
};
