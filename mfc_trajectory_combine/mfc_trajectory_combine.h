
// mfc_trajectory_combine.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmfc_trajectory_combineApp: 
// �йش����ʵ�֣������ mfc_trajectory_combine.cpp
//

class Cmfc_trajectory_combineApp : public CWinApp
{
public:
	Cmfc_trajectory_combineApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_trajectory_combineApp theApp;