
// RemoteControl_CL.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRemoteControl_CLApp:
// �� Ŭ������ ������ ���ؼ��� RemoteControl_CL.cpp�� �����Ͻʽÿ�.
//

class CRemoteControl_CLApp : public CWinApp
{
public:
	CRemoteControl_CLApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRemoteControl_CLApp theApp;