// MyList.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "MyList.h"
//#include "mysql.h"
#include "winsock.h"

// CMyList


IMPLEMENT_DYNAMIC(CMyList, CListCtrl)

CMyList::CMyList()
{
	//颜色
	m_HoverItemBkColor = RGB(112, 128, 144);//热点行背景颜色  
	m_SelectItemBkColor = GetSysColor(COLOR_HIGHLIGHT);//选中行背景颜色  
	m_HoverItemTextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);//热点行文本颜色  
	m_SelectItemTextColor = GetSysColor(COLOR_BTNTEXT);//选中行文本颜色  
	m_bTracking = FALSE;
	m_nHoverIndex = -1;

	m_imageList.Create(32, 32, ILC_COLOR32, 100, 100);
	//准备HICON图标 代表好友的状态
	HICON icons[6];
//	icons[0] = AfxGetApp()->LoadIconW(IDI_FRIENDGROUP);
	icons[0] = AfxGetApp()->LoadIconW(IDI_STATUS_ONLINE);		//status-0 在线
	icons[1] = AfxGetApp()->LoadIconW(IDI_STATUS_ONLINE);		//status-1 和我聊
	icons[2] = AfxGetApp()->LoadIconW(IDI_STATUS_AWAY);			//status-2 离开
	icons[3] = AfxGetApp()->LoadIconW(IDI_STATUS_BUSY);			//status-3 忙碌
	icons[4] = AfxGetApp()->LoadIconW(IDI_STATUS_BUSY);			//status-4 远远离开
	icons[5] = AfxGetApp()->LoadIconW(IDI_STATUS_OFFLINE);		//status-5 离线
	//添加具体的图片
	for (int i = 0; i < 6; i++)
	{
		m_imageList.Add(icons[i]);
	}
	memset(f_indexList, -1, sizeof(f_indexList));		//初始化数组
}

CMyList::~CMyList()
{
}
void CMyList::addRecentFriend(int f_index)
{
//	DeleteAllItems();
//	m_imageList.Remove(-1);
//	m_imageRecentList.Remove(-1);
	//链接数据库im
	static int imageIndex = 6;
	//判断是否已经加入
	for (int i = 0; i < f_indexListNum; i++)
	{
		if (f_index == f_indexList[i])
			return;
	}
	f_indexList[f_indexListNum++] = f_index;

	//数据操作，这里可以直接以row[i]的形式取出数据项，但是均为char*行  
	for (int cols = 0; cols < 4; cols++)
	{
		//初始化，装载图标；

		HICON hIcon = NULL;
		HBITMAP hbmp = NULL;
		CBitmap* pbmp = NULL;
		switch (cols)
		{
		case 0:
		{
			if (theApp.friends[f_index].headIconUrl == "")
			{
				m_imageList.Add(AfxGetApp()->LoadIconW(IDI_DEFAULTHEADICON));
				SetImageList(&m_imageList, LVSIL_SMALL);
				InsertItem(recentListrow, _T(""), imageIndex++);
				break;
			}
			//LoadImage
			hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), theApp.friends[f_index].headIconUrl, IMAGE_BITMAP, 32, 32, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			pbmp = CBitmap::FromHandle(hbmp);
			m_imageList.Add(pbmp, RGB(0, 0, 0));
			SetImageList(&m_imageList, LVSIL_SMALL);
			InsertItem(recentListrow, _T(""), imageIndex++);
			break;
		}
		case 1:
		{
			CString str;
			if (theApp.friends[f_index].f_note != "")
			{
				str += theApp.friends[f_index].f_note;
			}
			else if (theApp.friends[f_index].nickName != "")
			{
 				str += theApp.friends[f_index].nickName;
//  				str += _T("(");
// 				str += theApp.friends[f_index].friendId;
// 				str += _T(")");
			}
			else
			{
				str = theApp.friends[f_index].friendId;
			}
			SetItemText(recentListrow, cols, str);
			break;
		}
		case 2:
			SetItem(recentListrow, cols, LVIF_IMAGE, NULL, theApp.friends[f_index].friendStatus, LVIS_SELECTED, LVIS_SELECTED, 0);
			break;
		case 3:			//friendindex
		{
			CString str;
			str.Format(_T("%d"), f_index);
			SetItemText(recentListrow, cols, str);
			break;
		}
		default:
			break;
		}

	}
	recentListrow++;
}
void CMyList::delRecentFriend(int f_index, int delCol)
{
	DeleteItem(delCol);
	for (int i = 0; i < f_indexListNum; i++)
	{
		if (f_index == f_indexList[i])
		{
			for (int j = i; j < f_indexListNum - 1; j++)
			{
				f_indexList[j] = f_indexList[j + 1];
			}
			f_indexListNum--;
			break;
		}
	}
	recentListrow--;
}

//通过分组查询
void CMyList::showFriendByGroup(CString groupName)
{
	DeleteAllItems();
	m_imageList.Remove(-1);
	//准备HICON图标 代表好友的状态
	HICON icons[6];
	//	icons[0] = AfxGetApp()->LoadIconW(IDI_FRIENDGROUP);
	icons[0] = AfxGetApp()->LoadIconW(IDI_STATUS_ONLINE);		//status-0 在线
	icons[1] = AfxGetApp()->LoadIconW(IDI_STATUS_ONLINE);		//status-1 和我聊
	icons[2] = AfxGetApp()->LoadIconW(IDI_STATUS_AWAY);			//status-2 离开
	icons[3] = AfxGetApp()->LoadIconW(IDI_STATUS_BUSY);			//status-3 忙碌
	icons[4] = AfxGetApp()->LoadIconW(IDI_STATUS_BUSY);			//status-4 远远离开
	icons[5] = AfxGetApp()->LoadIconW(IDI_STATUS_OFFLINE);		//status-5 离线
	//添加具体的图片
	for (int i = 0; i < 6; i++)
	{
		m_imageList.Add(icons[i]);
	}
	int imageIndex = 6;
	int listrow = 0;
	for (int f_index = 1; f_index < theApp.friendNum; f_index++)
	{
		if (theApp.friends[f_index].friendGroup != groupName)
		{
			continue;
		}
		//数据操作，这里可以直接以row[i]的形式取出数据项，但是均为char*行  
		for (int cols = 0; cols < 4; cols++)
		{
			//初始化，装载图标；
			HICON hIcon = NULL;
			HBITMAP hbmp = NULL;
			CBitmap* pbmp = NULL;
			switch (cols)
			{
			case 0:
			{
				if (theApp.friends[f_index].headIconUrl == "")
				{
					m_imageList.Add(AfxGetApp()->LoadIconW(IDI_DEFAULTHEADICON));
					SetImageList(&m_imageList, LVSIL_SMALL);
					InsertItem(listrow, _T(""), imageIndex++);
					break;
				}

				//LoadImage
				hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), theApp.friends[f_index].headIconUrl, IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
				pbmp = CBitmap::FromHandle(hbmp);
				m_imageList.Add(pbmp, RGB(0, 0, 0));
				SetImageList(&m_imageList, LVSIL_SMALL);
				InsertItem(listrow, _T(""), imageIndex++);
				break;
			}
			case 1:
			{
				CString str;
				if (theApp.friends[f_index].f_note != "")
				{
					str += theApp.friends[f_index].f_note;
				}
				else if (theApp.friends[f_index].nickName != "")
				{
					str += theApp.friends[f_index].nickName;
					//  				str += _T("(");
					// 				str += theApp.friends[f_index].friendId;
					// 				str += _T(")");
				}
				else
				{
					str = theApp.friends[f_index].friendId;
				}
				SetItemText(listrow, cols, str);
				break;
			}
			case 2:
				SetItem(listrow, cols, LVIF_IMAGE, NULL, theApp.friends[f_index].friendStatus, LVIS_SELECTED, LVIS_SELECTED, 0);
				break;
			case 3:			//friendindex
			{
				CString str;
				str.Format(_T("%d"), f_index);
				SetItemText(listrow, cols, str);
				break;
			}
			default:
				break;
			}

		}
		listrow++;
	}
}
//模糊查询通过ID昵称备注
void CMyList::showFriendByFID(CString friendId)
{
	DeleteAllItems();
	m_imageList.Remove(-1);
	//准备HICON图标 代表好友的状态
	HICON icons[7];
	icons[0] = AfxGetApp()->LoadIconW(IDI_STATUS_ONLINE);		//status-0 在线
	icons[1] = AfxGetApp()->LoadIconW(IDI_STATUS_ONLINE);		//status-1 和我聊
	icons[2] = AfxGetApp()->LoadIconW(IDI_STATUS_AWAY);			//status-2 离开
	icons[3] = AfxGetApp()->LoadIconW(IDI_STATUS_BUSY);			//status-3 忙碌
	icons[4] = AfxGetApp()->LoadIconW(IDI_STATUS_BUSY);			//status-4 远远离开
	icons[5] = AfxGetApp()->LoadIconW(IDI_STATUS_OFFLINE);		//status-5 离线
	//添加具体的图片
	for (int i = 0; i < 6; i++)
	{
		m_imageList.Add(icons[i]);
	}
	int imageIndex = 6;
	int listrow = 0;
	for (int f_index = 1; f_index < theApp.friendNum; f_index++)
	{
		if (theApp.friends[f_index].friendId.Find(friendId) == -1
			&& theApp.friends[f_index].f_note.Find(friendId) == -1
			&& theApp.friends[f_index].nickName.Find(friendId) == -1)
		{
			continue;
		}
		for (int cols = 0; cols < 4; cols++)
		{
			//初始化，装载图标；
			HICON hIcon = NULL;
			HBITMAP hbmp = NULL;
			CBitmap* pbmp = NULL;
			switch (cols)
			{
			case 0:
			{
				if (theApp.friends[f_index].headIconUrl == "")
				{
					m_imageList.Add(AfxGetApp()->LoadIconW(IDI_DEFAULTHEADICON));
					SetImageList(&m_imageList, LVSIL_SMALL);
					InsertItem(listrow, _T(""), imageIndex++);
					break;
				}
				//LoadImage
				hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), theApp.friends[f_index].headIconUrl, IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
				pbmp = CBitmap::FromHandle(hbmp);
				m_imageList.Add(pbmp, RGB(0, 0, 0));
				SetImageList(&m_imageList, LVSIL_SMALL);
				InsertItem(listrow, _T(""), imageIndex++);
				break;
			}
			case 1:
			{
				CString str;
				if (theApp.friends[f_index].f_note != "")
				{
					str += theApp.friends[f_index].f_note;
					str += _T("(");
					str += theApp.friends[f_index].friendId;
					str += _T(")");
				}
				else if (theApp.friends[f_index].nickName != "")
				{
					str += theApp.friends[f_index].nickName;
					str += _T("(");
					str += theApp.friends[f_index].friendId;
					str += _T(")");
				}
				else
				{
					str = theApp.friends[f_index].friendId;
				}
				SetItemText(listrow, cols, str);
				break;
			}
			case 2:
				SetItem(listrow, cols, LVIF_IMAGE, NULL, theApp.friends[f_index].friendStatus, LVIS_SELECTED, LVIS_SELECTED, 0);
				break;
			case 3:			//friendindex
			{
				CString str;
				str.Format(_T("%d"), f_index);
				SetItemText(listrow, cols, str);
				break;
			}
			default:
				break;
			}

		}
		listrow++;
	}
}

void CMyList::showFriendList(char *query)
{
	/*MYSQL mysql;
	mysql_init(&mysql);
	DeleteAllItems();
	m_imageList.Remove(-1);
	//链接数据库im

	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "im", 3306, NULL, 0))
	{
		AfxMessageBox(_T("访问数据库失败!"));
	}
	else
	{
		//	AfxMessageBox(_T("链接数据库成功!"));
		mysql_query(&mysql, "SET NAMES 'GB2312'");//解决从数据库中读取数据后汉字乱码显示的问题  
	}
	MYSQL_RES *res;//声明数据查询结果  
	MYSQL_ROW row;//取每行结果的对象
	mysql_real_query(&mysql, query, strlen(query));//查询函数，语句用字符串表示  
	res = mysql_store_result(&mysql);//得到查询结果  
//	m_imageList.Create(32, 32, ILC_COLOR32, 100, 100);
	int index = 0;
	int listrow = 0;	//行
	while (row = mysql_fetch_row(res))//逐行去除查询结果并操作  
	{
		//数据操作，这里可以直接以row[i]的形式取出数据项，但是均为char*行  
		for (int cols = 0; cols < 3; cols++)
		{
			CString myreaddata(row[cols]);
			//初始化，装载图标；

			HICON hIcon = NULL;
			HBITMAP hbmp = NULL;
			CBitmap* pbmp = NULL;
			switch (cols)
			{
			case 0:
				//LoadImage
				hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), myreaddata, IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
				pbmp = CBitmap::FromHandle(hbmp);
				m_imageList.Add(pbmp, RGB(0, 0, 0));
				SetImageList(&m_imageList, LVSIL_SMALL);
				InsertItem(listrow, _T(""), index++);
				break;
			case 1:
				SetItemText(listrow, cols, myreaddata);
				break;
			case 2:
				if (_ttoi(myreaddata))
				{
					hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_STATUS_ONLINE), IMAGE_ICON,
						32, 32, 0);
					m_imageList.Add(hIcon);
					SetImageList(&m_imageList, LVSIL_SMALL);
					SetItem(listrow, cols, LVIF_IMAGE, NULL, index++, LVIS_SELECTED, LVIS_SELECTED, 0);
				}
				else if (!_ttoi(myreaddata))
				{
					hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_STATUS_OFFLINE), IMAGE_ICON,
						32, 32, 0);
					m_imageList.Add(hIcon);
					SetImageList(&m_imageList, LVSIL_SMALL);
					SetItem(listrow, cols, LVIF_IMAGE, NULL, index++, LVIS_SELECTED, LVIS_SELECTED, 0);
				}
				break;
			default:
				break;
			}

		}
		listrow++;

	}
	mysql_free_result(res);
	mysql_close(&mysql);*/
}
void CMyList::showGroupList(char *query)
{
/*
	DeleteAllItems();
	m_imageList.Remove(-1);
	MYSQL mysql;
	mysql_init(&mysql);
	//链接数据库im
	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "im", 3306, NULL, 0))
	{
		AfxMessageBox(_T("访问数据库失败!"));
	}
	else
	{
		mysql_query(&mysql, "SET NAMES 'GB2312'");//解决从数据库中读取数据后汉字乱码显示的问题  
	}
	MYSQL_RES *res;//声明数据查询结果  
	MYSQL_ROW row;//取每行结果的对象
	mysql_real_query(&mysql, query, strlen(query));//查询函数，语句用字符串表示  
	res = mysql_store_result(&mysql);//得到查询结果  
//	m_imageList.Create(32, 32, ILC_COLOR32, 100, 100);
	int index = 0;
	int listrow = 0;	//行
	while (row = mysql_fetch_row(res))//逐行去除查询结果并操作  
	{
		//数据操作，这里可以直接以row[i]的形式取出数据项，但是均为char*行  
		for (int cols = 0; cols < 3; cols++)
		{
			CString myreaddata(row[cols]);
			//初始化，装载图标；
			HICON hIcon = NULL;
			HBITMAP hbmp = NULL;
			CBitmap* pbmp = NULL;
			switch (cols)
			{
			case 0:
				//LoadImage
				hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), myreaddata, IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE);
				pbmp = CBitmap::FromHandle(hbmp);
				m_imageList.Add(pbmp, RGB(0, 0, 0));
				SetImageList(&m_imageList, LVSIL_SMALL);
				InsertItem(listrow, _T(""), index++);
				break;
			case 1:
			case 2:
				SetItemText(listrow, cols, myreaddata);
				break;
			default:
				break;
			}

		}
		listrow++;

	}
	mysql_free_result(res);
	mysql_close(&mysql);*/
}



BEGIN_MESSAGE_MAP(CMyList, CListCtrl)

	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMyList::OnNMCustomdraw)
END_MESSAGE_MAP()



// CMyList 消息处理程序




void CMyList::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	int nIndex = HitTest(point);
	if (nIndex != m_nHoverIndex){
		int nOldIndex = m_nHoverIndex;
		m_nHoverIndex = nIndex;
		CRect rc;
		if (nOldIndex != -1){
			GetItemRect(nOldIndex, &rc, LVIR_BOUNDS);
			InvalidateRect(&rc);
		}
		if (m_nHoverIndex != -1){
			GetItemRect(m_nHoverIndex, &rc, LVIR_BOUNDS);
			InvalidateRect(&rc);
		}
	}
	//=====================================================  
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT   tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;//   |   TME_HOVER;   
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CListCtrl::OnMouseMove(nFlags, point);
}


void CMyList::OnMouseLeave()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_bTracking = FALSE;
	if (m_nHoverIndex != -1){
		CRect rc;
		GetItemRect(m_nHoverIndex, &rc, LVIR_BOUNDS);
		m_nHoverIndex = -1;
		InvalidateRect(&rc);
	}
	CListCtrl::OnMouseLeave();
}


void CMyList::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	int nItemIndex = pNMCD->nmcd.dwItemSpec;
	if (pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT){
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else{
		if (nItemIndex == m_nHoverIndex){ //热点行  
			pNMCD->clrTextBk = m_HoverItemBkColor;
			pNMCD->clrText = m_HoverItemTextColor;
		}
		else if (GetItemState(nItemIndex, LVIS_SELECTED) == LVIS_SELECTED){ //选中行  
			pNMCD->clrTextBk = m_SelectItemBkColor;
			pNMCD->clrText = pNMCD->clrFace = m_SelectItemTextColor;
			::SetTextColor(pNMCD->nmcd.hdc, m_SelectItemTextColor);
		}
// 		else if (nItemIndex % 2 == 0){//偶数行 比如 0、2、4、6  
// 			pNMCD->clrTextBk = m_EvenItemBkColor;
// 			pNMCD->clrText = m_EvenItemTextColor;
// 		}
// 		else{  //奇数行 比如 1、3、5、7  
// 			pNMCD->clrTextBk = m_OddItemBkColor;
// 			pNMCD->clrText = m_OddItemTextColor;
// 		}
		*pResult = CDRF_NEWFONT;
	}
}
