
// 상벌점Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "상벌점.h"
#include "상벌점Dlg.h"
#include "afxdialogex.h"
#include "DLG_LOGIN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

BOOL Initialization();
unsigned WINAPI thread_main(void* arg);
BOOL connect_server();
C상벌점Dlg* cdlg = NULL;
BOOL get_memolist();

int C_Year = 0;
int C_Month = 0;

SOCKET sock = NULL;
HANDLE hThread = NULL;
BOOL WSA_SETUP = FALSE;
WSADATA wsaData;
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// C상벌점Dlg 대화 상자



C상벌점Dlg::C상벌점Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C상벌점Dlg::IDD, pParent)
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

void C상벌점Dlg::DoDataExchange(CDataExchange* pDX)
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

BEGIN_MESSAGE_MAP(C상벌점Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &C상벌점Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &C상벌점Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &C상벌점Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C상벌점Dlg::OnBnClickedButton2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &C상벌점Dlg::OnLvnItemchangedList3)
	ON_BN_CLICKED(IDC_BUTTON3, &C상벌점Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &C상벌점Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C상벌점Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C상벌점Dlg::OnBnClickedButton6)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR2, &C상벌점Dlg::OnMcnSelchangeMonthcalendar2)
	ON_BN_CLICKED(IDC_BUTTON7, &C상벌점Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C상벌점Dlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// C상벌점Dlg 메시지 처리기

BOOL C상벌점Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	cdlg = this;

	Initialization();

	RADIO_FIND.SetCheck(TRUE);
	RADIO_A.SetCheck(TRUE);

	LIST_CRT.GetHeaderCtrl()->EnableWindow(false);
	//LIST_CRT.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);
	// 리스트 초기화 
	LIST_CRT.DeleteAllItems();

	// 리스트 스타일 설정
	LIST_CRT.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// 타이틀 삽입
	LIST_CRT.InsertColumn(0, _T("학번"), LVCFMT_LEFT, 50, -1);
	LIST_CRT.InsertColumn(1, _T("이름"), LVCFMT_CENTER, 80, -1);
	LIST_CRT.InsertColumn(2, _T("누적 상벌점"), LVCFMT_CENTER, 70, -1);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void C상벌점Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void C상벌점Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR C상벌점Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C상벌점Dlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void C상벌점Dlg::OnBnClickedCancel()
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

	SetWindowText(cdlg->m_hWnd, _T("통합 관리 (연결됨)"));

	if (hThread == NULL)
		hThread = (HANDLE)_beginthreadex(NULL, 0, thread_main, (void*)&sock, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	//closesocket(sock);
	//WSACleanup();
}

unsigned WINAPI thread_main(void* arg)   // send thread main
{
	SOCKET hSock = *((SOCKET*)arg);
	//MessageBox(NULL, L"메인", 0, 0);

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
	cdlg->SetWindowText(_T("통합 관리"));

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


void C상벌점Dlg::OnBnClickedButton1()
{
	if (sock)
		return;
	cdlg->IP_ADR.UpdateData();
	if (connect_server() == FALSE)
		return;

	SetWindowText(_T("통합 관리 (연결됨)"));

	if (hThread == NULL)
		hThread = (HANDLE)_beginthreadex(NULL, 0, thread_main, (void*)&sock, 0, NULL);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

void C상벌점Dlg::OnBnClickedButton2()
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
		MessageBox(L"학생을 찾을 수 없습니다.");
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void C상벌점Dlg::OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CString str;
	int index = 0;
	index = pNMLV->iItem;
	if (index >= 0 && index < LIST_CRT.GetItemCount())
	{
		str.Format(L"%s(%s) - 상벌점:%s", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), LIST_CRT.GetItemText(index, 2));
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

void C상벌점Dlg::OnBnClickedButton3()
{
	UpdateData();
	int index = GetIndex();
	char temp = 2;
	int num;
	CString str;

	if (index == -1)
		return;

	str.Format(L"%s(%s)학생의 점수를 %d점 추가합니다.\n(추가 후 점수 : %d)", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), SET_SCORE, _wtoi(LIST_CRT.GetItemText(index, 2)) + SET_SCORE);
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
	str.Format(L"%s(%s) - 상벌점:%d", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), num);
	SELECT_.SetWindowText(str);
	UpdateData(FALSE);
}


void C상벌점Dlg::OnBnClickedButton4()
{
	UpdateData();
	int index = GetIndex();
	char temp = 2;
	int num;
	CString str;

	if (index == -1)
		return;

	str.Format(L"%s(%s)학생의 점수를 %d점 삭감합니다.\n(삭감 후 점수 : %d)", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), SET_SCORE, _wtoi(LIST_CRT.GetItemText(index, 2)) - SET_SCORE);
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
	str.Format(L"%s(%s) - 상벌점:%d", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), num);
	SELECT_.SetWindowText(str);
	UpdateData(FALSE);
}


void C상벌점Dlg::OnBnClickedButton5()
{
	UpdateData();
	int index = GetIndex();
	char temp = 2;
	int num;
	CString str;

	if (index == -1)
		return;

	str.Format(L"%s(%s)학생의 점수를 %d점으로 고정합니다.", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), SET_SCORE);
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
	str.Format(L"%s(%s) - 상벌점:%d", LIST_CRT.GetItemText(index, 1), LIST_CRT.GetItemText(index, 0), num);
	SELECT_.SetWindowText(str);
	UpdateData(FALSE);
}


void C상벌점Dlg::OnBnClickedButton6()
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
			MessageBox(L"학생을 찾을 수 없습니다.");
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


void C상벌점Dlg::OnMcnSelchangeMonthcalendar2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	//::_stprintf_s(tszSelectDate, MAX_PATH + 1, _T("%04d년 %02d월 %02d일"), select.GetYear(), select.GetMonth(), select.GetDay());
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


void C상벌점Dlg::OnBnClickedButton7()
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

void C상벌점Dlg::OnBnClickedButton8()
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
			MessageBox(L"학생을 찾을 수 없습니다.");
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
