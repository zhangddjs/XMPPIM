#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyList.h"
#include "MyTree.h"

// CFriendsDlg 对话框

class CFriendsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendsDlg)

public:
	CFriendsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFriendsDlg();

// 对话框数据
	enum { IDD = IDD_FRIENDS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnRecvFriend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvSubReq(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvRemved(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CStatic m_searchPic;
	CEdit m_friendEdit;
	bool m_friendEdit_flag;
	CImageList m_imageList;
	bool m_friendEdit_changedflag;
	void CFriendsDlg::ReloadTree(CString friendGroupName);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CMyList m_list;
	CMyTree m_tree;
	CImageList list;
	HTREEITEM *friendItem = new HTREEITEM[200];		//控件变量
	HTREEITEM fGroups[20];			//组
	int groupNum = 0;

public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnStnClickedSearch();
	virtual void OnOK();
	afx_msg void OnEnChangeEdit1();
//	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMKillfocusList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMKillfocusList3(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnHdnItemdblclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVCard();
	afx_msg void OnChat();
	afx_msg void OnSendFile();
	afx_msg void OnDelete();
	afx_msg void OnSetNickName();
	afx_msg void OnShield();
	afx_msg void OnMoveTo();
	afx_msg void OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeletegroup();
	afx_msg void OnChangegname();
	afx_msg void OnAddgroup();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
