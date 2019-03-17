#pragma once


// CFriendAddDlg 对话框

class CFriendAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendAddDlg)

public:
	CFriendAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFriendAddDlg();

// 对话框数据
	enum { IDD = IDD_FRIENDADD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_edit;
public:
	afx_msg void OnBnClickedOk();
	virtual void OnCancel();
};
