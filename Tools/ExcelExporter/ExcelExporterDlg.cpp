#include "stdafx.h"
#include "ExcelExporter.h"
#include "ExcelExporterDlg.h"
#include "afxdialogex.h"
#include "IllusionExcelFile.h"
#include "Pattern/C++/Boss.h"

#define STRUCT_FORMAT "#ifndef CFG_%s_H\r\n\
#define CFG_%s_H\r\n\
\r\n\
/************************************************************************/\r\n\
/* Author: daijunhua                                                    */\r\n\
/************************************************************************/\r\n\
#include \"HawkUtil.h\"\r\n\
\r\n\
#pragma pack(push)\r\n\
#pragma pack(1)\r\n\
\r\n\
struct %s\r\n\
{\r\n\
%s\
};\r\n\
\r\n\
#pragma pack(pop)\r\n\
\r\n\
#endif\r\n"

CExcelExporterDlg::CExcelExporterDlg(CWnd* pParent)
	: CDialogEx(CExcelExporterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExcelExporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXCEL, m_hExcelList);
}

BEGIN_MESSAGE_MAP(CExcelExporterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CExcelExporterDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CExcelExporterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

#ifdef _DEBUG
	HawkBinCfgFile file;
	if (file.LoadCfgData("BinCfg/Boss.bin"))
	{
		Int32 iCount = 0;
		if(!file.Read(&iCount,sizeof(iCount)))
			return false;

		for (Int32 i=0;i<iCount;i++)
		{
			Boss sCfg;
			if (!file.Read(&sCfg,sizeof(Boss)))
				return false;
		}
	}
	else
	{
		OutputDebugString("Load BinCfg Error.");
	}
#endif

	m_hExcelList.ModifyStyle(0,LVS_REPORT);
	m_hExcelList.SetExtendedStyle(
		m_hExcelList.GetExtendedStyle()|	
		LVS_EX_GRIDLINES|
		LVS_ALIGNLEFT|
		LVS_EX_FULLROWSELECT|
		LVS_EX_SUBITEMIMAGES);
	
	CHeaderCtrl* pHeader = m_hExcelList.GetHeaderCtrl();
	m_hExcelList.InsertColumn(0,"文件索引",LVCFMT_LEFT);
	m_hExcelList.InsertColumn(1,"文件路径",LVCFMT_LEFT);
	m_hExcelList.SetColumnWidth(0, 80);
	m_hExcelList.SetColumnWidth(1, 165);

	CenterWindow();
	ShowWindow(SW_SHOWNORMAL);	

	HawkFolder sFolder;
	if (sFolder.Open("Excels/", false))
	{
		HawkFolder::FileList::iterator it = sFolder.GetFolderPtr()->Files.begin();
		for (;it != sFolder.GetFolderPtr()->Files.end(); it++)
		{
			if (it->Name.find(".xls") != AString::npos && it->Name.find("~") == AString::npos)
			{
				m_vExcelFiles.push_back("Excels/"+it->Name);

				int iRow = m_hExcelList.GetItemCount();
				m_hExcelList.InsertItem(iRow, HawkStringUtil::IntToString<AString>(iRow + 1).c_str());
				m_hExcelList.SetItemText(iRow, 1, m_vExcelFiles[m_vExcelFiles.size()-1].c_str());
			}			
		}
	}

	return TRUE;
}

void CExcelExporterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CExcelExporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CExcelExporterDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedOk();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CExcelExporterDlg::OnBnClickedOk()
{
	AString sWorkDir = HawkOSOperator::GetWorkDir();
	HawkStringUtil::Replace<AString>(sWorkDir, "\\", "/");
	if (m_vExcelFiles.size())
	{
		for (Size_t i=0;i<m_vExcelFiles.size();i++)
		{
			AString sFullPath = sWorkDir + "/" + m_vExcelFiles[i];
			IllusionExcelFile sExcel;
			if (sExcel.OpenExcelFile(sFullPath.c_str()))
			{
				for (Int32 j=0;j<sExcel.GetSheetCount();j++)
				{
					if (sExcel.LoadSheetByIdx(j+1, TRUE) && !ExportExcel(sExcel, sExcel.GetSheetName(j+1).GetBuffer()))
					{
						Char szMsg[PAGE_SIZE] = {0};
						sprintf(szMsg, "导出【%s】失败, Page: %d, Sheet: %s.", m_vExcelFiles[i].c_str(), j+1, sExcel.GetSheetName(j+1).GetBuffer());
						MessageBox(szMsg, "《错误》", MB_OK | MB_ICONERROR);
						return;
					}
				}				
			}
			else
			{
				Char szMsg[PAGE_SIZE] = {0};
				sprintf(szMsg, "打开【%s】失败.", m_vExcelFiles[i].c_str());
				MessageBox(szMsg, "《错误》", MB_OK | MB_ICONERROR);
				return;
			}
		}

		MessageBox("$^__^$导出Excel数据成功$^__^$", "《成功》", MB_OK);
	}	
}

Bool CExcelExporterDlg::ExportExcel(IllusionExcelFile& sExcel, const AString& sSheetName)
{	
	int iRow = sExcel.GetRowCount();
	int iCol = sExcel.GetColumnCount();
	
	//第一行: 数据类型(int, float, uchar[n])
	//第二行: 字段名
	//第三行: 字段注释
	HawkAssert(iRow >= 3 && iCol > 0);	
	if (iRow >= 3 && iCol > 0)
	{
		CString		  sVariable;
		AStringVector vTypes;
		AString sSheet = sSheetName;
		HawkAssert(sSheet.size());
		if (!sSheet.size())
			return false;

		//计算导出的数据格式
		for (int i=1;i<=iCol;i++)
		{
			if (!sExcel.GetCellString(1, i).GetLength() || !sExcel.GetCellString(2, i).GetLength())
				return false;

			AString sTypeName = sExcel.GetCellString(1, i).GetBuffer(0);
			AString sVarName  = sExcel.GetCellString(2, i).GetBuffer(0);
			AString sVarDesc  = sExcel.GetCellString(3, i).GetBuffer(0);

			vTypes.push_back(sTypeName);
			if (sTypeName != "int" && sTypeName != "float" && sTypeName.find("uchar") == AString::npos)
				return false;

			//字符数组转换-> unsigned char ***[n]
			if ( sTypeName.find("uchar") != AString::npos)
			{
				int iCap = HawkStringUtil::StringToInt<AString>(sTypeName.c_str() + strlen("uchar["));
				sVariable.Format("%s\t//%s\r\n\tunsigned char %s[%d];\r\n", CString(sVariable).GetBuffer(0), sVarDesc.c_str(), sVarName.c_str(), iCap);
			}
			else
			{
				sVariable.Format("%s\t//%s\r\n\t%s %s;\r\n", CString(sVariable).GetBuffer(0), sVarDesc.c_str(), sTypeName.c_str(), sVarName.c_str());
			}
		}

		//保存原始名字
		AString sSheetName = sSheet;
		HawkStringUtil::UpCase<AString>(sSheet);

		//格式化导出模式文件
		CString sStructFmt;
		sStructFmt.Format(STRUCT_FORMAT, sSheet.c_str(), sSheet.c_str(), sSheetName.c_str(), sVariable.GetBuffer(0));

		OutputDebugString(sStructFmt.GetBuffer(0));

		//存储模式文件
		HawkDiskFile struct_file;
		char szExportFile[PAGE_SIZE] = {0};
		sprintf(szExportFile, "Pattern/C++/%s.h", sSheetName.c_str());
		_chmod(szExportFile, _S_IREAD | _S_IWRITE);
		if (struct_file.Open(szExportFile, HawkFile::OPEN_WRITE))
		{
			struct_file.Write(sStructFmt.GetBuffer(0), sStructFmt.GetLength());
			struct_file.Close();
		}
		else
		{
			return false;
		}

		//二进制excel数据
		OctetsStream xOS;
		//记录项数目
		Int32 iCount = iRow - 3;
		xOS.Push<Int32>(iCount);

		for (int i=4; i<=iRow; i++)
		{
			for (int j=1;j<=iCol;j++)
			{
				AString sCellText = sExcel.GetCellString(i, j).GetBuffer(0);				
				if (vTypes[j-1] == "int")
				{
					if (!sCellText.size())
						sCellText = "0";

					Int32 iVal = HawkStringUtil::StringToInt<AString>(sCellText);
					xOS.Push<Int32>(iVal);
				}
				else if (vTypes[j-1] == "float")
				{
					if (!sCellText.size())
						sCellText = "0";

					Float fVal = HawkStringUtil::StringToFloat<AString>(sCellText);
					xOS.Push<Float>(fVal);
				}
				else if (vTypes[j-1].find("uchar") != AString::npos)
				{
					UString sVal = HawkStringUtil::ToUtf8(sCellText);
					int iCap = HawkStringUtil::StringToInt<AString>(vTypes[j-1].c_str() + strlen("uchar["));
					UChar* pBuf = new UChar[iCap];
					memset(pBuf, 0, iCap);
					memcpy(pBuf, sVal.c_str(), sVal.size());					
					xOS.Push(pBuf, iCap);
					HAWK_DELETE_ARRAY(pBuf);
				}
			}
		}

		//数据压缩
		UInt32 iSrcSize = xOS.Size();
		ULong lComSize  = HawkZip::GetRequiredSize(iSrcSize);
		Char* pComBuf   = new Char[lComSize];
		memset(pComBuf, 0, lComSize);
		HawkScope::DataArrayPtr scope(pComBuf);
		if (!HawkZip::Compress(pComBuf, lComSize, xOS.Begin(), iSrcSize))
			return false;

		//压缩后做位反运算
		for (ULong i=0;i<lComSize;i++)
			pComBuf[i] = (~pComBuf[i]);

		//压缩后的CRC校验
		UInt32 iCrc = HawkOSOperator::CalcCrc((const UChar*)pComBuf, (UInt32)lComSize);

		HawkDiskFile bin_file;
		sprintf(szExportFile, "BinCfg/%s.bin", sSheetName.c_str());
		_chmod(szExportFile, _S_IREAD | _S_IWRITE);
		if (bin_file.Open(szExportFile, HawkFile::OPEN_WRITE))
		{
			bin_file.Write(&iSrcSize, sizeof(iSrcSize));
			bin_file.Write(&iCrc, sizeof(iCrc));
			bin_file.Write(pComBuf, lComSize, true);
			bin_file.Close();
		}
		else
		{
			return false;
		}

		return true;
	}	

	return false;
}
