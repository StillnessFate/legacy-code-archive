#pragma once


// DLG_LOGIN ��ȭ �����Դϴ�.

class DLG_LOGIN : public CDialogEx
{
	DECLARE_DYNAMIC(DLG_LOGIN)

public:
	DLG_LOGIN(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~DLG_LOGIN();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	bool LOGIN_OK;
	CString STR_ID;
	CString STR_PW;
	SOCKET sock;
	char LOGIN_NAME[50];
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
