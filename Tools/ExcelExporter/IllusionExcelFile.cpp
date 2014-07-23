#include "StdAfx.h"
#include "IllusionExcelFile.h"

static CApplication glb_ExcelApp;

COleVariant covTrue((short)TRUE);
COleVariant covFalse((short)FALSE);
COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  

IllusionExcelFile::IllusionExcelFile() : m_bAlreadyPreload(FALSE)
{
}

IllusionExcelFile::~IllusionExcelFile()
{ 
    CloseExcelFile();
}

BOOL IllusionExcelFile::InitExcel()
{
    //创建Excel 2000服务器(启动Excel) 
    if (!glb_ExcelApp.CreateDispatch(_T("Excel.Application"),NULL)) 
    { 
        AfxMessageBox(_T("创建Excel服务失败,你可能没有安装EXCEL，请检查!")); 
        return FALSE;
    }

    glb_ExcelApp.put_DisplayAlerts(FALSE); 
    return TRUE;
}

void IllusionExcelFile::ReleaseExcel()
{
    glb_ExcelApp.Quit();
    glb_ExcelApp.ReleaseDispatch();
    glb_ExcelApp = NULL;
}

BOOL IllusionExcelFile::OpenExcelFile(const char* szFileName)
{
    //先关闭
    CloseExcelFile();
    
    //利用模板文件建立新文档 
    m_sExcelBooks.AttachDispatch(glb_ExcelApp.get_Workbooks(), true); 

    LPDISPATCH lpDis = m_sExcelBooks.Add(COleVariant(szFileName)); 
    if (lpDis)
    {
        m_sWorkBook.AttachDispatch(lpDis); 
        m_sExcelSheets.AttachDispatch(m_sWorkBook.get_Worksheets(), true);
        m_sExcelFile = szFileName;
        return TRUE;
    }
    
    return FALSE;
}

//关闭打开的Excel 文件,默认情况不保存文件
void IllusionExcelFile::CloseExcelFile(BOOL bSave)
{
    //如果已经打开，关闭文件
    if (m_sExcelFile.IsEmpty() == FALSE)
    {
        //如果保存,交给用户控制,让用户自己存，如果自己SAVE，会出现莫名的等待
        if (bSave)
        {
            ShowInExcel(TRUE);
        }
        else
        {
            m_sWorkBook.Close(covFalse, COleVariant(m_sExcelFile), covOptional);
            m_sExcelBooks.Close();
        }

        //打开文件的名称清空
        m_sExcelFile.Empty();
    }

    m_sExcelSheets.ReleaseDispatch();
    m_sWorkSheet.ReleaseDispatch();
    m_sCurRange.ReleaseDispatch();
    m_sWorkBook.ReleaseDispatch();
    m_sExcelBooks.ReleaseDispatch();
}

void IllusionExcelFile::SaveasXSLFile(const CString& sXlsFile)
{
    m_sWorkBook.SaveAs(COleVariant(sXlsFile),
        covOptional,
        covOptional,
        covOptional,
        covOptional,
        covOptional,
        0,
        covOptional,
        covOptional,
        covOptional,
        covOptional,
        covOptional);

    return;
}


int IllusionExcelFile::GetSheetCount()
{
    return m_sExcelSheets.get_Count();
}

CString IllusionExcelFile::GetSheetName(long iTableIndex)
{
    CWorksheet sSheet;
    sSheet.AttachDispatch(m_sExcelSheets.get_Item(COleVariant((long)iTableIndex)), true);
    CString sSheetName = sSheet.get_Name();
    sSheet.ReleaseDispatch();
    return sSheetName;
}

BOOL IllusionExcelFile::LoadSheetByIdx(long iTableIndex,BOOL bPreLoad)
{
    m_sCurRange.ReleaseDispatch();
    m_sWorkSheet.ReleaseDispatch();

    LPDISPATCH lpDis = m_sExcelSheets.get_Item(COleVariant((long)iTableIndex));
    if (!lpDis)
		return FALSE;

	m_sWorkSheet.AttachDispatch(lpDis, true);
	m_sCurRange.AttachDispatch(m_sWorkSheet.get_Cells(), true);
    
    m_bAlreadyPreload = FALSE;
    if (bPreLoad)
    {
        PreLoadSheet();
        m_bAlreadyPreload = TRUE;
    }

    return TRUE;
}

BOOL IllusionExcelFile::LoadSheetByName(const char* szSheet,BOOL bPreLoad)
{
    m_sCurRange.ReleaseDispatch();
    m_sWorkSheet.ReleaseDispatch();

    LPDISPATCH lpDis = m_sExcelSheets.get_Item(COleVariant(szSheet));
    if (!lpDis)
		return FALSE;
  
	m_sWorkSheet.AttachDispatch(lpDis, true);
	m_sCurRange.AttachDispatch(m_sWorkSheet.get_Cells(), true);

    m_bAlreadyPreload = FALSE;
    if (bPreLoad)
    {
        m_bAlreadyPreload = TRUE;
        PreLoadSheet();
    }

    return TRUE;
}

int IllusionExcelFile::GetColumnCount()
{
    CRange sRange;
    CRange sUsedRange;
    sUsedRange.AttachDispatch(m_sWorkSheet.get_UsedRange(), true);
    sRange.AttachDispatch(sUsedRange.get_Columns(), true);
    int iCount = sRange.get_Count();
    sUsedRange.ReleaseDispatch();
    sRange.ReleaseDispatch();
    return iCount;
}

int IllusionExcelFile::GetRowCount()
{
	CRange sRange;
	CRange sUsedRange;
    sUsedRange.AttachDispatch(m_sWorkSheet.get_UsedRange(), true);
    sRange.AttachDispatch(sUsedRange.get_Rows(), true);
    int iCount = sRange.get_Count();
    sUsedRange.ReleaseDispatch();
    sRange.ReleaseDispatch();
    return iCount;
}

BOOL IllusionExcelFile::IsCellString(long iRow, long iColumn)
{
    CRange sRange;
    sRange.AttachDispatch(m_sCurRange.get_Item (COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);

    COleVariant vResult = sRange.get_Value2();

    //VT_BSTR标示字符串
    if(vResult.vt == VT_BSTR)       
        return TRUE;

    return FALSE;
}

BOOL IllusionExcelFile::IsCellInt(long iRow, long iColumn)
{
    CRange sRange;
    sRange.AttachDispatch(m_sCurRange.get_Item (COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
    COleVariant vResult = sRange.get_Value2();

    //好像一般都是VT_R8
    if(vResult.vt == VT_INT || vResult.vt == VT_R8)       
        return TRUE;

    return FALSE;
}

CString IllusionExcelFile::GetCellString(long iRow, long iColumn)
{
    COleVariant vResult ;
    CString		sValue;

    //字符串
    if (m_bAlreadyPreload == FALSE)
    {
        CRange sRange;
        sRange.AttachDispatch(m_sCurRange.get_Item (COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vResult = sRange.get_Value2();
        sRange.ReleaseDispatch();
    }
    //如果数据依据预先加载了
    else
    {
		long lReadAddr[2] = {0};
        VARIANT sVar;
        lReadAddr[0] = iRow;
        lReadAddr[1] = iColumn;
        m_sOleSafeArray.GetElement(lReadAddr, &sVar);
        vResult = sVar;
    }

    if(vResult.vt == VT_BSTR)
    {
        sValue = vResult.bstrVal;
    }
    //整数
    else if (vResult.vt==VT_INT)
    {
        sValue.Format("%d", vResult.pintVal);
    }
    //8字节的数字 
    else if (vResult.vt == VT_R8)     
    {
        sValue.Format("%0.0f", vResult.dblVal);
    }
    //时间格式
    else if(vResult.vt==VT_DATE)    
    {
        SYSTEMTIME sSysTm;
        VariantTimeToSystemTime(vResult.date, &sSysTm);
        sValue = CTime(sSysTm).Format("%Y-%m-%d");

    }
    //单元格空的
    else if(vResult.vt==VT_EMPTY)   
    {
        sValue = "";
    }  

    return sValue;
}

double IllusionExcelFile::GetCellDouble(long iRow, long iColumn)
{
    double dbValue = 0;
    COleVariant vResult;

    //字符串
    if (m_bAlreadyPreload == FALSE)
    {
        CRange sRange;
		sRange.AttachDispatch(m_sCurRange.get_Item (COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vResult = sRange.get_Value2();
        sRange.ReleaseDispatch();
    }
    //如果数据依据预先加载了
    else
    {
		long lReadAddr[2] = {0};
		VARIANT sVar;
		lReadAddr[0] = iRow;
		lReadAddr[1] = iColumn;
		m_sOleSafeArray.GetElement(lReadAddr, &sVar);
		vResult = sVar;
    }
    
    if (vResult.vt==VT_R8)     
    {
        dbValue = vResult.dblVal;
    }
    
    return dbValue;
}

int IllusionExcelFile::GetCellInt(long iRow, long iColumn)
{
    int iValue = 0;
    COleVariant vResult;

    if (m_bAlreadyPreload == FALSE)
    {
        CRange sRange;
        sRange.AttachDispatch(m_sCurRange.get_Item (COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
        vResult = sRange.get_Value2();
        sRange.ReleaseDispatch();
    }
    else
    {
		long lReadAddr[2] = {0};
		VARIANT sVar;
		lReadAddr[0] = iRow;
		lReadAddr[1] = iColumn;
		m_sOleSafeArray.GetElement(lReadAddr, &sVar);
		vResult = sVar;
    }

    iValue = static_cast<int>(vResult.dblVal);

    return iValue;
}

void IllusionExcelFile::SetCellString(long iRow, long iColumn, const CString& sValue)
{
    COleVariant new_value(sValue);
    CRange start_range = m_sWorkSheet.get_Range(COleVariant("A1"), covOptional);
    CRange write_range = start_range.get_Offset(COleVariant((long)iRow -1), COleVariant((long)iColumn -1) );
    write_range.put_Value2(new_value);
    start_range.ReleaseDispatch();
    write_range.ReleaseDispatch();
}

void IllusionExcelFile::SetCellInt(long iRow, long iColumn, int iValue)
{
    COleVariant new_value((long)iValue);    
    CRange start_range = m_sWorkSheet.get_Range(COleVariant("A1"), covOptional);
    CRange write_range = start_range.get_Offset(COleVariant((long)iRow -1),COleVariant((long)iColumn -1) );
    write_range.put_Value2(new_value);
    start_range.ReleaseDispatch();
    write_range.ReleaseDispatch();
}

void IllusionExcelFile::ShowInExcel(BOOL bShow)
{
    glb_ExcelApp.put_Visible(bShow);
    glb_ExcelApp.put_UserControl(bShow);
}

CString IllusionExcelFile::GetOpenFileName()
{
    return m_sExcelFile;
}

CString IllusionExcelFile::GetLoadSheetName()
{
    return m_sWorkSheet.get_Name();
}

void IllusionExcelFile::PreLoadSheet()
{
    CRange sRange = m_sWorkSheet.get_UsedRange();
    VARIANT sVal  = sRange.get_Value2();
    if (!(sVal.vt & VT_ARRAY))
        return;
    
    m_sOleSafeArray.Clear();
    m_sOleSafeArray.Attach(sVal); 
}
