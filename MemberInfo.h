#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMemberInfo 对话框

class CMemberInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMemberInfo)

public:
	CMemberInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMemberInfo();

// 对话框数据
	enum { IDD = IDD_MEMBERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CStatic m_infoTitle;
	CStatic m_pic;
	CString m_fullNameEdit;
	CString m_nickNameEdit;
	CString m_addressEdit;
	CString m_emailEdit;
	CString m_phoneNumEdit;
	virtual void OnOK();
	CButton m_btnSave;
public:
	afx_msg void OnBnClickedButton1();
private:
	CProgressCtrl m_progress;
	CString m_signatureEdit;
	CString m_descInfoEdit;
};
