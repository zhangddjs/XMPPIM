// EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "EditDlg.h"
#include "afxdialogex.h"


// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
	, m_edit(_T(""))
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
END_MESSAGE_MAP()


// CEditDlg 消息处理程序
