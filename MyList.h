#pragma once


// CMyList


class CMyList : public CListCtrl
{
	DECLARE_DYNAMIC(CMyList)

public:
	CMyList();
	virtual ~CMyList();
	virtual void addRecentFriend(int f_index);
	virtual void delRecentFriend(int f_index, int delCol);

	virtual void showFriendByGroup(CString groupName);
	virtual void showFriendByFID(CString friendId);

	virtual void showFriendList(char *query);
	virtual void showGroupList(char *query);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CImageList m_imageList;
	CImageList m_statusList;
	int f_indexList[50];
	int f_indexListNum = 0;
	int recentListrow = 0;	//ÐÐ

	COLORREF m_HoverItemBkColor;
	COLORREF m_SelectItemBkColor;
	COLORREF m_HoverItemTextColor;
	COLORREF m_SelectItemTextColor;
	int m_bTracking;
	BOOL m_nHoverIndex;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


