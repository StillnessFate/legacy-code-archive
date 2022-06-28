// DLG_LOGIN.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "�����.h"
#include "DLG_LOGIN.h"
#include "afxdialogex.h"


BOOL login(DLG_LOGIN* dlg, CString& id, CString& pw);
// DLG_LOGIN ��ȭ �����Դϴ�.

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


// DLG_LOGIN �޽��� ó�����Դϴ�.


void DLG_LOGIN::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void DLG_LOGIN::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		MessageBox(L"�������� �ʴ� ���̵� �̰ų�, �н����尡 Ʋ�Ƚ��ϴ�.");
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if ((nID & 0xFFF0) == SC_CLOSE) {
		CDialogEx::OnCancel();
		return;
	}
	else
		CDialogEx::OnSysCommand(nID, lParam);
}
