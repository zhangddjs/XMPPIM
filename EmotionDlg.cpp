// EmotionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "EmotionDlg.h"
#include "afxdialogex.h"


// CEmotionDlg 对话框

IMPLEMENT_DYNAMIC(CEmotionDlg, CDialogEx)

CEmotionDlg::CEmotionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmotionDlg::IDD, pParent)
{
}

CEmotionDlg::~CEmotionDlg()
{
}

void CEmotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST5, m_list3);
	DDX_Control(pDX, IDC_LIST6, m_list4);
}


BEGIN_MESSAGE_MAP(CEmotionDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CEmotionDlg::OnLvnItemchangedList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CEmotionDlg::OnLvnItemchangedList2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST5, &CEmotionDlg::OnLvnItemchangedList5)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST6, &CEmotionDlg::OnLvnItemchangedList6)
//	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CEmotionDlg 消息处理程序


BOOL CEmotionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(&CWnd::wndTop, re.TopLeft().x, re.TopLeft().y - 120, 0, 0, SWP_NOSIZE);
	// TODO:  在此添加额外的初始化
// 	m_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 50);		//虚列
// 	m_list.InsertColumn(1, _T(""), LVCFMT_LEFT, 50);
// 	m_list.InsertColumn(2, _T(""), LVCFMT_LEFT, 50);
// 	m_list.InsertColumn(3, _T(""), LVCFMT_LEFT, 50);	//对应friend类index
// 	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES);
// 	m_list.GetHeaderCtrl()->EnableWindow(false);
// 	int listrow = 0;
// 	m_list.InsertItem(0, _T("^_^"));
// 	m_list.SetItemText(0, 1, _T("~_~"));
// 	m_list.SetItemText(0, 2, _T(">_<"));
// 	m_list.SetItemText(0, 3, _T("T_T"));
// 	m_list.InsertItem(1, _T("X_X"));
// 	m_list.SetItemText(1, 1, _T("^0^"));
// 	m_list.SetItemText(1, 2, _T("+_+"));
// 	m_list.SetItemText(1, 3, _T("←_←"));
// 	m_list.InsertItem(2, _T("=_="));
// 	m_list.SetItemText(2, 1, _T("-_-"));
// 	m_list.SetItemText(2, 2, _T("O_O"));
// 	m_list.SetItemText(2, 3, _T("U_U"));
	m_list.InsertColumn(0, _T(""), LVCFMT_CENTER, 50);
	m_list2.InsertColumn(0, _T(""), LVCFMT_CENTER, 50);
	m_list3.InsertColumn(0, _T(""), LVCFMT_CENTER, 50);
	m_list4.InsertColumn(0, _T(""), LVCFMT_CENTER, 50);
	// 	m_list.InsertColumn(2, _T(""), LVCFMT_LEFT, 50);
	// 	m_list.InsertColumn(3, _T(""), LVCFMT_LEFT, 50);	//对应friend类index
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);
	m_list.GetHeaderCtrl()->EnableWindow(false);
	m_list2.SetExtendedStyle(m_list2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);
	m_list2.GetHeaderCtrl()->EnableWindow(false);
	m_list3.SetExtendedStyle(m_list3.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);
	m_list3.GetHeaderCtrl()->EnableWindow(false);
	m_list4.SetExtendedStyle(m_list4.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);
	m_list4.GetHeaderCtrl()->EnableWindow(false);
	m_list.InsertItem(0, _T("^_^"));
	m_list.InsertItem(1, _T("X_X"));
	m_list.InsertItem(2, _T("=_="));
	m_list2.InsertItem(0, _T("~_~"));
	m_list2.InsertItem(1, _T("^0^"));
	m_list2.InsertItem(2, _T("-_-"));
	m_list3.InsertItem(0, _T(">_<"));
	m_list3.InsertItem(1, _T("+_+"));
	m_list3.InsertItem(2, _T("O_O"));
	m_list4.InsertItem(0, _T("T_T"));
	m_list4.InsertItem(1, _T("←_←"));
	m_list4.InsertItem(2, _T("U_U"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CEmotionDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	emo = m_list.GetItemText(nIndex, 0);
	*pResult = 0;
	CEmotionDlg::OnOK();
}
void CEmotionDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	emo = m_list2.GetItemText(nIndex, 0);
	*pResult = 0;
	CEmotionDlg::OnOK();
}
void CEmotionDlg::OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	emo = m_list3.GetItemText(nIndex, 0);
	*pResult = 0;
	CEmotionDlg::OnOK();
}
void CEmotionDlg::OnLvnItemchangedList6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	emo = m_list4.GetItemText(nIndex, 0);
	*pResult = 0;
	CEmotionDlg::OnOK();
}

