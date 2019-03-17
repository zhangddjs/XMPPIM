// GroupsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "GroupsDlg.h"
#include "afxdialogex.h"
#include <iostream>


// CGroupsDlg 对话框

IMPLEMENT_DYNAMIC(CGroupsDlg, CDialogEx)

CGroupsDlg::CGroupsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupsDlg::IDD, pParent)
{

}

CGroupsDlg::~CGroupsDlg()
{
}

void CGroupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_groupEdit);
	DDX_Control(pDX, IDC_SEARCH, m_searchPic);
	DDX_Control(pDX, IDC_LIST3, m_list);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CGroupsDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT1, &CGroupsDlg::OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CGroupsDlg::OnEnKillfocusEdit1)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CGroupsDlg::OnTvnSelchangedTree1)
	ON_EN_CHANGE(IDC_EDIT1, &CGroupsDlg::OnEnChangeEdit1)
//	ON_NOTIFY(TVN_SINGLEEXPAND, IDC_TREE1, &CGroupsDlg::OnTvnSingleExpandTree1)
//ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &CGroupsDlg::OnTvnItemexpandingTree1)
ON_NOTIFY(NM_CLICK, IDC_TREE1, &CGroupsDlg::OnNMClickTree1)
END_MESSAGE_MAP()


// CGroupsDlg 消息处理程序


BOOL CGroupsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//search按钮
	HICON hIcon;
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEARCH), IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_searchPic.SetIcon(hIcon);

	//搜索框提示
	m_groupEdit.SetWindowTextW(_T("在这里搜索"));
	m_groupEdit_flag = false;
	m_groupEdit_changedflag = false;

	//tree
	char treeQuery[150] = "select im_group_class.groupClass"
		" from im_group_class"
		" where im_group_class.tUserId = ";
	strcat_s(treeQuery, theApp.tUserId);
	m_tree.showTree(treeQuery);


	//列表显示
	//listctrl
	m_list.InsertColumn(0, _T("头像"), LVCFMT_LEFT, 50);		//虚列
	m_list.InsertColumn(1, _T("群名"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("人数"), LVCFMT_LEFT, 50);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_list.GetHeaderCtrl()->EnableWindow(false);

	
	m_tree.SelectItem(m_tree.GetFirstVisibleItem());

	m_groupEdit_changedflag = true;		//总之这样就对了


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CGroupsDlg::OnEnSetfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_groupEdit_flag = true;
	CString strText = _T("");
	CEdit *pEdit = &m_groupEdit;
	pEdit->GetWindowText(strText);
	if (strText.Compare(_T("在这里搜索")) == 0)
		pEdit->SetWindowText(_T(""));
}


void CGroupsDlg::OnEnKillfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strText = _T("");
	CEdit *pEdit = &m_groupEdit;
	pEdit->GetWindowText(strText);
	if (strText.IsEmpty())
	{
		pEdit->SetWindowText(_T("在这里搜索"));
		m_groupEdit_flag = false;
	}
}


HBRUSH CGroupsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (!m_groupEdit_flag)
		pDC->SetTextColor(RGB(192, 192, 192));                  //灰色
	else
		pDC->SetTextColor(RGB(0, 0, 255));   //蓝色
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CGroupsDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	HTREEITEM item = m_tree.GetSelectedItem();
	CString groupClass = m_tree.GetItemText(item);
	CStringA temp(groupClass.GetBuffer(0)); //通过转化，temp接受了原来字符串的多字节形式 
	groupClass.ReleaseBuffer();
	std::string s(temp.GetBuffer(0));
	temp.ReleaseBuffer();
	char query1[300] = "select im_group.groupHeadIcon,im_group.groupName,im_group.groupMemberNum"
		" from im_user_group, im_group, im_group_class"
		" where im_user_group.tGroupId = im_group.tGroupId"
		" and im_user_group.tUserId = ";
	strcat_s(query1, theApp.tUserId);
	char query2[200] = " and im_user_group.tGroupClassId = im_group_class.tGroupClassId"
		" and im_group_class.groupClass = '";
	const char *query3 = s.c_str();
	char query[1000];
	strcpy_s(query, query1);
	strcat_s(query, query2);
	strcat_s(query, query3);
	strcat_s(query, "'");
//	MessageBox(groupClass);
	m_list.showGroupList(query);
	*pResult = 0;
}


void CGroupsDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_groupEdit_changedflag)
	{
		CString CurrName;
		CString GroupName;
		m_groupEdit.GetWindowTextW(CurrName);
		char *query = "select im_group.groupHeadIcon,im_group.groupName,im_group.groupMemberNum"
			" from im_user_group, im_group"
			" where im_user_group.tGroupId = im_group.tGroupId"
			" and im_user_group.tUserId = 7";
		if (!m_groupEdit_flag)
		{
			CurrName = _T("");
		}
		if (CurrName == _T("") || CurrName == _T("在这里搜索"))
		{
			return;
		}

		m_list.showGroupList(query);
		for (int i = 0; i < m_list.GetItemCount(); i++)
		{
			if (m_list.GetItemText(i, 1).Find(CurrName) == -1)
			{
				m_list.DeleteItem(i--);
			}
			else
			{
				continue;
			}
		}
	}
	else
	{
		m_groupEdit_changedflag = true;
	}




}


//void CGroupsDlg::OnTvnSingleExpandTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO:  在此添加控件通知处理程序代码
//	OnTvnSelchangedTree1(pNMHDR, pResult);
//	*pResult = 0;
//}


//void CGroupsDlg::OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO:  在此添加控件通知处理程序代码
//	OnTvnSelchangedTree1(pNMHDR, pResult);
//	*pResult = 0;
//}


void CGroupsDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	OnTvnSelchangedTree1(pNMHDR, pResult);
	*pResult = 0;
}


void CGroupsDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

//	CDialogEx::OnOK();
}
