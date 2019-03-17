
// Test1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "TabSheet.h"
#include "RecentDlg.h"
#include "FriendsDlg.h"
#include "GroupsDlg.h"
#include "FilesDlg.h"
#include "MessageTest3Dlg.h"
#include "afxcmn.h"

//线程结构体
struct RECVPARAM
{
	HWND hwnd;		//主窗口
};

// CTest1Dlg 对话框
class CTest1Dlg : public CDialogEx
{
// 构造
public:
	CTest1Dlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CTest1Dlg();
// 对话框数据
	enum { IDD = IDD_TEST1_DIALOG };

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

	//消息响应函数原型声明 data,friend,recent,group
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvFriend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClossChatWnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvSubReq(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvRemved(WPARAM wParam, LPARAM lParam);		//被删除
	afx_msg LRESULT OnRecvFile(WPARAM wParam, LPARAM lParam);		//接受文件后刷新
	DECLARE_MESSAGE_MAP()
private:
	CStatic m_pic;
	CStatic m_nickName;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CStatic m_signature;
	CTabSheet m_tab;
	CRecentDlg recentDlg;
	CFriendsDlg friendsDlg;
	CGroupsDlg groupsDlg;
	CFilesDlg filesDlg;
	CMessageTest3Dlg *m_msgDlg = new CMessageTest3Dlg();
	CMenu menu, *statusMenu;

public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	//线程回调函数
//	static DWORD WINAPI RecvProc(LPVOID lpParameter);

	virtual void OnOK();
	afx_msg void OnClose();
private:
	CStatic m_info;
public:
	afx_msg void OnStnClickedNickname();
//	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnStnClickedPic();
private:
	CProgressCtrl m_progress;
	CStatic m_statusPic;
public:
	afx_msg void OnStnClickedStatuspic();
	afx_msg void OnStatusonline();
	afx_msg void OnStatusbusy();
	afx_msg void OnStatusaway();
	afx_msg void OnStatushide();
	afx_msg void OnStatusoffline();
};
