#pragma once


// CMyTools

class CMyTools : public CWnd
{
	DECLARE_DYNAMIC(CMyTools)

public:
	CMyTools();
	virtual ~CMyTools();



protected:
	DECLARE_MESSAGE_MAP()
public:
	/************************************************************************/
	/*��ȡ�����ַ���֮����ַ���                                            */
	/*@���ܣ����ַ���str�н�ȡ�����ַ���str1,str2����ַ���,���ؽ�ȡ���ַ���*/
	/*@����ֵ��CString ���ؽ�ȡ����ַ���                                   */
	/*@����1��CString��Ҫ��ȡ���ַ���                                       */
	/*@����2��CString����ʼ�ַ���                                           */
	/*@����3��CString�������ַ���                                           */
	/************************************************************************/
	CString cutStrFromTwoStrs(CString str, CString str1, CString str2);


	/************************************************************************/
	/*��ȡ�����ַ���֮����ַ����Ұ����������ַ���                          */
	/*@���ܣ����ַ���str�н�ȡ�����ַ���str1,str2����ַ���,���ؽ�ȡ���ַ���*/
	/*       ��ȡ����ַ�������Ȼ��str1,str2                                */
	/*@����ֵ��CString ���ؽ�ȡ����ַ���                                   */
	/*@����1��CString��Ҫ��ȡ���ַ���                                       */
	/*@����2��CString����ʼ�ַ���                                           */
	/*@����3��CString�������ַ���                                           */
	/************************************************************************/
	CString cutStrFromTwoStrsInclude(CString str, CString str1, CString str2);
};


