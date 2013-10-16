#pragma once

#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CApplication.h"

class IllusionExcelFile
{
public:
	//构造
	IllusionExcelFile();
	//析构
	virtual ~IllusionExcelFile();

public:
	//初始化EXCEL OLE
	static BOOL InitExcel();
	//释放EXCEL的 OLE
	static void ReleaseExcel();

public:
	void		ShowInExcel(BOOL bShow);
	//检查一个CELL是否是字符串
	BOOL		IsCellString(long iRow, long iColumn);
	//检查一个CELL是否是数值
	BOOL		IsCellInt(long iRow, long iColumn);
	//得到一个CELL的String
	CString		GetCellString(long iRow, long iColumn);
	//得到整数
	int			GetCellInt(long iRow, long iColumn);
	//得到double的数据
	double		GetCellDouble(long iRow, long iColumn);
	//取得行的总数
	int			GetRowCount();
	//取得列的总数
	int			GetColumnCount();
	//使用某个sheet
	BOOL		LoadSheetByIdx(long iTableIndex, BOOL bPreLoad = FALSE);
	//通过名称使用某个sheet，
	BOOL		LoadSheetByName(const char* szSheet, BOOL bPreLoad = FALSE);
	//通过序号取得某个Sheet的名称
	CString		GetSheetName(long iTableIndex);
	//得到Sheet的总数
	int			GetSheetCount();
	//打开文件
	BOOL		OpenExcelFile(const char* szFileName);
	//关闭打开的Excel 文件，有时候打开EXCEL文件就要
	void		CloseExcelFile(BOOL bSave = FALSE);
	//另存为一个EXCEL文件
	void		SaveasXSLFile(const CString& sXlsFile);
	//取得打开文件的名称
	CString		GetOpenFileName();
	//取得打开sheet的名称
	CString		GetLoadSheetName();
	//写入一个CELL一个int
	void		SetCellInt(long iRow, long iColumn, int iValue);
	//写入一个CELL一个string
	void		SetCellString(long iRow, long iColumn, const CString& sValue);

protected:
	//预先加载
	void		PreLoadSheet();

protected:
	//打开的EXCEL文件名称
	CString       m_sExcelFile;
	//EXCEL BOOK集合，（多个文件时）
	CWorkbooks    m_sExcelBooks; 
	//当前使用的BOOK，当前处理的文件
	CWorkbook     m_sWorkBook; 
	//EXCEL的sheets集合
	CWorksheets   m_sExcelSheets; 
	//当前使用sheet
	CWorksheet    m_sWorkSheet; 
	//当前的操作区域
	CRange        m_sCurRange; 
	//是否已经预加载了某个sheet的数据
	BOOL          m_bAlreadyPreload;
	//Create the SAFEARRAY from the VARIANT ret.
	COleSafeArray m_sOleSafeArray;
};
