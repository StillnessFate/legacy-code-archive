// DLG_LOGIN.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "상벌점.h"
#include "DLG_LOGIN.h"
#include "afxdialogex.h"


BOOL login(DLG_LOGIN* dlg, CString& id, CString& pw);
// DLG_LOGIN 대화 상자입니다.

IMPLEMENT_DYNAMIC(DLG_LOGIN, CDialogEx)

DLG_LOGIN::DLG_LOGIN(CWnd* pParent /*=NULL*/)
	: CDialogEx(DLG_LOGIN::IDD, pParent)
	, LOGIN_OK(false)
	, STR_ID(_T(""))
	, STR_PW(_T(""))
{

}

DLG_LOGIN::~DLG_LOGIN()
{
}

void DLG_LOGIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, STR_ID);
	DDX_Text(pDX, IDC_EDIT4, STR_PW);
	DDV_MaxChars(pDX, STR_ID, 20);
	DDV_MaxChars(pDX, STR_PW, 20);
}


BEGIN_MESSAGE_MAP(DLG_LOGIN, CDialogEx)
	ON_BN_CLICKED(IDOK, &DLG_LOGIN::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DLG_LOGIN::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &DLG_LOGIN::OnBnClickedButton1)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// DLG_LOGIN 메시지 처리기입니다.


void DLG_LOGIN::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void DLG_LOGIN::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void DLG_LOGIN::OnBnClickedButton1()
{
	UpdateData();

	if (STR_ID.IsEmpty() || STR_PW.IsEmpty())
		return;

	if (login(this, STR_ID, STR_PW))
	{
		LOGIN_OK = TRUE;
		CDialogEx::OnOK();
	}
	else
		MessageBox(L"존재하지 않는 아이디 이거나, 패스워드가 틀렸습니다.");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL login(DLG_LOGIN* dlg, CString& id, CString& pw)
{
	char Temp = 7;

	if (send(dlg->sock, &Temp, 1, 0) < 0)
		return FALSE;

	Temp = strlen(CW2A(id));
	if (send(dlg->sock, &Temp, 1, 0) < 0)
		return FALSE;
	if (send(dlg->sock, CW2A(id), Temp, 0) < 0)
		return FALSE;

	Temp = strlen(CW2A(pw));
	if (send(dlg->sock, &Temp, 1, 0) < 0)
		return FALSE;
	if (send(dlg->sock, CW2A(pw), Temp, 0) < 0)
		return FALSE;

	if (recv(dlg->sock, &Temp, 1, 0) < 0)
		return FALSE;

	if (Temp == FALSE)
		return FALSE;

	if (recv(dlg->sock, dlg->LOGIN_NAME, Temp, 0) < 0)
		return FALSE;
	dlg->LOGIN_NAME[Temp] = 0;
	return TRUE;
}

void DLG_LOGIN::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ((nID & 0xFFF0) == SC_CLOSE) {
		CDialogEx::OnCancel();
		return;
	}
	else
		CDialogEx::OnSysCommand(nID, lParam);
}
