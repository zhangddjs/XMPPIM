// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
//#include "ImageDataObject.h"
#include <afxodlgs.h>
#include <afxpriv.h>
#include "oleimpl2.h"
#include "Richole.h"

// CChatDlg 对话框

//richeditctrl style
#define RS_FROMID 0
#define RS_MYID 1
#define RS_FROMBODY 2
#define RS_MYBODY 3


IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

LPUNKNOWN AFXAPI _AfxQueryInterface(LPUNKNOWN lpUnknown, REFIID iid)
{
	ASSERT(lpUnknown != NULL);
	LPUNKNOWN lpW = NULL;
	if (lpUnknown->QueryInterface(iid, (LPLP)&lpW) != S_OK)
		return NULL;
	return lpW;
}
void CChatDlg::InsertPlotBitmap(CString srcStr)		//插入图片
{
	USES_CONVERSION;
	SCODE sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &m_lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(m_lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(m_lpLockBytes,
		STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &m_lpStorage);
	if (sc != S_OK)
	{
		VERIFY(m_lpLockBytes->Release() == 0);
		m_lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}

	CString szFileName(srcStr);

	// attempt to create the object
	sc = ::OleCreateFromFile(CLSID_NULL, T2COLE(szFileName),
		IID_IUnknown, OLERENDER_DRAW, NULL, NULL,
		m_lpStorage, (void **)&m_lpObject);
	if (sc != S_OK)
	{
		TCHAR * lpMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		CString msg(lpMsgBuf);
		// 		msg += _T("\n\n\nThe following file, created in\n"
		// 				  "Simulation->Plot, may be missing due\n"
		// 				  "to not doing a File->Save Workspace:\n\n" );
		msg += szFileName;
//		AfxMessageBox(msg, MB_OK);			//加载不出
		::LocalFree(lpMsgBuf);
		return;
	}

	// m_lpObject is currently an IUnknown, convert to IOleObject
	if (m_lpObject != NULL)
	{
		LPUNKNOWN lpUnk = m_lpObject;
		m_lpObject = QUERYINTERFACE(lpUnk, IOleObject);
		lpUnk->Release();
		if (m_lpObject == NULL)
			AfxThrowOleException(E_OUTOFMEMORY);
	}

	// cache the IViewObject interface
	m_lpViewObject = QUERYINTERFACE(m_lpObject, IViewObject2);
	if (m_lpViewObject == NULL)
		return;

	// setup for advises; we assume that OLE cleans them up properly
	LPADVISESINK lpAdviseSink =
		(LPADVISESINK)GetInterface(&IID_IAdviseSink);

	// set up view advise
	VERIFY(m_lpViewObject->SetAdvise(DVASPECT_CONTENT, 0, lpAdviseSink)
		== S_OK);

	// the server shows these in its user-interface
	//  (as document title and in File Exit menu)
	m_lpObject->SetHostNames(T2COLE(AfxGetAppName()),
		T2COLE(_T("Test")));

	// all items are "contained" -- this makes our reference to this object
	//  weak -- which is needed for links to embedding silent update.
	OleSetContainedObject(m_lpObject, TRUE);

	CHARRANGE cr;
	m_recvRichEdit.GetSel(cr);
	cr.cpMin = cr.cpMax - 1;
	m_recvRichEdit.SetSel(cr);

	REOBJECT reo;
	memset(&reo, 0, sizeof(reo));
	reo.cbStruct = sizeof(reo);
	CLSID classID;
	if (m_lpObject->GetUserClassID(&classID) != S_OK)
		classID = CLSID_NULL;
	reo.clsid = classID;
	reo.cp = REO_CP_SELECTION;
	reo.poleobj = m_lpObject;
	reo.pstg = m_lpStorage;
	LPOLECLIENTSITE lpClientSite;
	m_recvRichEdit.GetIRichEditOle()->GetClientSite(&lpClientSite);
	reo.polesite = lpClientSite;
	SIZEL sizel;
	sizel.cx = sizel.cy = 600; // let richedit determine initial size
	reo.sizel = sizel;
	reo.dvaspect = DVASPECT_CONTENT;
	reo.dwFlags = REO_RESIZABLE;
	reo.dwUser = 0;
	HRESULT hr = m_recvRichEdit.GetIRichEditOle()->InsertObject(&reo);
}



CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
{

	//字体
//	CString str_temp("宋体");
//	sprintf(recvcf.szFaceName, "%s",  _T("隶书"));
	//字号
//	str_temp = 12;
//	recvcf.yHeight = atoi(str_temp) * 20;
	//粗体
//	recvcf.dwMask = recvcf.dwMask | CFM_BOLD;
//	recvcf.dwEffects = recvcf.dwEffects | CFE_BOLD;
	//斜体
//	recvcf.dwEffects = recvcf.dwEffects | CFE_ITALIC;
//	recvcf.dwMask = recvcf.dwMask | CFM_ITALIC;
	//下划线
//	recvcf.dwEffects = recvcf.dwEffects | CFE_UNDERLINE;
//	recvcf.dwMask = recvcf.dwMask | CFM_UNDERLINE;
	//颜色
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT22, m_recvRichEdit);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	//消息映射  消息本身，消息响应函数
	ON_MESSAGE(WM_RECVDATA, OnRecvData)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChatDlg 消息处理程序
// 信号量对象句柄 互斥接收数据
HANDLE hSemaphore2;
LRESULT CChatDlg::OnRecvData(WPARAM wParam, LPARAM lParam)
{
	// 试图进入信号量关口
	WaitForSingleObject(hSemaphore2, INFINITE);
	CString recvStr;
	CString oneStr;
	
	if (isNewSession)	//第一次打开窗口，或者重新生成窗口时，加载全部已接受数据
	{
		f_index = (int)wParam;
		isNewSession = false;
		CString cfromId, cbody;
		recvStr = theApp.friends[f_index].f_receiveStr;
		theApp.friends[f_index].f_receiveStr = "";
		while (recvStr.Find(_T("<immessage password:1231231231>")) != -1)
		{
			oneStr = theApp.myTools->cutStrFromTwoStrsInclude(recvStr, CString(_T("<immessage password:1231231231>")), CString(_T("</immessage password:1231231231>")));
			recvStr = recvStr.Right(recvStr.GetLength() - recvStr.Find(oneStr) - oneStr.GetLength());
			showOneStr(oneStr, f_index);
		}
	//	SetDlgItemText(IDC_RICHEDIT22, theApp.friends[f_index].f_receiveStr);
	}
	else
	{
		theApp.friends[f_index].f_receiveStr = "";
		recvStr = (LPCTSTR)lParam;
		if (recvStr == "")
		{
			return 1;
		}
		oneStr = theApp.myTools->cutStrFromTwoStrsInclude(recvStr, CString(_T("<immessage password:1231231231>")), CString(_T("</immessage password:1231231231>")));
		showOneStr(oneStr, f_index);
	}
	m_recvRichEdit.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);


	// 释放信号量计数
	ReleaseSemaphore(hSemaphore2, 1, NULL);

	return 1;
}

HBRUSH CChatDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CChatDlg::setRichEditStyle(int type)
{
	switch (type)
	{
	case RS_FROMID:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_COLOR | CFM_FACE | CFM_BOLD |
			CFM_ITALIC | CFM_SIZE;//设置颜色，字体，字号
		//字号
		recvcf.yHeight = 16 * 16;
		//粗体
		recvcf.dwEffects = recvcf.dwEffects | CFE_BOLD;
		//颜色
		int r = 0, g = 0, b = 255;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//设置字体
		recvcf.crTextColor = RGB(r, g, b);
		m_recvRichEdit.SetWordCharFormat(recvcf);

		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_LEFT;			//居左
		m_recvRichEdit.SetParaFormat(recvpf);
		break;
	}
	case RS_MYID:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_COLOR | CFM_FACE | CFM_BOLD |
			CFM_ITALIC | CFM_SIZE;//设置颜色，字体，字号
		//字号
		recvcf.yHeight = 16 * 16;
		//粗体
		recvcf.dwEffects = recvcf.dwEffects | CFE_BOLD;
		//颜色
		int r = 255, g = 0, b = 0;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//设置字体
		recvcf.crTextColor = RGB(r, g, b);
		m_recvRichEdit.SetWordCharFormat(recvcf);

		//段落居右
		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_RIGHT;
		m_recvRichEdit.SetParaFormat(recvpf);
		break;
	}
	case RS_FROMBODY:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;//设置颜色，字体，字号
		//字号
		recvcf.yHeight = 15 * 15;
		//颜色
		int r = 0, g = 0, b = 0;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//设置字体
		recvcf.crTextColor = RGB(r, g, b);
		m_recvRichEdit.SetWordCharFormat(recvcf);

		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_LEFT;
		m_recvRichEdit.SetParaFormat(recvpf);
		break;
	}
	case RS_MYBODY:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;//设置颜色，字体，字号
		//字号
		recvcf.yHeight = 15 * 15;
		//颜色
		int r = 0, g = 0, b = 0;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//设置字体
		recvcf.crTextColor = RGB(r, g, b);
		m_recvRichEdit.SetWordCharFormat(recvcf);
		//段落居右
		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_RIGHT;
		m_recvRichEdit.SetParaFormat(recvpf);
		break;
	}
	default:
		break;
	}
}

void CChatDlg::showOneStr(CString oneStr, int f_index)
{
	CString cfromId, cbody, cnickName, cf_note;
	cfromId = theApp.myTools->cutStrFromTwoStrs(oneStr, CString(_T("<immessagefromId>")), CString(_T("</immessagefromId>")));
	cbody = theApp.myTools->cutStrFromTwoStrs(oneStr, CString(_T("<immessagebody>")), CString(_T("</immessagebody>")));		//铁定会BUG
	cbody += _T("\n");
	//加一个判断，判断是否是自己的ID
	m_recvRichEdit.SetSel(-1, -1);
	if (cfromId == theApp.friends[0].friendId)
	{
		cfromId = _T("me");
		setRichEditStyle(RS_MYID);
		m_recvRichEdit.SetSel(-1, -1);
		cfromId += _T(" ");
		m_recvRichEdit.ReplaceSel((LPCTSTR)cfromId);
		InsertPlotBitmap(theApp.friends[0].headIconUrl);			//会取代最后一个字符！
		m_recvRichEdit.ReplaceSel(_T("\n"));
		m_recvRichEdit.SetSel(-1, -1);
		setRichEditStyle(RS_MYBODY);
		m_recvRichEdit.ReplaceSel((LPCTSTR)cbody);
	}
	else
	{
		cnickName = theApp.friends[f_index].nickName;
		cf_note = theApp.friends[f_index].f_note;
		if (cf_note != "")
			cfromId = cf_note;
		else if (cnickName != "")
			cfromId = cnickName;
		setRichEditStyle(RS_FROMID);
		m_recvRichEdit.SetSel(-1, -1);
		m_recvRichEdit.ReplaceSel(_T(" "));
		InsertPlotBitmap(theApp.friends[f_index].headIconUrl);
		m_recvRichEdit.ReplaceSel((LPCTSTR)cfromId);
		m_recvRichEdit.ReplaceSel(_T("\n"));
		m_recvRichEdit.SetSel(-1, -1);
		setRichEditStyle(RS_FROMBODY);
		m_recvRichEdit.ReplaceSel((LPCTSTR)cbody);
	}
}

/*
void CChatDlg::InsertBitmap(CRichEditCtrl *edit, HBITMAP hBitmap, int position)
{
	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL;

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;
	fm.ptd = NULL;
	fm.dwAspect = DVASPECT_CONTENT;
	fm.lindex = -1;
	fm.tymed = TYMED_GDI;

	COleDataSource oleDataSource;
	oleDataSource.CacheData(CF_BITMAP, &stgm);
	LPDATAOBJECT dataObject = (LPDATAOBJECT)oleDataSource.GetInterface(&IID_IDataObject);

	if (OleQueryCreateFromData(dataObject) != OLE_S_STATIC)
		return;

	LPOLECLIENTSITE oleClientSite;
	if (S_OK != edit->GetIRichEditOle()->GetClientSite(&oleClientSite))
		return;

	//allocate memory
	LPLOCKBYTES lockBytes = NULL;
	if (S_OK == CreateILockBytesOnHGlobal(NULL, TRUE, &lockBytes) && lockBytes)
	{
		IStorage *storage = NULL;
		if (S_OK == StgCreateDocfileOnILockBytes(lockBytes, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &storage) && storage)
		{
			IOleObject *oleObject = NULL;
			if (S_OK == OleCreateStaticFromData(dataObject, IID_IOleObject, OLERENDER_FORMAT, &fm, oleClientSite, storage, (void **)&oleObject) && oleObject)
			{
				CLSID clsid;
				if (S_OK == oleObject->GetUserClassID(&clsid))
				{
					REOBJECT reobject = { sizeof(REOBJECT) };
					reobject.clsid = clsid;
					reobject.cp = position;
					reobject.dvaspect = DVASPECT_CONTENT;
					reobject.poleobj = oleObject;
					reobject.polesite = oleClientSite;
					reobject.pstg = storage;
					edit->GetIRichEditOle()->InsertObject(&reobject);
				}
				oleObject->Release();
			}
			storage->Release();
		}
		lockBytes->Release();
	}
	oleClientSite->Release();
}*/