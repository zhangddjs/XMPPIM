#pragma once
#include "MyShellListCtrl.h"
#include "afxwin.h"


// CFilesDlg �Ի���

class CFilesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilesDlg)

public:
	CFilesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFilesDlg();

// �Ի�������
	enum { IDD = IDD_FILES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMyShellListCtrl m_mfcShellList;
	afx_msg void OnNMClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CEdit m_fileEdit;
	bool m_fileEdit_flag;
	virtual void OnOK();
	CStatic m_searchPic;
public:
	afx_msg void OnStnClickedSearch();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnSetfocusEdit1();
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
