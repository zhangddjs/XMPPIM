#pragma once
#include "afxcmn.h"
#include "MyList.h"


// CEmotionDlg 对话框

class CEmotionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEmotionDlg)

public:
	CEmotionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEmotionDlg();

	CRect re;		//显示位置
	CString emo;
// 对话框数据
	enum { IDD = IDD_EMOTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CMyList m_list;
	CMyList m_list2;
	CMyList m_list3;
	CMyList m_list4;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList6(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
