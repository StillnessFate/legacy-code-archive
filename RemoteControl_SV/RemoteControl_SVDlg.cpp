
// RemoteControl_SVDlg.cpp : 구현 파일
//


#include "stdafx.h"
#include "RemoteControl_SV.h"
#include "RemoteControl_SVDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>
#include <dwmapi.h>
#include <mmsystem.h>
#include "Dsound.h"

#pragma comment(lib, "Dsound.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#pragma warning(disable : 4995)
#pragma comment(lib, "dwmapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <imm.h>
#pragma comment(lib, "imm32.lib")

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
BOOL fin = 1;
int SVC = 0;
int join = 0;
int ec = 0;
WSADATA wsaData;

int D_len = 0;
BOOL D_com = 0;
char* D_m_pBuffer = 0;
char* D_pBuffer = 0;

SOCKET* D_clnt_sock;
SOCKET* D_serv_sock;

char U_ip[20] = { 0, };
WAVEFORMATEX  my_wave_format;
int Buffer_L = 22050;	//2205
int SPS = 44100;	//11025
int BPS = 16;	//8
int Chan = 2;	//1

UINT ServerThread(LPVOID lParam);
UINT D_ServerThread(LPVOID lParam);
UINT ControlThread(LPVOID lParam);
UINT SoundThread(LPVOID lParam);
UINT End(LPVOID lParam);
CWinThread* End1;
CWinThread* End2;
CWinThread* End3;
CWinThread* End4;
CWinThread* End0 = NULL;

DWORD nExitCode1 = NULL;
DWORD nExitCode2 = NULL;
DWORD nExitCode3 = NULL;
DWORD nExitCode4 = NULL;

void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
BOOL CALLBACK DSEnumCallbackAll(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext);
void record();
SOCKET sock;
CRemoteControl_SVDlg* cdlg = NULL;

WAVEHDR* mp_wave_header;
WAVEHDR* mp_wave_header_2;
HWAVEIN waveInDevice;
HANDLE mh_wave_data, mh_wave_header;
HANDLE mh_wave_data_2, mh_wave_header_2;

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
CString GetInIpAddress()
{
	CString strIP; // 이 변수에 IP주소가 저장된다.
	strIP = "";

	HOSTENT* p_host_info;
	IN_ADDR in;
	char host_name[128] = { 0, };


	gethostname(host_name, 128);
	p_host_info = gethostbyname(host_name);

	if (p_host_info != NULL)
	{
		for (int i = 0; p_host_info->h_addr_list[i]; i++)
		{
			memcpy(&in, p_host_info->h_addr_list[i], 4);
			strIP = inet_ntoa(in);
		}
	}
	return strIP;
}

// 연결된 소켓으로 IP 주소 가져오기.. 
void get_peer_ip(SOCKET skt, char* ip)
{
	SOCKADDR_IN sock_addr;
	char* p;
	int soklen;

	soklen = sizeof(sock_addr);

	getpeername(skt, (struct sockaddr*)&sock_addr, &soklen);

	p = (char*)inet_ntoa((struct in_addr)sock_addr.sin_addr);
	strcpy(ip, p);
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


// CRemoteControl_SVDlg 대화 상자


CRemoteControl_SVDlg::CRemoteControl_SVDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteControl_SVDlg::IDD, pParent)
	, state(_T(""))
	, port(0)
	, password(_T(""))
	, c_state(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteControl_SVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, state);
	DDX_Text(pDX, IDC_EDIT1, port);
	DDX_Text(pDX, IDC_EDIT2, password);
	DDV_MaxChars(pDX, password, 15);
	//  DDX_Control(pDX, IDC_IPADDRESS1, ip);
	DDX_Text(pDX, IDC_EDIT4, c_state);
	//  DDX_Text(pDX, IDC_EDIT5, aaa);
	DDX_Control(pDX, IDC_IPADDRESS1, in_ip);
	//  DDX_Control(pDX, IDC_IPADDRESS2, ip_out);
	//  DDX_Control(pDX, IDC_IPADDRESS2, ip_out);
	DDX_Control(pDX, IDC_COMBO1, combo);
}

BEGIN_MESSAGE_MAP(CRemoteControl_SVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRemoteControl_SVDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRemoteControl_SVDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &CRemoteControl_SVDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CRemoteControl_SVDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CRemoteControl_SVDlg::OnEnChangeEdit2)
	//	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(IDC_BUTTON2, &CRemoteControl_SVDlg::OnBnClickedButton2)
	//ON_WM_KEYDOWN()
	//ON_WM_RBUTTONUP()
	//ON_WM_RBUTTONDOWN()
	//ON_WM_RBUTTONDBLCLK()
	//ON_BN_CLICKED(IDC_BUTTON3, &CRemoteControl_SVDlg::OnBnClickedButton3)
	//ON_BN_CLICKED(IDC_BUTTON2, &CRemoteControl_SVDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CRemoteControl_SVDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CRemoteControl_SVDlg 메시지 처리기

BOOL CRemoteControl_SVDlg::OnInitDialog()
{
	HANDLE m_hMutex = ::CreateMutex(FALSE, 0, L"RemoteControl_SV_AntiOverlap");
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(_T("이미 프로그램이 실행 중입니다!"), _T("에러"), MB_OK);
		exit(0);
	}

	cdlg = this;
	CDialogEx::OnInitDialog();

	CoInitialize(NULL);
	DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)DSEnumCallbackAll, NULL);
	CoUninitialize();
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

	in_ip.SetWindowText(GetInIpAddress());
	state = "꺼짐";
	c_state = "원격제어 연결안됨";
	port = 1597;

	UpdateData(FALSE);

	my_wave_format.wFormatTag = WAVE_FORMAT_PCM;
	my_wave_format.nChannels = Chan;                      // 마이크 녹음방식 : 모노
	my_wave_format.nSamplesPerSec = SPS;    // 샘플링 주기 : 11.025 KHz
	my_wave_format.wBitsPerSample = BPS;             // 샘플링 단위 : 8 Bits
	my_wave_format.nBlockAlign = my_wave_format.nChannels * (my_wave_format.wBitsPerSample / 8);
	my_wave_format.nAvgBytesPerSec = my_wave_format.nSamplesPerSec * my_wave_format.nBlockAlign;
	my_wave_format.cbSize = 0;                            // WAVEFORMATEX 구조체 정보외에 추가적인 정보가 없다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



BOOL CALLBACK DSEnumCallbackAll(LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext)
{
	cdlg->combo.AddString((LPCTSTR)lpcstrDescription);
	return true;
}


void CRemoteControl_SVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_CLOSE) {
		SVC = 0;
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

void CRemoteControl_SVDlg::OnPaint()
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
HCURSOR CRemoteControl_SVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteControl_SVDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CRemoteControl_SVDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void CRemoteControl_SVDlg::OnEnChangeEdit1()
{
	UpdateData(TRUE);
	if (port > 65535)
		port = 65535;
	if (port == 0)
		port = 1;
	UpdateData(FALSE);
}


void CRemoteControl_SVDlg::OnBnClickedButton1()
{
	if (SVC == 0)
	{
		if (fin)
		{
			if (password == "")
			{
				MessageBox(_T("비밀번호를 설정하세요!"), _T("에러"), MB_OK);
				return;
			}
			if (combo.GetCurSel() == -1)
			{
				MessageBox(_T("녹음장치를 선택하세요!"), _T("에러"), MB_OK);
				return;
			}
			join = 0;
			D_com = 0;
			ZeroMemory(U_ip, sizeof(U_ip));
			GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
			End1 = AfxBeginThread(ServerThread, this);
			GetExitCodeThread(End1->m_hThread, &nExitCode1);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		}
	}
	else
	{
		if (join == 0)
		{
			SVC = 0;
			struct sockaddr_in serv_addr;
			SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
			memset(&serv_addr, 0, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(port);
			if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
			{
				MessageBox(_T("작동중지 에러"), _T("에러"), MB_OK);
				return;
			}
			closesocket(sock);
		}
		else
		{
			SVC = 0;
		}
	}

	//UpdateData(FALSE);
	return;
}

#define MAX_BUFFER	40960
UINT ServerThread(LPVOID lParam)
{
	CRemoteControl_SVDlg* dlg = (CRemoteControl_SVDlg*)lParam; //호출한클래스의포인터객체생성
	SVC = 1;
	wchar_t pass_check[32];
	int pass_check_num = 0;
	int i = 0;
	typedef struct screenxy
	{
		int x;
		int y;
	}screen_xy;
	screen_xy scr_xy;
	scr_xy.x = GetSystemMetrics(SM_CXSCREEN);
	scr_xy.y = GetSystemMetrics(SM_CYSCREEN);

	struct sockaddr_in serv_adr, clnt_adr;
	SOCKET serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	D_serv_sock = &serv_sock;

	if (serv_sock == INVALID_SOCKET) {
		SVC = 0;
		dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
		dlg->state = "꺼짐";
		dlg->UpdateData(FALSE);
		closesocket(serv_sock);
		MessageBox(dlg->m_hWnd, _T("소켓에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);

		TerminateThread(End2->m_hThread, nExitCode2);
		TerminateThread(End3->m_hThread, nExitCode3);
		TerminateThread(End4->m_hThread, nExitCode4);
		return 0;
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(dlg->port);

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		SVC = 0;
		dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
		dlg->state = "꺼짐";
		dlg->UpdateData(FALSE);
		closesocket(serv_sock);
		MessageBox(dlg->m_hWnd, _T("바인드에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		return 0;
	}

	if (listen(serv_sock, 5) == -1) {
		SVC = 0;
		dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
		dlg->state = "꺼짐";
		dlg->UpdateData(FALSE);
		closesocket(serv_sock);
		MessageBox(dlg->m_hWnd, _T("리슨에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		return 0;
	}

	dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 끄기"));
	dlg->state = "켜짐";
	dlg->UpdateData(FALSE);
	dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

	int clnt_adr_sz = sizeof(clnt_adr);
re_a:
	SOCKET clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	D_clnt_sock = &clnt_sock;

	if (SVC == 0)
	{
		dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
		dlg->state = "꺼짐";
		dlg->UpdateData(FALSE);
		closesocket(serv_sock);
		closesocket(clnt_sock);
		dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		return 0;
	}
	int opt_val = TRUE;
	setsockopt(clnt_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));//Nagle알고리즘!------------------------------------



	memset(pass_check, 0, 32);
	recv(clnt_sock, (char*)pass_check, 32, 0);
	if (_tcscmp(pass_check, dlg->password.GetBuffer(dlg->password.GetLength() * 2)) == 0)
	{
		pass_check_num = 1;
		send(clnt_sock, (char*)&pass_check_num, sizeof(int), 0);
	}
	else
	{
		pass_check_num = 0;
		send(clnt_sock, (char*)&pass_check_num, sizeof(int), 0);
		goto re_a;
	}
	//--------------------------------------------------------------연결완료
	SOCKET clnt_sock2 = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	get_peer_ip(clnt_sock, U_ip);
	setsockopt(clnt_sock2, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));//Nagle알고리즘!------------------------------------

	End2 = AfxBeginThread(ControlThread, &clnt_sock2);
	GetExitCodeThread(End2->m_hThread, &nExitCode2);
	End3 = AfxBeginThread(D_ServerThread, dlg);
	GetExitCodeThread(End3->m_hThread, &nExitCode3);
	End4 = AfxBeginThread(SoundThread, dlg);
	GetExitCodeThread(End4->m_hThread, &nExitCode4);
	End0 = AfxBeginThread(End, dlg);
	fin = 0;

	join = 1;
	dlg->c_state = "원격제어 연결됨";
	dlg->UpdateData(FALSE);
	closesocket(serv_sock);

	send(clnt_sock, (char*)&scr_xy, sizeof(scr_xy), 0);
	ShowWindow(dlg->m_hWnd, SW_SHOWMINIMIZED);
	while (true)
	{
		if (D_com == 0)
		{

			IStream* pStream = NULL;
			CreateStreamOnHGlobal(0, TRUE, &pStream);
			HGLOBAL hGlobal;
			GetHGlobalFromStream(pStream, &hGlobal);

			int len = 0;
			char* m_pBuffer;

			CImage capImage;
			CWnd* pDesktopWnd = dlg->GetDesktopWindow();
			HDC hDC = NULL;
			if (!pDesktopWnd)
				return 0;
			CWindowDC DeskTopDC(pDesktopWnd);
			if (!capImage.Create(scr_xy.x, scr_xy.y, 32))
				return 0;
			hDC = capImage.GetDC();
			BitBlt(hDC, 0, 0, scr_xy.x, scr_xy.y, DeskTopDC.m_hDC, 0, 0, SRCCOPY);
			capImage.Save(pStream, Gdiplus::ImageFormatJPEG);
			capImage.ReleaseDC();
			//----------
			ULARGE_INTEGER liSize;
			IStream_Size(pStream, &liSize);
			len = liSize.LowPart;
			LPVOID pData = GlobalLock(hGlobal);
			char* pBuffer = new char[len + sizeof(int)];
			CopyMemory(&pBuffer[sizeof(int)], pData, len);
			m_pBuffer = pBuffer;
			GlobalUnlock(hGlobal);
			pStream->Release();
			memcpy(m_pBuffer, (char*)&len, sizeof(int));

			D_pBuffer = pBuffer;
			D_m_pBuffer = m_pBuffer;
			D_len = len;

			D_com = 1;
		}
		if (SVC == 0)
		{
			join = 0;
			dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
			dlg->state = "꺼짐";
			dlg->c_state = "원격제어 연결안됨";
			dlg->UpdateData(FALSE);
			closesocket(serv_sock);
			closesocket(clnt_sock);
			closesocket(clnt_sock2);
			dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
			return 0;
		}

	}


	return 0;
}
typedef struct apos
{
	double ax;
	double ay;
}apos;
UINT D_ServerThread(LPVOID lParam)
{
	CRemoteControl_SVDlg* dlg = (CRemoteControl_SVDlg*)lParam; //호출한클래스의포인터객체생성


	int ImageWriteSize = 0, ImageBuffSize = 0;
	SOCKET clnt_sock = *D_clnt_sock;
	SOCKET serv_sock = *D_serv_sock;

	while (true)
	{
		if (D_com)
		{
			char* pBuffer = D_pBuffer;
			char* m_pBuffer = D_m_pBuffer;
			int len = D_len;
			D_com = 0;

			int r_point = 0;
			if (len > MAX_BUFFER)
				ImageBuffSize = MAX_BUFFER;
			else
				ImageBuffSize = len;

			ImageWriteSize = send(clnt_sock, &m_pBuffer[r_point], ImageBuffSize + sizeof(int), 0);
			if (ImageWriteSize < 0) {
				SVC = 0;
				join = 0;
				dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
				dlg->state = "꺼짐";
				dlg->c_state = "원격제어 연결안됨";
				dlg->UpdateData(FALSE);
				closesocket(serv_sock);
				closesocket(clnt_sock);
				dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
				dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
				dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
				delete[] pBuffer;
				MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다"), _T("통신에러"), MB_OK);
				return 0;;
			}

			r_point += ImageWriteSize;
			len -= ImageWriteSize - sizeof(int);
			while (len > 0)
			{
				if (len > MAX_BUFFER)
					ImageBuffSize = MAX_BUFFER;
				else
					ImageBuffSize = len;

				ImageWriteSize = send(clnt_sock, &m_pBuffer[r_point], ImageBuffSize, 0);
				r_point += ImageWriteSize;
				len -= ImageWriteSize;

				if (ImageWriteSize < 0) {
					SVC = 0;
					join = 0;
					dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
					dlg->state = "꺼짐";
					dlg->c_state = "원격제어 연결안됨";
					dlg->UpdateData(FALSE);
					closesocket(serv_sock);
					closesocket(clnt_sock);
					dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
					dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
					dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
					delete[] pBuffer;
					MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다"), _T("통신에러"), MB_OK);
					return 0;;
				}

			}
			delete[] pBuffer;
		}
		if (SVC == 0)
		{
			join = 0;
			dlg->GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("원격제어 켜기"));
			dlg->state = "꺼짐";
			dlg->c_state = "원격제어 연결안됨";
			dlg->UpdateData(FALSE);
			closesocket(serv_sock);
			closesocket(clnt_sock);
			dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
			return 0;
		}
	}

	return 0;
}

UINT ControlThread(LPVOID lParam)
{
	SOCKET sock = *(SOCKET*)lParam; //호출한클래스의포인터객체생성
	char cc = 0;
	apos a_pos;
	UINT nChar;
	short zDelta;

	while (SVC)
	{
		if (recv(sock, &cc, 1, 0) < 0)
		{
			SVC = 0;
			closesocket(sock);
			return 0;
		}

		if (cc == 0)
		{
			if (recv(sock, (char*)&a_pos, sizeof(a_pos), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
				SetCursorPos((int)((double)GetSystemMetrics(SM_CXSCREEN) * a_pos.ax), (int)((double)GetSystemMetrics(SM_CYSCREEN) * a_pos.ay));
			else
			{
				closesocket(sock);
				return 0;
			}

		}

		if (cc == 1)
		{
			if (recv(sock, (char*)&a_pos, sizeof(a_pos), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				SetCursorPos((int)((double)GetSystemMetrics(SM_CXSCREEN) * a_pos.ax), (int)((double)GetSystemMetrics(SM_CYSCREEN) * a_pos.ay));
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (cc == 2)
		{
			if (recv(sock, (char*)&a_pos, sizeof(a_pos), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				SetCursorPos((int)((double)GetSystemMetrics(SM_CXSCREEN) * a_pos.ax), (int)((double)GetSystemMetrics(SM_CYSCREEN) * a_pos.ay));
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (cc == 3)
		{
			if (recv(sock, (char*)&nChar, sizeof(nChar), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				keybd_event(nChar, MapVirtualKey(nChar, 0), 0, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}
		if (cc == 4)
		{
			if (recv(sock, (char*)&nChar, sizeof(nChar), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				keybd_event(nChar, MapVirtualKey(nChar, 0), KEYEVENTF_KEYUP, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (cc == 5)
		{
			if (recv(sock, (char*)&a_pos, sizeof(a_pos), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				SetCursorPos((int)((double)GetSystemMetrics(SM_CXSCREEN) * a_pos.ax), (int)((double)GetSystemMetrics(SM_CYSCREEN) * a_pos.ay));
				mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (cc == 6)
		{
			if (recv(sock, (char*)&a_pos, sizeof(a_pos), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				SetCursorPos((int)((double)GetSystemMetrics(SM_CXSCREEN) * a_pos.ax), (int)((double)GetSystemMetrics(SM_CYSCREEN) * a_pos.ay));
				mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (cc == 7)
		{
			if (SVC)
			{
				keybd_event(VK_HANGEUL, MapVirtualKey(VK_HANGEUL, 0), 0, 0);
				keybd_event(VK_HANGEUL, MapVirtualKey(VK_HANGEUL, 0), KEYEVENTF_KEYUP, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (cc == 9)
		{
			if (recv(sock, (char*)&zDelta, sizeof(zDelta), 0) < 0)
			{
				SVC = 0;
				closesocket(sock);
				return 0;
			}

			if (SVC)
			{
				mouse_event(MOUSEEVENTF_WHEEL, 0, 0, zDelta, 0);
			}
			else
			{
				closesocket(sock);
				return 0;
			}
		}

		if (SVC == 0)
		{
			closesocket(sock);
			return 0;
		}
	}

	closesocket(sock);
	return 0;
}

UINT SoundThread(LPVOID lParam)
{
	SOCKADDR_IN servAdr;
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	//servAdr.sin_addr.s_addr = inet_addr("192.168.0.33");
	servAdr.sin_addr.s_addr = inet_addr(U_ip);
	servAdr.sin_port = htons(cdlg->port);

	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	record();

	return 0;
}

int in_open = 0;

void record()
{
	int ser;

	ser = waveInOpen(&waveInDevice, cdlg->combo.GetCurSel(), &my_wave_format, (DWORD)waveInProc, 0, CALLBACK_FUNCTION);
	if (ser != MMSYSERR_NOERROR)
	{
		MessageBox(cdlg->m_hWnd, _T("In_오픈에러"), _T("에러"), MB_OK);
		in_open = 1;
		return;
	}

	unsigned char* mp_wave_data;
	unsigned char* mp_wave_data_2;

	mh_wave_data = GlobalAlloc(GMEM_MOVEABLE, Buffer_L);
	mh_wave_data_2 = GlobalAlloc(GMEM_MOVEABLE, Buffer_L);
	mp_wave_data = (unsigned char*)GlobalLock(mh_wave_data);
	mp_wave_data_2 = (unsigned char*)GlobalLock(mh_wave_data_2);

	mh_wave_header = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(WAVEHDR));
	mh_wave_header_2 = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(WAVEHDR));
	mp_wave_header = (LPWAVEHDR)GlobalLock(mh_wave_header);
	mp_wave_header_2 = (LPWAVEHDR)GlobalLock(mh_wave_header_2);

	mp_wave_header->lpData = (char*)mp_wave_data;
	mp_wave_header_2->lpData = (char*)mp_wave_data_2;
	mp_wave_header->dwBufferLength = Buffer_L;
	mp_wave_header_2->dwBufferLength = Buffer_L;
	mp_wave_header->dwFlags = 0;
	mp_wave_header_2->dwFlags = 0;
	mp_wave_header->dwLoops = 0;
	mp_wave_header_2->dwLoops = 0;

	waveInPrepareHeader(waveInDevice, mp_wave_header, sizeof(WAVEHDR));
	waveInAddBuffer(waveInDevice, mp_wave_header, sizeof(WAVEHDR));
	waveInPrepareHeader(waveInDevice, mp_wave_header_2, sizeof(WAVEHDR));
	waveInAddBuffer(waveInDevice, mp_wave_header_2, sizeof(WAVEHDR));

	ser = waveInStart(waveInDevice);
	if (ser != MMSYSERR_NOERROR)
	{
		MessageBox(cdlg->m_hWnd, _T("In_스타트에러"), _T("에러"), MB_OK);
		return;
	}

	return;
}

void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	WAVEHDR* pHdr = NULL;
	WAVEHDR pHdr2;
	unsigned int strLen = 0;
	if (SVC)
	{
		switch (uMsg)
		{
		case WIM_CLOSE:
			break;

		case WIM_DATA:
		{
			pHdr = (WAVEHDR*)dwParam1;
			pHdr2.lpData = (LPSTR)pHdr->lpData;
			pHdr2.dwBufferLength = pHdr->dwBufferLength;
			pHdr2.dwBytesRecorded = pHdr->dwBytesRecorded;
			pHdr2.dwUser = 0;
			pHdr2.dwFlags = 0;
			pHdr2.dwLoops = 0;

			while (strLen < pHdr2.dwBytesRecorded)
			{
				strLen += send(sock, &pHdr2.lpData[strLen], pHdr2.dwBytesRecorded, 0);
			}

			waveInPrepareHeader(hwi, pHdr, sizeof(WAVEHDR));
			waveInAddBuffer(hwi, pHdr, sizeof(WAVEHDR));
		}
		break;

		case WIM_OPEN:
			break;

		default:
			break;
		}
	}

	return;
	//mp_wave_header->dwUser = 0;
}

void CRemoteControl_SVDlg::OnEnChangeEdit2()
{
	UpdateData(TRUE);
}

BOOL T_DWM = 0;
void CRemoteControl_SVDlg::OnBnClickedButton5()
{
	if (T_DWM)
	{
		DwmEnableComposition(DWM_EC_ENABLECOMPOSITION);
		T_DWM = 0;
		GetDlgItem(IDC_BUTTON5)->SetWindowText(_T("DWM off"));
	}
	else
	{
		DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
		T_DWM = 1;
		GetDlgItem(IDC_BUTTON5)->SetWindowText(_T("DWM on"));
	}
}

UINT End(LPVOID lParam)
{
	WaitForSingleObject(End1->m_hThread, INFINITE);
	WaitForSingleObject(End2->m_hThread, INFINITE);
	WaitForSingleObject(End3->m_hThread, INFINITE);
	WaitForSingleObject(End4->m_hThread, INFINITE);
	fin = 1;
	waveInStop(waveInDevice);
	waveInReset(waveInDevice);
	waveInUnprepareHeader(waveInDevice, mp_wave_header, sizeof(WAVEHDR));
	waveInUnprepareHeader(waveInDevice, mp_wave_header_2, sizeof(WAVEHDR));
	waveInClose(waveInDevice);

	if (in_open == 0)
	{
		GlobalUnlock(mp_wave_header->lpData);
		GlobalUnlock(mp_wave_header_2->lpData);
		GlobalUnlock(mp_wave_header);
		GlobalUnlock(mp_wave_header_2);

		GlobalFree(mh_wave_data);
		GlobalFree(mh_wave_data_2);
		GlobalFree(mh_wave_header);
		GlobalFree(mh_wave_header_2);
	}

	closesocket(sock);
	cdlg->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	return 0;
}