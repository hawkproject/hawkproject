#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"

class CExcelExporterApp : public CWinApp
{
public:
	CExcelExporterApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
extern CExcelExporterApp theApp;
