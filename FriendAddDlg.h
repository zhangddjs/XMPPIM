#pragma once


// CFriendAddDlg �Ի���

class CFriendAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFriendAddDlg)

public:
	CFriendAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFriendAddDlg();

// �Ի�������
	enum { IDD = IDD_FRIENDADD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_edit;
public:
	afx_msg void OnBnClickedOk();
	virtual void OnCancel();
};
