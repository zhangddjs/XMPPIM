// RegisterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg 对话框

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterDlg::IDD, pParent)
	, m_user(_T(""))
	, m_pwd(_T(""))
	, m_cfmpwd(_T(""))
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_USER, m_user);
	DDX_Text(pDX, IDC_PWD, m_pwd);
	DDX_Text(pDX, IDC_CFMPWD, m_cfmpwd);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegisterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRegisterDlg 消息处理程序


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CRegisterDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("输入内容不能为空"));
		return;
	}
	if (m_pwd != m_cfmpwd)
	{
		MessageBox(TEXT("两次密码输入不匹配"));
		return;
	}
//	strcpy_s(theApp.regUser, theApp.r->CStringToChar(m_user));

	CDialogEx::OnOK();
}


void CRegisterDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	OnBnClickedOk();

}
