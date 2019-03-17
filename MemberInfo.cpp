// MemberInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "MemberInfo.h"
#include "afxdialogex.h"
#include "Friend.h"


// CMemberInfo 对话框

IMPLEMENT_DYNAMIC(CMemberInfo, CDialogEx)

CMemberInfo::CMemberInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMemberInfo::IDD, pParent)
	, m_fullNameEdit(_T(""))
	, m_nickNameEdit(_T(""))
	, m_addressEdit(_T(""))
	, m_emailEdit(_T(""))
	, m_phoneNumEdit(_T(""))
	, m_signatureEdit(_T(""))
	, m_descInfoEdit(_T(""))
{

}

CMemberInfo::~CMemberInfo()
{
}

void CMemberInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFOTITLE, m_infoTitle);
	DDX_Control(pDX, IDC_PICTURE, m_pic);
	DDX_Text(pDX, IDC_FULLNAME, m_fullNameEdit);
	DDX_Text(pDX, IDC_NICKNAME, m_nickNameEdit);
	DDX_Text(pDX, IDC_FADDRESS, m_addressEdit);
	DDX_Text(pDX, IDC_EMAIL, m_emailEdit);
	DDX_Text(pDX, IDC_TELEPHONE, m_phoneNumEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSave);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_SIGNATURE, m_signatureEdit);
	DDX_Text(pDX, IDC_DESCINFO, m_descInfoEdit);
}


BEGIN_MESSAGE_MAP(CMemberInfo, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMemberInfo::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMemberInfo 消息处理程序


BOOL CMemberInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//获取对象
	CFriend *myFriend;
	myFriend = &(theApp.friends[theApp.friendInfoNum]);
	if (0 == theApp.friendInfoNum)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_FULLNAME);
		pEdit->SetReadOnly(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_NICKNAME);
		pEdit->SetReadOnly(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_FADDRESS);
		pEdit->SetReadOnly(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_EMAIL);
		pEdit->SetReadOnly(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_TELEPHONE);
		pEdit->SetReadOnly(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_SIGNATURE);
		pEdit->SetReadOnly(FALSE);
		pEdit = (CEdit*)GetDlgItem(IDC_DESCINFO);
		pEdit->SetReadOnly(FALSE);
		m_btnSave.ShowWindow(SW_SHOW);
		m_btnSave.EnableWindow(TRUE);
	}
	//显示标题
	CFont * cFont = new CFont;
	cFont->CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH&FF_SWISS, _T("Arial"));
	m_infoTitle.SetFont(cFont);
	delete cFont;
	m_infoTitle.SetWindowTextW(myFriend->friendId);
	//显示信息
	m_fullNameEdit = myFriend->f_name;
	m_nickNameEdit = myFriend->nickName;
	m_addressEdit = myFriend->f_address;
	m_emailEdit = myFriend->f_email;
	m_phoneNumEdit = myFriend->f_phoneNum;
	m_signatureEdit = myFriend->f_signature;
	m_descInfoEdit = myFriend->descInfo;
	UpdateData(FALSE);







	//显示头像
	if (myFriend->headIconUrl != "")
	{
		CRect bmpRect;
		m_pic.GetWindowRect(&bmpRect);
		CString friendHeadIcon(myFriend->headIconUrl);		//位图路径
		CImage image;
		image.Load(friendHeadIcon);
		HBITMAP hBmp = image.Detach();
		m_pic.ModifyStyle(NULL, SS_BITMAP);
		m_pic.SetBitmap(hBmp);
		m_pic.ShowWindow(true);
		image.Destroy();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CMemberInfo::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
//	CDialogEx::OnOK();
}

//保存
void CMemberInfo::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	theApp.friends[0].f_name = m_fullNameEdit;
	theApp.friends[0].nickName = m_nickNameEdit;
	theApp.friends[0].f_address = m_addressEdit;
	theApp.friends[0].f_email = m_emailEdit;
	theApp.friends[0].f_phoneNum = m_phoneNumEdit;
	theApp.friends[0].f_signature = m_signatureEdit;
	theApp.friends[0].descInfo = m_descInfoEdit;



	//获取进度条长度
	CFile file;
	DWORD dwDataLen;
	if (file.Open(theApp.friends[0].headIconUrl, CFile::modeRead | CFile::typeBinary))
	{
		dwDataLen = file.GetLength();
		file.Close();
	}
	else
	{
		dwDataLen = 30;
	}
	//修改信息
	char* pFid = new char[200];
	sprintf(pFid, "%S", theApp.friends[0].friendId);
	JID jid(pFid);
	theApp.r->setVcard(jid, 0, 2);		//修改基本信息
	delete[] pFid;
	//设置进度条
	//获取进度条的位置范围的最大值和最小值
	m_progress.SetRange(0, 100);//设置进度条的范围
	m_progress.SetStep(1);	//设置进度条的每一步的增量
	m_progress.SetPos(0);	//设置进度条的当前位置
	m_progress.ShowWindow(SW_SHOW);
	int n = dwDataLen / 30;
	int count = 0;
	while (count < 150)
	{
		if (count >= 100)
		{
			m_progress.SetPos(100);
			//m_progress.StepIt();
			Sleep(10);
			count++;
			continue;
		}
		m_progress.SetPos(count);
		m_progress.StepIt();
		Sleep(n);
		count++;
	}
	m_progress.ShowWindow(SW_HIDE);



}
