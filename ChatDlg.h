#pragma once
#include "afxcmn.h"

// CChatDlg 对话框

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatDlg();

// 对话框数据
	enum { IDD = IDD_CHAT };
	int f_index;
	CHARFORMAT2 recvcf;
	PARAFORMAT2 recvpf;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//消息响应函数原型声明
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);


private:
	CString fromId;
	bool isNewSession = true;
	virtual void setRichEditStyle(int type);
	virtual void showOneStr(CString oneStr, int f_index);
	virtual void InsertPlotBitmap(CString srcStr);
//	virtual void InsertBitmap(CRichEditCtrl *edit, HBITMAP hBitmap, int position);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CRichEditCtrl m_recvRichEdit;
	LPSTORAGE m_lpStorage;  // provides storage for m_lpObject
	LPLOCKBYTES m_lpLockBytes;  // part of implementation of m_lpStorage
	LPOLEOBJECT m_lpObject; // in case you want direct access to the OLE object
	LPVIEWOBJECT2 m_lpViewObject;// IViewObject for IOleObject above
};
