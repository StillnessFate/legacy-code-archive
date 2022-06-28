#pragma once


// RemoteView 대화 상자입니다.

class RemoteView : public CDialogEx
{
	DECLARE_DYNAMIC(RemoteView)

public:
	RemoteView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~RemoteView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	SOCKET sock;
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
//	double aspectRatio;
	int min_x;
	int min_y;
	double aspectx;
	double aspecty;
	virtual BOOL OnInitDialog();
//	int aaa;
//	int scr_x;
//	int scr_y;
	SOCKET sock2;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnMouseLeave();
//	afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
//	afx_msg void OnNcMButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
//	int QR;
	bool QR;
	HHOOK hook;
//	afx_msg void OnDestroy();
	int port;
};
