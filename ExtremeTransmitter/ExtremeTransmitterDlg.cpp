// ExtremeTransmitterDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ExtremeTransmitter.h"
#include "ExtremeTransmitterDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#pragma warning(disable : 4995)

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
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

TCHAR* StringToTCHAR(CString str)
{
	TCHAR* tszStr = NULL;
	int nLen = str.GetLength() + 1;
	tszStr = new TCHAR[nLen];
	memset(tszStr, 0x00, nLen * sizeof(TCHAR));
	_tcscpy(tszStr, str);

	return tszStr;
}

WSADATA wsaData;

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


// CExtremeTransmitterDlg 대화 상자



CExtremeTransmitterDlg::CExtremeTransmitterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExtremeTransmitterDlg::IDD, pParent)
	, dr(_T(""))
	, Speed(0)
	, upSpeed(0)
	, portad(0)
	, sstay(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExtremeTransmitterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, dr);
	DDX_Text(pDX, IDC_EDIT3, Speed);
	DDX_Control(pDX, IDC_PROGRESS1, dwp);
	DDX_Control(pDX, IDC_PROGRESS2, upp);
	DDX_Text(pDX, IDC_EDIT5, upSpeed);
	DDX_Control(pDX, IDC_IPADDRESS1, ipad);
	DDX_Text(pDX, IDC_EDIT7, portad);
	DDX_Text(pDX, IDC_EDIT8, sstay);
}

BEGIN_MESSAGE_MAP(CExtremeTransmitterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CExtremeTransmitterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CExtremeTransmitterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON4, &CExtremeTransmitterDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CExtremeTransmitterDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON9, &CExtremeTransmitterDlg::OnBnClickedButton9)
	ON_EN_CHANGE(IDC_EDIT7, &CExtremeTransmitterDlg::OnEnChangeEdit7)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CExtremeTransmitterDlg::OnIpnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BUTTON2, &CExtremeTransmitterDlg::OnBnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON1, &CExtremeTransmitterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CExtremeTransmitterDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CExtremeTransmitterDlg 메시지 처리기

BOOL CExtremeTransmitterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ipad.SetWindowText(_T("127.0.0.1"));
	portad = 1597;
	sstay = "서버 OFF";
	UpdateData(FALSE);



	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(_T("WSA에러"), _T("에러"), MB_OK);
		return 0;
	}


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

	CFont fnt;
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 52;
	lf.lfWeight = FW_BOLD;
	lstrcpy(lf.lfFaceName, L"Rosewood Std Regular");
	fnt.CreateFontIndirect(&lf);
	GetDlgItem(IDC_STATIC_1)->SetFont(&fnt);
	fnt.Detach();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CExtremeTransmitterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_CLOSE) {
		EndDialog(IDCANCEL);
		WSACleanup();
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
__int64 getFileSize(wchar_t* filename);


void CExtremeTransmitterDlg::OnPaint()
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
HCURSOR CExtremeTransmitterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


wchar_t* fname;
CString strIP = _T("127.0.0.1");

void CExtremeTransmitterDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}

void CExtremeTransmitterDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


#define MAX_BUFFER	40960
UINT ServerThread(LPVOID lParam)
{
	CExtremeTransmitterDlg* dlg = (CExtremeTransmitterDlg*)lParam; //호출한클래스의포인터객체생성
	LARGE_INTEGER upliCounter1, upliCounter2, upliFrequency;

	struct sockaddr_in serv_adr, clnt_adr;
	SOCKET serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == INVALID_SOCKET) {
		MessageBox(dlg->m_hWnd, _T("소켓에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		dlg->sstay = "서버 OFF";
		dlg->UpdateData(FALSE);
		return 0;;
	}
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(dlg->portad);

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		MessageBox(dlg->m_hWnd, _T("바인드에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		dlg->sstay = "서버 OFF";
		dlg->UpdateData(FALSE);
		return 0;;
	}
	if (listen(serv_sock, 5) == -1) {
		MessageBox(dlg->m_hWnd, _T("리슨에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		dlg->sstay = "서버 OFF";
		dlg->UpdateData(FALSE);
		return 0;;
	}

	MessageBox(dlg->m_hWnd, _T("준비 완료"), _T("준비 완료"), MB_OK);
	dlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	int clnt_adr_sz = sizeof(clnt_adr);
	SOCKET clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

	dlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	FILE* fpSrc;
	errno_t errSrc;
	int FileReadSize, FileWriteSize, FileBuffSize;
	_int64 SrcFileSize;
	char pFileBuff[MAX_BUFFER];

	int opt_val = TRUE;
	setsockopt(clnt_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));//Nagle알고리즘!------------------------------------

	errSrc = _wfopen_s(&fpSrc, dlg->dr, _T("rb"));// w 가 아닌 wb 즉 binary 모드로 열어야 한다.
	if (0 != errSrc) {
		closesocket(clnt_sock);
		closesocket(serv_sock);
		MessageBox(dlg->m_hWnd, _T(" 파일오픈 에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		dlg->sstay = "서버 OFF";
		dlg->UpdateData(FALSE);
		return 0;
	}
	SrcFileSize = getFileSize(StringToTCHAR(dlg->dr));


	int tss = 0;
	send(clnt_sock, (char*)&SrcFileSize, sizeof(SrcFileSize), 0);

	int cslen = lstrlen(fname) * 2;
	send(clnt_sock, (char*)&cslen, sizeof(int), 0);
	if (send(clnt_sock, (char*)fname, cslen, 0) < 0)
	{
		fclose(fpSrc);
		closesocket(clnt_sock);
		closesocket(serv_sock);
		dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
		dlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		dlg->sstay = "서버 OFF";
		dlg->UpdateData(FALSE);
		return 0;;
	}


	_int64 SrcFileSize2 = SrcFileSize;
	dlg->upp.SetRange(0, 1000);

	int qp = 0, tt = 0;
	QueryPerformanceFrequency(&upliFrequency);
	upliFrequency.QuadPart /= 1000;
	QueryPerformanceCounter(&upliCounter1);
	while (SrcFileSize > 0)
	{
		memset(pFileBuff, '\0', MAX_BUFFER);
		if (SrcFileSize > MAX_BUFFER)
			FileBuffSize = MAX_BUFFER;
		else
			FileBuffSize = SrcFileSize;

		FileReadSize = fread(pFileBuff, sizeof(char), FileBuffSize, fpSrc);
		FileWriteSize = send(clnt_sock, pFileBuff, FileReadSize, 0);

		SrcFileSize -= FileWriteSize;

		qp += FileWriteSize;
		dlg->upp.SetPos(1000 - (((double)SrcFileSize / (double)SrcFileSize2) * 1000));
		QueryPerformanceCounter(&upliCounter2);
		tt = (upliCounter2.QuadPart - upliCounter1.QuadPart) / upliFrequency.QuadPart;
		if (tt > 500) {
			dlg->upSpeed = qp / tt;
			dlg->UpdateData(FALSE);
			qp = 0;
			upliCounter1.QuadPart = upliCounter2.QuadPart;
		}
		if (FileWriteSize < 0) {
			fclose(fpSrc);
			closesocket(clnt_sock);
			closesocket(serv_sock);
			MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다."), _T("통신에러"), MB_OK);
			dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
			dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
			dlg->sstay = "서버 OFF";
			dlg->UpdateData(FALSE);
			return 0;;
		}


	}
	fclose(fpSrc);
	closesocket(clnt_sock);
	closesocket(serv_sock);
	MessageBox(dlg->m_hWnd, _T("완료"), _T("전송 완료"), MB_OK);
	dlg->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	dlg->GetDlgItem(IDC_BUTTON9)->EnableWindow(TRUE);
	dlg->sstay = "서버 OFF";
	dlg->UpdateData(FALSE);
	return 0;
}
UINT ClThread(LPVOID lParam)
{
	CExtremeTransmitterDlg* dlg = (CExtremeTransmitterDlg*)lParam; //호출한클래스의포인터객체생성
	int sock, nbyte;
	struct sockaddr_in serv_addr;
	_int64 filesize = 0;
	char buf[MAX_BUFFER];
	memset(buf, 0, sizeof(buf));

	LARGE_INTEGER liCounter1, liCounter2, liFrequency;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		MessageBox(dlg->m_hWnd, _T("소켓에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		return 0;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(StringToChar(strIP));
	serv_addr.sin_port = htons(dlg->portad);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("커넥트에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		return 0;
	}

	MessageBox(dlg->m_hWnd, _T("접속 완료"), _T("접속 완료"), MB_OK);
	int namelen = 0;
	int tss = 0;
	wchar_t nameget[150];
	memset(nameget, 0, sizeof(nameget));
	recv(sock, (char*)&filesize, sizeof(filesize), 0);
	recv(sock, (char*)&namelen, sizeof(int), 0);
	tss = recv(sock, (char*)nameget, namelen, 0);
	if (tss == NULL)
	{
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T("접속 에러"), _T("통신 에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		return 0;
	}
	_int64 filesize2 = filesize;

	CString path;
	wchar_t* ftypeget = _tcsrchr(nameget, '.');
	wchar_t* ftype = new wchar_t[lstrlen(ftypeget) + 5];

	wsprintf(ftype, _T("%s%s"), ftypeget + 1, _T("|*.*|"));

	CFileDialog FileDialog(FALSE, ftypeget + 1, nameget, OFN_FILEMUSTEXIST, ftype, NULL);
	FileDialog.m_ofn.lpstrTitle = _T("파일 저장");
	if (FileDialog.DoModal() == IDOK)
		path = FileDialog.GetPathName();
	delete[]ftype;

	FILE* file;
	errno_t errSrc = _wfopen_s(&file, path, _T("wb"));
	if (0 != errSrc) {
		closesocket(sock);
		MessageBox(dlg->m_hWnd, _T(" 파일오픈 에러"), _T("에러"), MB_OK);
		dlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
		return 0;
	}
	dlg->dwp.SetRange(0, 1000);
	int qp = 0, tt = 0;
	QueryPerformanceFrequency(&liFrequency);  // retrieves the frequency of the high-resolution performance counter
	liFrequency.QuadPart /= 1000;
	QueryPerformanceCounter(&liCounter1);
	while (filesize > 0)
	{
		memset(buf, '\0', sizeof(buf));
		nbyte = recv(sock, buf, MAX_BUFFER, 0);
		fwrite(buf, nbyte, 1, file);
		filesize -= nbyte;

		qp += nbyte;
		dlg->dwp.SetPos(1000 - (((double)filesize / (double)filesize2) * 1000));

		QueryPerformanceCounter(&liCounter2);
		tt = (liCounter2.QuadPart - liCounter1.QuadPart) / liFrequency.QuadPart;
		if (tt > 500) {
			dlg->Speed = qp / tt;
			dlg->UpdateData(FALSE);
			qp = 0;
			liCounter1.QuadPart = liCounter2.QuadPart;
		}

		if (nbyte < 0) {
			fclose(file);
			closesocket(sock);
			MessageBox(dlg->m_hWnd, _T("연결이 끊어졌습니다."), _T("통신에러"), MB_OK);
			dlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
			return 0;;
		}
	}
	fclose(file);
	closesocket(sock);
	MessageBox(dlg->m_hWnd, _T("다운로드 완료"), _T("완료"), MB_OK);
	dlg->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	return 0;
}

void CExtremeTransmitterDlg::OnBnClickedButton4()
{
	if (dr == "")
		MessageBox(_T("파일을 선택하세요!"), _T("오류"), MB_OK);
	else
	{
		sstay = "서버 ON";
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON9)->EnableWindow(FALSE);
		AfxBeginThread(ServerThread, this);
		UpdateData(FALSE);
	}
}

void CExtremeTransmitterDlg::OnBnClickedButton5()
{
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	AfxBeginThread(ClThread, this);
}


__int64 getFileSize(wchar_t* filename) {
	struct _stati64 statbuf;

	if (_wstati64(filename, &statbuf)) return -1; // 파일 정보 얻기: 에러 있으면 -1 을 반환

	return statbuf.st_size;                        // 파일 크기 반환
}



void CExtremeTransmitterDlg::OnBnClickedButton9()
{
	CString path;
	LPSTR szFilter = "All Files (*.*)|*.*|";
	CFileDialog FileDialog(TRUE);
	FileDialog.m_ofn.lpstrTitle = _T("파일 선택");
	if (FileDialog.DoModal() == IDOK)
	{
		CString check = FileDialog.GetFileExt();
		path = FileDialog.GetPathName();
	}

	dr = path;
	fname = _tcsrchr(StringToTCHAR(path), '\\') + 1;
	UpdateData(FALSE);
	if (dr == "")
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
}



void CExtremeTransmitterDlg::OnEnChangeEdit7()
{
	UpdateData(TRUE);
	if (portad > 65535)
		portad = 65535;
	UpdateData(FALSE);
}


void CExtremeTransmitterDlg::OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);

	ipad.GetWindowText(strIP);

	//*pResult = 0;
}

void CExtremeTransmitterDlg::OnBnClickedButton2()
{
	SOCKET sock;
	struct sockaddr_in serv_addr;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		MessageBox(_T("소켓에러"), _T("에러"), MB_OK);
		return;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portad);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		closesocket(sock);
		MessageBox(_T("커넥트에러"), _T("에러"), MB_OK);
		return;
	}
	closesocket(sock);
}


void CExtremeTransmitterDlg::OnBnClickedButton3()
{
	MessageBox(_T("Extreme Transmitter는 P2P 방식의 파일 전송기입니다.\n\n\n[사용법]\n-----파일 전송측-----\n포트 입력 -> 파일 선택 -> 서버\n\n-----파일 수신측-----\n아이피 입력 -> 포트 입력 -> 다운로드"), _T("도움말"), MB_OK);
}
