
// Test1Dlg.h : ͷ�ļ�
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

//�߳̽ṹ��
struct RECVPARAM
{
	HWND hwnd;		//������
};

// CTest1Dlg �Ի���
class CTest1Dlg : public CDialogEx
{
// ����
public:
	CTest1Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CTest1Dlg();
// �Ի�������
	enum { IDD = IDD_TEST1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//��Ϣ��Ӧ����ԭ������ data,friend,recent,group
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvFriend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClossChatWnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvSubReq(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvRemved(WPARAM wParam, LPARAM lParam);		//��ɾ��
	afx_msg LRESULT OnRecvFile(WPARAM wParam, LPARAM lParam);		//�����ļ���ˢ��
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
	//�̻߳ص�����
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
