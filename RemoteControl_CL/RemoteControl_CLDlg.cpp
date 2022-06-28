
// RemoteControl_CLDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "RemoteControl_CL.h"
#include "RemoteControl_CLDlg.h"
#include "afxdialogex.h"
#include "RemoteView.h"
#include <winsock2.h>


#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#pragma warning(disable : 4995)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
WSADATA wsaData;
int scc;

void r_connect(LPVOID lParam);

char* StringToChar(CString str)
{
	char* szStr = NULL;
#if defined(UNICODE) || defined(_UNICODE)
	int nLen = str.GetLength() + 1;
	TCHAR* tszTemp = NULL;
	tszTemp = new TCHAR[nLen];
	memset(tszTemp, 0x00, nLen * sizeof(TCHAR));
	_tcscpy(tszTemp, str);
	// Get size (실제사용되는바이트사이즈)
	int nSize = WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nSize];
	memset(szStr, 0x00, nSize);
	WideCharToMultiByte(CP_ACP, 0, tszTemp, -1, szStr, nSize, NULL, NULL);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
#else
	int nLen = str.GetLength() + 1;
	szStr = new char[nLen];
	memset(szStr, 0x00, nLen);
	strcpy(szStr, str);
#endif
	return szStr;
}

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


// CRemoteControl_CLDlg 대화 상자



CRemoteControl_CLDlg::CRemoteControl_CLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteControl_CLDlg::IDD, pParent)
	, password(_T(""))
	, port(0)
	, rad(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteControl_CLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, password);
	DDV_MaxChars(pDX, password, 15);
	DDX_Text(pDX, IDC_EDIT1, port);
	DDX_Control(pDX, IDC_IPADDRESS1, ip);
	DDX_Radio(pDX, IDC_RADIO1, rad);
}

BEGIN_MESSAGE_MAP(CRemoteControl_CLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRemoteControl_CLDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRemoteControl_CLDlg::OnBnClickedCancel)
	//	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_EDIT1, &CRemoteControl_CLDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CRemoteControl_CLDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CRemoteControl_CLDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CRemoteControl_CLDlg 메시지 처리기

BOOL CRemoteControl_CLDlg::OnInitDialog()
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
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(_T("WSA에러"), _T("에러"), MB_OK);
		return 0;
	}

	ip.SetWindowText(_T("127.0.0.1"));
	port = 1597;

	UpdateData(FALSE);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRemoteControl_CLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_CLOSE) {
		EndDialog(IDCANCEL);
		exit(0);
		//WSACleanup();
		return;
	}

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
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

void CRemoteControl_CLDlg::OnPaint()
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
HCURSOR CRemoteControl_CLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteControl_CLDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CRemoteControl_CLDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}




void CRemoteControl_CLDlg::OnEnChangeEdit1()
{
	UpdateData(TRUE);
	if (port > 65535)
		port = 65535;
	if (port == 0)
		port = 1;
	UpdateData(FALSE);
}


void CRemoteControl_CLDlg::OnBnClickedButton1()
{
	if (password == "")
	{
		MessageBox(_T("비밀번호를 설정하세요!"), _T("에러"), MB_OK);
		return;
	}

	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	r_connect(this);
}

void r_connect(LPVOID lParam)
{
	CRemoteControl_CLDlg* dlg = (CRemoteControl_CLDlg*)lParam; //호출한클래스의포인터객체생성
	CString strIP;
	dlg->ip.GetWindowText(strIP);
	int pass_check = 0;
	typedef struct screenxy
	{
		int x;
		int y;
	}screen_xy;
	screen_xy scr_xy;

	struct sockaddr_in serv_adr;
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("소켓에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		return;
	}

	SOCKET sock2 = socket(PF_INET, SOCK_STREAM, 0);
	if (sock2 == INVALID_SOCKET) {
		closesocket(sock2);
		MessageBox(dlg->m_hWnd, _T("소켓에러(2)"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		return;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(StringToChar(strIP));
	serv_adr.sin_port = htons(dlg->port);

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("커넥트에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		return;
	}

	scc = send(sock, (char*)dlg->password.GetBuffer(dlg->password.GetLength() * 2), dlg->password.GetLength() * 2, 0);
	if (scc < 0)
	{
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다"), _T("통신에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		return;
	}
	scc = recv(sock, (char*)&pass_check, sizeof(int), 0);
	if (scc < 0)
	{
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다"), _T("통신에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		return;
	}
	if (pass_check == 1)
	{
		if (connect(sock2, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
			closesocket(sock2);
			MessageBox(dlg->m_hWnd, _T("커넥트에러(2)"), _T("에러"), MB_OK);
			dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
			return;
		}

		scc = recv(sock, (char*)&scr_xy, sizeof(scr_xy), 0);
		if (scc < 0)
		{
			closesocket(sock);
			MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다"), _T("통신에러"), MB_OK);
			dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
			return;
		}

		dlg->UpdateData(TRUE);
		RemoteView view;
		view.sock = sock;
		view.sock2 = sock2;
		view.aspectx = scr_xy.x;
		view.aspecty = scr_xy.y;
		view.min_x = 500;
		view.QR = dlg->rad;
		view.port = dlg->port;
		view.DoModal();


		UnhookWindowsHookEx(view.hook);
		closesocket(sock);
		closesocket(sock2);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	}
	else
	{
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("비밀번호가 맞지 않습니다!"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		return;
	}

	return;
}


void CRemoteControl_CLDlg::OnEnChangeEdit2()
{
	UpdateData(TRUE);
}