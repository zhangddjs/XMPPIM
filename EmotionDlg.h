#pragma once
#include "afxcmn.h"
#include "MyList.h"


// CEmotionDlg �Ի���

class CEmotionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEmotionDlg)

public:
	CEmotionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEmotionDlg();

	CRect re;		//��ʾλ��
	CString emo;
// �Ի�������
	enum { IDD = IDD_EMOTIONDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
