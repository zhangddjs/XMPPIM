
// Test1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Message_Test.h"
#include "Friend.h"
#include "MyTools.h"
// CTest1App: 
// �йش����ʵ�֣������ Test1.cpp
//




class CTest1App : public CWinApp
{
public:
	CTest1App();
	~CTest1App();
// ��д
public:
	virtual BOOL InitInstance();

//ȫ�ֱ�������AfxGetApp()�õ�һ��App����ָ��Ϳ����� 
public:
	char tUserId[12];
	char userId[50];
//	char userPwd[50];
	char regUser[50];
	char regPwd[50];


	Message_Test *r = new Message_Test();
	bool isConFlag = true;
	int m_RoomsNum;

	//friend�б�
	CFriend *friends = new CFriend[200];
	int friendNum = 1;

	//�鿴����Ϣ�ĳ�Ա���
	int friendInfoNum = -1;

	void delFriend(int findex)
	{
		for (int j = findex; j < friendNum; j++)
		{
			friends[j] = friends[j + 1];
		}
		friendNum--;
	}


	//������
	CMyTools *myTools = new CMyTools();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTest1App theApp;
