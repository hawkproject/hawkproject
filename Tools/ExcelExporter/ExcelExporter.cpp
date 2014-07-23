#include "stdafx.h"
#include "ExcelExporter.h"
#include "ExcelExporterDlg.h"
#include "IllusionExcelFile.h"

BEGIN_MESSAGE_MAP(CExcelExporterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CExcelExporterApp::CExcelExporterApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CExcelExporterApp theApp;

BOOL CExcelExporterApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	::CoInitialize(0);

	IllusionExcelFile::InitExcel();
	
	AfxEnableControlContainer();

	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CExcelExporterDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{		
	}
	else if (nResponse == IDCANCEL)
	{
	}

	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	IllusionExcelFile::ReleaseExcel();

	::CoUninitialize();

	return FALSE;
}

