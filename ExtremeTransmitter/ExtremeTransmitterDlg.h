
// ExtremeTransmitterDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"


// CExtremeTransmitterDlg 대화 상자
class CExtremeTransmitterDlg : public CDialogEx
{
// 생성입니다.
public:
	CExtremeTransmitterDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ExtremeTransmitter_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButton3();
//	int ggg;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
//	int ggg2;
//	afx_msg void OnBnClickedButton6();
//	afx_msg void OnBnClickedButton7();
//	CString node;
//	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	CString dr;
//	int speed;
	int Speed;
	CProgressCtrl dwp;
	CProgressCtrl upp;
	int upSpeed;
//	CString ts;
//	afx_msg void OnBnClickedButton1();
	CIPAddressCtrl ipad;
//	int portad;
	afx_msg void OnEnChangeEdit7();
	int portad;
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	CString sstay;
//	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton7();
//	afx_msg void OnBnClickedButton10();
//	afx_msg void OnBnClickedButton8();
//	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
};
