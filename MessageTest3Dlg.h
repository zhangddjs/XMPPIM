
// MessageTest3Dlg.h : 头文件
//

#pragma once
#include "TabSheet.h"
#include "ChatDlg.h"
#include "afxwin.h"
#include "afxcmn.h"

//线程结构体
// struct RECVPARAM
// {
// 	HWND hwnd;
// };
#define WM_RECVDATA WM_USER+1
#define WM_CLOSSCHATWND WM_USER+4
// CMessageTest3Dlg 对话框
class CMessageTest3Dlg : public CDialogEx
{
// 构造
public:
	CMessageTest3Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMessageTest3Dlg();
// 对话框数据
	enum { IDD = IDD_MESSAGETEST3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//消息响应函数原型声明
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CChatDlg *chatDlg = new CChatDlg[200];

public:
	//线程
	static DWORD WINAPI RecvProc(LPVOID lpParameter);


	afx_msg void OnBnClicked1003();

private:
//	CString fromId;
	virtual void OnOK();
	CTabSheet m_tab;
	CString sessionList[50];		//会话个数，记录fromId
	int sessionNum = 0;
public:
	afx_msg void OnClose();
private:
	CStatic m_sendFilePic;
public:
	afx_msg void OnStnClickedSendfilepic();
	afx_msg void OnStnClickedEmotion();
private:
	CStatic m_emotionPic;
	CRichEditCtrl m_send;
	CStatic m_historyPic;
public:
	afx_msg void OnStnClickedHistory();
};

//static CMessageTest3Dlg m_MsgDlg = new CMessageTest3Dlg();

