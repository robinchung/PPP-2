// PaperPlane.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPaperPlaneApp:
// �йش����ʵ�֣������ PaperPlane.cpp
//

class CPaperPlaneApp : public CWinApp
{
public:
	CPaperPlaneApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPaperPlaneApp theApp;