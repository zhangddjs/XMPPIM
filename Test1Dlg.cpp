
// Test1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "Test1Dlg.h"
#include "afxdialogex.h"
#include "winsock.h"    
//#include "mysql.h" 
#include "Message_Test.h"
#include "MessageTest3Dlg.h"
#include "LoginDlg.h"
#include "MemberInfo.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTest1Dlg 对话框



CTest1Dlg::CTest1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTest1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTest1Dlg::~CTest1Dlg()
{
	delete m_msgDlg;
}

void CTest1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Control(pDX, IDC_NICKNAME, m_nickName);
	DDX_Control(pDX, IDC_SIGNATURE, m_signature);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_PROGRESS2, m_progress);
	DDX_Control(pDX, IDC_STATUSPIC, m_statusPic);
}

BEGIN_MESSAGE_MAP(CTest1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	//消息映射  消息本身，消息响应函数
	ON_MESSAGE(WM_RECVDATA, OnRecvData)				//1
	ON_MESSAGE(WM_RECVFRIEND, OnRecvFriend)			//2
	ON_MESSAGE(WM_RECVINFO, OnRecvInfo)				//3
	ON_MESSAGE(WM_CLOSSCHATWND, OnClossChatWnd)		//4
	ON_MESSAGE(WM_UPDATEINFO, OnUpdateInfo)			//5
	ON_MESSAGE(WM_RECVSUBREQ, OnRecvSubReq)			//6
	ON_MESSAGE(WM_RECVREMVED, OnRecvRemved)			//7
	ON_MESSAGE(WM_RECVFILE, OnRecvFile)			//8
//	ON_WM_SIZE()
ON_WM_CLOSE()
ON_STN_CLICKED(IDC_NICKNAME, &CTest1Dlg::OnStnClickedNickname)
//ON_WM_DROPFILES()
ON_STN_CLICKED(IDC_PIC, &CTest1Dlg::OnStnClickedPic)
ON_STN_CLICKED(IDC_STATUSPIC, &CTest1Dlg::OnStnClickedStatuspic)
ON_COMMAND(ID_STATUSONLINE, &CTest1Dlg::OnStatusonline)
ON_COMMAND(ID_STATUSBUSY, &CTest1Dlg::OnStatusbusy)
ON_COMMAND(ID_STATUSAWAY, &CTest1Dlg::OnStatusaway)
ON_COMMAND(ID_STATUSHIDE, &CTest1Dlg::OnStatushide)
ON_COMMAND(ID_STATUSOFFLINE, &CTest1Dlg::OnStatusoffline)
END_MESSAGE_MAP()



// CTest1Dlg 消息处理程序
//全局线程
// DWORD WINAPI RecvProc(
// 	_In_ LPVOID lpParameter
// );
BOOL CTest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
//	//创建线程接受数据，给线程传递套接字、接收对话框的句柄
	RECVPARAM *pRecvParam = new RECVPARAM();
	pRecvParam->hwnd = m_hWnd;
// 	HANDLE hThread = CreateThread(NULL, 0, RecvProc, pRecvParam, 0, NULL);
// 	CloseHandle(hThread);
	theApp.r->SetFromHwnd(pRecvParam);
	delete pRecvParam;
	CLoginDlg loginDlg;
	loginDlg.DoModal();

	CString welcome(_T("欢迎回来！"));
	CString uid(theApp.userId);
	welcome += uid;
//	AfxMessageBox(welcome);



	//设置对话框大小,暂定尺寸350,400
//	CRect temprect(0, 0, 350, 400);
//	CWnd::SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	//在CStatic静态文本框中，显示位图，控制位图大小
// 	CRect bmpRect;
// 	m_pic.GetWindowRect(&bmpRect);
// 	CString userHeadIcon;
// 	userHeadIcon = theApp.friends[0].headIconUrl == "" ? theApp.friends[0].headIconUrl : (LPCTSTR)theApp.userHeadIcon;
// 	HBITMAP hBmp = (HBITMAP)::LoadImage(0, userHeadIcon, IMAGE_BITMAP, bmpRect.Width(), bmpRect.Height(), LR_LOADFROMFILE);
// 	m_pic.ModifyStyle(NULL, SS_BITMAP);
// 	m_pic.SetBitmap(hBmp);
	//初始化状态功能
	m_statusPic.ModifyStyle(0, SS_ICON);
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_ONLINE));
	menu.LoadMenu(IDR_TREEMENU1);//加载菜单资源
	statusMenu = menu.GetSubMenu(2);//获取菜单子项
	//加载图标
	CBitmap *pMenuBitmap; //需要定义为成员变量
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_ONLINE);
	statusMenu->SetMenuItemBitmaps(0,/*第几个功能，从0开始*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*属性*/pMenuBitmap, pMenuBitmap);
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_BUSY);
	statusMenu->SetMenuItemBitmaps(1,/*第几个功能，从0开始*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*属性*/pMenuBitmap, pMenuBitmap);
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_AWAY);
	statusMenu->SetMenuItemBitmaps(2,/*第几个功能，从0开始*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*属性*/pMenuBitmap, pMenuBitmap);
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_OFFLINE);//需要加载的图标资源
	statusMenu->SetMenuItemBitmaps(3,/*第几个功能，从0开始*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*属性*/pMenuBitmap, pMenuBitmap);
	statusMenu->SetMenuItemBitmaps(4,/*第几个功能，从0开始*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*属性*/pMenuBitmap, pMenuBitmap);

	
	m_pic.ModifyStyle(NULL, SS_BITMAP);
	if (theApp.friends[0].headIconUrl != "")
	{
		CRect bmpRect;
		m_pic.GetWindowRect(&bmpRect);
		CString friendHeadIcon(theApp.friends[0].headIconUrl);		//位图路径
		CImage image;
		image.Load(friendHeadIcon);
		HBITMAP hBmp = image.Detach();
		m_pic.SetBitmap(hBmp);
		m_pic.ShowWindow(true);
		image.Destroy();
	}
	CFont * cFont = new CFont;
	cFont->CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH&FF_SWISS, _T("Times New Roman"));

	m_nickName.SetFont(cFont);
	delete cFont;
	CString userName;
	userName = theApp.friends[0].nickName == "" ? theApp.friends[0].friendId : theApp.friends[0].nickName;
	CString userSignature;
	userSignature = theApp.friends[0].f_signature == "" ? _T("请编辑签名") : theApp.friends[0].f_signature;
	CString userInfo(theApp.friends[0].descInfo);
	m_nickName.SetWindowTextW(userName);
	m_signature.SetWindowTextW(userSignature);
	m_info.SetWindowTextW(userInfo);
	

	//TabCtrl

	m_tab.AddPage(TEXT("消息"), &recentDlg, IDD_RECENT);
	m_tab.AddPage(TEXT("好友"), &friendsDlg, IDD_FRIENDS);
//	m_tab.AddPage(TEXT("群"), &groupsDlg, IDD_GROUPS);
	m_tab.AddPage(TEXT("文件"), &filesDlg, IDD_FILES);
	m_tab.Show();

	//链接数据库test
/*
	MYSQL m_sqlCon;
	mysql_init(&m_sqlCon);
	if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "im", 3306, NULL, 0))
	{
		AfxMessageBox(_T("访问数据库失败!"));
	}
	else
	{
		AfxMessageBox(_T("链接数据库成功!"));
		mysql_query(&m_sqlCon, "SET NAMES 'GB2312'");//解决从数据库中读取数据后汉字乱码显示的问题  
	}
*/
	::SetClassLongPtr(m_nickName.GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));		//鼠标移动到昵称上变成手指
	::SetClassLongPtr(m_pic.GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));		//鼠标移动到昵称上变成手指

	
	m_progress.SetRange(0, 100);//设置进度条的范围
	m_progress.SetStep(1);	//设置进度条的每一步的增量
	m_progress.SetPos(0);	//设置进度条的当前位置


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTest1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTest1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CTest1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == GetDlgItem(IDC_NICKNAME))
	{
		pDC->SetTextColor(RGB(0, 0, 200));
	}
	else if (pWnd == GetDlgItem(IDC_SIGNATURE))
	{
		pDC->SetTextColor(RGB(144, 144, 144));
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


//void CTest1Dlg::OnSize(UINT nType, int cx, int cy)
//{
//	CDialogEx::OnSize(nType, cx, cy);
//
//	// TODO:  在此处添加消息处理程序代码
//
//
//
//}


void CTest1Dlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
//	CDialogEx::OnOK();
}


void CTest1Dlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
// 	MYSQL mysql;
// 	mysql_init(&mysql);
// 	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "im", 3306, NULL, 0))
// 	{
// 		AfxMessageBox(_T("访问数据库失败!"));
// 	}
// 	else
// 	{
// 		//	AfxMessageBox(_T("链接数据库成功!"));
// 		mysql_query(&mysql, "SET NAMES 'GB2312'");//解决从数据库中读取数据后汉字乱码显示的问题  
// 	}
// 
// 	char updateQuery[100] = "update im_user set userStatus=0 where tUserId=";
// 	strcat_s(updateQuery, theApp.tUserId);
// 	//	CString str(query);
// 	//	AfxMessageBox(str);
// 	if (mysql_query(&mysql, updateQuery))
// 	{
// 		AfxMessageBox(_T("修改数据表条目失败"));
// 	}
// 	mysql_close(&mysql);




	CDialogEx::OnClose();
}



//线程
/*
DWORD WINAPI CTest1Dlg::RecvProc(LPVOID lpParameter)
{
	/ *SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;

	SOCKADDR_IN addrFrom;
	int len = sizeof(SOCKADDR);

	char recvBuf[200];
	char tempBuf[200];
	int retval;
	while (1)
	{
	retval = recvfrom(sock, recvBuf, 200, 0, (SOCKADDR*)&addrFrom, &len);
	if (SOCKET_ERROR == retval)
	break;
	sprintf_s(tempBuf, "%s 说 : %s", inet_ntoa(addrFrom.sin_addr), recvBuf);
	//将数据传给对话框
	::PostMessage(hwnd, WM_RECVDATA, 0, (LPARAM)tempBuf);


	}
	AfxMessageBox(_T("err"));
	* /

// 	HWND mainHwnd = ((RECVPARAM*)lpParameter)->hwnd;		//记录Test1Dlg窗口的handle
// 	CMessageTest3Dlg m_msgDlg = new CMessageTest3Dlg();
// 	HWND msgHwnd = m_msgDlg.m_hWnd;
//	Message_Test *r = new Message_Test(lpParameter);
	theApp.r->SetFromHwnd(lpParameter);
//	theApp.r->start();	AfxMessageBox(_T("haha"));
//	delete(r);// 	m_msgDlg.Create(IDD_MESSAGETEST3_DIALOG);
// 	m_msgDlg.ShowWindow(SW_SHOWNORMAL); 
	return 0;
}*/

//收到来自对方的消息
HANDLE hTest1RecvDataSemaphore;
LRESULT CTest1Dlg::OnRecvData(WPARAM wParam, LPARAM lParam)		//wParam为f_index， lParam为字符串
{
	WaitForSingleObject(hTest1RecvDataSemaphore, INFINITE);
	CString sTemp((LPCTSTR)lParam);
	static CString xmlStr;
	xmlStr.Format(_T("%s"), sTemp);
//	CString friendId((LPCTSTR)wParam);

	//向messageDlg窗口发送消息
	//将信息存到friend对应属性中
	static int i;
	i = (int)wParam;
	theApp.r->msgSaveToFile(theApp.friends[0].friendId, theApp.friends[i].friendId, xmlStr);		//保存到聊天记录
	if (theApp.friends[i].chatFlag)
	{
		if (m_msgDlg->GetSafeHwnd())   //为TRUE 说明对话框已经被创建
		{
			m_msgDlg->ShowWindow(SW_SHOWNORMAL);
			::PostMessage(m_msgDlg->GetSafeHwnd(), WM_RECVDATA, (WPARAM)i, (LPARAM)(LPCTSTR)xmlStr);
		}
		else
		{
			m_msgDlg->Create(IDD_MESSAGETEST3_DIALOG);
			::PostMessage(m_msgDlg->GetSafeHwnd(), WM_RECVDATA, (WPARAM)i, (LPARAM)(LPCTSTR)xmlStr);
		}
	}
	else
	{
		//向recent窗口发送消息。
		::PostMessage(recentDlg.GetSafeHwnd(), WM_RECVDATA, (WPARAM)i, (LPARAM)(LPCTSTR)xmlStr);
	}

 	ReleaseSemaphore(hTest1RecvDataSemaphore, 1, NULL);
	return 1;
}
//收到来自朋友状态的消息
HANDLE hTest1RecvFriendSemaphore;
LRESULT CTest1Dlg::OnRecvFriend(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(hTest1RecvFriendSemaphore, INFINITE);
	static int w, l;
	w = (int)wParam;
	l = (int)lParam;
	::PostMessage(friendsDlg.GetSafeHwnd(), WM_RECVFRIEND, w, l);

	ReleaseSemaphore(hTest1RecvFriendSemaphore, 1, NULL);
	return 1;
}
//查看朋友信息
HANDLE hTest1RecvInfoSemaphore;
LRESULT CTest1Dlg::OnRecvInfo(WPARAM wParam, LPARAM lParam)
{
	WaitForSingleObject(hTest1RecvInfoSemaphore, INFINITE);
	theApp.friendInfoNum = (int)lParam;
	CMemberInfo infoDlg;
	infoDlg.DoModal();



	ReleaseSemaphore(hTest1RecvInfoSemaphore, 1, NULL);
	return 1;
}
//销毁聊天窗口
LRESULT CTest1Dlg::OnClossChatWnd(WPARAM wParam, LPARAM lParam)
{
	delete m_msgDlg;

	m_msgDlg = new CMessageTest3Dlg();
	return 1;
}
//更新个人信息
LRESULT CTest1Dlg::OnUpdateInfo(WPARAM wParam, LPARAM lParam)
{
	CString userName;
	userName = theApp.friends[0].nickName == "" ? theApp.friends[0].friendId : theApp.friends[0].nickName;
	CString userSignature;
	userSignature = theApp.friends[0].f_signature == "" ? _T("请编辑签名") : theApp.friends[0].f_signature;
	CString userInfo(theApp.friends[0].descInfo);
	m_nickName.SetWindowTextW(userName);
	m_signature.SetWindowTextW(userSignature);
	m_info.SetWindowTextW(userInfo);
	return 1;
}
//收到好友请求	wParam为jidFull，lParam为flag
LRESULT CTest1Dlg::OnRecvSubReq(WPARAM wParam, LPARAM lParam)
{
	if ((bool)lParam)
	{
		CString jidFull((LPCTSTR)wParam);
		JID jid(theApp.r->CStringToJID(jidFull));
		static int f_index;
		theApp.friends[theApp.friendNum].friendId = jid.bare().c_str();
		theApp.friends[theApp.friendNum].friendIdFull = jid.full().c_str();
		theApp.friends[theApp.friendNum].friendGroup = "Buddies";
		theApp.r->getVcard(jid, theApp.friendNum);
		theApp.friendNum++;
//		theApp.friends[theApp.friendNum++].friendStatus = 5;
		f_index = theApp.friendNum - 1;
		::PostMessage(friendsDlg.GetSafeHwnd(), WM_RECVSUBREQ, (WPARAM)f_index, NULL);
	}
	return 1;
}
//收到被删除的消息，同时删除该好友	wParam为jidFull
LRESULT CTest1Dlg::OnRecvRemved(WPARAM wParam, LPARAM lParam)
{
	static WPARAM w;
	w = wParam;
	::PostMessage(friendsDlg.GetSafeHwnd(), WM_RECVREMVED, w, NULL);
	return 1;
}
//文件接收完毕，刷新控件
LRESULT CTest1Dlg::OnRecvFile(WPARAM wParam, LPARAM lParam)
{
	filesDlg.m_mfcShellList.Refresh();
	return 1;
}

//查看个人信息
void CTest1Dlg::OnStnClickedNickname()
{
	// TODO:  在此添加控件通知处理程序代码
	char *pJid = new char[200];
	sprintf(pJid, "%S", theApp.friends[0].friendId);
	JID jid(pJid);
	theApp.r->getVcard(jid, 0);
	delete pJid;
	static int i = 0;
	::PostMessage(this->GetSafeHwnd(), WM_RECVINFO, NULL, (LPARAM)i);
}

//点击头像
void CTest1Dlg::OnStnClickedPic()
{
	// TODO:  在此添加控件通知处理程序代码

	//文件筛选器
	CString str1;
	CString filter = _T("Bitmap    Files(*.bmp)|*.bmp|Wmf   Files(*.wmf)|*.wmf||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		str1 = dlg.GetPathName();
		
		//判断大小
		CFile file;
		file.Open(str1, CFile::modeRead | CFile::typeBinary);     
		DWORD dwDataLen = file.GetLength();
		file.Close();
		if (dwDataLen > 10240)
		{
			AfxMessageBox(_T("头像应小于10K"));
			return;
		}

		

		theApp.friends[0].headIconUrl = str1;
		char* pFid = new char[200];
		sprintf(pFid, "%S", theApp.friends[0].friendId);
		JID jid(pFid);
		theApp.r->setVcard(jid, 0, 1);		//修改头像
		delete pFid;
		//设置进度条
		//获取进度条的位置范围的最大值和最小值

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
				count ++;
				continue;
			}
			m_progress.SetPos(count);
			m_progress.StepIt();
			Sleep(n);
			count++;
		}
		m_progress.ShowWindow(SW_HIDE);
		//用每一步的增量来增加进度条的当前位置
		//获取进度条的当前位置
// 		int nPos = (m_ctrlProgress.GetPos() - nLower) * 100 / (nUpper - nLower);
// 		CString s;
// 		s.Format("%d", nPos);
// 		m_strText = s + "%";

		//显示头像
		CRect bmpRect;
		m_pic.GetWindowRect(&bmpRect);
		CString friendHeadIcon(theApp.friends[0].headIconUrl);		//位图路径
		CImage image;
		image.Load(friendHeadIcon);
		HBITMAP hBmp = image.Detach();
		m_pic.SetBitmap(hBmp);
		m_pic.ShowWindow(true);
		image.Destroy();
		
	}

}

//单击状态按钮
void CTest1Dlg::OnStnClickedStatuspic()
{
	// TODO:  在此添加控件通知处理程序代码
	CRect itemRect;
	m_statusPic.GetWindowRect(&itemRect);
	
	//菜单弹出的位置
	CRect ptMenu;
	ptMenu.left = itemRect.left;
	ptMenu.top = itemRect.bottom;
	//动态创建子菜单
	statusMenu->TrackPopupMenu(TPM_LEFTALIGN, ptMenu.left, ptMenu.top, this);    //显示菜单

}
//切换状态->在线
void CTest1Dlg::OnStatusonline()
{
	// TODO:  在此添加命令处理程序代码
	theApp.r->j->setPresence(Presence::Available, 0);
	theApp.friends[0].friendStatus = Presence::Available;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_ONLINE));
}
//切换状态->忙碌
void CTest1Dlg::OnStatusbusy()
{
	// TODO:  在此添加命令处理程序代码
	theApp.r->j->setPresence(Presence::DND, 0);
	theApp.friends[0].friendStatus = Presence::DND;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_BUSY));
}
//切换状态->离开
void CTest1Dlg::OnStatusaway()
{
	// TODO:  在此添加命令处理程序代码
	theApp.r->j->setPresence(Presence::Away, 0);
	theApp.friends[0].friendStatus = Presence::Away;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_AWAY));
}
//切换状态->隐身
void CTest1Dlg::OnStatushide()
{
	// TODO:  在此添加命令处理程序代码
	theApp.r->j->setPresence(Presence::Unavailable, 0);
	theApp.friends[0].friendStatus = Presence::Unavailable;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_OFFLINE));
}
//切换状态->离线
void CTest1Dlg::OnStatusoffline()
{
	// TODO:  在此添加命令处理程序代码
	OnStatushide();
}
