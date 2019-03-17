// FriendAddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "FriendAddDlg.h"
#include "afxdialogex.h"


// CFriendAddDlg 对话框

IMPLEMENT_DYNAMIC(CFriendAddDlg, CDialogEx)

CFriendAddDlg::CFriendAddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFriendAddDlg::IDD, pParent)
	, m_edit(_T(""))
{

}

CFriendAddDlg::~CFriendAddDlg()
{
}

void CFriendAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CFriendAddDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFriendAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFriendAddDlg 消息处理程序


void CFriendAddDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);	//同步到m_edit中
	if (m_edit == "")
	{
		MessageBox(_T("不能为空！"));
		return;
	}
	CString str_connect = m_edit + "@xmpp.jp";
	char* pFid = new char[200];
	sprintf(pFid, "%S", str_connect);
	JID jid(pFid);
	delete[] pFid;
	for (int i = 0; i < theApp.friendNum; i++)
	{
		if (str_connect == theApp.friends[i].friendId)
		{
			MessageBox(_T("好友已添加！"));
			return;
		}
	}
	theApp.r->WDAddFriend(jid, "");
	CDialogEx::OnOK();
}


void CFriendAddDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}
