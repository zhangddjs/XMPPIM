
// MessageTest3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "MessageTest3Dlg.h"
#include "afxdialogex.h"
#include "Message_Test.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <ios>
#include "EmotionDlg.h"
#include "ChatRecordDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMessageTest3Dlg 对话框

CMessageTest3Dlg::CMessageTest3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageTest3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMessageTest3Dlg::~CMessageTest3Dlg()
{
	for (int i = 1; i < theApp.friendNum; i++)
		theApp.friends[i].chatFlag = false;
	delete[] chatDlg;
	DestroyWindow();
}

void CMessageTest3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_tab);
	DDX_Control(pDX, IDC_SENDFILEPIC, m_sendFilePic);
	DDX_Control(pDX, IDC_EMOTION, m_emotionPic);
	DDX_Control(pDX, IDC_MSGSEND, m_send);
	DDX_Control(pDX, IDC_HISTORY, m_historyPic);
}

BEGIN_MESSAGE_MAP(CMessageTest3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//消息映射  消息本身，消息响应函数
	ON_MESSAGE(WM_RECVDATA, OnRecvData)
	ON_BN_CLICKED(1003, &CMessageTest3Dlg::OnBnClicked1003)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_SENDFILEPIC, &CMessageTest3Dlg::OnStnClickedSendfilepic)
	ON_STN_CLICKED(IDC_EMOTION, &CMessageTest3Dlg::OnStnClickedEmotion)
	ON_STN_CLICKED(IDC_HISTORY, &CMessageTest3Dlg::OnStnClickedHistory)
END_MESSAGE_MAP()


// CMessageTest3Dlg 消息处理程序
//全局线程
DWORD WINAPI RecvProc(
	_In_ LPVOID lpParameter
);
BOOL CMessageTest3Dlg::OnInitDialog()
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
	

	m_sendFilePic.ModifyStyle(NULL, SS_BITMAP);
	m_emotionPic.ModifyStyle(NULL, SS_BITMAP);
	m_historyPic.ModifyStyle(NULL, SS_BITMAP);
	CRect bmpRect;
	m_sendFilePic.GetWindowRect(&bmpRect);
	CImage image;
	CString fileIcon(_T("IM图标\\file.bmp"));		//位图路径
	image.Load(fileIcon);
	HBITMAP hBmp = image.Detach();
	m_sendFilePic.SetBitmap(hBmp);
	m_sendFilePic.ShowWindow(true);
	image.Destroy();
	CString emotionIcon(_T("IM图标\\emotion.bmp"));		//位图路径
	image.Load(emotionIcon);
	hBmp = image.Detach();
	m_emotionPic.SetBitmap(hBmp);
	m_emotionPic.ShowWindow(true);
	image.Destroy();
	CString historyIcon(_T("IM图标\\history.bmp"));		//位图路径
	image.Load(historyIcon);
	hBmp = image.Detach();
	m_historyPic.SetBitmap(hBmp);
	m_historyPic.ShowWindow(true);
	image.Destroy();
	



//  	m_sendFilePic.ModifyStyle(0, WS_CHILD | BS_BITMAP, NULL);
//  	HBITMAP h = (HBITMAP)LoadImage(AfxGetInstanceHandle(), _T("D:\\毕设\\IMGO\\IM图标\\file-send.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
// 	m_sendFilePic.SetBitmap(h);
	// TODO:  在此添加额外的初始化代码
//  	RECVPARAM *pRecvParam = new RECVPARAM();
//  	pRecvParam->hwnd = m_hWnd;
// 	HANDLE hThread = CreateThread(NULL, 0, RecvProc, pRecvParam, 0, NULL);
// 	CloseHandle(hThread);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMessageTest3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMessageTest3Dlg::OnPaint()
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
HCURSOR CMessageTest3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




//线程
DWORD WINAPI CMessageTest3Dlg::RecvProc(LPVOID lpParameter)
{


	return 0;
}

// 信号量对象句柄 互斥接收数据
HANDLE hSemaphore;

bool createFlag = false;//是否创建tabctrl
LRESULT CMessageTest3Dlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	// 试图进入信号量关口
	WaitForSingleObject(hSemaphore, INFINITE);
	int i;
	int f_index = (int)wParam;
	theApp.friends[f_index].chatFlag = true;
	CString tabName;
	CString toId = theApp.friends[f_index].friendId;
	CString noteName = theApp.friends[f_index].f_note;
	CString nickName = theApp.friends[f_index].nickName;
	if (noteName != "")
		tabName = noteName;
	else if (nickName != "")
		tabName = nickName;
	else
		tabName = toId;
	for (i = 0; i < sessionNum; i++)		//查找是否是旧的session
	{
		if (toId == sessionList[i])
		{
			CString recvStr((LPCTSTR)lParam);
			int oldSel = m_tab.GetCurSel();
			if (recvStr != "")
			{

				//变化字体
				if (oldSel != i)
				{
					m_tab.m_msgTabs[i] = true;

					//重绘
					CRect rect;
					m_tab.GetItemRect(i, &rect);
					rect.left += 10;
					rect.right += 10;
					rect.top += 10;
					rect.bottom += 10;
					InvalidateRect(rect);//刷新 
				}
				//会闪屏
//				m_tab.SelChange(oldSel, oldSel);
//				CRect rect;
// 				m_tab.GetWindowRect(&rect);
// 				//获取控件区域  
// 				ScreenToClient(&rect);
// 				//屏幕区域对客户区域的转换    
// 				InvalidateRect(rect);//刷新 

				break;
			}
			m_tab.SetCurSel(i);
			m_tab.SelChange(oldSel, i);
			break;
		}
	}
	if (i >= sessionNum && createFlag)	//是新的且之前已有其他session则添加
	{
		sessionList[sessionNum] = toId;
		m_tab.AddPage(tabName, &chatDlg[sessionNum++], IDD_CHAT);
		m_tab.AddTab();
		int oldSel = m_tab.GetCurSel();
		m_tab.m_msgTabs[i] = false;
		m_tab.SetCurSel(i);
		m_tab.SelChange(oldSel, i);
	}
	if (!createFlag)	//最初的session则创建
	{
		sessionList[0] = toId;
		m_tab.AddPage(tabName, &chatDlg[sessionNum++], IDD_CHAT);
		m_tab.Show();
		m_tab.SetCurSel(0);
		m_tab.SelChange(0, 0);
		createFlag = true;
	}

	//向chatDlg[i]发送消息，数据让其显示
	static WPARAM ctoId;
	ctoId = wParam;
	static LPARAM crecvText;
	crecvText = lParam;


	::PostMessage(chatDlg[i].GetSafeHwnd(), WM_RECVDATA, ctoId, crecvText);

	// 释放信号量计数
	ReleaseSemaphore(hSemaphore, 1, NULL);

	return 1;
}

HANDLE h_SendSemaphore;
//发送文本按钮
void CMessageTest3Dlg::OnBnClicked1003()
{
	// TODO:  在此添加控件通知处理程序代码
	WaitForSingleObject(h_SendSemaphore, INFINITE);
	CString str(_T("<immessage password:1231231231><immessagefromId>"));
	str += theApp.friends[0].friendId;
	str += _T("</immessagefromId><immessagebody>");
	CString sendText;
	static CString toId;
	GetDlgItemText(IDC_MSGSEND, sendText);
	if (sendText == "")
	{
		return;
	}

	//获取标签
// 	TCITEM tcItem;
// 	TCHAR buffer[256] = { 0 };
// 	tcItem.pszText = buffer;
// 	tcItem.cchTextMax = 256;
// 	tcItem.mask = TCIF_TEXT;
	int curSel = m_tab.GetCurSel();
	CFriend *myFriend = &theApp.friends[chatDlg[curSel].f_index];
//	m_tab.GetItem(curSel, &tcItem);
//	toId = tcItem.pszText;
	toId = myFriend->friendId;
	theApp.r->SendMessage(toId, sendText);
	str += sendText;
	str += _T("</immessagebody></immessage password:1231231231>\n");

	static CString csendText;
	csendText = str;

// 	if (myFriend->f_receiveStr != "")
// 		myFriend->f_receiveStr += "\r\n";
	myFriend->f_receiveStr += csendText;
//	delete myFriend;
	theApp.r->msgSaveToFile(theApp.friends[0].friendId, theApp.friends[chatDlg[curSel].f_index].friendId, csendText);		//保存到聊天记录
	::PostMessage(chatDlg[curSel].GetSafeHwnd(), WM_RECVDATA, (WPARAM)chatDlg[curSel].f_index, (LPARAM)(LPCTSTR)csendText);


	SetDlgItemText(IDC_MSGSEND, _T(""));

	ReleaseSemaphore(h_SendSemaphore, 1, NULL);
}


void CMessageTest3Dlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类
	OnBnClicked1003();
//	CDialogEx::OnOK();
}


void CMessageTest3Dlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
// 	for (int i = 0; i < sessionNum; i++)
// 	{
// 		sessionList[i] = "";
// 	}
// 	sessionNum = 0;
// 	m_tab.DeleteAllPages();
	
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSSCHATWND, NULL, NULL);
	
//	CDialogEx::OnClose();
}

//发送文件
UINT _SendFile(LPVOID pParam);
void CMessageTest3Dlg::OnStnClickedSendfilepic()
{
	// TODO:  在此添加控件通知处理程序代码
	//判断用户是否在线
	int curSel = m_tab.GetCurSel();
	if (theApp.friends[chatDlg[curSel].f_index].friendStatus == 5)
	{
		MessageBox(_T("用户已离线！无法发送文件"));
		return;
	}
	CString toJidStr = theApp.friends[chatDlg[curSel].f_index].friendId;
	toJidStr += "/zdd";
	JID toJid(theApp.r->CStringToJID(toJidStr));

	CFileDialog cfd(true, _T(".txt"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Text file (*.txt)|*.txt|All   Files   (*.*)|*.*||"), this);
	//文件选择框打开的文件两个格式：jpg或所有格式   若想只打开bmp格式，括号内改为 _T("Image file (*.jpg)|*.jpg|)注意最后的|可以多一个但是不能少否则会乱码
	if (cfd.DoModal() != IDOK)return;
	CString m_name = cfd.GetFileName();
	char file_name[200];
	strcpy_s(file_name, theApp.r->CStringToChar(m_name));
	CString m_path = cfd.GetPathName();
 	char file_path[1000];//cstring转char[] 
	strcpy_s(file_path, theApp.r->CStringToChar(m_path));
	struct stat f_stat;
	if (stat(file_path, &f_stat))
		return;
	int m_size = f_stat.st_size;
	theApp.r->sendFileFlag = true;
	theApp.r->f->requestFT(toJid, file_name, m_size);

	AfxBeginThread(_SendFile, (LPVOID)file_path, THREAD_PRIORITY_NORMAL);

	

}
UINT _SendFile(LPVOID pParam)
{
	std::string m_file((char*)pParam);
	//加载进度条等待对方接受
	//读取文件
	struct stat f_stat;
	if (stat(m_file.c_str(), &f_stat))
	{
		AfxMessageBox(_T("m_file error"));
		return 0;
	}
	int m_size = f_stat.st_size;
	std::ifstream ifile(m_file.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!ifile)
	{
		AfxMessageBox(_T("ifile error"));
		return 0;
	}
	ConnectionError se = ConnNoError;
	char input[200024];
	ConnectionError ce = ConnNoError;
	Message_Test * wdgloox = theApp.r;
	while (true)
	{
	//	Sleep(1000);
		if (wdgloox->m_server)
		{
			se = wdgloox->m_server->recv(1);
			if (se != ConnNoError)
			{
				delete wdgloox->m_server;
				wdgloox->m_server = 0;
				AfxMessageBox(_T("send error"));
				break;
			}
		}
		if (wdgloox->m_bs2 && !ifile.eof())
		{
			if (wdgloox->m_bs2->isOpen())
			{
				ifile.read(input, 200024);
				std::string t(input, ifile.gcount());
				if (!wdgloox->m_bs2->send(t))
					break;
			}
			wdgloox->m_bs2->recv(1);
		}
		else if (wdgloox->m_bs2)
		{
			wdgloox->m_bs2->close();
			break;
		}
	}
	return 0;

}

//表情
void CMessageTest3Dlg::OnStnClickedEmotion()
{
	// TODO:  在此添加控件通知处理程序代码
	CEmotionDlg emoDlg;
	CRect itemRect;
	m_emotionPic.GetWindowRect(&itemRect);
//	ClientToScreen(itemRect);
	emoDlg.re = itemRect;
	if (emoDlg.DoModal() == IDOK)
	{
		m_send.SetSel(-1, -1);
		m_send.ReplaceSel((LPCTSTR)emoDlg.emo);
	}
}


void CMessageTest3Dlg::OnStnClickedHistory()
{
	// TODO:  在此添加控件通知处理程序代码
	int curSel = m_tab.GetCurSel();
	CChatRecordDlg chatrecDlg;
	chatrecDlg.f_index = chatDlg[curSel].f_index;
	chatrecDlg.DoModal();
}
