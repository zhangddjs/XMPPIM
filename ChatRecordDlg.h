#pragma once
#include "afxcmn.h"


// CChatRecordDlg �Ի���

class CChatRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatRecordDlg)

public:
	CChatRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatRecordDlg();

// �Ի�������
	enum { IDD = IDD_CHATRECORDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
