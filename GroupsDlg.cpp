// GroupsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "GroupsDlg.h"
#include "afxdialogex.h"
#include <iostream>


// CGroupsDlg �Ի���

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


// CGroupsDlg ��Ϣ�������


BOOL CGroupsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//search��ť
	HICON hIcon;
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEARCH), IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_searchPic.SetIcon(hIcon);

	//��������ʾ
	m_groupEdit.SetWindowTextW(_T("����������"));
	m_groupEdit_flag = false;
	m_groupEdit_changedflag = false;

	//tree
	char treeQuery[150] = "select im_group_class.groupClass"
		" from im_group_class"
		" where im_group_class.tUserId = ";
	strcat_s(treeQuery, theApp.tUserId);
	m_tree.showTree(treeQuery);


	//�б���ʾ
	//listctrl
	m_list.InsertColumn(0, _T("ͷ��"), LVCFMT_LEFT, 50);		//����
	m_list.InsertColumn(1, _T("Ⱥ��"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("����"), LVCFMT_LEFT, 50);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_list.GetHeaderCtrl()->EnableWindow(false);

	
	m_tree.SelectItem(m_tree.GetFirstVisibleItem());

	m_groupEdit_changedflag = true;		//��֮�����Ͷ���


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CGroupsDlg::OnEnSetfocusEdit1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_groupEdit_flag = true;
	CString strText = _T("");
	CEdit *pEdit = &m_groupEdit;
	pEdit->GetWindowText(strText);
	if (strText.Compare(_T("����������")) == 0)
		pEdit->SetWindowText(_T(""));
}


void CGroupsDlg::OnEnKillfocusEdit1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strText = _T("");
	CEdit *pEdit = &m_groupEdit;
	pEdit->GetWindowText(strText);
	if (strText.IsEmpty())
	{
		pEdit->SetWindowText(_T("����������"));
		m_groupEdit_flag = false;
	}
}


HBRUSH CGroupsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (!m_groupEdit_flag)
		pDC->SetTextColor(RGB(192, 192, 192));                  //��ɫ
	else
		pDC->SetTextColor(RGB(0, 0, 255));   //��ɫ
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CGroupsDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM item = m_tree.GetSelectedItem();
	CString groupClass = m_tree.GetItemText(item);
	CStringA temp(groupClass.GetBuffer(0)); //ͨ��ת����temp������ԭ���ַ����Ķ��ֽ���ʽ 
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		if (CurrName == _T("") || CurrName == _T("����������"))
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
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	OnTvnSelchangedTree1(pNMHDR, pResult);
//	*pResult = 0;
//}


//void CGroupsDlg::OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	OnTvnSelchangedTree1(pNMHDR, pResult);
//	*pResult = 0;
//}


void CGroupsDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnTvnSelchangedTree1(pNMHDR, pResult);
	*pResult = 0;
}


void CGroupsDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

//	CDialogEx::OnOK();
}
