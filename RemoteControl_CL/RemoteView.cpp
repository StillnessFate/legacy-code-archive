// RemoteView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RemoteControl_CL.h"
#include "RemoteView.h"
#include "afxdialogex.h"
#include "RemoteControl_CL.h"
#include <mmsystem.h>

#include <imm.h>
#pragma comment(lib, "imm32.lib")
// RemoteView 대화 상자입니다.


int ec = 0;
int self_end = 0;
int s_mode;
UINT ViewThread(LPVOID lParam);
UINT D_ViewThread(LPVOID lParam);
UINT M_ControlThread(LPVOID lParam);
UINT SoundThread(LPVOID lParam);
UINT End(LPVOID lParam);

DWORD nExitCode1 = NULL;
DWORD nExitCode2 = NULL;
DWORD nExitCode3 = NULL;
DWORD nExitCode4 = NULL;

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
WAVEFORMATEX  my_wave_format;
int Buffer_L = 22050;	//2205
int SPS = 44100;	//11025
int BPS = 16;	//8
int Chan = 2;	//1

CWinThread* End1;
CWinThread* End2;
CWinThread* End3;
CWinThread* End4;

BOOL D_com = 0;
CRect D_m_rect = 0;
IStream* D_pStream = 0;

LARGE_INTEGER liCounter1, liCounter2, liFrequency;

IMPLEMENT_DYNAMIC(RemoteView, CDialogEx)

LRESULT CALLBACK WinKeyHook(int code, WPARAM wparam, LPARAM lparam);
RemoteView* cdlg = NULL;

RemoteView::RemoteView(CWnd* pParent /*=NULL*/)
	: CDialogEx(RemoteView::IDD, pParent)
	, min_x(0)
	, min_y(0)
	, aspectx(0)
	, aspecty(0)
	, QR(0)

	, port(0)
{

}

RemoteView::~RemoteView()
{
}

void RemoteView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, aaa);
}

BEGIN_MESSAGE_MAP(RemoteView, CDialogEx)
	ON_BN_CLICKED(IDOK, &RemoteView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &RemoteView::OnBnClickedCancel)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZING()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//	ON_WM_MOUSELEAVE()
	//ON_WM_NCMOUSEHOVER()
	ON_WM_MOUSELEAVE()
	//ON_WM_NCMBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//ON_WM_ACTIVATE()
	//ON_WM_MBUTTONDOWN()
	//ON_WM_MOUSEHWHEEL()
	//ON_WM_DESTROY()
END_MESSAGE_MAP()


// RemoteView 메시지 처리기입니다.


void RemoteView::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void RemoteView::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void RemoteView::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_CLOSE) {
		UnhookWindowsHookEx(hook);
		//EndDialog(IDCANCEL);
		self_end = 1;
		ec = 0;
		//WSACleanup();
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


void RemoteView::OnSizing(UINT fwSide, LPRECT pRect)
{
	RECT sz;
	int w, h;
	BOOL lockAspect = TRUE;
	//double aspectx = 16;
	//double aspecty = 9;
	//int min_x = 320;
	//int min_y = 180;

	double aspectRatio = (double)aspecty / (double)aspectx;

	// If the aspect is locked, resize the window equally
	if (lockAspect)
	{
		// Copy over the new size that was determined by windows
		memcpy(&sz, (const void*)pRect, sizeof(RECT));

		// Calculate the width and height of the window
		w = sz.right - sz.left;
		if (w < min_x)
		{
			w = min_x;
			sz.right = sz.left + min_x;
		}
		h = sz.bottom - sz.top;
		if (h < min_y)
		{
			h = min_y;
			sz.bottom = sz.top + min_y;
		}

		switch (fwSide)
		{
		case WMSZ_LEFT:
		case WMSZ_RIGHT:
			// Modify the Heigh of the window
			sz.bottom = LONG(w * aspectRatio) + sz.top;
			break;

		case WMSZ_TOP:
		case WMSZ_BOTTOM:
			// Modify the Width of the window
			sz.right = LONG(h * (1 / aspectRatio)) + sz.left;
			break;

		case WMSZ_TOPRIGHT:
		case WMSZ_TOPLEFT:
		case WMSZ_BOTTOMRIGHT:
		case WMSZ_BOTTOMLEFT:
			// Adjust the width and height of the window to match aspect ratio
			if (float(h) / float(w) > aspectRatio)
			{
				w = int(float(h) / aspectRatio);
			}
			else {
				h = int(float(w) * aspectRatio);
			}

			// Adjust Height
			if (fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT)
			{
				sz.top = sz.bottom - h;
			}
			else {
				sz.bottom = sz.top + h;
			}

			// Adjust Width
			if (fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
			{
				sz.left = sz.right - w;
			}
			else {
				sz.right = sz.left + w;
			}
			break;
		}

		// Copy back the size of the window
		memcpy((void*)pRect, &sz, sizeof(RECT));
		return;
	}
}


BOOL RemoteView::OnInitDialog()
{
	this->MoveWindow(0, 0, aspectx / 2, aspecty / 2);
	CDialogEx::OnInitDialog();

	my_wave_format.wFormatTag = WAVE_FORMAT_PCM;
	my_wave_format.nChannels = Chan;                      // 마이크 녹음방식 : 모노
	my_wave_format.nSamplesPerSec = SPS;    // 샘플링 주기 : 11.025 KHz
	my_wave_format.wBitsPerSample = BPS;             // 샘플링 단위 : 8 Bits
	my_wave_format.nBlockAlign = my_wave_format.nChannels * (my_wave_format.wBitsPerSample / 8);
	my_wave_format.nAvgBytesPerSec = my_wave_format.nSamplesPerSec * my_wave_format.nBlockAlign;
	my_wave_format.cbSize = 0;                            // WAVEFORMATEX 구조체 정보외에 추가적인 정보가 없다.

	int opt_val = TRUE;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));//Nagle알고리즘!------------------------------------
	setsockopt(sock2, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));//Nagle알고리즘!------------------------------------
	ec = 1;

	if (QR == 0)
		s_mode = HALFTONE;
	else
		s_mode = COLORONCOLOR;

	cdlg = this;

	HINSTANCE inst = AfxGetInstanceHandle();
	hook = SetWindowsHookEx(WH_KEYBOARD_LL, WinKeyHook, inst, 0);
	QueryPerformanceFrequency(&liFrequency);  // retrieves the frequency of the high-resolution performance counter   

	End1 = AfxBeginThread(ViewThread, this);
	GetExitCodeThread(End1->m_hThread, &nExitCode1);
	End2 = AfxBeginThread(D_ViewThread, this);
	GetExitCodeThread(End2->m_hThread, &nExitCode2);
	End3 = AfxBeginThread(M_ControlThread, this);
	GetExitCodeThread(End3->m_hThread, &nExitCode3);
	End4 = AfxBeginThread(SoundThread, this);
	GetExitCodeThread(End4->m_hThread, &nExitCode4);
	AfxBeginThread(End, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#define MAX_BUFFER	40960
UINT ViewThread(LPVOID lParam)
{
	RemoteView* dlg = (RemoteView*)lParam; //호출한클래스의포인터객체생성
	int len = 0;
	int r_size = 0;
	int r_b = 0;
	char m_pBuffer[MAX_BUFFER];
	WINDOWPLACEMENT place;
	place.length = (UINT)sizeof(WINDOWPLACEMENT);
	HWND hWnd = dlg->m_hWnd;
	/*
	if (GetWindowPlacement(hWnd, &place))
	{
		switch (place.showCmd)
		{
		case SW_SHOWMAXIMIZED: // 최대화일때
			break;

		case SW_SHOWMINIMIZED: // 최소화일때
			break;

		case SW_SHOWNORMAL: // 일반일때
			break;
		}
	}
	*/
	QueryPerformanceCounter(&liCounter1);         // Start
	while (ec)
	{
		if (D_com == 0)
		{
			GetWindowPlacement(hWnd, &place);
			if (place.showCmd != SW_SHOWMINIMIZED)
			{

				CRect m_rect;    //클라이언트 영역을 얻어요.
				dlg->GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.  
				r_size = recv(dlg->sock, (char*)&len, sizeof(int), 0);
				if (r_size < 0) {
					if (ec)
						ec = 0;
					return 0;
				}
				IStream* pStream = NULL;
				CreateStreamOnHGlobal(NULL, TRUE, &pStream);


				while (len > 0)
				{
					if (len > MAX_BUFFER)
						r_b = MAX_BUFFER;
					else
						r_b = len;
					//MessageBox(dlg->m_hWnd, _T("CL_데이터읽기."), _T("통신"), MB_OK);
					r_size = recv(dlg->sock, m_pBuffer, r_b, 0);
					pStream->Write(m_pBuffer, r_size, 0);
					len -= r_size;
					if (r_size < 0) {
						if (ec)
							ec = 0;
						return 0;
					}
				}
				D_pStream = pStream;
				D_m_rect = m_rect;

				D_com = 1;
			}
			//--
		}
	}

	return 0;
}
UINT D_ViewThread(LPVOID lParam)
{
	CString fps;
	while (ec)
	{
		if (D_com)
		{
			CRect m_rect = D_m_rect;
			IStream* pStream = D_pStream;
			D_com = 0;

			CClientDC dc(cdlg);
			CImage Image;
			Image.Load(pStream);
			SetStretchBltMode(dc.m_hDC, s_mode);
			Image.StretchBlt(dc, 0, 0, m_rect.Width(), m_rect.Height(), 0, 0, cdlg->aspectx, cdlg->aspecty, SRCCOPY);
			pStream->Release();
			QueryPerformanceCounter(&liCounter2);         // End
			fps.Format(_T("RemoteView          FPS : %d"), (int)(1 / ((double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart)));
			cdlg->SetWindowText(fps);
			QueryPerformanceCounter(&liCounter1);         // Start

		}
	}

	return 0;
}

CPoint M_pos = { 0, 0 };


typedef struct apos
{
	double ax;
	double ay;
}apos;
int MB_D = 0;
int MBR_D = 0;

UINT M_ControlThread(LPVOID lParam)
{
	RemoteView* dlg = (RemoteView*)lParam; //호출한클래스의포인터객체생성
	CPoint od_M_pos = { 0, 0 };

	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 0;

	SetCapture(dlg->m_hWnd);
	while (ec)
	{
		if (od_M_pos != M_pos)
		{
			CRect m_rect;    //클라이언트 영역을 얻어요.
			dlg->GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
			a_pos.ax = (double)M_pos.x / (double)m_rect.Width();
			a_pos.ay = (double)M_pos.y / (double)m_rect.Height();

			memcpy(&buff[1], &a_pos, sizeof(a_pos));
			if (send(dlg->sock2, buff, sizeof(buff), 0) < 0)
			{
				if (ec)
					ec = 0;
				return 0;
			}
			od_M_pos = M_pos;
			Sleep(30);
		}
		Sleep(1);
	}

	return 0;
}
void RemoteView::OnMouseMove(UINT nFlags, CPoint point)
{
	M_pos = point;
	TRACKMOUSEEVENT trackMouseEvent;
	trackMouseEvent.cbSize = sizeof(trackMouseEvent);
	trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
	trackMouseEvent.hwndTrack = GetSafeHwnd();
	trackMouseEvent.dwHoverTime = 0x00000001;
	_TrackMouseEvent(&trackMouseEvent);
	//CDialogEx::OnMouseMove(nFlags, point);
}


void RemoteView::OnLButtonDown(UINT nFlags, CPoint point)
{
	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 1;
	CRect m_rect;    //클라이언트 영역을 얻어요.
	GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
	a_pos.ax = (double)point.x / (double)m_rect.Width();
	a_pos.ay = (double)point.y / (double)m_rect.Height();
	memcpy(&buff[1], &a_pos, sizeof(a_pos));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	MB_D = 1;
	//CDialogEx::OnLButtonDown(nFlags, point);
}


void RemoteView::OnLButtonUp(UINT nFlags, CPoint point)
{
	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 2;
	CRect m_rect;    //클라이언트 영역을 얻어요.
	GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
	a_pos.ax = (double)point.x / (double)m_rect.Width();
	a_pos.ay = (double)point.y / (double)m_rect.Height();
	memcpy(&buff[1], &a_pos, sizeof(a_pos));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	MB_D = 0;

	//CDialogEx::OnLButtonUp(nFlags, point);
}


void RemoteView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 1;
	CRect m_rect;    //클라이언트 영역을 얻어요.
	GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
	a_pos.ax = (double)point.x / (double)m_rect.Width();
	a_pos.ay = (double)point.y / (double)m_rect.Height();
	memcpy(&buff[1], &a_pos, sizeof(a_pos));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	MB_D = 1;
	//CDialogEx::OnLButtonDblClk(nFlags, point);
}




void RemoteView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (MB_D)
	{
		apos a_pos;
		char buff[sizeof(a_pos) + 1];
		buff[0] = 2;
		CRect m_rect;    //클라이언트 영역을 얻어요.
		GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
		a_pos.ax = (double)M_pos.x / (double)m_rect.Width();
		a_pos.ay = (double)M_pos.y / (double)m_rect.Height();
		memcpy(&buff[1], &a_pos, sizeof(a_pos));
		if (send(sock2, buff, sizeof(buff), 0) < 0)
		{
			if (ec)
				ec = 0;
			return;
		}
		MB_D = 0;
	}
	if (MBR_D)
	{
		apos a_pos;
		char buff[sizeof(a_pos) + 1];
		buff[0] = 6;
		CRect m_rect;    //클라이언트 영역을 얻어요.
		GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
		a_pos.ax = (double)M_pos.x / (double)m_rect.Width();
		a_pos.ay = (double)M_pos.y / (double)m_rect.Height();
		memcpy(&buff[1], &a_pos, sizeof(a_pos));
		if (send(sock2, buff, sizeof(buff), 0) < 0)
		{
			if (ec)
				ec = 0;
			return;
		}
		MBR_D = 0;
	}
	//CDialogEx::OnMouseLeave();
}




void RemoteView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	char buff[sizeof(nChar) + 1];
	if (nChar == 229)
	{
		buff[0] = 7;
		if (send(sock2, buff, sizeof(char), 0) < 0)
		{
			if (ec)
				ec = 0;
			return;
		}
		HIMC data = ImmGetContext(this->m_hWnd);
		ImmSetConversionStatus(data, 0, 0);
		//ImmReleaseContext(this->m_hWnd, data);
		return;
	}

	buff[0] = 3;
	memcpy(&buff[1], &nChar, sizeof(nChar));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	return;
	//CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void RemoteView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	char buff[sizeof(nChar) + 1];
	buff[0] = 4;
	memcpy(&buff[1], &nChar, sizeof(nChar));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	//CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void RemoteView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 5;
	CRect m_rect;    //클라이언트 영역을 얻어요.
	GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
	a_pos.ax = (double)point.x / (double)m_rect.Width();
	a_pos.ay = (double)point.y / (double)m_rect.Height();
	memcpy(&buff[1], &a_pos, sizeof(a_pos));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	MBR_D = 1;

	//CDialogEx::OnRButtonDblClk(nFlags, point);
}


void RemoteView::OnRButtonDown(UINT nFlags, CPoint point)
{
	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 5;
	CRect m_rect;    //클라이언트 영역을 얻어요.
	GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
	a_pos.ax = (double)point.x / (double)m_rect.Width();
	a_pos.ay = (double)point.y / (double)m_rect.Height();
	memcpy(&buff[1], &a_pos, sizeof(a_pos));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	MBR_D = 1;

	//CDialogEx::OnRButtonDown(nFlags, point);
}


void RemoteView::OnRButtonUp(UINT nFlags, CPoint point)
{
	apos a_pos;
	char buff[sizeof(a_pos) + 1];
	buff[0] = 6;
	CRect m_rect;    //클라이언트 영역을 얻어요.
	GetClientRect(&m_rect);    //클라이언트 영역을 얻어요.
	a_pos.ax = (double)point.x / (double)m_rect.Width();
	a_pos.ay = (double)point.y / (double)m_rect.Height();
	memcpy(&buff[1], &a_pos, sizeof(a_pos));
	if (send(sock2, buff, sizeof(buff), 0) < 0)
	{
		if (ec)
			ec = 0;
		return;
	}
	MBR_D = 0;

	//CDialogEx::OnRButtonUp(nFlags, point);
}



BOOL RemoteView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		short zDelta;
		char buff[sizeof(zDelta) + 1];
		if ((int)pMsg->wParam > 0)
			zDelta = 120;
		else
			zDelta = -120;

		buff[0] = 9;
		memcpy(&buff[1], &zDelta, sizeof(zDelta));
		if (send(sock2, buff, sizeof(buff), 0) < 0)
		{
			if (ec)
				ec = 0;
			return 0;
		}
	}

	return 0;
}


LRESULT CALLBACK WinKeyHook(int code, WPARAM wparam, LPARAM lparam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lparam;
	HWND hDlg = GetForegroundWindow();
	//CWnd *pWnd = AfxGetMainWnd();
	//HWND hWnd = pWnd->m_hWnd;
	UINT nChar;
	char buff[sizeof(nChar) + 1];

	if (hDlg == cdlg->m_hWnd)
	{
		switch (wparam)
		{
		case(WM_KEYDOWN):
		case(WM_SYSKEYDOWN):
			if ((key->vkCode == VK_MENU) || (key->vkCode == VK_LMENU) || (key->vkCode == VK_RMENU))
			{
				nChar = key->vkCode;
				buff[0] = 3;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_TAB))
			{
				nChar = key->vkCode;
				buff[0] = 3;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_ESCAPE))
			{
				nChar = key->vkCode;
				buff[0] = 3;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_SNAPSHOT))
			{
				nChar = key->vkCode;
				buff[0] = 3;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_LWIN) || (key->vkCode == VK_RWIN))
			{
				nChar = key->vkCode;
				buff[0] = 3;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}
			break;

		case(WM_KEYUP):
		case(WM_SYSKEYUP):
			if ((key->vkCode == VK_MENU) || (key->vkCode == VK_LMENU) || (key->vkCode == VK_RMENU))
			{
				nChar = key->vkCode;
				buff[0] = 4;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_TAB))
			{
				nChar = key->vkCode;
				buff[0] = 4;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_ESCAPE))
			{
				nChar = key->vkCode;
				buff[0] = 4;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_SNAPSHOT))
			{
				nChar = key->vkCode;
				buff[0] = 4;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}

			if ((key->vkCode == VK_LWIN) || (key->vkCode == VK_RWIN))
			{
				nChar = key->vkCode;
				buff[0] = 4;
				memcpy(&buff[1], &nChar, sizeof(nChar));
				if (send(cdlg->sock2, buff, sizeof(buff), 0) < 0)
				{
					if (ec)
						ec = 0;
					return TRUE;
				}
				return TRUE;
			}
			break;

		default:
			break;
		}
		return 0;
	}

	return 0;

}

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if (ec)
	{
		switch (uMsg)
		{
		case WOM_OPEN: // waveOutOpen 함수가 실행되었을 경우에는 여기 부분이 처리
			//_tprintf(TEXT("waveOutProc: WOM_OPEN\n"));
			break;
		case WOM_DONE: // waveOutWrite 함수로 작성한 데이터의 재생이 완료되었을 경우에는 여기 부분이 처리
			//_tprintf(TEXT("waveOutProc: WOM_DONE\n"));
			break;
		case WOM_CLOSE: // waveOutClose 함수가 실행되었을 경우에는 여기 부분이 처리
			//_tprintf(TEXT("waveOutProc: WOM_CLOSE\n"));
			break;
		default:
			break;
		}
	}

	return;
}

SOCKET servSock;
UINT SoundThread(LPVOID lParam)
{
	BOOL cc = 0;
	//WAVEHDR pHdr;
	HWAVEOUT m_hWaveOut;
	HANDLE h_wave_data = GlobalAlloc(GMEM_MOVEABLE, Buffer_L);
	unsigned char* p_wave_data = (unsigned char*)GlobalLock(h_wave_data);

	HANDLE h_wave_data2 = GlobalAlloc(GMEM_MOVEABLE, Buffer_L);
	unsigned char* p_wave_data2 = (unsigned char*)GlobalLock(h_wave_data2);

	HANDLE h_wave_data3 = GlobalAlloc(GMEM_MOVEABLE, Buffer_L);
	unsigned char* p_wave_data3 = (unsigned char*)GlobalLock(h_wave_data3);

	BOOL F = 1;

	HANDLE mh_wave_header = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(WAVEHDR));
	WAVEHDR* p_wave_header = (LPWAVEHDR)GlobalLock(mh_wave_header);

	HANDLE mh_wave_header2 = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(WAVEHDR));
	WAVEHDR* p_wave_header2 = (LPWAVEHDR)GlobalLock(mh_wave_header2);

	HANDLE mh_wave_header3 = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(WAVEHDR));
	WAVEHDR* p_wave_header3 = (LPWAVEHDR)GlobalLock(mh_wave_header3);

	p_wave_header->lpData = (char*)p_wave_data;
	p_wave_header->dwBufferLength = Buffer_L;
	p_wave_header->dwUser = 0;
	p_wave_header->dwFlags = 0;
	p_wave_header->dwLoops = 0;

	p_wave_header2->lpData = (char*)p_wave_data2;
	p_wave_header2->dwBufferLength = Buffer_L;
	p_wave_header2->dwUser = 0;
	p_wave_header2->dwFlags = 0;
	p_wave_header2->dwLoops = 0;

	p_wave_header3->lpData = (char*)p_wave_data3;
	p_wave_header3->dwBufferLength = Buffer_L;
	p_wave_header3->dwUser = 0;
	p_wave_header3->dwFlags = 0;
	p_wave_header3->dwLoops = 0;

	if (waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &my_wave_format, (DWORD)waveOutProc, 0, WAVE_FORMAT_DIRECT) != MMSYSERR_NOERROR)
	{
		MessageBox(cdlg->m_hWnd, _T("Out_오픈에러"), _T("에러"), MB_OK);
		goto Sound_End;
	}

	//p_wave_header->dwBufferLength = Buffer_L;
	//p_wave_header->dwBytesRecorded = 0;

	SOCKADDR_IN servAdr, clntAdr;

	servSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (servSock == INVALID_SOCKET)
	{
		MessageBox(cdlg->m_hWnd, _T("소켓에러"), _T("에러"), MB_OK);
		goto Sound_End;
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(cdlg->port);

	//---타임아웃
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int optlen = sizeof(timeout);
	setsockopt(servSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, optlen);
	//---타임아웃

	if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		MessageBox(cdlg->m_hWnd, _T("바인드에러"), _T("에러"), MB_OK);
		goto Sound_End;
	}

	int strLen;
	int clntAdrSz;
	clntAdrSz = sizeof(clntAdr);

	while (ec)
	{
		strLen = recvfrom(servSock, (char*)p_wave_data3, Buffer_L, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
		p_wave_header3->dwBytesRecorded = strLen;
		p_wave_header3->dwBufferLength = strLen;
		if (strLen > 0)
		{
			if (waveOutPrepareHeader(m_hWaveOut, p_wave_header3, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
			{
				MessageBox(cdlg->m_hWnd, _T("Out_헤더에러"), _T("에러"), MB_OK);
				goto Sound_End;
			}
			break;
		}

	}

	while (ec)
	{

		if (cc)
		{
			strLen = recvfrom(servSock, (char*)p_wave_data, Buffer_L, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
			p_wave_header->dwBytesRecorded = strLen;
			p_wave_header->dwBufferLength = strLen;

			if (strLen > 0)
			{
				if (F)
				{
					if (waveOutWrite(m_hWaveOut, p_wave_header3, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
					{
						MessageBox(cdlg->m_hWnd, _T("Out_최초쓰기에러"), _T("에러"), MB_OK);
						goto Sound_End;
						//continue;
					}
					F = 0;
				}
				if (waveOutPrepareHeader(m_hWaveOut, p_wave_header, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
				{
					MessageBox(cdlg->m_hWnd, _T("Out_헤더에러"), _T("에러"), MB_OK);
					goto Sound_End;
				}
				if (waveOutWrite(m_hWaveOut, p_wave_header, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
				{
					MessageBox(cdlg->m_hWnd, _T("Out_쓰기에러"), _T("에러"), MB_OK);
					//goto Sound_End;
					continue;
				}
			}
			cc = 0;
		}
		else
		{
			strLen = recvfrom(servSock, (char*)p_wave_data2, Buffer_L, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);
			p_wave_header2->dwBytesRecorded = strLen;
			p_wave_header2->dwBufferLength = strLen;

			if (strLen > 0)
			{
				if (F)
				{
					if (waveOutWrite(m_hWaveOut, p_wave_header3, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
					{
						MessageBox(cdlg->m_hWnd, _T("Out_최초쓰기에러"), _T("에러"), MB_OK);
						goto Sound_End;
						//continue;
					}
					F = 0;
				}
				if (waveOutPrepareHeader(m_hWaveOut, p_wave_header2, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
				{
					MessageBox(cdlg->m_hWnd, _T("Out_헤더에러"), _T("에러"), MB_OK);
					return 0;
				}
				if (waveOutWrite(m_hWaveOut, p_wave_header2, sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
				{
					//MessageBox(cdlg->m_hWnd, _T("Out_쓰기에러"), _T("에러"), MB_OK);
					//goto Sound_End;
					continue;
				}
			}
			cc = 1;
		}
	}

Sound_End:

	waveOutReset(m_hWaveOut);

	waveOutUnprepareHeader(m_hWaveOut, p_wave_header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(m_hWaveOut, p_wave_header2, sizeof(WAVEHDR));
	waveOutUnprepareHeader(m_hWaveOut, p_wave_header3, sizeof(WAVEHDR));
	waveOutClose(m_hWaveOut);



	GlobalUnlock(p_wave_data);
	GlobalFree(h_wave_data);
	GlobalUnlock(p_wave_data2);
	GlobalFree(h_wave_data2);
	GlobalUnlock(p_wave_data3);
	GlobalFree(h_wave_data3);

	GlobalUnlock(p_wave_header);
	GlobalFree(mh_wave_header);
	GlobalUnlock(p_wave_header2);
	GlobalFree(mh_wave_header2);
	GlobalUnlock(p_wave_header3);
	GlobalFree(mh_wave_header3);

	closesocket(servSock);
	return 0;
}




UINT End(LPVOID lParam)
{
	WaitForSingleObject(End1->m_hThread, INFINITE);
	//MessageBox(cdlg->m_hWnd, _T("1"), _T("에러"), MB_OK);
	WaitForSingleObject(End2->m_hThread, INFINITE);
	//MessageBox(cdlg->m_hWnd, _T("2"), _T("에러"), MB_OK);
	WaitForSingleObject(End3->m_hThread, INFINITE);
	//MessageBox(cdlg->m_hWnd, _T("3"), _T("에러"), MB_OK);
	WaitForSingleObject(End4->m_hThread, INFINITE);
	//MessageBox(cdlg->m_hWnd, _T("4"), _T("에러"), MB_OK);
	//if (ec == 0)
	//{
	D_com = 0;
	UnhookWindowsHookEx(cdlg->hook);
	closesocket(cdlg->sock);
	closesocket(cdlg->sock2);
	closesocket(servSock);
	EndDialog(cdlg->m_hWnd, 0);
	//EndDialog(cdlg->m_hWnd, IDCANCEL);
	AfxGetMainWnd()->ShowWindow(SW_RESTORE);
	AfxGetMainWnd()->SetForegroundWindow(); //최상위로
	AfxGetMainWnd()->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	AfxGetMainWnd()->GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	if (self_end == 0)
		MessageBox(cdlg->m_hWnd, _T("연결이 끊어졌습니다"), _T("통신에러"), MB_OK);
	else
		self_end = 0;

	//}
	return 0;
}