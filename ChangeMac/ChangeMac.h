
// ChangeMac.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChangeMacApp:
// �йش����ʵ�֣������ ChangeMac.cpp
//

class CChangeMacApp : public CWinApp
{
public:
	CChangeMacApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChangeMacApp theApp;