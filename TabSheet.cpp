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
	//����Ŀǰ�ܵ��ӶԻ�����
	m_nNumOfPages++;
	//��¼�ӶԻ����ָ�롢��ԴID��Ҫ�ڱ�ǩ����ʾ������
	m_pPages[m_nNumOfPages - 1] = pDialog;
	m_IDD[m_nNumOfPages - 1] = ID;
	m_Title[m_nNumOfPages - 1] = title;
	return TRUE;
}

void CTabSheet::Show()
{
	//����CDialog::Create�������ӶԻ��򣬲���ʹ��CTabCtrl::InsertItem��������Ӧ�ı�ǩ
	for (int i = 0; i < m_nNumOfPages; i++)
	{
		m_pPages[i]->Create(m_IDD[i], this);
		InsertItem(i, m_Title[i]);
	}

	//���ڶԻ�����ʾʱĬ�ϵ��ǵ�һ����ǩ��ѡ�У�����Ӧ���õ�һ���ӶԻ�����ʾ�������ӶԻ�������
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
 	//�õ�Tab Control�Ĵ�С
 	GetClientRect(&tabRect);
 	GetItemRect(0, &itemRect);
 	//��������ӶԻ���������Tab Control��λ�úʹ�С
 	nX = itemRect.left;
 	nY = itemRect.bottom + 1;
 	nXc = tabRect.right - itemRect.left - 2;
 	nYc = tabRect.bottom - nY - 2;
 	//���ü���������ݶԸ��ӶԻ�����е���
 	m_pPages[m_nNumOfPages - 1]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
}

void CTabSheet::SetRect()
{

	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;
	//�õ�Tab Control�Ĵ�С
	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);
	//��������ӶԻ���������Tab Control��λ�úʹ�С
	nX = itemRect.left;
	nY = itemRect.bottom + 1;
	nXc = tabRect.right - itemRect.left - 2;
	nYc = tabRect.bottom - nY - 2;
	//���ü���������ݶԸ��ӶԻ�����е���
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
	//��ʾ��ǰ��ǩ����Ӧ���ӶԻ���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�ж��Ƿ񵥻���������ǩ
	/*
	if(m_nCurrentPage != GetCurFocus())
	{
	//��ԭ�ȵ��ӶԻ�������
	m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
	m_nCurrentPage=GetCurFocus();
	//��ʾ��ǰ��ǩ����Ӧ���ӶԻ���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_nCurrentPage != GetCurFocus())
	{
		//��ԭ�ȵ��ӶԻ�������
		m_pPages[m_nCurrentPage]->ShowWindow(SW_HIDE);
		m_nCurrentPage = GetCurFocus();
		//��ʾ��ǰ��ǩ����Ӧ���ӶԻ���
		m_pPages[m_nCurrentPage]->ShowWindow(SW_SHOW);
		m_msgTabs[m_nCurrentPage] = false;
	}

	CTabCtrl::OnLButtonUp(nFlags, point);
}

void CTabSheet::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  ������Ĵ����Ի���ָ����
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

		//��䱳��ɫ  
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

		//����ͼƬ  
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

		//��������  
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
