
// MessageTest3Dlg.h : ͷ�ļ�
//

#pragma once
#include "TabSheet.h"
#include "ChatDlg.h"
#include "afxwin.h"
#include "afxcmn.h"

//�߳̽ṹ��
// struct RECVPARAM
// {
// 	HWND hwnd;
// };
#define WM_RECVDATA WM_USER+1
#define WM_CLOSSCHATWND WM_USER+4
// CMessageTest3Dlg �Ի���
class CMessageTest3Dlg : public CDialogEx
{
// ����
public:
	CMessageTest3Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMessageTest3Dlg();
// �Ի�������
	enum { IDD = IDD_MESSAGETEST3_DIALOG };

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

	//��Ϣ��Ӧ����ԭ������
	afx_msg LRESULT OnRecvData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CChatDlg *chatDlg = new CChatDlg[200];

public:
	//�߳�
	static DWORD WINAPI RecvProc(LPVOID lpParameter);


	afx_msg void OnBnClicked1003();

private:
//	CString fromId;
	virtual void OnOK();
	CTabSheet m_tab;
	CString sessionList[50];		//�Ự��������¼fromId
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

