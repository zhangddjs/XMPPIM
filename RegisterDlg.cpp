// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg �Ի���

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


// CRegisterDlg ��Ϣ�������


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CRegisterDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_user.IsEmpty() || m_pwd.IsEmpty())
	{
		MessageBox(TEXT("�������ݲ���Ϊ��"));
		return;
	}
	if (m_pwd != m_cfmpwd)
	{
		MessageBox(TEXT("�����������벻ƥ��"));
		return;
	}
//	strcpy_s(theApp.regUser, theApp.r->CStringToChar(m_user));

	CDialogEx::OnOK();
}


void CRegisterDlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
	OnBnClickedOk();

}
