// MyTools.cpp : 实现文件
//

#include "stdafx.h"
#include "Test1.h"
#include "MyTools.h"


// CMyTools

IMPLEMENT_DYNAMIC(CMyTools, CWnd)

CMyTools::CMyTools()
{

}

CMyTools::~CMyTools()
{
}


BEGIN_MESSAGE_MAP(CMyTools, CWnd)
END_MESSAGE_MAP()



// CMyTools 消息处理程序




CString CMyTools::cutStrFromTwoStrs(CString str, CString str1, CString str2)
{
	str = str.Left(str.Find(str2));
	str = str.Right(str.GetLength() - str.Find(str1) - str1.GetLength());
	return str;
}

CString CMyTools::cutStrFromTwoStrsInclude(CString str, CString str1, CString str2)
{
	str = str.Left(str.Find(str2) + str2.GetLength());
	str = str.Right(str.GetLength() - str.Find(str1));
	return str;
}