
// Test1.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Message_Test.h"
#include "Friend.h"
#include "MyTools.h"
// CTest1App: 
// 有关此类的实现，请参阅 Test1.cpp
//




class CTest1App : public CWinApp
{
public:
	CTest1App();
	~CTest1App();
// 重写
public:
	virtual BOOL InitInstance();

//全局变量，用AfxGetApp()得到一个App对象指针就可引用 
public:
	char tUserId[12];
	char userId[50];
//	char userPwd[50];
	char regUser[50];
	char regPwd[50];


	Message_Test *r = new Message_Test();
	bool isConFlag = true;
	int m_RoomsNum;

	//friend列表
	CFriend *friends = new CFriend[200];
	int friendNum = 1;

	//查看的信息的成员序号
	int friendInfoNum = -1;

	void delFriend(int findex)
	{
		for (int j = findex; j < friendNum; j++)
		{
			friends[j] = friends[j + 1];
		}
		friendNum--;
	}


	//工具类
	CMyTools *myTools = new CMyTools();
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTest1App theApp;
