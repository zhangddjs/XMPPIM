// MyTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "MyTree.h"
//#include "mysql.h"
#include "winsock.h"

// CMyTree

IMPLEMENT_DYNAMIC(CMyTree, CTreeCtrl)

CMyTree::CMyTree()
{
	//SelectItem(NULL);

}

CMyTree::~CMyTree()
{
}


void CMyTree::showTree(char *query)
{
/*
	MYSQL mysql;
	mysql_init(&mysql);
	//�������ݿ�im
	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "im", 3306, NULL, 0))
	{
		AfxMessageBox(_T("�������ݿ�ʧ��!"));
	}
	else
	{
		//	AfxMessageBox(_T("�������ݿ�ɹ�!"));
		mysql_query(&mysql, "SET NAMES 'GB2312'");//��������ݿ��ж�ȡ���ݺ���������ʾ������  
	}
	MYSQL_RES *res;//�������ݲ�ѯ���  
	MYSQL_ROW row;//ȡÿ�н���Ķ���
	mysql_real_query(&mysql, query, strlen(query));//��ѯ������������ַ�����ʾ  
	res = mysql_store_result(&mysql);//�õ���ѯ���  
	

	//��ʼ�� ����ͼ
	//1.׼��ͼƬ����
	HICON icon = AfxGetApp()->LoadIconW(IDI_QQ);
	m_imageList.Create(16, 16, ILC_COLOR32, 1, 1);
	m_imageList.Add(icon);
	SetImageList(&m_imageList, TVSIL_NORMAL);

	int index = 0;
	int listrow = 0;	//��
	while (row = mysql_fetch_row(res))//����ȥ����ѯ���������  
	{
		//���ݲ������������ֱ����row[i]����ʽȡ����������Ǿ�Ϊchar*��  
		//2.��ӽڵ�
		CString myreaddata(row[0]);
		InsertItem(myreaddata, 0, 0, NULL);
		listrow++;

	}
	mysql_free_result(res);
	mysql_close(&mysql);*/
}


BEGIN_MESSAGE_MAP(CMyTree, CTreeCtrl)
END_MESSAGE_MAP()



// CMyTree ��Ϣ�������


