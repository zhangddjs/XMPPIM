// ChatRecordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "ChatRecordDlg.h"
#include "afxdialogex.h"


// CChatRecordDlg �Ի���
//richeditctrl style
#define RS_FROMID 0
#define RS_MYID 1
#define RS_FROMBODY 2
#define RS_MYBODY 3


IMPLEMENT_DYNAMIC(CChatRecordDlg, CDialogEx)

CChatRecordDlg::CChatRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatRecordDlg::IDD, pParent)
{
	logSrc = theApp.r->chatLogPath;
	logSrc += theApp.friends[0].friendId;
	logSrc += _T("\\");
}

CChatRecordDlg::~CChatRecordDlg()
{
}

void CChatRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_record);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}



BEGIN_MESSAGE_MAP(CChatRecordDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatRecordDlg ��Ϣ�������


BOOL CChatRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	logSrc += theApp.friends[f_index].friendId;
	logSrc += _T("\\chatLog.txt");
	
	CStdioFile fFile;
	CString readline;          //�����ı����м�������
	CString s1;           //�ı�����
	if (!fFile.Open(logSrc, CFile::typeText | CFile::modeRead))
	{
		MessageBox(_T("���������¼"));
		return TRUE;
	}
	char * pOldLocale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");
	while (fFile.ReadString(readline))
	{
		s1 += readline;
		readline.Empty();
	}
	// ������Ϻ�,�ͷ���Դ
	setlocale(LC_CTYPE, pOldLocale);
	free(pOldLocale);
	fFile.Close();
	/*CFile R;		�ڴ治��
	R.Open(logSrc, CFile::modeRead);
	long long size;
	R.Read(&size, sizeof(long long));
	TCHAR * buf = new TCHAR[size];
	wmemset(buf, 0, size);   // ע����WMEMSET��������MEMSET
	R.Read(buf, size);
	CString s1 = (CString)buf;
	delete[]buf;
	R.Close();*/

	CString cfromId, cbody, oneStr;
	while (s1.Find(_T("<immessage password:1231231231>")) != -1)
	{
		oneStr = theApp.myTools->cutStrFromTwoStrsInclude(s1, CString(_T("<immessage password:1231231231>")), CString(_T("</immessage password:1231231231>")));
		s1 = s1.Right(s1.GetLength() - s1.Find(oneStr) - oneStr.GetLength());
		showOneStr(oneStr, f_index);
	}

	m_record.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

void CChatRecordDlg::showOneStr(CString oneStr, int f_index)
{
	CString cfromId, cbody, cnickName, cf_note;
	cfromId = theApp.myTools->cutStrFromTwoStrs(oneStr, CString(_T("<immessagefromId>")), CString(_T("</immessagefromId>")));
	cbody = theApp.myTools->cutStrFromTwoStrs(oneStr, CString(_T("<immessagebody>")), CString(_T("</immessagebody>")));		//������BUG
	cbody += _T("\n");
	//��һ���жϣ��ж��Ƿ����Լ���ID
	m_record.SetSel(-1, -1);
	if (cfromId == theApp.friends[0].friendId)
	{
		cfromId = _T("me��");
		setRichEditStyle(RS_MYID);
		m_record.SetSel(-1, -1);
		m_record.ReplaceSel((LPCTSTR)cfromId);
		m_record.SetSel(-1, -1);
		setRichEditStyle(RS_MYBODY);
		m_record.ReplaceSel((LPCTSTR)cbody);
	}
	else
	{
		cnickName = theApp.friends[f_index].nickName;
		cf_note = theApp.friends[f_index].f_note;
		if (cf_note != "")
			cfromId = cf_note;
		else if (cnickName != "")
			cfromId = cnickName;
		cfromId += _T("��");
		setRichEditStyle(RS_FROMID);
		m_record.SetSel(-1, -1);
		m_record.ReplaceSel((LPCTSTR)cfromId);
		m_record.SetSel(-1, -1);
		setRichEditStyle(RS_FROMBODY);
		m_record.ReplaceSel((LPCTSTR)cbody);
	}
}
void CChatRecordDlg::setRichEditStyle(int type)
{
	switch (type)
	{
	case RS_FROMID:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_COLOR | CFM_FACE | CFM_BOLD |
			CFM_ITALIC | CFM_SIZE;//������ɫ�����壬�ֺ�
		//�ֺ�
		recvcf.yHeight = 14 * 14;
		//����
		recvcf.dwEffects = recvcf.dwEffects | CFE_BOLD;
		//��ɫ
		int r = 0, g = 0, b = 255;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//��������
		recvcf.crTextColor = RGB(r, g, b);
		m_record.SetWordCharFormat(recvcf);

		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_LEFT;			//����
		m_record.SetParaFormat(recvpf);
		break;
	}
	case RS_MYID:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_COLOR | CFM_FACE | CFM_BOLD |
			CFM_ITALIC | CFM_SIZE;//������ɫ�����壬�ֺ�
		//�ֺ�
		recvcf.yHeight = 14 * 14;
		//����
		recvcf.dwEffects = recvcf.dwEffects | CFE_BOLD;
		//��ɫ
		int r = 255, g = 0, b = 0;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//��������
		recvcf.crTextColor = RGB(r, g, b);
		m_record.SetWordCharFormat(recvcf);

		//�������
		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_LEFT;
		m_record.SetParaFormat(recvpf);
		break;
	}
	case RS_FROMBODY:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;//������ɫ�����壬�ֺ�
		//�ֺ�
		recvcf.yHeight = 14 * 14;
		//��ɫ
		int r = 0, g = 0, b = 0;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//��������
		recvcf.crTextColor = RGB(r, g, b);
		m_record.SetWordCharFormat(recvcf);

		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_LEFT;
		m_record.SetParaFormat(recvpf);
		break;
	}
	case RS_MYBODY:
	{
		ZeroMemory(&recvcf, sizeof(CHARFORMAT2));
		recvcf.cbSize = sizeof(CHARFORMAT2);
		recvcf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;//������ɫ�����壬�ֺ�
		//�ֺ�
		recvcf.yHeight = 14 * 14;
		//��ɫ
		int r = 0, g = 0, b = 0;
		wcscpy(recvcf.szFaceName, _T("Times New Roman"));//��������
		recvcf.crTextColor = RGB(r, g, b);
		m_record.SetWordCharFormat(recvcf);
		//�������
		ZeroMemory(&recvpf, sizeof(PARAFORMAT2));
		// Modify the paragraph format so that the text is centered. 
		recvpf.cbSize = sizeof(PARAFORMAT2);
		recvpf.dwMask = PFM_ALIGNMENT;
		recvpf.wAlignment = PFA_LEFT;
		m_record.SetParaFormat(recvpf);
		break;
	}
	default:
		break;
	}
}
