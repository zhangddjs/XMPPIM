// FriendsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "FriendsDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "FriendAddDlg.h"
#include "EditDlg.h"

// CFriendsDlg 对话框
#define ID_TESTMENU 5000		//菜单宏
IMPLEMENT_DYNAMIC(CFriendsDlg, CDialogEx)

CFriendsDlg::CFriendsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFriendsDlg::IDD, pParent)
{

}

CFriendsDlg::~CFriendsDlg()
{
	delete[] friendItem;
}

void CFriendsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH, m_searchPic);
	DDX_Control(pDX, IDC_EDIT1, m_friendEdit);
	DDX_Control(pDX, IDC_LIST3, m_list);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CFriendsDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT1, &CFriendsDlg::OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CFriendsDlg::OnEnKillfocusEdit1)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CFriendsDlg::OnTvnSelchangedTree1)
//	ON_STN_CLICKED(IDC_SEARCH, &CFriendsDlg::OnStnClickedSearch)
	ON_EN_CHANGE(IDC_EDIT1, &CFriendsDlg::OnEnChangeEdit1)
	ON_MESSAGE(WM_RECVFRIEND, OnRecvFriend)
	ON_MESSAGE(WM_RECVSUBREQ, OnRecvSubReq)			//6接收好友申请
	ON_MESSAGE(WM_RECVREMVED, OnRecvRemved)			//7接收被好友删除
//	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CFriendsDlg::OnNMClickTree1)
//ON_NOTIFY(NM_KILLFOCUS, IDC_LIST3, &CFriendsDlg::OnNMKillfocusList3)
ON_NOTIFY(NM_CLICK, IDC_TREE1, &CFriendsDlg::OnNMClickTree1)
ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CFriendsDlg::OnNMDblclkTree1)
//ON_NOTIFY(NM_KILLFOCUS, IDC_LIST3, &CFriendsDlg::OnNMKillfocusList3)
//ON_NOTIFY(NM_CLICK, IDC_LIST3, &CFriendsDlg::OnNMClickList3)
//ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CFriendsDlg::OnHdnItemdblclickList3)
ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &CFriendsDlg::OnNMDblclkList3)
ON_BN_CLICKED(IDC_BUTTON1, &CFriendsDlg::OnBnClickedButton1)
//ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CFriendsDlg::OnNMRClickTree1)
ON_COMMAND(ID_32771, &CFriendsDlg::OnVCard)
ON_COMMAND(ID_32772, &CFriendsDlg::OnChat)
ON_COMMAND(ID_32774, &CFriendsDlg::OnDelete)
ON_COMMAND(ID_32775, &CFriendsDlg::OnSetNickName)
ON_COMMAND(ID_32776, &CFriendsDlg::OnShield)
ON_COMMAND(ID_32777, &CFriendsDlg::OnMoveTo)
ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CFriendsDlg::OnNMRClickList3)
ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CFriendsDlg::OnNMRClickTree1)
ON_COMMAND(ID_DELETEGROUP, &CFriendsDlg::OnDeletegroup)
ON_COMMAND(ID_CHANGEGNAME, &CFriendsDlg::OnChangegname)
ON_COMMAND(ID_ADDGROUP, &CFriendsDlg::OnAddgroup)
END_MESSAGE_MAP()


// CFriendsDlg 消息处理程序


BOOL CFriendsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//search按钮
	HICON hIcon;
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEARCH), IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_searchPic.SetIcon(hIcon);

	//搜索框提示
	m_friendEdit.SetWindowTextW(_T("在这里搜索"));
	m_friendEdit_flag = false;
	m_friendEdit_changedflag = false;

	//加载好友列表
	//准备HICON图标
	HICON icons[2];
	icons[0] = AfxGetApp()->LoadIconW(IDI_DEFAULTHEADICON);
	//CImageList list;	//必须保存住这个集合  写到.h做成员属性
	//创建图片集合
	list.Create(16, 16, ILC_COLOR32, 4, 4);
	//添加具体的图片
	list.Add(icons[0]);
	m_tree.SetImageList(&list, LVSIL_NORMAL);
	CString friendId;
	CString friendGroupName;
	Sleep(2000);
	int k;
	friendItem[0] = NULL;
	//初始化树空控件
	HTREEITEM defaultGroup = m_tree.InsertItem(_T("Buddies"));
	fGroups[groupNum++] = defaultGroup;

	for (k = 1; k < theApp.friendNum; k++)
	{
		friendId = theApp.friends[k].friendId;
		friendGroupName = theApp.friends[k].friendGroup;
		char* pJid = new char[200];
		sprintf(pJid, "%S", friendId);
		JID jid(pJid);
		theApp.r->getVcard(jid, k);
		delete[] pJid;
		//groupIsExist
		CString Name;
		bool groupFlag = false;
		//判断是否存在该组，不存在则增加分组，存在则获取
		int i = 0;
		HTREEITEM friendGroup = m_tree.GetRootItem();
		
		//CTreeCtrl获取每个根节点文本
		Name = m_tree.GetItemText(friendGroup);
		if (friendGroupName == Name)
		{
			groupFlag = true;
		}
		else
		{
			while (friendGroup = m_tree.GetNextItem(friendGroup, TVGN_NEXT), ++i, friendGroup)
			{
				Name = m_tree.GetItemText(friendGroup);
				if (friendGroupName == Name)
				{
					groupFlag = true;
					break;
				}
			}
		}
		if (!groupFlag)
		{
			friendGroup = m_tree.InsertItem(friendGroupName);
			fGroups[groupNum++] = friendGroup;
		}
	}


	//列表显示
	m_list.InsertColumn(0, _T("头像"), LVCFMT_LEFT, 50);		//虚列
	m_list.InsertColumn(1, _T("昵称"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("状态"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(3, _T("findex"), LVCFMT_LEFT, 0);	//对应friend类index
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
 	m_list.GetHeaderCtrl()->EnableWindow(false);
	m_list.SetImageList(&list, LVSIL_SMALL);
	

	m_friendEdit_changedflag = true;		//总之这样就对了
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
HBRUSH CFriendsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (!m_friendEdit_flag)
		pDC->SetTextColor(RGB(192, 192, 192));                  //灰色
	else
		pDC->SetTextColor(RGB(0, 0, 255));   //蓝色
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
void CFriendsDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
//	CFriendsDlg::OnStnClickedSearch();
//	CDialogEx::OnOK();
}
//void CFriendsDlg::OnStnClickedSearch()
//{
// 	// TODO:  在此添加控件通知处理程序代码
// 	//搜索列表
// 	//刷新
// 	char *query = "select im_user.userHeadIcon,im_user.userName,im_user.userStatus"
// 		" from im_user_friend,im_user"
// 		" where im_user_friend.tFriendId = im_user.tUserId"
// 		" and im_user_friend.tUserId = 7";
// 	m_list.showFriendList(query);
// 
// 	CString CurrName;
// 	CString FriendName;
// 	m_friendEdit.GetWindowTextW(CurrName);
// 	if (!m_friendEdit_flag)
// 	{
// 		CurrName = _T("");
// 	}
// 
// 	for (int i = 0; i < m_list.GetItemCount(); i++)
// 	{
// 		if (m_list.GetItemText(i, 1).Find(CurrName) == -1)
// 		{
// 			m_list.DeleteItem(i--);
// 		}
// 		else
// 		{
// 			continue;
// 		}
// 	}
//}



//搜索栏事件
//光标被搜素栏得到
void CFriendsDlg::OnEnSetfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_friendEdit_flag = true;
	CString strText = _T("");
	CEdit *pEdit = &m_friendEdit;
	pEdit->GetWindowText(strText);
	if (strText.Compare(_T("在这里搜索")) == 0)
		pEdit->SetWindowText(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
//搜索栏失去光标
void CFriendsDlg::OnEnKillfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strText = _T("");
	CEdit *pEdit = &m_friendEdit;
	pEdit->GetWindowText(strText);
	if (strText.IsEmpty())
	{
		pEdit->SetWindowText(_T("在这里搜索"));
		m_friendEdit_flag = false;
	}
}
//搜索栏变化
void CFriendsDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (m_friendEdit_changedflag)
	{

		CString CurrName;
		m_friendEdit.GetWindowTextW(CurrName);
		if (!m_friendEdit_flag)
		{
			CurrName = _T("");
		}
		if (CurrName == _T("") || CurrName == _T("在这里搜索"))
		{
			return;
		}
		m_list.showFriendByFID(CurrName);

	}
	else
	{
		m_friendEdit_changedflag = true;
	}
}
///////////////////////////////////////////////////////////////////////////////

//列表事件
//双击列表
void CFriendsDlg::OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	//弹出聊天会话框
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	static int findex;
	findex = _ttoi(fIndexStr);
	theApp.friends[findex].chatFlag = true;
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVDATA, (WPARAM)findex, (LPARAM)(LPCTSTR)"");
	*pResult = 0;
}
//右击列表  弹出菜单
void CFriendsDlg::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	int findex;
	findex = _ttoi(fIndexStr);
	//右键弹出菜单
	CPoint pt;//用于获取CTreeCtrl右击点在CTreeCtrl的坐标 这里主要因为CTreeCtrl的点击测试的坐标点是基于自身坐标系 （HitTest为CTreeCtrl的成员函数）
	CPoint ptSc;//右击菜单的右上角的位置是基于屏幕坐标系
	UINT  flag;
	GetCursorPos(&pt); //获取当前点击坐标的全局坐标
	ptSc = pt;
	ScreenToClient(&pt);
	MapWindowPoints(&m_list, &pt, 1);//MapWindowPoint  为父类（CDialog）的成员函数,  将坐标系映射为CTreeCtrl的坐标系
	//	m_tree.Select(hItem, TVGN_CARET);//设置点击节点为当前选中节点
	CMenu m, *mn, *submn;
	m.LoadMenu(IDR_TREEMENU1);//加载菜单资源
	mn = m.GetSubMenu(0);//获取菜单子项
	submn = mn->GetSubMenu(5);
	//动态创建子菜单
	bool itemFlag = false;
	for (int i = 1; i < groupNum; i++)
	{
		submn->AppendMenu(0, (UINT)(ID_TESTMENU + i), m_tree.GetItemText(fGroups[i]));
	}
// 	submn->AppendMenu(MF_BYPOSITION | MF_POPUP | MF_STRING,
// 		(UINT)submn->m_hMenu, _T("子菜单"));
	mn->TrackPopupMenu(TPM_LEFTALIGN, ptSc.x, ptSc.y, this);    //显示菜单

	*pResult = 0;
}

//树事件
//单击树
void CFriendsDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	OnTvnSelchangedTree1(pNMHDR, pResult);
	*pResult = 0;
}
//右击树  弹出菜单
void CFriendsDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	CPoint pt;//用于获取CTreeCtrl右击点在CTreeCtrl的坐标 这里主要因为CTreeCtrl的点击测试的坐标点是基于自身坐标系 （HitTest为CTreeCt
	CPoint ptSc;//右击菜单的右上角的位置是基于屏幕坐标系  
	UINT  flag;
	GetCursorPos(&pt); //获取当前点击坐标的全局坐标 
	ptSc = pt;
	ScreenToClient(&pt);
	MapWindowPoints(&m_tree, &pt, 1);//MapWindowPoint  为父类（CDialog）的成员函数,  将坐标系映射为CTreeCtrl的坐标系
	HTREEITEM hItem = m_tree.HitTest(pt, &flag);
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	
	if (NULL != hItem){
		m_tree.Select(hItem, TVGN_CARET);//设置点击节点为当前选中节点
		CMenu m, *mn;
		m.LoadMenu(IDR_TREEMENU1);//加载菜单资源
		mn = m.GetSubMenu(1);//获取菜单子项
		mn->EnableMenuItem(ID_ADDGROUP, MF_GRAYED);
		mn->TrackPopupMenu(TPM_LEFTALIGN, ptSc.x, ptSc.y, this);    //显示菜单
	}
	else
	{
		CMenu m, *mn;
		m.LoadMenu(IDR_TREEMENU1);//加载菜单资源
		mn = m.GetSubMenu(1);//获取菜单子项
		mn->EnableMenuItem(ID_CHANGEGNAME, MF_GRAYED);
		mn->EnableMenuItem(ID_DELETEGROUP, MF_GRAYED);
		mn->TrackPopupMenu(TPM_LEFTALIGN, ptSc.x, ptSc.y, this);    //显示菜单
	}


	*pResult = 0;
}
//双击树 编辑组
void CFriendsDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	//判断是否是父节点，是则返回
	int i = 0;
/*
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	HTREEITEM rootItem = m_tree.GetRootItem();
	if (rootItem == m_hTreeItem)
	{
		*pResult = 0;
		return;
	}
	else
	{
		while (rootItem = m_tree.GetNextItem(rootItem, TVGN_NEXT), ++i, rootItem)
		{
			if (rootItem == m_hTreeItem)
			{
				*pResult = 0;
				return;
			}
		}
	}

	
	static CString friendId;
	friendId = m_tree.GetItemText(m_hTreeItem);
	static int j;
	for (j = 1; j < theApp.friendNum; j++)
	{
		if (theApp.friends[j].friendId == friendId)
		{
			theApp.friends[j].chatFlag = true;
			break;
		}
	}
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVDATA, (WPARAM)j, (LPARAM)(LPCTSTR)"");*/


	*pResult = 0;
}
//组菜单
//删除组按钮
void CFriendsDlg::OnDeletegroup()
{
	// TODO:  在此添加命令处理程序代码
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	CString itemText = m_tree.GetItemText(m_hTreeItem);
	if ("Buddies" == itemText)
	{
		MessageBox(_T("不能删除默认分组"));
		return;
	}
	if (::MessageBox(NULL, _T("确定要删除分组吗？好友将被移到默认分组Buddies下。"), _T("删除分组"), MB_YESNO) == IDYES)
	{
		for (int i = 1; i < theApp.friendNum; i++)
		{
			if (itemText == theApp.friends[i].friendGroup)
			{
				theApp.friends[i].friendGroup = "Buddies";
				StringList groups;
				groups.push_back("Buddies");
				theApp.r->j->rosterManager()->getRosterItem(theApp.r->CStringToJID(theApp.friends[i].friendIdFull))->setGroups(groups);
			}
		}
		theApp.r->j->rosterManager()->synchronize();
		for (int j = 0; j < groupNum; j++)
		{
			if (m_tree.GetItemText(fGroups[j]) == itemText)
			{
				fGroups[j] = fGroups[groupNum - 1];
				fGroups[groupNum - 1] = NULL;
				groupNum--;
				MessageBox(_T("OK"));
				break;
			}
		}
		m_tree.DeleteItem(m_hTreeItem);
	}
}
//修改组名
void CFriendsDlg::OnChangegname()
{
	// TODO:  在此添加命令处理程序代码
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	CString itemText = m_tree.GetItemText(m_hTreeItem);
	CString newItemText;
	if ("Buddies" == itemText)
	{
		MessageBox(_T("不能修改默认分组"));
		return;
	}
	CEditDlg editDlg;
	editDlg.m_edit = itemText;
	if (editDlg.DoModal() == IDOK)
	{
		newItemText = editDlg.m_edit;
		m_tree.SetItemText(m_hTreeItem, newItemText);
		for (int i = 1; i < theApp.friendNum; i++)
		{
			if (itemText == theApp.friends[i].friendGroup)
			{
				theApp.friends[i].friendGroup = newItemText;
				StringList groups;
				groups.push_back(theApp.r->CStringToChar(newItemText));
				theApp.r->j->rosterManager()->getRosterItem(theApp.r->CStringToJID(theApp.friends[i].friendIdFull))->setGroups(groups);
			}
		}
		theApp.r->j->rosterManager()->synchronize();
	}
}
//添加分组
void CFriendsDlg::OnAddgroup()
{
	// TODO:  在此添加命令处理程序代码
	CEditDlg editDlg;
	if (editDlg.DoModal() == IDOK)
	{
		//判断分组是否已添加
		if (editDlg.m_edit == "")
		{
			MessageBox(_T("请输入分组名"));
			return CFriendsDlg::OnAddgroup();
		}
		for (int i = 0; i < groupNum; i++)
		{
			if (editDlg.m_edit == m_tree.GetItemText(fGroups[i]))
			{
				MessageBox(_T("分组已添加"));
				return CFriendsDlg::OnAddgroup();
			}
		}
		fGroups[groupNum++] = m_tree.InsertItem(editDlg.m_edit);
	}
}
//移动分组
BOOL CFriendsDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO:  在此添加专用代码和/或调用基类
	bool flag = false;
	int i;
	UINT uMsg = LOWORD(wParam);
	UINT menuIndex = NULL;
	if (uMsg == ID_MOVETOBUDDIES)
	{
		flag = true;
		menuIndex = uMsg - ID_MOVETOBUDDIES;
	}
	else
	{
		for (i = 1; i < groupNum; i++)
		{
			if (uMsg == ID_TESTMENU + i)
			{
				flag = true;
				menuIndex = uMsg - ID_TESTMENU;
			}
		}
	}
	if (!flag)
	{
		return CDialogEx::OnCommand(wParam, lParam);
	}
	//第二种方法获取List当前选择
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return CDialogEx::OnCommand(wParam, lParam);
	int nIndex = (int)m_list.GetNextSelectedItem(pos);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	int findex;
	findex = _ttoi(fIndexStr);

	//当前选择的菜单对应的group
	CString str_menustring = m_tree.GetItemText(fGroups[menuIndex]);
	if (theApp.friends[findex].friendGroup == str_menustring)
		return CDialogEx::OnCommand(wParam, lParam);
	CString curGroup = theApp.friends[findex].friendGroup;
	theApp.friends[findex].friendGroup = str_menustring;
	StringList groups;
	groups.push_back(theApp.r->CStringToChar(str_menustring));
	theApp.r->j->rosterManager()->getRosterItem(theApp.r->CStringToJID(theApp.friends[findex].friendIdFull))->setGroups(groups);
	theApp.r->j->rosterManager()->synchronize();

	m_list.showFriendByGroup(curGroup);
	//当前选择的菜单
// 	CMenu m, *mn, *submn;
// 	m.LoadMenu(IDR_TREEMENU1);//加载菜单资源
// 	mn = m.GetSubMenu(0);//获取菜单子项
// 	submn = mn->GetSubMenu(5);
// 	CString str_menustring;
// 	submn = GetMenu();
// 	submn->GetMenuString(0, str_menustring, MF_BYPOSITION);
// 	MessageBox(str_menustring);
	return CDialogEx::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
//朋友的状态
HANDLE hFriendsRecvFriendSemaphore;
LRESULT CFriendsDlg::OnRecvFriend(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(hFriendsRecvFriendSemaphore, INFINITE);
	int i = (int)wParam;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	CString groupName = m_tree.GetItemText(m_hTreeItem);
	m_list.showFriendByGroup(groupName);
	ReleaseSemaphore(hFriendsRecvFriendSemaphore, 1, NULL);
	return 1;
}
//切换树
void CFriendsDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	CString groupName;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	groupName = m_tree.GetItemText(m_hTreeItem);
	m_list.showFriendByGroup(groupName);
	*pResult = 0;
}



//添加好友按钮
void CFriendsDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	if (theApp.friendNum >= 50)
	{
		AfxMessageBox(_T("好友已达上限"));
		return;
	}
	CFriendAddDlg *faddDlg = new CFriendAddDlg();
	if (faddDlg->DoModal() == IDOK)
	{
		CString groupName("Buddies");			//之后将同步message
		ReloadTree(groupName);
		HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
		groupName = m_tree.GetItemText(m_hTreeItem);
		m_list.showFriendByGroup(groupName);
	}
	delete faddDlg;
}
//菜单
void CFriendsDlg::OnVCard()
{
	// TODO:  在此添加命令处理程序代码
//	NMLISTVIEW* pMListView = (NMLISTVIEW*)m_list.GetHotItem();
	CString Name;
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	static int findex;
	findex = _ttoi(fIndexStr);
	Name = theApp.friends[findex].friendId;
	char* pFid = new char[200];
	sprintf(pFid, "%S", Name);
	JID jid(pFid);
	delete[] pFid;
	theApp.r->getVcard(jid, findex);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVINFO, NULL, (LPARAM)findex);
//树控件代码
//	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
//	Name = m_tree.GetItemText(m_hTreeItem);
// 	char* pFid = new char[200];
// 	sprintf(pFid, "%S", Name);
// 	JID jid(pFid);
// 	int friendIndex;
// 	for (friendIndex = 0; friendIndex < theApp.friendNum; friendIndex++)
// 	{
// 		if (jid.bare().c_str() == theApp.friends[friendIndex].friendId)
// 		{
// 			break;
// 		}
// 	}
// 	theApp.r->getVcard(jid, friendIndex);
// 	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVINFO, NULL, (LPARAM)friendIndex);
// 	delete pFid;
}
void CFriendsDlg::OnChat()
{
	// TODO:  在此添加命令处理程序代码
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	static int findex;
	findex = _ttoi(fIndexStr);
	theApp.friends[findex].chatFlag = true;
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVDATA, (WPARAM)(LPCTSTR)findex, (LPARAM)(LPCTSTR)"");

}
void CFriendsDlg::OnDelete()		//删除好友，太麻烦了
{
	// TODO:  在此添加命令处理程序代码
	//删除好友
	CString groupName;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	groupName = m_tree.GetItemText(m_hTreeItem);
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	int findex;
	findex = _ttoi(fIndexStr);
	CString FullName(theApp.friends[findex].friendIdFull);
	CString BareName(theApp.friends[findex].friendId);
	CString tipStr;
	tipStr.Format(_T("确定要删除%s吗？"), FullName);
	if (::MessageBox(NULL, tipStr, _T("删除好友"), MB_YESNO) == IDYES)
	{
		if (theApp.friends[findex].chatFlag)		//如果聊天窗口打开，则销毁该窗口
			::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSSCHATWND, NULL, NULL);
		theApp.r->WDDelFriend(FullName);
		theApp.r->WDDelFriend(BareName);
		theApp.delFriend(findex);
		AfxMessageBox(_T("删除成功!"));
		m_list.showFriendByGroup(groupName);
	}

	/*CString Name;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	Name = m_tree.GetItemText(m_hTreeItem);
	if (::MessageBox(NULL, _T("确定要删除吗？"), _T("删除好友"), MB_YESNO) == IDYES)
	{
		theApp.r->WDDelFriend(Name);
		AfxMessageBox(_T("删除成功!"));
		m_tree.DeleteItem(m_hTreeItem);
	}*/
	///////////////////////////////////////////////////////////////////
}
void CFriendsDlg::OnSetNickName()		//设置备注
{
	// TODO:  在此添加命令处理程序代码
	//rosterItem.setName;
	CString friendId;
	CString NoteName;
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//获取对应findex
	int findex;
	findex = _ttoi(fIndexStr);
	friendId = theApp.friends[findex].friendId;

	CEditDlg editDlg;
	editDlg.m_edit = theApp.friends[findex].f_note;
	if (editDlg.DoModal() == IDOK)
	{
		NoteName = editDlg.m_edit;
		JID jid(theApp.r->CStringToJID(friendId));
		theApp.r->setNoteName(jid, NoteName);
		theApp.friends[findex].f_note = NoteName;
		//刷新好友列表
		if (NoteName != "")
			m_list.SetItemText(nIndex, 1, NoteName);
		else if (theApp.friends[findex].nickName != "")
			m_list.SetItemText(nIndex, 1, theApp.friends[findex].nickName);
		else
			m_list.SetItemText(nIndex, 1, friendId);
	}
// 	char* pFid = new char[200];
// 	sprintf(pFid, "%S", NoteName);
// 	JID jid(pFid);
// 	delete pFid;
}
///////////////////////////////////////////////////////////////////
void CFriendsDlg::OnShield()		//屏蔽
{
	// TODO:  在此添加命令处理程序代码
	AfxMessageBox(_T("Shield"));
}
///////////////////////////////////////////////////////////////////
void CFriendsDlg::OnMoveTo()
{
	// TODO:  在此添加命令处理程序代码
	AfxMessageBox(_T("MoveTo"));
}
///////////////////////////////////////////////////////////////////

//收到好友请求，刷新,wparam为f_index
LRESULT CFriendsDlg::OnRecvSubReq(WPARAM wParam, LPARAM lParam)
{
	//初始化树空控件
	int f_index = (int)wParam;
	CString friendGroupName = theApp.friends[f_index].friendGroup;
	ReloadTree(friendGroupName);
	//CTreeCtrl获取每个根节点文本
	
	return 1;
}
//收到被删除的消息，同时删除该好友	wParam为jidFull
LRESULT CFriendsDlg::OnRecvRemved(WPARAM wParam, LPARAM lParam)
{
	CString jidFull((LPCTSTR)wParam);
	JID jid(theApp.r->CStringToJID(jidFull));
	CString jidStr(jid.bare().c_str());
	CString groupName;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	groupName = m_tree.GetItemText(m_hTreeItem);			//当前选择的group
	for (int i = 1; i < theApp.friendNum; i++)
	{
		if (jidFull == theApp.friends[i].friendIdFull || jidStr == theApp.friends[i].friendId)
		{
			if (theApp.friends[i].chatFlag)		//如果聊天窗口打开，则销毁该窗口
				::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSSCHATWND, NULL, NULL);
			theApp.delFriend(i);
			m_list.showFriendByGroup(groupName);
			break;
		}
	}
	return 1;
}

void CFriendsDlg::ReloadTree(CString friendGroupName)
{
	HTREEITEM friendGroup = m_tree.GetRootItem();
	CString Name = m_tree.GetItemText(friendGroup);
	int i = 0;
	bool groupFlag = false;
	if (friendGroupName == Name)
	{
		groupFlag = true;

	}
	else
	{
		while (friendGroup = m_tree.GetNextItem(friendGroup, TVGN_NEXT), ++i, friendGroup)
		{
			Name = m_tree.GetItemText(friendGroup);
			if (friendGroupName == Name)
			{
				groupFlag = true;
				break;
			}
		}
	}
	if (!groupFlag)
	{
		friendGroup = m_tree.InsertItem(friendGroupName);
		fGroups[groupNum++] = friendGroup;
	}
}





