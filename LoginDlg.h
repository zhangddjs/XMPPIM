#pragma once
#include "afxcmn.h"


// CLoginDlg �Ի���

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

	// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_user;
	CString m_pwd;
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	virtual void OnOK();
	afx_msg void OnClose();
	virtual DWORD CLoginDlg::WinExecAndWait32(LPCTSTR lpszAppPath,
		LPCTSTR lpParameters,
		LPCTSTR lpszDirectory,
		DWORD dwMilliseconds,
		BOOL bIsWait,
		int nShow);
	bool CLoginDlg::judgeNetwork();

private:
	CProgressCtrl m_progress;
};
