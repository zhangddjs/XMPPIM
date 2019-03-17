#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyList.h"
#include "MyTree.h"

// CGroupsDlg 对话框

class CGroupsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupsDlg)

public:
	CGroupsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupsDlg();

// 对话框数据
	enum { IDD = IDD_GROUPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_groupEdit;
	CStatic m_searchPic;
	CImageList m_imageList;
	bool m_groupEdit_flag;
	bool m_groupEdit_changedflag;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CMyList m_list;
	CMyTree m_tree;
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
//	afx_msg void OnTvnSingleExpandTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnOK();
};
