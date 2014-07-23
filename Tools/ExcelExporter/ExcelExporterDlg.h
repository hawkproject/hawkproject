#pragma once

#include "afxcmn.h"
#include "HawkUtil.h"
#include "IllusionExcelFile.h"

class CExcelExporterDlg : public CDialogEx
{
public:
	CExcelExporterDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_EXCELEXPORTER_DIALOG };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void	OnBnClickedOk();
	DECLARE_MESSAGE_MAP()

public:
	virtual Bool	ExportExcel(IllusionExcelFile& sExcel, const AString& sSheetName);

protected:
	HICON			m_hIcon;
	CListCtrl		m_hExcelList;
	AStringVector	m_vExcelFiles;
};
