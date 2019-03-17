// MyTree.cpp : 实现文件
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
	//链接数据库im
	if (!mysql_real_connect(&mysql, "localhost", "root", "123456", "im", 3306, NULL, 0))
	{
		AfxMessageBox(_T("访问数据库失败!"));
	}
	else
	{
		//	AfxMessageBox(_T("链接数据库成功!"));
		mysql_query(&mysql, "SET NAMES 'GB2312'");//解决从数据库中读取数据后汉字乱码显示的问题  
	}
	MYSQL_RES *res;//声明数据查询结果  
	MYSQL_ROW row;//取每行结果的对象
	mysql_real_query(&mysql, query, strlen(query));//查询函数，语句用字符串表示  
	res = mysql_store_result(&mysql);//得到查询结果  
	

	//初始化 树视图
	//1.准备图片集合
	HICON icon = AfxGetApp()->LoadIconW(IDI_QQ);
	m_imageList.Create(16, 16, ILC_COLOR32, 1, 1);
	m_imageList.Add(icon);
	SetImageList(&m_imageList, TVSIL_NORMAL);

	int index = 0;
	int listrow = 0;	//行
	while (row = mysql_fetch_row(res))//逐行去除查询结果并操作  
	{
		//数据操作，这里可以直接以row[i]的形式取出数据项，但是均为char*行  
		//2.添加节点
		CString myreaddata(row[0]);
		InsertItem(myreaddata, 0, 0, NULL);
		listrow++;

	}
	mysql_free_result(res);
	mysql_close(&mysql);*/
}


BEGIN_MESSAGE_MAP(CMyTree, CTreeCtrl)
END_MESSAGE_MAP()



// CMyTree 消息处理程序


