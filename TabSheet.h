#pragma once
#include "afxcmn.h"
#define MAXPAGE 50
class CTabSheet :
	public CTabCtrl
{
public:
	CTabSheet(void);
	~CTabSheet(void);
	CDialog* m_pPages[MAXPAGE];
	BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
	void Show();
	void SetRect();
	void CTabSheet::AddTab();
	void CTabSheet::DeleteAllPages();
	void CTabSheet::SelChange(int oldSel, int newSel);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	int m_nNumOfPages;
	UINT m_IDD[MAXPAGE];
	LPCTSTR m_Title[MAXPAGE];
	int m_nCurrentPage;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

public:
	bool m_msgTabs[50];
};