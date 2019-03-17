#pragma once
#include "afxcmn.h"


// CChatRecordDlg 对话框

class CChatRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRecordDlg)

public:
	CChatRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatRecordDlg();

// 对话框数据
	enum { IDD = IDD_CHATRECORDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int f_index;
	CString logSrc;
private:
	CRichEditCtrl m_record;
	CTreeCtrl m_tree;
	CHARFORMAT2 recvcf;
	PARAFORMAT2 recvpf;

private:
	virtual void showOneStr(CString oneStr, int f_index);
	virtual void setRichEditStyle(int type);
public:
	virtual BOOL OnInitDialog();
};
