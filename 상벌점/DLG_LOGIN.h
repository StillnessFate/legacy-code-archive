#pragma once


// DLG_LOGIN 대화 상자입니다.

class DLG_LOGIN : public CDialogEx
{
	DECLARE_DYNAMIC(DLG_LOGIN)

public:
	DLG_LOGIN(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DLG_LOGIN();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
