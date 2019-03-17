// FriendAddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "FriendAddDlg.h"
#include "afxdialogex.h"


// CFriendAddDlg �Ի���

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


// CFriendAddDlg ��Ϣ�������


void CFriendAddDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);	//ͬ����m_edit��
	if (m_edit == "")
	{
		MessageBox(_T("����Ϊ�գ�"));
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
			MessageBox(_T("��������ӣ�"));
			return;
		}
	}
	theApp.r->WDAddFriend(jid, "");
	CDialogEx::OnOK();
}


void CFriendAddDlg::OnCancel()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}
