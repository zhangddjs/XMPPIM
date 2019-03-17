#include "stdafx.h"
#include "TabSheet.h"


CTabSheet::CTabSheet(void)
{
	m_nNumOfPages = 0;
	m_nCurrentPage = 0;
	for (int i = 0; i < 50; i++)
	{
		m_msgTabs[i] = { false };
	}
}


CTabSheet::~CTabSheet(void)
{
}

BOOL CTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT ID)
{
	if (MAXPAGE == m_nNumOfPages)
		return FALSE;
	//保存目前总的子对话框数
	m_nNumOfPages++;
	//记录子对话框的指针、资源ID、要在标签上显示的文字
	m_pPages[m_nNumOfPages - 1] = pDialog;
	m_IDD[m_nNumOfPages - 1] = ID;
	m_Title[m_nNumOfPages - 1] = title;
	return TRUE;
}

void CTabSheet::Show()
{
	//利用CDialog::Create来创建子对话框，并且使用CTabCtrl::InsertItem来加上相应的标签
	for (int i = 0; i < m_nNumOfPages; i++)
	{
		m_pPages[i]->Create(m_IDD[i], this);
		InsertItem(i, m_Title[i]);
	}

	//由于对话框显示时默认的是第一个标签被选中，所以应该让第一个子对话框显示，其他子对话框隐藏
	m_nCurrentPage = 0;
	m_pPages[0]->ShowWindow(SW_SHOW);
	for (int i = 1; i < m_nNumOfPages; i++)
		m_pPages[i]->ShowWindow(SW_HIDE);
	SetRect();
//	AfxMessageBox(_T("2"));
}

void CTabSheet::AddTab()
{
	m_pPages[m_nNumOfPages - 1]->Create(m_IDD[m_nNumOfPages - 1], this);
	InsertItem(m_nNumOfPages - 1, m_Title[m_nNumOfPages - 1]);
	m_pPages[m_nNumOfPages - 1]->ShowWindow(SW_HIDE);

//	AfxMessageBox(_T("2"));

 	CRect tabRect, itemRect;
 	int nX, nY, nXc, nYc;
 	//得到Tab Control的大小
 	GetClientRect(&tabRect);
 	GetItemRect(0, &itemRect);
 	//计算出各子对话框的相对于Tab Control的位置和大小
 	nX = itemRect.left;
 	nY = itemRect.bottom + 1;
 	nXc = tabRect.right - itemRect.left - 2;
 	nYc = tabRect.bottom - nY - 2;
 	//利用计算出的数据对各子对话框进行调整
 	m_pPages[m_nNumOfPages - 1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
}

void CTabSheet::SetRect()
{

	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;
	//得到Tab Control的大小
	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);
	//计算出各子对话框的相对于Tab Control的位置和大小
	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 2;
	nYc = tabRect.bottom - nY - 2;
	//利用计算出的数据对各子对话框进行调整
	m_pPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for (int nCount = 1; nCount < m_nNumOfPages; nCount++)
		m_pPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);

}

void CTabSheet::DeleteAllPages()
{
	m_nNumOfPages = 0;
	m_nCurrentPage = 0;
	DeleteAllItems();
}

void CTabSheet::SelChange(int oldSel, int newSel)
{
	m_pPages[oldSel]->ShowWindow(SW_HIDE);
	//显示当前标签所对应的子对话框
	m_nCurrentPage = newSel;
	m_pPages[newSel]->ShowWindow(SW_SHOW);
	m_msgTabs[newSel] = false;

}


BEGIN_MESSAGE_MAP(CTabSheet, CTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CTabSheet::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//判断是否单击了其他标签
	/*
	if(m_nCurrentPage != GetCurFocus())
	{
	//将原先的子对话框隐藏
	m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
	m_nCurrentPage=GetCurFocus();
	//显示当前标签所对应的子对话框
	m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
	if(m_nCurrentPage == 0)
	MessageBox(L"0",L"0");
	else if(m_nCurrentPage == 1)
	MessageBox(L"1",L"1");
	else
	MessageBox(L"2",L"2");
	}
	*/

	CTabCtrl::OnLButtonDown(nFlags, point);
}


void CTabSheet::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nCurrentPage != GetCurFocus())
	{
		//将原先的子对话框隐藏
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = GetCurFocus();
		//显示当前标签所对应的子对话框
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		m_msgTabs[m_nCurrentPage] = false;
	}

	CTabCtrl::OnLButtonUp(nFlags, point);
}

void CTabSheet::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项
	if (lpDrawItemStruct->CtlType == ODT_TAB)
	{
		CRect rect = lpDrawItemStruct->rcItem;
		INT nTabIndex = lpDrawItemStruct->itemID;
		if (nTabIndex < 0) return;

		TCHAR label[64];
		TC_ITEM tci;
		tci.mask = TCIF_TEXT | TCIF_IMAGE;
		tci.pszText = label;
		tci.cchTextMax = 63;
		GetItem(nTabIndex, &tci);

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		if (!pDC) return;
		int nSavedDC = pDC->SaveDC();

		//填充背景色  
		CRect rClient;
		GetClientRect(rClient);
		COLORREF rcBack;
		if (lpDrawItemStruct->itemState & CDIS_SELECTED)
		{
			rcBack = RGB(228, 220, 252);
		}
		else if (lpDrawItemStruct->itemState & (CDIS_DISABLED | CDIS_GRAYED))
		{
			rcBack = RGB(0, 255, 0);
		}
		else
		{
			if (m_msgTabs[nTabIndex])
				rcBack = RGB(255, 215, 0);
			else
				rcBack = GetSysColor(COLOR_BTNFACE);
		}
		pDC->FillSolidRect(rect, rcBack);

		rect.top += ::GetSystemMetrics(SM_CYEDGE);

		pDC->SetBkMode(TRANSPARENT);

		//绘制图片  
		CImageList* pImageList = GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			rect.left += pDC->GetTextExtent(_T(" ")).cx;       // Margin  

			// Get height of image so we   
			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect ImageRect(info.rcImage);
			INT nYpos = rect.top;

			pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
			rect.left += ImageRect.Width();
		}

		//绘制字体  
		COLORREF txtColor;
		if (lpDrawItemStruct->itemState & CDIS_SELECTED)
		{
			rect.top -= ::GetSystemMetrics(SM_CYEDGE);

			txtColor = RGB(0, 0, 0);
			CFont * cFont = new CFont;
			cFont->CreateFont(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH&FF_SWISS, _T("Arial"));
			pDC->SelectObject(cFont);
			delete cFont;
		}
		else if (lpDrawItemStruct->itemState & (CDIS_DISABLED | CDIS_GRAYED))
		{
			txtColor = RGB(128, 128, 128);
		}
		else
		{
			if (m_msgTabs[nTabIndex])
				txtColor = RGB(0, 0, 255);
			else
				txtColor = GetSysColor(COLOR_WINDOWTEXT);
		}

		pDC->SetTextColor(txtColor);
		pDC->DrawText(label, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		pDC->RestoreDC(nSavedDC);
	}
}
