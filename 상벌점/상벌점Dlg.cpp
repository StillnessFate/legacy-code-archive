
// �����Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "�����.h"
#include "�����Dlg.h"
#include "afxdialogex.h"
#include "DLG_LOGIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

BOOL Initialization();
unsigned WINAPI thread_main(void* arg);
BOOL connect_server();
C�����Dlg* cdlg = NULL;
BOOL get_memolist();

int C_Year = 0;
int C_Month = 0;

SOCKET sock = NULL;
HANDLE hThread = NULL;
BOOL WSA_SETUP = FALSE;
WSADATA wsaData;
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C�����Dlg ��ȭ ����



C�����Dlg::C�����Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C�����Dlg::IDD, pParent)
	, StudentNum(0)
	, StudentName(_T(""))
	, SET_SCORE(0)
	, RADIO_ABC(0)
	, ABC_NUM(0)
	, MEMO_TEXT(_T(""))
	, LOGIN_NAME(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C�����Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, IP_ADR);
	DDX_Control(pDX, IDC_LIST3, LIST_CRT);
	DDX_Control(pDX, IDC_RADIO1, RADIO_FIND);
	DDX_Text(pDX, IDC_EDIT2, StudentNum);
	DDX_Text(pDX, IDC_EDIT3, StudentName);
	DDX_Control(pDX, IDC_EDIT1, SELECT_);
	DDX_Text(pDX, IDC_EDIT4, SET_SCORE);
	DDX_Control(pDX, IDC_RADIO3, RADIO_A);
	DDX_Radio(pDX, IDC_RADIO3, RADIO_ABC);
	DDX_Text(pDX, IDC_EDIT5, ABC_NUM);
	DDX_Control(pDX, IDC_MONTHCALENDAR2, CAL_CTR);
	DDX_Control(pDX, IDC_LIST1, LIST_BOX);
	DDX_Text(pDX, IDC_EDIT6, MEMO_TEXT);
	DDV_MaxChars(pDX, MEMO_TEXT, 500);
	DDX_Text(pDX, IDC_EDIT7, LOGIN_NAME);
}

BEGIN_MESSAGE_MAP(C�����Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &C�����Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &C�����Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &C�����Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C�����Dlg::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &C�����Dlg::OnLvnItemchangedList3)
	ON_BN_CLICKED(IDC_BUTTON3, &C�����Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &C�����Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C�����Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C�����Dlg::OnBnClickedButton6)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR2, &C�����Dlg::OnMcnSelchangeMonthcalendar2)
	ON_BN_CLICKED(IDC_BUTTON7, &C�����Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C�����Dlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// C�����Dlg �޽��� ó����

BOOL C�����Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	cdlg = this;

	Initialization();

	RADIO_FIND.SetCheck(TRUE);
	RADIO_A.SetCheck(TRUE);

	LIST_CRT.GetHeaderCtrl()->EnableWindow(false);
	//LIST_CRT.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	// ����Ʈ �ʱ�ȭ 
	LIST_CRT.DeleteAllItems();

	// ����Ʈ ��Ÿ�� ����
	LIST_CRT.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// Ÿ��Ʋ ����
	LIST_CRT.InsertColumn(0, _T("�й�"), LVCFMT_LEFT, 50, -1);
	LIST_CRT.InsertColumn(1, _T("�̸�"), LVCFMT_CENTER, 80, -1);
	LIST_CRT.InsertColumn(2, _T("���� �����"), LVCFMT_CENTER, 70, -1);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void C�����Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_CLOSE) {
		CDialogEx::OnCancel();
		return;
	}
	else if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void C�����Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR C�����Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C�����Dlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void C�����Dlg::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}

BOOL Initialization()
{
	char buf[256];
	FILE* fp;

	fp = fopen("setting.txt", "r");
	if (fp == NULL)
	{
		cdlg->IP_ADR.SetWindowText(L"127.0.0.1");
		return FALSE;
	}
	fgets(buf, sizeof(buf), fp);
	cdlg->IP_ADR.SetWindowText(CA2W(buf));
	fclose(fp);

	if (connect_server() == FALSE)
		return FALSE;

	SetWindowText(cdlg->m_hWnd, _T("���� ���� (�����)"));

	if (hThread == NULL)
		hThread = (HANDLE)_beginthreadex(NULL, 0, thread_main, (void*)&sock, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	//closesocket(sock);
	//WSACleanup();
}

unsigned WINAPI thread_main(void* arg)   // send thread main
{
	SOCKET hSock = *((SOCKET*)arg);
	//MessageBox(NULL, L"����", 0, 0);

	/*
	char nameMsg[NAME_SIZE + BUF_SIZE];
	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSock);
			exit(0);
		}
		sprintf(nameMsg, "%s %s", name, msg);
		send(hSock, nameMsg, strlen(nameMsg), 0);
	}*/
	return 0;
}

BOOL disconnect_server()
{
	closesocket(sock);
	sock = NULL;
	cdlg->SetWindowText(_T("���� ����"));

	return TRUE;
}

BOOL connect_server()
{
	SOCKADDR_IN servAdr;
	CString strIP;

	if (WSA_SETUP == NULL)
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			MessageBox(NULL, L"WSAStartup() error!", 0, 0);
			return FALSE;
		}
		else
			WSA_SETUP = TRUE;
	sock = socket(PF_INET, SOCK_STREAM, 0);

	cdlg->IP_ADR.GetWindowText(strIP);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(CW2A(strIP));
	servAdr.sin_port = htons(3388);

	if (connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, L"connect() error", 0, 0);
		sock = NULL;
		return FALSE;
	}

	FILE* fp;
	fp = fopen("setting.txt", "w");
	if (fp)
	{
		fprintf(fp, "%s", CStringA(strIP));
		fclose(fp);
	}


	DLG_LOGIN login_dlg;
	login_dlg.sock = sock;
	login_dlg.LOGIN_OK = FALSE;

	if (login_dlg.DoModal() == IDOK)
	{
		cdlg->LOGIN_NAME = login_dlg.LOGIN_NAME;
		cdlg->UpdateData(FALSE);
	}
	else
	{
		disconnect_server();
		return FALSE;
	}


	get_memolist();

	return TRUE;
}


void C�����Dlg::OnBnClickedButton1()
{
	if (sock)
		return;
	cdlg->IP_ADR.UpdateData();
	if (connect_server() == FALSE)
		return;

	SetWindowText(_T("���� ���� (�����)"));

	if (hThread == NULL)
		hThread = (HANDLE)_beginthreadex(NULL, 0, thread_main, (void*)&sock, 0, NULL);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

BOOL add_list(int num, char* name, int score, BOOL reset)
{
	CString str;
	if (reset)
		cdlg->LIST_CRT.DeleteAllItems();
	int count = cdlg->LIST_CRT.GetItemCount();
	str.Format(L"%d", num);
	cdlg->LIST_CRT.InsertItem(count, str);
	cdlg->LIST_CRT.SetItem(count, 1, LVIF_TEXT, CA2W(name), 0, 0, 0, NULL);
	str.Format(L"%d", score);
	cdlg->LIST_CRT.SetItem(count, 2, LVIF_TEXT, str, 0, 0, 0, NULL);

	return TRUE;
}

void C�����Dlg::OnBnClickedButton2()
{
	char Temp = 0;
	int num = 0;
	char name[10] = { 0 };
	int score = 0;

	if (sock == NULL)
		return;

	cdlg->UpdateData();

	if (cdlg->RADIO_FIND.GetCheck())
	{
		Temp = 0;
		if (send(sock, &Temp, 1, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (send(sock, (char*)&StudentNum, 4, 0) < 0)
		{
			disconnect_server();
			return;
		}
	}
	else
	{
		if (!wcscmp(StudentName, L""))
			return;
		Temp = 1;
		if (send(sock, &Temp, 1, 0) < 0)
		{
			disconnect_server();
			return;
		}
		Temp = strlen(CW2A(StudentName));
		if (send(sock, &Temp, 1, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (send(sock, CW2A(StudentName), Temp, 0) < 0)
		{
			disconnect_server();
			return;
		}
	}

	if (recv(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (num == -1)
	{
		MessageBox(L"�л��� ã�� �� �����ϴ�.");
		cdlg->SELECT_.SetWindowText(L"");
		cdlg->LIST_CRT.DeleteAllItems();
		return;
	}
	if (recv(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (recv(sock, name, Temp, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (recv(sock, (char*)&score, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}

	add_list(num, name, score, TRUE);
	cdlg->UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void C�����Dlg::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CString str;
	int index = 0;
	index = pNMLV->iItem;
	if (index >= 0 && index < LIST_CRT.GetItemCount())
	{
		str.Format(L"%s(%s) - �����:%s", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), LIST_CRT.GetItemText(index, 2));
		SELECT_.SetWindowText(str);
	}

	*pResult = 0;
}

int GetIndex()
{
	int nSelectedItem = cdlg->LIST_CRT.GetNextItem(-1, LVNI_SELECTED);
	CString strKey = cdlg->LIST_CRT.GetItemText(nSelectedItem, 0);
	if (strKey == "")
	{
		return -1;
	}
	return nSelectedItem;
}

void C�����Dlg::OnBnClickedButton3()
{
	UpdateData();
	int index = GetIndex();
	char temp = 2;
	int num;
	CString str;

	if (index == -1)
		return;

	str.Format(L"%s(%s)�л��� ������ %d�� �߰��մϴ�.\n(�߰� �� ���� : %d)", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), SET_SCORE, _wtoi(LIST_CRT.GetItemText(index, 2)) + SET_SCORE);
	if (MessageBox(str, 0, MB_OKCANCEL) == IDCANCEL)
		return;

	if (send(sock, &temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	num = _wtoi(LIST_CRT.GetItemText(index, 0));
	if (send(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	num = _wtoi(LIST_CRT.GetItemText(index, 2)) + SET_SCORE;
	if (send(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	str.Format(L"%d", num);
	LIST_CRT.SetItemText(index, 2, str);
	str.Format(L"%s(%s) - �����:%d", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), num);
	SELECT_.SetWindowText(str);
	UpdateData(FALSE);
}


void C�����Dlg::OnBnClickedButton4()
{
	UpdateData();
	int index = GetIndex();
	char temp = 2;
	int num;
	CString str;

	if (index == -1)
		return;

	str.Format(L"%s(%s)�л��� ������ %d�� �谨�մϴ�.\n(�谨 �� ���� : %d)", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), SET_SCORE, _wtoi(LIST_CRT.GetItemText(index, 2)) - SET_SCORE);
	if (MessageBox(str, 0, MB_OKCANCEL) == IDCANCEL)
		return;

	if (send(sock, &temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	num = _wtoi(LIST_CRT.GetItemText(index, 0));
	if (send(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	num = _wtoi(LIST_CRT.GetItemText(index, 2)) - SET_SCORE;
	if (send(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	str.Format(L"%d", num);
	LIST_CRT.SetItemText(index, 2, str);
	str.Format(L"%s(%s) - �����:%d", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), num);
	SELECT_.SetWindowText(str);
	UpdateData(FALSE);
}


void C�����Dlg::OnBnClickedButton5()
{
	UpdateData();
	int index = GetIndex();
	char temp = 2;
	int num;
	CString str;

	if (index == -1)
		return;

	str.Format(L"%s(%s)�л��� ������ %d������ �����մϴ�.", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), SET_SCORE);
	if (MessageBox(str, 0, MB_OKCANCEL) == IDCANCEL)
		return;

	if (send(sock, &temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	num = _wtoi(LIST_CRT.GetItemText(index, 0));
	if (send(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	num = SET_SCORE;
	if (send(sock, (char*)&num, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	str.Format(L"%d", num);
	LIST_CRT.SetItemText(index, 2, str);
	str.Format(L"%s(%s) - �����:%d", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), num);
	SELECT_.SetWindowText(str);
	UpdateData(FALSE);
}


void C�����Dlg::OnBnClickedButton6()
{
	char Temp = 0;
	int num = 0;
	char name[10] = { 0 };
	int score = 0;

	if (sock == NULL)
		return;

	UpdateData();

	Temp = 3;
	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	Temp = RADIO_ABC;
	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&ABC_NUM, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}

	LIST_CRT.DeleteAllItems();
	while (true)
	{
		if (recv(sock, (char*)&num, 4, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (num == -1)
		{
			MessageBox(L"�л��� ã�� �� �����ϴ�.");
			SELECT_.SetWindowText(L"");
			LIST_CRT.DeleteAllItems();
			return;
		}
		else if (num == -2)
			break;;

		if (recv(sock, &Temp, 1, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (recv(sock, name, Temp, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (recv(sock, (char*)&score, 4, 0) < 0)
		{
			disconnect_server();
			return;
		}

		add_list(num, name, score, FALSE);
	}

	UpdateData(FALSE);
}


void C�����Dlg::OnMcnSelchangeMonthcalendar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	get_memolist();

	int Year = 0;
	int Month = 0;
	int Day = 0;
	char Temp = 4;
	int len = 0;
	char buff[1024] = { 0 };

	CTime select;
	/////////////////////////////////////////////////////////////////////////////////
	// Step1.
	UpdateData(TRUE);

	/////////////////////////////////////////////////////////////////////////////////
	// Step2.
	CAL_CTR.GetCurSel(select);
	Year = select.GetYear();
	Month = select.GetMonth();
	Day = select.GetDay();
	//WCHAR tszSelectDate[300];
	//::_stprintf_s(tszSelectDate, MAX_PATH + 1, _T("%04d�� %02d�� %02d��"), select.GetYear(), select.GetMonth(), select.GetDay());
	//MessageBox(tszSelectDate);

	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&Year, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&Month, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&Day, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}


	if (recv(sock, (char*)&len, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (len == 0)
	{
		MEMO_TEXT = "";
		UpdateData(FALSE);
		return;
	}

	if (recv(sock, buff, len, 0) < 0)
	{
		disconnect_server();
		return;
	}
	MEMO_TEXT = buff;
	/////////////////////////////////////////////////////////////////////////////////
	// Step3.
	UpdateData(FALSE);

	*pResult = 0;
}


void C�����Dlg::OnBnClickedButton7()
{
	int Year = 0;
	int Month = 0;
	int Day = 0;
	char Temp = 6;
	int len = 0;
	char buff[1024] = { 0 };

	CTime select;
	UpdateData(TRUE);

	CAL_CTR.GetCurSel(select);
	Year = select.GetYear();
	Month = select.GetMonth();
	Day = select.GetDay();

	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&Year, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&Month, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	if (send(sock, (char*)&Day, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}
	len = strlen(CW2A(MEMO_TEXT));
	if (send(sock, (char*)&len, 4, 0) < 0)
	{
		disconnect_server();
		return;
	}

	if (send(sock, CW2A(MEMO_TEXT), len, 0) < 0)
	{
		disconnect_server();
		return;
	}

	CString str;
	str.Format(L"%d", Day);
	int index = LIST_BOX.FindString(-1, str);

	if (index != LB_ERR && len == 0)
		LIST_BOX.DeleteString(index);
	else if (len != 0 && index == LB_ERR)
		LIST_BOX.AddString(str);
	UpdateData(FALSE);
	//get_memolist();
}


BOOL get_memolist()
{

	int Year = 0;
	int Month = 0;
	char Temp = 5;

	CTime select;
	CString str;

	cdlg->CAL_CTR.GetCurSel(select);
	Year = select.GetYear();
	Month = select.GetMonth();
	if (Year == C_Year && Month == C_Month)
		return FALSE;
	C_Year = Year;
	C_Month = Month;
	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return FALSE;
	}
	if (send(sock, (char*)&Year, 4, 0) < 0)
	{
		disconnect_server();
		return FALSE;
	}
	if (send(sock, (char*)&Month, 4, 0) < 0)
	{
		disconnect_server();
		return FALSE;
	}
	cdlg->LIST_BOX.ResetContent();
	while (true)
	{
		if (recv(sock, &Temp, 1, 0) < 0)
		{
			disconnect_server();
			return FALSE;
		}
		if (Temp == 0)
			break;
		str.Format(L"%d", Temp);
		cdlg->LIST_BOX.AddString(str);
	}
	cdlg->UpdateData(FALSE);
	return TRUE;
}

void C�����Dlg::OnBnClickedButton8()
{
	char Temp = 0;
	int num = 0;
	char name[10] = { 0 };
	int score = 0;

	if (sock == NULL)
		return;

	UpdateData();

	Temp = 3;
	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}

	if (send(sock, &Temp, 1, 0) < 0)
	{
		disconnect_server();
		return;
	}

	LIST_CRT.DeleteAllItems();
	while (true)
	{
		if (recv(sock, (char*)&num, 4, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (num == -1)
		{
			MessageBox(L"�л��� ã�� �� �����ϴ�.");
			SELECT_.SetWindowText(L"");
			LIST_CRT.DeleteAllItems();
			return;
		}
		else if (num == -2)
			break;;

		if (recv(sock, &Temp, 1, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (recv(sock, name, Temp, 0) < 0)
		{
			disconnect_server();
			return;
		}
		if (recv(sock, (char*)&score, 4, 0) < 0)
		{
			disconnect_server();
			return;
		}

		add_list(num, name, score, FALSE);
	}

	UpdateData(FALSE);
}
