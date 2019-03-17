#pragma once


// CFriend ����friend��

class CFriend : public CWnd
{
	DECLARE_DYNAMIC(CFriend)

public:
	CFriend();
	CFriend(const CFriend& rhs) 
	{ /* copy construction from rhs*/
		headIconUrl = rhs.headIconUrl;	//ͷ��
		friendId = rhs.friendId;		//friend��JID
		friendIdFull = rhs.friendIdFull;		//friend��JID.full
		friendGroup = rhs.friendGroup;	//��������
		nickName = rhs.nickName;		//�ǳ�
		f_resource = rhs.f_resource;		//��Դ
		f_address = rhs.f_address;		//��ַ
		f_phoneNum = rhs.f_phoneNum;		//�ֻ���
		f_email = rhs.f_email;		//����
		f_name = rhs.f_name;			//����
		descInfo = rhs.descInfo;		//���
		f_signature = rhs.f_signature;
		f_receiveStr = rhs.f_receiveStr;	//�յ�����Ϣ
		f_note = rhs.f_note;		//��ע
		chatFlag = rhs.chatFlag;	//�Ƿ����ڻỰ
		friendStatus = rhs.friendStatus;		//״̬ :0-6*/
	}
	virtual ~CFriend();
	CFriend& operator=(const CFriend& rhs) 
	{
		/* copy construction from rhs*/
		headIconUrl = rhs.headIconUrl;	//ͷ��
		friendId = rhs.friendId;		//friend��JID
		friendIdFull = rhs.friendIdFull;		//friend��JID.full
		friendGroup = rhs.friendGroup;	//��������
		nickName = rhs.nickName;		//�ǳ�
		f_resource = rhs.f_resource;		//��Դ
		f_address = rhs.f_address;		//��ַ
		f_phoneNum = rhs.f_phoneNum;		//�ֻ���
		f_email = rhs.f_email;		//����
		f_name = rhs.f_name;			//����
		descInfo = rhs.descInfo;		//���
		f_signature = rhs.f_signature;
		f_receiveStr = rhs.f_receiveStr;	//�յ�����Ϣ
		f_note = rhs.f_note;		//��ע
		chatFlag = rhs.chatFlag;	//�Ƿ����ڻỰ
		friendStatus = rhs.friendStatus;		//״̬ :0-6*/
		return *this;
	}
protected:
	DECLARE_MESSAGE_MAP()

public:
	CString headIconUrl;	//ͷ��
	CString friendId;		//friend��JID
	CString friendIdFull;	//friend��JID.full
	CString friendGroup;	//��������
	CString nickName;		//�ǳ�
	CString f_resource;		//��Դ
	CString f_address;		//��ַ
	CString f_phoneNum;		//�ֻ���
	CString f_email;		//����
	CString f_name;			//����
	CString descInfo;		//���
	CString f_signature;
	CString f_receiveStr;	//�յ�����Ϣ
	CString f_note;			//��ע


	bool chatFlag = false;	//�Ƿ����ڻỰ
	int friendStatus;		//״̬ :0-6
	// friendDlg.cpp��ondelete()ҲҪ��

};


