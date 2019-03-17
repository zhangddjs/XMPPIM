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
	/*截取两个字符串之间的字符串                                            */
	/*@功能：从字符串str中截取两个字符串str1,str2间的字符串,返回截取后字符串*/
	/*@返回值：CString 返回截取后的字符串                                   */
	/*@参数1：CString，要截取的字符串                                       */
	/*@参数2：CString，开始字符串                                           */
	/*@参数3：CString，结束字符串                                           */
	/************************************************************************/
	CString cutStrFromTwoStrs(CString str, CString str1, CString str2);


	/************************************************************************/
	/*截取两个字符串之间的字符串且包含这两个字符串                          */
	/*@功能：从字符串str中截取两个字符串str1,str2间的字符串,返回截取后字符串*/
	/*       截取后的字符串包任然含str1,str2                                */
	/*@返回值：CString 返回截取后的字符串                                   */
	/*@参数1：CString，要截取的字符串                                       */
	/*@参数2：CString，开始字符串                                           */
	/*@参数3：CString，结束字符串                                           */
	/************************************************************************/
	CString cutStrFromTwoStrsInclude(CString str, CString str1, CString str2);
};


