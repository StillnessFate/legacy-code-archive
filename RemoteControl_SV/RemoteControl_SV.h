
// RemoteControl_SV.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRemoteControl_SVApp:
// �� Ŭ������ ������ ���ؼ��� RemoteControl_SV.cpp�� �����Ͻʽÿ�.
//

class CRemoteControl_SVApp : public CWinApp
{
public:
	CRemoteControl_SVApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRemoteControl_SVApp theApp;