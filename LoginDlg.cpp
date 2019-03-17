// LoginDlg.cpp : 实现文件
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


// CLoginDlg 对话框

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


// CLoginDlg 消息处理程序

UINT _TestConnect(LPVOID pParam);
void CLoginDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	//登陆按钮点击事件
//	theApp.r = new Message_Test();
	UpdateData(TRUE);

	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("输入内容不能为空"));
		return;
	}
	
	//CString 转 char*
	char* pUser = new char[50];
	sprintf(pUser, "%S", m_user);
	char* pPwd = new char[50];
	sprintf(pPwd, "%S", m_pwd);
	theApp.r->init(pUser, pPwd);
	delete []pUser;
	delete []pPwd;
	//测试网络联通状况
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

	//调用线程链接
	AfxBeginThread(_TestConnect, theApp.r, THREAD_PRIORITY_NORMAL);

//	Sleep(4000);		//3000ms都不够。。。怎么办应该
	//所以可以while循环判定，直到链接成功退出循环
	//循环判断是否链接上，直到超时
	m_progress.SetRange(0, 100);//设置进度条的范围
	m_progress.SetStep(1);	//设置进度条的每一步的增量
	m_progress.SetPos(0);	//设置进度条的当前位置
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
//		MessageBox(TEXT("链接成功！"));
		m_progress.ShowWindow(SW_HIDE);
	}
	else
	{
		theApp.r->conFlag = true;
		MessageBox(TEXT("无法连接！"), _T("错误"), MB_ICONERROR);
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
		::CreateDirectory(theApp.r->myfileRecvPath, NULL);//创建目录,已有的话不影响  
	}
	theApp.r->myfileRecvPath += _T("\\");

	CDialog::OnCancel();
	//链接数据库
// 	MYSQL mysql;
// 	mysql_init(&mysql);
// 	if (!mysql_real_connect(&mysql, /*"144.202.14.151"*/"localhost", "root", "123456", "im", 3306, NULL, 0))
// 	{
// 		AfxMessageBox(_T("访问数据库失败!"));
// 		exit(0);
// 	}
// 	else
// 	{
// 		//	AfxMessageBox(_T("链接数据库成功!"));
// 		mysql_query(&mysql, "SET NAMES 'GB2312'");//解决从数据库中读取数据后汉字乱码显示的问题  
// 	}
// 	MYSQL_RES *res;//声明数据查询结果  
// 	MYSQL_ROW row;//取每行结果的对象
// 
// 	char query[100] = "select *"
// 		" from im_user"
// 		" where im_user.userId = '";
// 	CStringA temp(m_user.GetBuffer(0)); //通过转化，temp接受了原来字符串的多字节形式 
// 	m_user.ReleaseBuffer();
// 	std::string s(temp.GetBuffer(0));
// 	temp.ReleaseBuffer();
// 	const char *uid = s.c_str();
// 	strcat_s(query, uid);
// 	strcat_s(query, "'");
// 	mysql_real_query(&mysql, query, strlen(query));//查询函数，语句用字符串表示  
// 	res = mysql_store_result(&mysql);//得到查询结果  
// 	if (row = mysql_fetch_row(res))//逐行去除查询结果并操作  	//用户名一致
// 	{
// 		//数据操作，这里可以直接以row[i]的形式取出数据项，但是均为char*行  
// 		//CString myreaddata(row[0]);
// 		strcat_s(theApp.tUserId, row[0]);
// 		strcat_s(theApp.userId, pUser);
// //		strcat_s(theApp.userPwd, pPwd);
// /*		char updateQuery[100] = "update im_user set userStatus=1 where tUserId=";*/
// //		strcat_s(updateQuery, theApp.tUserId);
// // 		if (mysql_query(&mysql, updateQuery))
// // 		{
// // 			AfxMessageBox(_T("修改数据表条目失败"));
// // 		}
// 	}
// 	else
// 	{
// 		MessageBox(TEXT("用户名有误"));
// 	}
// 	mysql_free_result(res);
// 	mysql_close(&mysql);




	//获取自己的信息







	

}


//judgeNetwork;
bool CLoginDlg::judgeNetwork()
{
	//Judge Network is Connected  
	DWORD dw;
	if (!IsNetworkAlive(&dw))
	{
		MessageBox(TEXT("网络连接异常！"), _T("错误"), MB_ICONERROR);
		return false;
	}
	//ping baidu
	DWORD n = WinExecAndWait32(_T("ping.exe"), _T("119.75.217.109 -n 2"), NULL, 3000, TRUE, SW_HIDE);
	if (n == 0)
	{
		//	MessageBox(_T("通信正常"));
		return true;
	}
	else
	{
		CString sNetWorkConnect;
		sNetWorkConnect = _T("网络连接超时！");
		MessageBox(sNetWorkConnect, _T("错误"), MB_ICONERROR);
		return false;
	}
}
//线程

UINT _TestConnect(LPVOID pParam)
{
	theApp.r->start();//	AfxMessageBox(TEXT("正在连接！"));		//进度条
	return 0;
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//默认登陆信息
	m_user = "hfuu@xmpp.jp";
	m_pwd = "123456";

	//同步到控件中
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CLoginDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	exit(0);
}


void CLoginDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	OnBnClickedButton1();
	//CDialogEx::OnOK();
}


void CLoginDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//关闭当前对话框
	//CDialogEx::OnClose();

	//退出程序
	exit(0);
}


//ping程序
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