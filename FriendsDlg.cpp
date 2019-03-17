// FriendsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "FriendsDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "FriendAddDlg.h"
#include "EditDlg.h"

// CFriendsDlg �Ի���
#define ID_TESTMENU 5000		//�˵���
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
	ON_MESSAGE(WM_RECVSUBREQ, OnRecvSubReq)			//6���պ�������
	ON_MESSAGE(WM_RECVREMVED, OnRecvRemved)			//7���ձ�����ɾ��
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


// CFriendsDlg ��Ϣ�������


BOOL CFriendsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//search��ť
	HICON hIcon;
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEARCH), IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_searchPic.SetIcon(hIcon);

	//��������ʾ
	m_friendEdit.SetWindowTextW(_T("����������"));
	m_friendEdit_flag = false;
	m_friendEdit_changedflag = false;

	//���غ����б�
	//׼��HICONͼ��
	HICON icons[2];
	icons[0] = AfxGetApp()->LoadIconW(IDI_DEFAULTHEADICON);
	//CImageList list;	//���뱣��ס�������  д��.h����Ա����
	//����ͼƬ����
	list.Create(16, 16, ILC_COLOR32, 4, 4);
	//��Ӿ����ͼƬ
	list.Add(icons[0]);
	m_tree.SetImageList(&list, LVSIL_NORMAL);
	CString friendId;
	CString friendGroupName;
	Sleep(2000);
	int k;
	friendItem[0] = NULL;
	//��ʼ�����տؼ�
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
		//�ж��Ƿ���ڸ��飬�����������ӷ��飬�������ȡ
		int i = 0;
		HTREEITEM friendGroup = m_tree.GetRootItem();
		
		//CTreeCtrl��ȡÿ�����ڵ��ı�
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


	//�б���ʾ
	m_list.InsertColumn(0, _T("ͷ��"), LVCFMT_LEFT, 50);		//����
	m_list.InsertColumn(1, _T("�ǳ�"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("״̬"), LVCFMT_LEFT, 50);
	m_list.InsertColumn(3, _T("findex"), LVCFMT_LEFT, 0);	//��Ӧfriend��index
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
 	m_list.GetHeaderCtrl()->EnableWindow(false);
	m_list.SetImageList(&list, LVSIL_SMALL);
	

	m_friendEdit_changedflag = true;		//��֮�����Ͷ���
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
HBRUSH CFriendsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (!m_friendEdit_flag)
		pDC->SetTextColor(RGB(192, 192, 192));                  //��ɫ
	else
		pDC->SetTextColor(RGB(0, 0, 255));   //��ɫ
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
void CFriendsDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
//	CFriendsDlg::OnStnClickedSearch();
//	CDialogEx::OnOK();
}
//void CFriendsDlg::OnStnClickedSearch()
//{
// 	// TODO:  �ڴ���ӿؼ�֪ͨ����������
// 	//�����б�
// 	//ˢ��
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



//�������¼�
//��걻�������õ�
void CFriendsDlg::OnEnSetfocusEdit1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_friendEdit_flag = true;
	CString strText = _T("");
	CEdit *pEdit = &m_friendEdit;
	pEdit->GetWindowText(strText);
	if (strText.Compare(_T("����������")) == 0)
		pEdit->SetWindowText(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
//������ʧȥ���
void CFriendsDlg::OnEnKillfocusEdit1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strText = _T("");
	CEdit *pEdit = &m_friendEdit;
	pEdit->GetWindowText(strText);
	if (strText.IsEmpty())
	{
		pEdit->SetWindowText(_T("����������"));
		m_friendEdit_flag = false;
	}
}
//�������仯
void CFriendsDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_friendEdit_changedflag)
	{

		CString CurrName;
		m_friendEdit.GetWindowTextW(CurrName);
		if (!m_friendEdit_flag)
		{
			CurrName = _T("");
		}
		if (CurrName == _T("") || CurrName == _T("����������"))
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

//�б��¼�
//˫���б�
void CFriendsDlg::OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	//��������Ự��
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
	static int findex;
	findex = _ttoi(fIndexStr);
	theApp.friends[findex].chatFlag = true;
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVDATA, (WPARAM)findex, (LPARAM)(LPCTSTR)"");
	*pResult = 0;
}
//�һ��б�  �����˵�
void CFriendsDlg::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	NMLISTVIEW* pMListView = (NMLISTVIEW*)pNMHDR;
	int nIndex = pMListView->iItem;
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
	int findex;
	findex = _ttoi(fIndexStr);
	//�Ҽ������˵�
	CPoint pt;//���ڻ�ȡCTreeCtrl�һ�����CTreeCtrl������ ������Ҫ��ΪCTreeCtrl�ĵ�����Ե�������ǻ�����������ϵ ��HitTestΪCTreeCtrl�ĳ�Ա������
	CPoint ptSc;//�һ��˵������Ͻǵ�λ���ǻ�����Ļ����ϵ
	UINT  flag;
	GetCursorPos(&pt); //��ȡ��ǰ��������ȫ������
	ptSc = pt;
	ScreenToClient(&pt);
	MapWindowPoints(&m_list, &pt, 1);//MapWindowPoint  Ϊ���ࣨCDialog���ĳ�Ա����,  ������ϵӳ��ΪCTreeCtrl������ϵ
	//	m_tree.Select(hItem, TVGN_CARET);//���õ���ڵ�Ϊ��ǰѡ�нڵ�
	CMenu m, *mn, *submn;
	m.LoadMenu(IDR_TREEMENU1);//���ز˵���Դ
	mn = m.GetSubMenu(0);//��ȡ�˵�����
	submn = mn->GetSubMenu(5);
	//��̬�����Ӳ˵�
	bool itemFlag = false;
	for (int i = 1; i < groupNum; i++)
	{
		submn->AppendMenu(0, (UINT)(ID_TESTMENU + i), m_tree.GetItemText(fGroups[i]));
	}
// 	submn->AppendMenu(MF_BYPOSITION | MF_POPUP | MF_STRING,
// 		(UINT)submn->m_hMenu, _T("�Ӳ˵�"));
	mn->TrackPopupMenu(TPM_LEFTALIGN, ptSc.x, ptSc.y, this);    //��ʾ�˵�

	*pResult = 0;
}

//���¼�
//������
void CFriendsDlg::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnTvnSelchangedTree1(pNMHDR, pResult);
	*pResult = 0;
}
//�һ���  �����˵�
void CFriendsDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CPoint pt;//���ڻ�ȡCTreeCtrl�һ�����CTreeCtrl������ ������Ҫ��ΪCTreeCtrl�ĵ�����Ե�������ǻ�����������ϵ ��HitTestΪCTreeCt
	CPoint ptSc;//�һ��˵������Ͻǵ�λ���ǻ�����Ļ����ϵ  
	UINT  flag;
	GetCursorPos(&pt); //��ȡ��ǰ��������ȫ������ 
	ptSc = pt;
	ScreenToClient(&pt);
	MapWindowPoints(&m_tree, &pt, 1);//MapWindowPoint  Ϊ���ࣨCDialog���ĳ�Ա����,  ������ϵӳ��ΪCTreeCtrl������ϵ
	HTREEITEM hItem = m_tree.HitTest(pt, &flag);
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	
	if (NULL != hItem){
		m_tree.Select(hItem, TVGN_CARET);//���õ���ڵ�Ϊ��ǰѡ�нڵ�
		CMenu m, *mn;
		m.LoadMenu(IDR_TREEMENU1);//���ز˵���Դ
		mn = m.GetSubMenu(1);//��ȡ�˵�����
		mn->EnableMenuItem(ID_ADDGROUP, MF_GRAYED);
		mn->TrackPopupMenu(TPM_LEFTALIGN, ptSc.x, ptSc.y, this);    //��ʾ�˵�
	}
	else
	{
		CMenu m, *mn;
		m.LoadMenu(IDR_TREEMENU1);//���ز˵���Դ
		mn = m.GetSubMenu(1);//��ȡ�˵�����
		mn->EnableMenuItem(ID_CHANGEGNAME, MF_GRAYED);
		mn->EnableMenuItem(ID_DELETEGROUP, MF_GRAYED);
		mn->TrackPopupMenu(TPM_LEFTALIGN, ptSc.x, ptSc.y, this);    //��ʾ�˵�
	}


	*pResult = 0;
}
//˫���� �༭��
void CFriendsDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�ж��Ƿ��Ǹ��ڵ㣬���򷵻�
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
//��˵�
//ɾ���鰴ť
void CFriendsDlg::OnDeletegroup()
{
	// TODO:  �ڴ���������������
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	CString itemText = m_tree.GetItemText(m_hTreeItem);
	if ("Buddies" == itemText)
	{
		MessageBox(_T("����ɾ��Ĭ�Ϸ���"));
		return;
	}
	if (::MessageBox(NULL, _T("ȷ��Ҫɾ�������𣿺��ѽ����Ƶ�Ĭ�Ϸ���Buddies�¡�"), _T("ɾ������"), MB_YESNO) == IDYES)
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
//�޸�����
void CFriendsDlg::OnChangegname()
{
	// TODO:  �ڴ���������������
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	CString itemText = m_tree.GetItemText(m_hTreeItem);
	CString newItemText;
	if ("Buddies" == itemText)
	{
		MessageBox(_T("�����޸�Ĭ�Ϸ���"));
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
//��ӷ���
void CFriendsDlg::OnAddgroup()
{
	// TODO:  �ڴ���������������
	CEditDlg editDlg;
	if (editDlg.DoModal() == IDOK)
	{
		//�жϷ����Ƿ������
		if (editDlg.m_edit == "")
		{
			MessageBox(_T("�����������"));
			return CFriendsDlg::OnAddgroup();
		}
		for (int i = 0; i < groupNum; i++)
		{
			if (editDlg.m_edit == m_tree.GetItemText(fGroups[i]))
			{
				MessageBox(_T("���������"));
				return CFriendsDlg::OnAddgroup();
			}
		}
		fGroups[groupNum++] = m_tree.InsertItem(editDlg.m_edit);
	}
}
//�ƶ�����
BOOL CFriendsDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO:  �ڴ����ר�ô����/����û���
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
	//�ڶ��ַ�����ȡList��ǰѡ��
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return CDialogEx::OnCommand(wParam, lParam);
	int nIndex = (int)m_list.GetNextSelectedItem(pos);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
	int findex;
	findex = _ttoi(fIndexStr);

	//��ǰѡ��Ĳ˵���Ӧ��group
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
	//��ǰѡ��Ĳ˵�
// 	CMenu m, *mn, *submn;
// 	m.LoadMenu(IDR_TREEMENU1);//���ز˵���Դ
// 	mn = m.GetSubMenu(0);//��ȡ�˵�����
// 	submn = mn->GetSubMenu(5);
// 	CString str_menustring;
// 	submn = GetMenu();
// 	submn->GetMenuString(0, str_menustring, MF_BYPOSITION);
// 	MessageBox(str_menustring);
	return CDialogEx::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
//���ѵ�״̬
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
//�л���
void CFriendsDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString groupName;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	groupName = m_tree.GetItemText(m_hTreeItem);
	m_list.showFriendByGroup(groupName);
	*pResult = 0;
}



//��Ӻ��Ѱ�ť
void CFriendsDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (theApp.friendNum >= 50)
	{
		AfxMessageBox(_T("�����Ѵ�����"));
		return;
	}
	CFriendAddDlg *faddDlg = new CFriendAddDlg();
	if (faddDlg->DoModal() == IDOK)
	{
		CString groupName("Buddies");			//֮��ͬ��message
		ReloadTree(groupName);
		HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
		groupName = m_tree.GetItemText(m_hTreeItem);
		m_list.showFriendByGroup(groupName);
	}
	delete faddDlg;
}
//�˵�
void CFriendsDlg::OnVCard()
{
	// TODO:  �ڴ���������������
//	NMLISTVIEW* pMListView = (NMLISTVIEW*)m_list.GetHotItem();
	CString Name;
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
	static int findex;
	findex = _ttoi(fIndexStr);
	Name = theApp.friends[findex].friendId;
	char* pFid = new char[200];
	sprintf(pFid, "%S", Name);
	JID jid(pFid);
	delete[] pFid;
	theApp.r->getVcard(jid, findex);
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVINFO, NULL, (LPARAM)findex);
//���ؼ�����
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
	// TODO:  �ڴ���������������
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
	static int findex;
	findex = _ttoi(fIndexStr);
	theApp.friends[findex].chatFlag = true;
	::PostMessage(GetParent()->GetParent()->GetSafeHwnd(), WM_RECVDATA, (WPARAM)(LPCTSTR)findex, (LPARAM)(LPCTSTR)"");

}
void CFriendsDlg::OnDelete()		//ɾ�����ѣ�̫�鷳��
{
	// TODO:  �ڴ���������������
	//ɾ������
	CString groupName;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	groupName = m_tree.GetItemText(m_hTreeItem);
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
	int findex;
	findex = _ttoi(fIndexStr);
	CString FullName(theApp.friends[findex].friendIdFull);
	CString BareName(theApp.friends[findex].friendId);
	CString tipStr;
	tipStr.Format(_T("ȷ��Ҫɾ��%s��"), FullName);
	if (::MessageBox(NULL, tipStr, _T("ɾ������"), MB_YESNO) == IDYES)
	{
		if (theApp.friends[findex].chatFlag)		//������촰�ڴ򿪣������ٸô���
			::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSSCHATWND, NULL, NULL);
		theApp.r->WDDelFriend(FullName);
		theApp.r->WDDelFriend(BareName);
		theApp.delFriend(findex);
		AfxMessageBox(_T("ɾ���ɹ�!"));
		m_list.showFriendByGroup(groupName);
	}

	/*CString Name;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	Name = m_tree.GetItemText(m_hTreeItem);
	if (::MessageBox(NULL, _T("ȷ��Ҫɾ����"), _T("ɾ������"), MB_YESNO) == IDYES)
	{
		theApp.r->WDDelFriend(Name);
		AfxMessageBox(_T("ɾ���ɹ�!"));
		m_tree.DeleteItem(m_hTreeItem);
	}*/
	///////////////////////////////////////////////////////////////////
}
void CFriendsDlg::OnSetNickName()		//���ñ�ע
{
	// TODO:  �ڴ���������������
	//rosterItem.setName;
	CString friendId;
	CString NoteName;
	int nIndex = m_list.GetNextItem(-1, LVIS_SELECTED);
	CString fIndexStr;
	fIndexStr = m_list.GetItemText(nIndex, 3);			//��ȡ��Ӧfindex
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
		//ˢ�º����б�
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
void CFriendsDlg::OnShield()		//����
{
	// TODO:  �ڴ���������������
	AfxMessageBox(_T("Shield"));
}
///////////////////////////////////////////////////////////////////
void CFriendsDlg::OnMoveTo()
{
	// TODO:  �ڴ���������������
	AfxMessageBox(_T("MoveTo"));
}
///////////////////////////////////////////////////////////////////

//�յ���������ˢ��,wparamΪf_index
LRESULT CFriendsDlg::OnRecvSubReq(WPARAM wParam, LPARAM lParam)
{
	//��ʼ�����տؼ�
	int f_index = (int)wParam;
	CString friendGroupName = theApp.friends[f_index].friendGroup;
	ReloadTree(friendGroupName);
	//CTreeCtrl��ȡÿ�����ڵ��ı�
	
	return 1;
}
//�յ���ɾ������Ϣ��ͬʱɾ���ú���	wParamΪjidFull
LRESULT CFriendsDlg::OnRecvRemved(WPARAM wParam, LPARAM lParam)
{
	CString jidFull((LPCTSTR)wParam);
	JID jid(theApp.r->CStringToJID(jidFull));
	CString jidStr(jid.bare().c_str());
	CString groupName;
	HTREEITEM m_hTreeItem = m_tree.GetSelectedItem();
	groupName = m_tree.GetItemText(m_hTreeItem);			//��ǰѡ���group
	for (int i = 1; i < theApp.friendNum; i++)
	{
		if (jidFull == theApp.friends[i].friendIdFull || jidStr == theApp.friends[i].friendId)
		{
			if (theApp.friends[i].chatFlag)		//������촰�ڴ򿪣������ٸô���
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





