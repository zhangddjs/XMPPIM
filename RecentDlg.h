#pragma once
#include "afxcmn.h"
#include "MyList.h"

// RecentDlg �Ի���

class CRecentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecentDlg)

public:
	CRecentDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRecentDlg();
//	void AutoAdjustColumnWidth(CListCtrl *pListCtrl);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// �Ի�������
	enum { IDD = IDD_RECENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CMyList m_list;
public:
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
};
