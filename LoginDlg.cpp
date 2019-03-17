// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include <iostream>
//#include "mysql.h"
#include "winsock.h"
#include "Netlistmgr.h"
#include <Sensapi.h>
#pragma comment(lib, "Sensapi.lib")


// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CLoginDlg::IDD, pParent)
, m_user(_T(""))
, m_pwd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_user);
	DDX_Text(pDX, IDC_EDIT2, m_pwd);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������

UINT _TestConnect(LPVOID pParam);
void CLoginDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��½��ť����¼�
//	theApp.r = new Message_Test();
	UpdateData(TRUE);

	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("�������ݲ���Ϊ��"));
		return;
	}
	
	//CString ת char*
	char* pUser = new char[50];
	sprintf(pUser, "%S", m_user);
	char* pPwd = new char[50];
	sprintf(pPwd, "%S", m_pwd);
	theApp.r->init(pUser, pPwd);
	delete []pUser;
	delete []pPwd;
	//����������ͨ״��
	if (!judgeNetwork())
	{
		return;
	}
// 	if (theApp.r->j->login())
// 	{
// 		MessageBox(_T("access"));
// 		return;
// 	}
// 	else
// 	{
// 		MessageBox(_T("not access"));
// 		return;
// 	}

	//�����߳�����
	AfxBeginThread(_TestConnect, theApp.r, THREAD_PRIORITY_NORMAL);

//	Sleep(4000);		//3000ms��������������ô��Ӧ��
	//���Կ���whileѭ���ж���ֱ�����ӳɹ��˳�ѭ��
	//ѭ���ж��Ƿ������ϣ�ֱ����ʱ
	m_progress.SetRange(0, 100);//���ý������ķ�Χ
	m_progress.SetStep(1);	//���ý�������ÿһ��������
	m_progress.SetPos(0);	//���ý������ĵ�ǰλ��
	m_progress.ShowWindow(SW_SHOW);
	int count = 0;
	while (theApp.r->conFlag)
	{
		m_progress.SetPos(count);
		m_progress.StepIt();
		Sleep(50);
		count++;
		if (count > 100) break;
	}
	if (theApp.r->conFlag)
	{
//		MessageBox(TEXT("���ӳɹ���"));
		m_progress.ShowWindow(SW_HIDE);
	}
	else
	{
		theApp.r->conFlag = true;
		MessageBox(TEXT("�޷����ӣ�"), _T("����"), MB_ICONERROR);
//		delete(theApp.r);
		m_progress.ShowWindow(SW_HIDE);
		return;
	}

	JID jid(theApp.r->CStringToJID(m_user));
	theApp.r->getVcard(jid, 0);
	theApp.r->myfileRecvPath = theApp.r->fileRecvPath;
	theApp.r->myfileRecvPath += jid.bare().c_str();
	if (!PathIsDirectory(theApp.r->myfileRecvPath))
	{
		::CreateDirectory(theApp.r->myfileRecvPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
	}
	theApp.r->myfileRecvPath += _T("\\");

	CDialog::OnCancel();
	//�������ݿ�
// 	MYSQL mysql;
// 	mysql_init(&mysql);
// 	if (!mysql_real_connect(&mysql, /*"144.202.14.151"*/"localhost", "root", "123456", "im", 3306, NULL, 0))
// 	{
// 		AfxMessageBox(_T("�������ݿ�ʧ��!"));
// 		exit(0);
// 	}
// 	else
// 	{
// 		//	AfxMessageBox(_T("�������ݿ�ɹ�!"));
// 		mysql_query(&mysql, "SET NAMES 'GB2312'");//��������ݿ��ж�ȡ���ݺ���������ʾ������  
// 	}
// 	MYSQL_RES *res;//�������ݲ�ѯ���  
// 	MYSQL_ROW row;//ȡÿ�н���Ķ���
// 
// 	char query[100] = "select *"
// 		" from im_user"
// 		" where im_user.userId = '";
// 	CStringA temp(m_user.GetBuffer(0)); //ͨ��ת����temp������ԭ���ַ����Ķ��ֽ���ʽ 
// 	m_user.ReleaseBuffer();
// 	std::string s(temp.GetBuffer(0));
// 	temp.ReleaseBuffer();
// 	const char *uid = s.c_str();
// 	strcat_s(query, uid);
// 	strcat_s(query, "'");
// 	mysql_real_query(&mysql, query, strlen(query));//��ѯ������������ַ�����ʾ  
// 	res = mysql_store_result(&mysql);//�õ���ѯ���  
// 	if (row = mysql_fetch_row(res))//����ȥ����ѯ���������  	//�û���һ��
// 	{
// 		//���ݲ������������ֱ����row[i]����ʽȡ����������Ǿ�Ϊchar*��  
// 		//CString myreaddata(row[0]);
// 		strcat_s(theApp.tUserId, row[0]);
// 		strcat_s(theApp.userId, pUser);
// //		strcat_s(theApp.userPwd, pPwd);
// /*		char updateQuery[100] = "update im_user set userStatus=1 where tUserId=";*/
// //		strcat_s(updateQuery, theApp.tUserId);
// // 		if (mysql_query(&mysql, updateQuery))
// // 		{
// // 			AfxMessageBox(_T("�޸����ݱ���Ŀʧ��"));
// // 		}
// 	}
// 	else
// 	{
// 		MessageBox(TEXT("�û�������"));
// 	}
// 	mysql_free_result(res);
// 	mysql_close(&mysql);




	//��ȡ�Լ�����Ϣ







	

}


//judgeNetwork;
bool CLoginDlg::judgeNetwork()
{
	//Judge Network is Connected  
	DWORD dw;
	if (!IsNetworkAlive(&dw))
	{
		MessageBox(TEXT("���������쳣��"), _T("����"), MB_ICONERROR);
		return false;
	}
	//ping baidu
	DWORD n = WinExecAndWait32(_T("ping.exe"), _T("119.75.217.109 -n 2"), NULL, 3000, TRUE, SW_HIDE);
	if (n == 0)
	{
		//	MessageBox(_T("ͨ������"));
		return true;
	}
	else
	{
		CString sNetWorkConnect;
		sNetWorkConnect = _T("�������ӳ�ʱ��");
		MessageBox(sNetWorkConnect, _T("����"), MB_ICONERROR);
		return false;
	}
}
//�߳�

UINT _TestConnect(LPVOID pParam)
{
	theApp.r->start();//	AfxMessageBox(TEXT("�������ӣ�"));		//������
	return 0;
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//Ĭ�ϵ�½��Ϣ
	m_user = "hfuu@xmpp.jp";
	m_pwd = "123456";

	//ͬ�����ؼ���
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CLoginDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	exit(0);
}


void CLoginDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	OnBnClickedButton1();
	//CDialogEx::OnOK();
}


void CLoginDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�رյ�ǰ�Ի���
	//CDialogEx::OnClose();

	//�˳�����
	exit(0);
}


//ping����
DWORD CLoginDlg::WinExecAndWait32(LPCTSTR lpszAppPath,
	LPCTSTR lpParameters,
	LPCTSTR lpszDirectory,
	DWORD dwMilliseconds,
	BOOL bIsWait,
	int nShow)
{
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = lpszAppPath;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.lpDirectory = lpszDirectory;
	ShExecInfo.nShow = nShow; //SW_SHOW  
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	if (ShExecInfo.hProcess == NULL)
		return 1;
	if (!bIsWait)
		return 0;
	if (WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds) == WAIT_TIMEOUT)
	{
		TerminateProcess(ShExecInfo.hProcess, 0);
		return 1;
	}
	DWORD dwExitCode;
	BOOL bOK = GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
	ASSERT(bOK);
	return dwExitCode;
}