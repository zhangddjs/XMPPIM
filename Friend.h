#pragma once


// CFriend 这是friend类

class CFriend : public CWnd
{
	DECLARE_DYNAMIC(CFriend)

public:
	CFriend();
	CFriend(const CFriend& rhs) 
	{ /* copy construction from rhs*/
		headIconUrl = rhs.headIconUrl;	//头像
		friendId = rhs.friendId;		//friend的JID
		friendIdFull = rhs.friendIdFull;		//friend的JID.full
		friendGroup = rhs.friendGroup;	//所属的组
		nickName = rhs.nickName;		//昵称
		f_resource = rhs.f_resource;		//资源
		f_address = rhs.f_address;		//地址
		f_phoneNum = rhs.f_phoneNum;		//手机号
		f_email = rhs.f_email;		//邮箱
		f_name = rhs.f_name;			//真名
		descInfo = rhs.descInfo;		//简介
		f_signature = rhs.f_signature;
		f_receiveStr = rhs.f_receiveStr;	//收到的消息
		f_note = rhs.f_note;		//备注
		chatFlag = rhs.chatFlag;	//是否正在会话
		friendStatus = rhs.friendStatus;		//状态 :0-6*/
	}
	virtual ~CFriend();
	CFriend& operator=(const CFriend& rhs) 
	{
		/* copy construction from rhs*/
		headIconUrl = rhs.headIconUrl;	//头像
		friendId = rhs.friendId;		//friend的JID
		friendIdFull = rhs.friendIdFull;		//friend的JID.full
		friendGroup = rhs.friendGroup;	//所属的组
		nickName = rhs.nickName;		//昵称
		f_resource = rhs.f_resource;		//资源
		f_address = rhs.f_address;		//地址
		f_phoneNum = rhs.f_phoneNum;		//手机号
		f_email = rhs.f_email;		//邮箱
		f_name = rhs.f_name;			//真名
		descInfo = rhs.descInfo;		//简介
		f_signature = rhs.f_signature;
		f_receiveStr = rhs.f_receiveStr;	//收到的消息
		f_note = rhs.f_note;		//备注
		chatFlag = rhs.chatFlag;	//是否正在会话
		friendStatus = rhs.friendStatus;		//状态 :0-6*/
		return *this;
	}
protected:
	DECLARE_MESSAGE_MAP()

public:
	CString headIconUrl;	//头像
	CString friendId;		//friend的JID
	CString friendIdFull;	//friend的JID.full
	CString friendGroup;	//所属的组
	CString nickName;		//昵称
	CString f_resource;		//资源
	CString f_address;		//地址
	CString f_phoneNum;		//手机号
	CString f_email;		//邮箱
	CString f_name;			//真名
	CString descInfo;		//简介
	CString f_signature;
	CString f_receiveStr;	//收到的消息
	CString f_note;			//备注


	bool chatFlag = false;	//是否正在会话
	int friendStatus;		//状态 :0-6
	// friendDlg.cpp的ondelete()也要改

};


