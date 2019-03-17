#pragma once
#include "stdafx.h"
#include "MessageTest3Dlg.h"
#include "Friend.h"

using namespace gloox;

// Message_Test
struct RECVPARAM2
{
	HWND hwnd;
};
#define WM_RECVDATA WM_USER+1
#define WM_RECVFRIEND WM_USER+2
#define WM_RECVINFO WM_USER+3
#define WM_UPDATEINFO WM_USER+5
#define WM_RECVSUBREQ WM_USER+6
#define WM_RECVREMVED WM_USER+7
#define WM_RECVFILE WM_USER+8

class Message_Test : public CWnd, MessageSessionHandler, ConnectionListener, LogHandler,
	MessageEventHandler, MessageHandler, ChatStateHandler, RosterListener, VCardHandler,
	SIProfileFTHandler, BytestreamDataHandler, RegistrationHandler
{
	DECLARE_DYNAMIC(Message_Test)

public:
	Message_Test();
	//Message_Test(LPVOID lpParameter);
	virtual ~Message_Test();
	virtual void init(char *m_jid, char *m_pwd);
	virtual void initFileSrc();	//��ʼ�������ļ�·��
	virtual void start();
	virtual void onConnect();
	virtual void onDisconnect(ConnectionError e);
	virtual bool onTLSConnect(const CertInfo& info);
	virtual void handleMessage(const Message& msg, MessageSession * /*session*/);
	virtual void SetFromHwnd(LPVOID lpParameter);			//���þ��
	virtual void SendMessage(CString toId, CString str);
	virtual void handleMessageEvent(const JID& from, MessageEventType event);
	virtual void handleChatState(const JID& from, ChatStateType state);
	virtual void handleMessageSession(MessageSession *session);
	virtual void handleLog(LogLevel level, LogArea area, const std::string& message);
	virtual void msgSaveToFile(CString myId, CString fromId, CString message);

	//���ѹ���
	virtual void handleRoster(const Roster& roster);
	virtual void onResourceBindError(ResourceBindError error){}
	virtual void onSessionCreateError(SessionCreateError error){}
	virtual void handleItemSubscribed(const JID& jid);
	virtual void handleItemAdded(const JID& jid);		//�յ���������
	virtual void handleItemUnsubscribed(const JID& jid);
	virtual void handleItemRemoved(const JID& jid);	//�յ���ɾ������Ϣ
	virtual void handleItemUpdated(const JID& jid){}	//���Ѹ�����Ϣ
	virtual void handleRosterError(const IQ& /*iq*/){}
	virtual void handleRosterPresence(const RosterItem& item, const std::string& resource,
		Presence::PresenceType presence, const std::string& /*msg*/);
	virtual void handleSelfPresence(const RosterItem& item, const std::string& resource,
		Presence::PresenceType presence, const std::string& /*msg*/){}
	virtual bool handleSubscriptionRequest(const JID& jid, const std::string& /*msg*/);		//��Ӻ���
	virtual bool handleUnsubscriptionRequest(const JID& jid, const std::string& /*msg*/){return true;}
	virtual void handleNonrosterPresence(const Presence& presence){}
	virtual int WDAddFriend(const JID& jid, const std::string& /*msg*/);
	virtual void WDDelFriend(CString FullName);		//ɾ������
	virtual void setNoteName(const JID& jid, CString noteName);		//���ñ�ע

	//vcard
	virtual void setVcard(const JID& jid, int findex, int setType);	//setType��ʾ�������ͣ�����ͷ������1���ǳ�����2��������Ϣ����3����
	virtual void getVcard(const JID& jid, int findex);
	virtual void handleVCard(const JID& jid, const VCard *v);
	virtual void handleVCardResult(VCardContext context, const JID& jid,
		StanzaError se);

	//�����ļ�
	virtual void handleFTRequest(const JID& from, const JID& /*to*/, const std::string& sid,
		const std::string& name, long size, const std::string& hash,
		const std::string& date, const std::string& mimetype,
		const std::string& desc, int /*stypes*/);
	virtual void handleFTRequestError(const IQ& /*iq*/, const std::string& /*sid*/);
	virtual void handleFTBytestream(Bytestream* bs);
	virtual const std::string handleOOBRequestResult(const JID& /*from*/, const JID& /*to*/, const std::string& /*sid*/){return std::string();}
	virtual void handleBytestreamData(Bytestream* /*s5b*/, const std::string& data);
	virtual void handleBytestreamError(Bytestream* /*s5b*/, const IQ& /*stanza*/){}
	virtual void handleBytestreamOpen(Bytestream* /*s5b*/){}
	virtual void handleBytestreamClose(Bytestream* /*s5b*/)
	{
		::PostMessage(hwnd, WM_RECVFILE, NULL, NULL);		//ˢ�¿ؼ�
		MessageBox(_T("����/�����ļ����"));
	}

	//ע���û�
	virtual void handleRegistrationFields(const JID& from, int fields, std::string instructions);
	virtual void handleAlreadyRegistered(const JID& from);
	virtual void handleRegistrationResult(const JID& from, RegistrationResult regResult);
	virtual void handleDataForm(const JID& from, const DataForm &form);
	virtual void handleOOB(const JID& from, const OOB& oob);



	//���ú���
	JID CStringToJID(CString str);
	char * CStringToChar(CString str);


public:
	Client *j;
	bool conFlag = true;
	Roster::const_iterator it;
	Roster::const_iterator itEnd;
	int sessionFriendId = -1;

	SIProfileFT* f;
	SOCKS5BytestreamManager* s5b;
	std::list<Bytestream*> m_bs;
	SOCKS5BytestreamServer* m_server;
	Bytestream* m_bs2;
	bool sendFileFlag = false;

	//�����ļ���ַ
	CString rootPath;
	CString headIconPath;
	CString chatLogPath;
	CString recvFile;			//���ܵ��ļ�·��+����
	CString fileRecvPath;
	CString myfileRecvPath;

	//ע���û�
	Registration *m_reg;
private:
	MessageSession *m_session;
	MessageEventFilter *m_messageEventFilter;
	ChatStateFilter *m_chatStateFilter;

	CString friendId;
	HWND hwnd;
	bool isSessionChanged = false;

private:
	VCardManager *m_vManager;
	int friendIndex = -1;
	int m_count = 0;
protected:
	DECLARE_MESSAGE_MAP()
};


