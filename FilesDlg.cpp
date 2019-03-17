// FilesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "FilesDlg.h"
#include "afxdialogex.h"


// CFilesDlg �Ի���

IMPLEMENT_DYNAMIC(CFilesDlg, CDialogEx)

CFilesDlg::CFilesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilesDlg::IDD, pParent)
{

}

CFilesDlg::~CFilesDlg()
{
}

void CFilesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_mfcShellList);
	DDX_Control(pDX, IDC_EDIT1, m_fileEdit);
	DDX_Control(pDX, IDC_SEARCH, m_searchPic);
}


BEGIN_MESSAGE_MAP(CFilesDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_MFCSHELLLIST1, &CFilesDlg::OnNMClickMfcshelllist1)
	ON_STN_CLICKED(IDC_SEARCH, &CFilesDlg::OnStnClickedSearch)
	ON_WM_CTLCOLOR()
	ON_EN_KILLFOCUS(IDC_EDIT1, &CFilesDlg::OnEnKillfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT1, &CFilesDlg::OnEnSetfocusEdit1)
//	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CFilesDlg ��Ϣ�������


BOOL CFilesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//mfcshelllist
	m_mfcShellList.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	//�滻�����������ı��п�������ʾ
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	LPWSTR sName[4] = { _T("����"), _T("��С"), _T("����"), _T("�޸�") };
	int nWidth[4] = { 100, 60, 100, 200 };

	for (int i = 0; i < 4; i++)
	{
		lvColumn.pszText = sName[i];
		lvColumn.cx = nWidth[i];
		switch (i)
		{
		case 0:
		case 2:
		case 3:
			lvColumn.fmt = LVCFMT_LEFT;
			break;
		case 1:
			lvColumn.fmt = LVCFMT_RIGHT;
		default:
			break;
		}
		m_mfcShellList.SetColumn(i, &lvColumn);
	}
	m_mfcShellList.DisplayFolder(theApp.r->myfileRecvPath);
	m_mfcShellList.EnableShellContextMenu(TRUE);
	m_mfcShellList.SetItemTypes(m_mfcShellList.GetItemTypes() | SHCONTF_CHECKING_FOR_CHILDREN);
	m_mfcShellList.SetExtendedStyle(m_mfcShellList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	//search��ť
	HICON hIcon;
	hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SEARCH), IMAGE_ICON, 24, 24, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_searchPic.SetIcon(hIcon);

	//��������ʾ
	m_fileEdit.SetWindowTextW(_T("����������"));
	m_fileEdit_flag = false;


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CFilesDlg::OnNMClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
/*
	CString CurrName;
	CString Filename;
	if (pNMListView->iItem != -1)
	{
		//��ȡ����

		CurrName = m_mfcShellList.GetItemText(pNMListView->iItem,
			pNMListView->iSubItem);

		//��ȡ·��
		m_mfcShellList.GetItemPath(Filename, pNMListView->iItem);
		m_fileEdit.SetWindowText(Filename);

	}

*/


	*pResult = 0;
}


void CFilesDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	CFilesDlg::OnStnClickedSearch();
//	CDialogEx::OnOK();
}


void CFilesDlg::OnStnClickedSearch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//�����б�
	m_mfcShellList.Refresh();		//ˢ��
	CString CurrName;
	CString FileName;
	m_fileEdit.GetWindowTextW(CurrName);
	if (!m_fileEdit_flag)
	{
		CurrName = _T("");
	}

	for (int i = 0; i < m_mfcShellList.GetItemCount(); i++)
	{
		if (m_mfcShellList.GetItemText(i, 0).Find(CurrName) == -1)
		{
			m_mfcShellList.DeleteItem(i--);
		}
		else
		{
			continue;
		}
	}


}



/*//���ú���
for (int i = 0; i < m_mfcShellList.GetItemCount(); i++)
{
	if (m_mfcShellList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
	{
		FileName = m_mfcShellList.GetItemText(i, 0);
		AfxMessageBox(FileName);
	}
}
*/


HBRUSH CFilesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if (!m_fileEdit_flag)
		pDC->SetTextColor(RGB(192, 192, 192));                  //��ɫ
	else
		pDC->SetTextColor(RGB(0, 0, 255));   //��ɫ



	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CFilesDlg::OnEnKillfocusEdit1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strText = _T("");
	CEdit *pEdit = &m_fileEdit;
	pEdit->GetWindowText(strText);
	if (strText.IsEmpty())
	{
		pEdit->SetWindowText(_T("����������"));
		m_fileEdit_flag = false;
	}
//	m_fileEdit_flag = false;

}


void CFilesDlg::OnEnSetfocusEdit1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_fileEdit_flag = true;
	CString strText = _T("");
	CEdit *pEdit = &m_fileEdit;
	pEdit->GetWindowText(strText);
	if (strText.Compare(_T("����������")) == 0)
		pEdit->SetWindowText(_T(""));
}


//BOOL CFilesDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO:  �ڴ����ר�ô����/����û���
//	return CDialogEx::OnNotify(wParam, lParam, pResult);
//}


//void CFilesDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
//	m_mfcShellList.Refresh();
//	// TODO:  �ڴ˴������Ϣ����������
//}
