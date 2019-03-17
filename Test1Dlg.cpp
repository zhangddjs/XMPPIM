
// Test1Dlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTest1Dlg �Ի���



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
	//��Ϣӳ��  ��Ϣ������Ϣ��Ӧ����
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



// CTest1Dlg ��Ϣ�������
//ȫ���߳�
// DWORD WINAPI RecvProc(
// 	_In_ LPVOID lpParameter
// );
BOOL CTest1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
//	//�����߳̽������ݣ����̴߳����׽��֡����նԻ���ľ��
	RECVPARAM *pRecvParam = new RECVPARAM();
	pRecvParam->hwnd = m_hWnd;
// 	HANDLE hThread = CreateThread(NULL, 0, RecvProc, pRecvParam, 0, NULL);
// 	CloseHandle(hThread);
	theApp.r->SetFromHwnd(pRecvParam);
	delete pRecvParam;
	CLoginDlg loginDlg;
	loginDlg.DoModal();

	CString welcome(_T("��ӭ������"));
	CString uid(theApp.userId);
	welcome += uid;
//	AfxMessageBox(welcome);



	//���öԻ����С,�ݶ��ߴ�350,400
//	CRect temprect(0, 0, 350, 400);
//	CWnd::SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	//��CStatic��̬�ı����У���ʾλͼ������λͼ��С
// 	CRect bmpRect;
// 	m_pic.GetWindowRect(&bmpRect);
// 	CString userHeadIcon;
// 	userHeadIcon = theApp.friends[0].headIconUrl == "" ? theApp.friends[0].headIconUrl : (LPCTSTR)theApp.userHeadIcon;
// 	HBITMAP hBmp = (HBITMAP)::LoadImage(0, userHeadIcon, IMAGE_BITMAP, bmpRect.Width(), bmpRect.Height(), LR_LOADFROMFILE);
// 	m_pic.ModifyStyle(NULL, SS_BITMAP);
// 	m_pic.SetBitmap(hBmp);
	//��ʼ��״̬����
	m_statusPic.ModifyStyle(0, SS_ICON);
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_ONLINE));
	menu.LoadMenu(IDR_TREEMENU1);//���ز˵���Դ
	statusMenu = menu.GetSubMenu(2);//��ȡ�˵�����
	//����ͼ��
	CBitmap *pMenuBitmap; //��Ҫ����Ϊ��Ա����
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_ONLINE);
	statusMenu->SetMenuItemBitmaps(0,/*�ڼ������ܣ���0��ʼ*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*����*/pMenuBitmap, pMenuBitmap);
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_BUSY);
	statusMenu->SetMenuItemBitmaps(1,/*�ڼ������ܣ���0��ʼ*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*����*/pMenuBitmap, pMenuBitmap);
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_AWAY);
	statusMenu->SetMenuItemBitmaps(2,/*�ڼ������ܣ���0��ʼ*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*����*/pMenuBitmap, pMenuBitmap);
	pMenuBitmap = new CBitmap;
	pMenuBitmap->LoadBitmap(IDB_OFFLINE);//��Ҫ���ص�ͼ����Դ
	statusMenu->SetMenuItemBitmaps(3,/*�ڼ������ܣ���0��ʼ*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*����*/pMenuBitmap, pMenuBitmap);
	statusMenu->SetMenuItemBitmaps(4,/*�ڼ������ܣ���0��ʼ*/MF_BYPOSITION | MF_STRING | MF_ENABLED,/*����*/pMenuBitmap, pMenuBitmap);

	
	m_pic.ModifyStyle(NULL, SS_BITMAP);
	if (theApp.friends[0].headIconUrl != "")
	{
		CRect bmpRect;
		m_pic.GetWindowRect(&bmpRect);
		CString friendHeadIcon(theApp.friends[0].headIconUrl);		//λͼ·��
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
	userSignature = theApp.friends[0].f_signature == "" ? _T("��༭ǩ��") : theApp.friends[0].f_signature;
	CString userInfo(theApp.friends[0].descInfo);
	m_nickName.SetWindowTextW(userName);
	m_signature.SetWindowTextW(userSignature);
	m_info.SetWindowTextW(userInfo);
	

	//TabCtrl

	m_tab.AddPage(TEXT("��Ϣ"), &recentDlg, IDD_RECENT);
	m_tab.AddPage(TEXT("����"), &friendsDlg, IDD_FRIENDS);
//	m_tab.AddPage(TEXT("Ⱥ"), &groupsDlg, IDD_GROUPS);
	m_tab.AddPage(TEXT("�ļ�"), &filesDlg, IDD_FILES);
	m_tab.Show();

	//�������ݿ�test
/*
	MYSQL m_sqlCon;
	mysql_init(&m_sqlCon);
	if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "im", 3306, NULL, 0))
	{
		AfxMessageBox(_T("�������ݿ�ʧ��!"));
	}
	else
	{
		AfxMessageBox(_T("�������ݿ�ɹ�!"));
		mysql_query(&m_sqlCon, "SET NAMES 'GB2312'");//��������ݿ��ж�ȡ���ݺ���������ʾ������  
	}
*/
	::SetClassLongPtr(m_nickName.GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));		//����ƶ����ǳ��ϱ����ָ
	::SetClassLongPtr(m_pic.GetSafeHwnd(), GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_HAND));		//����ƶ����ǳ��ϱ����ָ

	
	m_progress.SetRange(0, 100);//���ý������ķ�Χ
	m_progress.SetStep(1);	//���ý�������ÿһ��������
	m_progress.SetPos(0);	//���ý������ĵ�ǰλ��


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTest1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTest1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CTest1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == GetDlgItem(IDC_NICKNAME))
	{
		pDC->SetTextColor(RGB(0, 0, 200));
	}
	else if (pWnd == GetDlgItem(IDC_SIGNATURE))
	{
		pDC->SetTextColor(RGB(144, 144, 144));
	}


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


//void CTest1Dlg::OnSize(UINT nType, int cx, int cy)
//{
//	CDialogEx::OnSize(nType, cx, cy);
//
//	// TODO:  �ڴ˴������Ϣ����������
//
//
//
//}


void CTest1Dlg::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���
//	CDialogEx::OnOK();
}


void CTest1Dlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	MYSQL mysql;
// 	mysql_init(&mysql);
// 	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "im", 3306, NULL, 0))
// 	{
// 		AfxMessageBox(_T("�������ݿ�ʧ��!"));
// 	}
// 	else
// 	{
// 		//	AfxMessageBox(_T("�������ݿ�ɹ�!"));
// 		mysql_query(&mysql, "SET NAMES 'GB2312'");//��������ݿ��ж�ȡ���ݺ���������ʾ������  
// 	}
// 
// 	char updateQuery[100] = "update im_user set userStatus=0 where tUserId=";
// 	strcat_s(updateQuery, theApp.tUserId);
// 	//	CString str(query);
// 	//	AfxMessageBox(str);
// 	if (mysql_query(&mysql, updateQuery))
// 	{
// 		AfxMessageBox(_T("�޸����ݱ���Ŀʧ��"));
// 	}
// 	mysql_close(&mysql);




	CDialogEx::OnClose();
}



//�߳�
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
	sprintf_s(tempBuf, "%s ˵ : %s", inet_ntoa(addrFrom.sin_addr), recvBuf);
	//�����ݴ����Ի���
	::PostMessage(hwnd, WM_RECVDATA, 0, (LPARAM)tempBuf);


	}
	AfxMessageBox(_T("err"));
	* /

// 	HWND mainHwnd = ((RECVPARAM*)lpParameter)->hwnd;		//��¼Test1Dlg���ڵ�handle
// 	CMessageTest3Dlg m_msgDlg = new CMessageTest3Dlg();
// 	HWND msgHwnd = m_msgDlg.m_hWnd;
//	Message_Test *r = new Message_Test(lpParameter);
	theApp.r->SetFromHwnd(lpParameter);
//	theApp.r->start();	AfxMessageBox(_T("haha"));
//	delete(r);// 	m_msgDlg.Create(IDD_MESSAGETEST3_DIALOG);
// 	m_msgDlg.ShowWindow(SW_SHOWNORMAL); 
	return 0;
}*/

//�յ����ԶԷ�����Ϣ
HANDLE hTest1RecvDataSemaphore;
LRESULT CTest1Dlg::OnRecvData(WPARAM wParam, LPARAM lParam)		//wParamΪf_index�� lParamΪ�ַ���
{
	WaitForSingleObject(hTest1RecvDataSemaphore, INFINITE);
	CString sTemp((LPCTSTR)lParam);
	static CString xmlStr;
	xmlStr.Format(_T("%s"), sTemp);
//	CString friendId((LPCTSTR)wParam);

	//��messageDlg���ڷ�����Ϣ
	//����Ϣ�浽friend��Ӧ������
	static int i;
	i = (int)wParam;
	theApp.r->msgSaveToFile(theApp.friends[0].friendId, theApp.friends[i].friendId, xmlStr);		//���浽�����¼
	if (theApp.friends[i].chatFlag)
	{
		if (m_msgDlg->GetSafeHwnd())   //ΪTRUE ˵���Ի����Ѿ�������
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
		//��recent���ڷ�����Ϣ��
		::PostMessage(recentDlg.GetSafeHwnd(), WM_RECVDATA, (WPARAM)i, (LPARAM)(LPCTSTR)xmlStr);
	}

 	ReleaseSemaphore(hTest1RecvDataSemaphore, 1, NULL);
	return 1;
}
//�յ���������״̬����Ϣ
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
//�鿴������Ϣ
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
//�������촰��
LRESULT CTest1Dlg::OnClossChatWnd(WPARAM wParam, LPARAM lParam)
{
	delete m_msgDlg;

	m_msgDlg = new CMessageTest3Dlg();
	return 1;
}
//���¸�����Ϣ
LRESULT CTest1Dlg::OnUpdateInfo(WPARAM wParam, LPARAM lParam)
{
	CString userName;
	userName = theApp.friends[0].nickName == "" ? theApp.friends[0].friendId : theApp.friends[0].nickName;
	CString userSignature;
	userSignature = theApp.friends[0].f_signature == "" ? _T("��༭ǩ��") : theApp.friends[0].f_signature;
	CString userInfo(theApp.friends[0].descInfo);
	m_nickName.SetWindowTextW(userName);
	m_signature.SetWindowTextW(userSignature);
	m_info.SetWindowTextW(userInfo);
	return 1;
}
//�յ���������	wParamΪjidFull��lParamΪflag
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
//�յ���ɾ������Ϣ��ͬʱɾ���ú���	wParamΪjidFull
LRESULT CTest1Dlg::OnRecvRemved(WPARAM wParam, LPARAM lParam)
{
	static WPARAM w;
	w = wParam;
	::PostMessage(friendsDlg.GetSafeHwnd(), WM_RECVREMVED, w, NULL);
	return 1;
}
//�ļ�������ϣ�ˢ�¿ؼ�
LRESULT CTest1Dlg::OnRecvFile(WPARAM wParam, LPARAM lParam)
{
	filesDlg.m_mfcShellList.Refresh();
	return 1;
}

//�鿴������Ϣ
void CTest1Dlg::OnStnClickedNickname()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char *pJid = new char[200];
	sprintf(pJid, "%S", theApp.friends[0].friendId);
	JID jid(pJid);
	theApp.r->getVcard(jid, 0);
	delete pJid;
	static int i = 0;
	::PostMessage(this->GetSafeHwnd(), WM_RECVINFO, NULL, (LPARAM)i);
}

//���ͷ��
void CTest1Dlg::OnStnClickedPic()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�ļ�ɸѡ��
	CString str1;
	CString filter = _T("Bitmap    Files(*.bmp)|*.bmp|Wmf   Files(*.wmf)|*.wmf||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (LPCTSTR)filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		str1 = dlg.GetPathName();
		
		//�жϴ�С
		CFile file;
		file.Open(str1, CFile::modeRead | CFile::typeBinary);     
		DWORD dwDataLen = file.GetLength();
		file.Close();
		if (dwDataLen > 10240)
		{
			AfxMessageBox(_T("ͷ��ӦС��10K"));
			return;
		}

		

		theApp.friends[0].headIconUrl = str1;
		char* pFid = new char[200];
		sprintf(pFid, "%S", theApp.friends[0].friendId);
		JID jid(pFid);
		theApp.r->setVcard(jid, 0, 1);		//�޸�ͷ��
		delete pFid;
		//���ý�����
		//��ȡ��������λ�÷�Χ�����ֵ����Сֵ

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
		//��ÿһ�������������ӽ������ĵ�ǰλ��
		//��ȡ�������ĵ�ǰλ��
// 		int nPos = (m_ctrlProgress.GetPos() - nLower) * 100 / (nUpper - nLower);
// 		CString s;
// 		s.Format("%d", nPos);
// 		m_strText = s + "%";

		//��ʾͷ��
		CRect bmpRect;
		m_pic.GetWindowRect(&bmpRect);
		CString friendHeadIcon(theApp.friends[0].headIconUrl);		//λͼ·��
		CImage image;
		image.Load(friendHeadIcon);
		HBITMAP hBmp = image.Detach();
		m_pic.SetBitmap(hBmp);
		m_pic.ShowWindow(true);
		image.Destroy();
		
	}

}

//����״̬��ť
void CTest1Dlg::OnStnClickedStatuspic()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CRect itemRect;
	m_statusPic.GetWindowRect(&itemRect);
	
	//�˵�������λ��
	CRect ptMenu;
	ptMenu.left = itemRect.left;
	ptMenu.top = itemRect.bottom;
	//��̬�����Ӳ˵�
	statusMenu->TrackPopupMenu(TPM_LEFTALIGN, ptMenu.left, ptMenu.top, this);    //��ʾ�˵�

}
//�л�״̬->����
void CTest1Dlg::OnStatusonline()
{
	// TODO:  �ڴ���������������
	theApp.r->j->setPresence(Presence::Available, 0);
	theApp.friends[0].friendStatus = Presence::Available;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_ONLINE));
}
//�л�״̬->æµ
void CTest1Dlg::OnStatusbusy()
{
	// TODO:  �ڴ���������������
	theApp.r->j->setPresence(Presence::DND, 0);
	theApp.friends[0].friendStatus = Presence::DND;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_BUSY));
}
//�л�״̬->�뿪
void CTest1Dlg::OnStatusaway()
{
	// TODO:  �ڴ���������������
	theApp.r->j->setPresence(Presence::Away, 0);
	theApp.friends[0].friendStatus = Presence::Away;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_AWAY));
}
//�л�״̬->����
void CTest1Dlg::OnStatushide()
{
	// TODO:  �ڴ���������������
	theApp.r->j->setPresence(Presence::Unavailable, 0);
	theApp.friends[0].friendStatus = Presence::Unavailable;
	m_statusPic.SetIcon(AfxGetApp()->LoadIcon(IDI_STATUS_OFFLINE));
}
//�л�״̬->����
void CTest1Dlg::OnStatusoffline()
{
	// TODO:  �ڴ���������������
	OnStatushide();
}
