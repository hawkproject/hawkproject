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
	//����
	IllusionExcelFile();
	//����
	virtual ~IllusionExcelFile();

public:
	//��ʼ��EXCEL OLE
	static BOOL InitExcel();
	//�ͷ�EXCEL�� OLE
	static void ReleaseExcel();

public:
	void		ShowInExcel(BOOL bShow);
	//���һ��CELL�Ƿ����ַ���
	BOOL		IsCellString(long iRow, long iColumn);
	//���һ��CELL�Ƿ�����ֵ
	BOOL		IsCellInt(long iRow, long iColumn);
	//�õ�һ��CELL��String
	CString		GetCellString(long iRow, long iColumn);
	//�õ�����
	int			GetCellInt(long iRow, long iColumn);
	//�õ�double������
	double		GetCellDouble(long iRow, long iColumn);
	//ȡ���е�����
	int			GetRowCount();
	//ȡ���е�����
	int			GetColumnCount();
	//ʹ��ĳ��sheet
	BOOL		LoadSheetByIdx(long iTableIndex, BOOL bPreLoad = FALSE);
	//ͨ������ʹ��ĳ��sheet��
	BOOL		LoadSheetByName(const char* szSheet, BOOL bPreLoad = FALSE);
	//ͨ�����ȡ��ĳ��Sheet������
	CString		GetSheetName(long iTableIndex);
	//�õ�Sheet������
	int			GetSheetCount();
	//���ļ�
	BOOL		OpenExcelFile(const char* szFileName);
	//�رմ򿪵�Excel �ļ�����ʱ���EXCEL�ļ���Ҫ
	void		CloseExcelFile(BOOL bSave = FALSE);
	//���Ϊһ��EXCEL�ļ�
	void		SaveasXSLFile(const CString& sXlsFile);
	//ȡ�ô��ļ�������
	CString		GetOpenFileName();
	//ȡ�ô�sheet������
	CString		GetLoadSheetName();
	//д��һ��CELLһ��int
	void		SetCellInt(long iRow, long iColumn, int iValue);
	//д��һ��CELLһ��string
	void		SetCellString(long iRow, long iColumn, const CString& sValue);

protected:
	//Ԥ�ȼ���
	void		PreLoadSheet();

protected:
	//�򿪵�EXCEL�ļ�����
	CString       m_sExcelFile;
	//EXCEL BOOK���ϣ�������ļ�ʱ��
	CWorkbooks    m_sExcelBooks; 
	//��ǰʹ�õ�BOOK����ǰ������ļ�
	CWorkbook     m_sWorkBook; 
	//EXCEL��sheets����
	CWorksheets   m_sExcelSheets; 
	//��ǰʹ��sheet
	CWorksheet    m_sWorkSheet; 
	//��ǰ�Ĳ�������
	CRange        m_sCurRange; 
	//�Ƿ��Ѿ�Ԥ������ĳ��sheet������
	BOOL          m_bAlreadyPreload;
	//Create the SAFEARRAY from the VARIANT ret.
	COleSafeArray m_sOleSafeArray;
};
