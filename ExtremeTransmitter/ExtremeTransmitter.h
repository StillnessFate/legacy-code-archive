
// ExtremeTransmitter.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CExtremeTransmitterApp:
// �� Ŭ������ ������ ���ؼ��� ExtremeTransmitter.cpp�� �����Ͻʽÿ�.
//

class CExtremeTransmitterApp : public CWinApp
{
public:
	CExtremeTransmitterApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CExtremeTransmitterApp theApp;