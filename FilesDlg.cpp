// FilesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "FilesDlg.h"
#include "afxdialogex.h"


// CFilesDlg 对话框

IMPLEMENT_DYNAMIC(CFilesDlg, CDialogEx)

CFilesDlg::CFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilesDlg::IDD, pParent)
{

}

CFilesDlg::~CFilesDlg()
{
}

void CFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_mfcShellList);
	DDX_Control(pDX, IDC_EDIT1, m_fileEdit);
	DDX_Control(pDX, IDC_SEARCH, m_searchPic);
}


BEGIN_MESSAGE_MAP(CFilesDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_MFCSHELLLIST1, &CFilesDlg::OnNMClickMfcshelllist1)
	ON_STN_CLICKED(IDC_SEARCH, &CFilesDlg::OnStnClickedSearch)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT1, &CFilesDlg::OnEnKillfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT1, &CFilesDlg::OnEnSetfocusEdit1)
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CFilesDlg 消息处理程序


BOOL CFilesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//mfcshelllist
	m_mfcShellList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	//替换中文列名并改变列宽及居中显示
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	LPWSTR sName[4] = { _T("名称"), _T("大小"), _T("类型"), _T("修改") };
	int nWidth[4] = { 100, 60, 100, 200 };

	for (int i = 0; i < 4; i++)
	{
		lvColumn.pszText = sName[i];
		lvColumn.cx = nWidth[i];
		switch (i)
		{
		case 0:
		case 2:
		case 3:
			lvColumn.fmt = LVCFMT_LEFT;
			break;
		case 1:
			lvColumn.fmt = LVCFMT_RIGHT;
		default:
			break;
		}
		m_mfcShellList.SetColumn(i, &lvColumn);
	}
	m_mfcShellList.DisplayFolder(theApp.r->myfileRecvPath);
	m_mfcShellList.EnableShellContextMenu(TRUE);
	m_mfcShellList.SetItemTypes(m_mfcShellList.GetItemTypes() | SHCONTF_CHECKING_FOR_CHILDREN);
	m_mfcShellList.SetExtendedStyle(m_mfcShellList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	//search按钮
	HICON hIcon;
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEARCH), IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_searchPic.SetIcon(hIcon);

	//搜索框提示
	m_fileEdit.SetWindowTextW(_T("在这里搜索"));
	m_fileEdit_flag = false;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CFilesDlg::OnNMClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码


	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
/*
	CString CurrName;
	CString Filename;
	if (pNMListView->iItem != -1)
	{
		//获取名称

		CurrName = m_mfcShellList.GetItemText(pNMListView->iItem,
			pNMListView->iSubItem);

		//获取路径
		m_mfcShellList.GetItemPath(Filename, pNMListView->iItem);
		m_fileEdit.SetWindowText(Filename);

	}

*/


	*pResult = 0;
}


void CFilesDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	CFilesDlg::OnStnClickedSearch();
//	CDialogEx::OnOK();
}


void CFilesDlg::OnStnClickedSearch()
{
	// TODO:  在此添加控件通知处理程序代码
	//搜索列表
	m_mfcShellList.Refresh();		//刷新
	CString CurrName;
	CString FileName;
	m_fileEdit.GetWindowTextW(CurrName);
	if (!m_fileEdit_flag)
	{
		CurrName = _T("");
	}

	for (int i = 0; i < m_mfcShellList.GetItemCount(); i++)
	{
		if (m_mfcShellList.GetItemText(i, 0).Find(CurrName) == -1)
		{
			m_mfcShellList.DeleteItem(i--);
		}
		else
		{
			continue;
		}
	}


}



/*//常用函数
for (int i = 0; i < m_mfcShellList.GetItemCount(); i++)
{
	if (m_mfcShellList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
	{
		FileName = m_mfcShellList.GetItemText(i, 0);
		AfxMessageBox(FileName);
	}
}
*/


HBRUSH CFilesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (!m_fileEdit_flag)
		pDC->SetTextColor(RGB(192, 192, 192));                  //灰色
	else
		pDC->SetTextColor(RGB(0, 0, 255));   //蓝色



	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CFilesDlg::OnEnKillfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strText = _T("");
	CEdit *pEdit = &m_fileEdit;
	pEdit->GetWindowText(strText);
	if (strText.IsEmpty())
	{
		pEdit->SetWindowText(_T("在这里搜索"));
		m_fileEdit_flag = false;
	}
//	m_fileEdit_flag = false;

}


void CFilesDlg::OnEnSetfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_fileEdit_flag = true;
	CString strText = _T("");
	CEdit *pEdit = &m_fileEdit;
	pEdit->GetWindowText(strText);
	if (strText.Compare(_T("在这里搜索")) == 0)
		pEdit->SetWindowText(_T(""));
}


//BOOL CFilesDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO:  在此添加专用代码和/或调用基类
//	return CDialogEx::OnNotify(wParam, lParam, pResult);
//}


//void CFilesDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
//	m_mfcShellList.Refresh();
//	// TODO:  在此处添加消息处理程序代码
//}
