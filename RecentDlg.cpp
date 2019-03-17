// RecentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "RecentDlg.h"
#include "afxdialogex.h"
#include "MessageTest3Dlg.h"


// RecentDlg 对话框

IMPLEMENT_DYNAMIC(CRecentDlg, CDialogEx)

CRecentDlg::CRecentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecentDlg::IDD, pParent)
{

}

CRecentDlg::~CRecentDlg()
{
}

void CRecentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_list);
}


BEGIN_MESSAGE_MAP(CRecentDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CRecentDlg::OnNMDblclkList)
	ON_MESSAGE(WM_RECVDATA, OnRecvData)				//1
END_MESSAGE_MAP()


// RecentDlg 消息处理程序


BOOL CRecentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	

	//listctrl
	m_list.InsertColumn(0, _T("头像"), LVCFMT_LEFT, 50);		//虚列
	m_list.InsertColumn(1, _T("昵称"), LVCFMT_LEFT, 150);
	m_list.InsertColumn(2, _T("状态"), LVCFMT_LEFT, 77);
	m_list.InsertColumn(3, _T("findex"), LVCFMT_LEFT, 0);	//对应friend类index
//	m_list.InsertColumn(3, _T("id"), LVCFMT_LEFT, 0);
	//设置属性 整行选中状态  显示网格
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_list.GetHeaderCtrl()->EnableWindow(false);
	char query[200] = "select im_user.userHeadIcon,im_user.userId,im_user.userStatus"
		" from im_recent,im_user"
		" where im_recent.tRecentFriendId = im_user.tUserId"
		" and im_recent.tUserId = ";
/*
	char query[200] = "select im_recent.userHeadIcon,im_recent.friendId,im_recent.level"
		" from im_recent"
		" where im_recent.tUserId = ";*/
	strcat_s(query, theApp.tUserId);
//	m_list.showRecentFriendList(query);
//	m_list.showFriendList(query);
	











	//AutoAdjustColumnWidth(&m_list);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//列表自适应
/*
void CRecentDlg::AutoAdjustColumnWidth(CListCtrl *pListCtrl)
{
	pListCtrl->SetRedraw(FALSE);
	CHeaderCtrl *pHeader = pListCtrl->GetHeaderCtrl();
	int nColumnCount = pHeader->GetItemCount();


	for (int i = 0; i < nColumnCount; i++)
	{
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = pListCtrl->GetColumnWidth(i);
		pListCtrl->SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = pListCtrl->GetColumnWidth(i);


		pListCtrl->SetColumnWidth(i, max(nColumnWidth, nHeaderWidth) + 5);
	}
	pListCtrl->SetRedraw(TRUE);
}*/

//禁止控件列宽可变    
/*
BOOL CRecentDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;
	switch (((NMHDR*)lParam)->code)
	{
	case HDN_BEGINTRACKW://拖动    
	case HDN_BEGINTRACKA:
		if (pHDN->iItem == 0)  // Col—设定为自己不想改变的列值，比如Col=0，就是第1列    
		{
			*pResult = TRUE;
			return TRUE;
		}
		break;
	}
	return CRecentDlg::OnNotify(wParam, lParam, pResult); //注意这里   
}

*/


void CRecentDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	//弹出聊天窗口
	//传jid到父窗口GetParent()->GetSafeHwnd()
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	static int findex;
	findex = _ttoi(fIndexStr);
	theApp.friends[findex].chatFlag = true;
	m_list.delRecentFriend(findex, nIndex);

/*
	for (i = 1; i < theApp.friendNum; i++)
	{
		if (theApp.friends[i].friendId == friendId)
		{
			theApp.friends[i].chatFlag = true;
			break;
		}
	}*/

	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVDATA, (WPARAM)findex, (LPARAM)(LPCTSTR)"");
	*pResult = 0;
}



LRESULT CRecentDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	m_list.addRecentFriend((int)wParam);
	return 1;
}