#pragma once
#include "afxcmn.h"
#include "MyList.h"

// RecentDlg 对话框

class CRecentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecentDlg)

public:
	CRecentDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecentDlg();
//	void AutoAdjustColumnWidth(CListCtrl *pListCtrl);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// 对话框数据
	enum { IDD = IDD_RECENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CMyList m_list;
public:
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
