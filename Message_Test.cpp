// Message_Test.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Test1.h"
#include "Message_Test.h"
#include "MessageTest3Dlg.h"

using namespace gloox;
// Message_Test

IMPLEMENT_DYNAMIC(Message_Test, CWnd)

static int Utf8ToGB2312(const char* utf8, char* gb2312);
static int GB2312ToUtf8(const char* gb2312, char* utf8);


/************************************************************************/
/*   Ԥ����                                                             */
/************************************************************************/
Message_Test::Message_Test()
{
	m_session = NULL, m_messageEventFilter = NULL, m_chatStateFilter = NULL;
	m_bs2 = 0;
	sendFileFlag = false;
	initFileSrc();
}
Message_Test::~Message_Test()
{
	conFlag = false;
//	theApp.isConFlag = false;
}
void Message_Test::init(char *m_jid, char *m_pwd)
{
	char pJid[200];
	strcpy_s(pJid, m_jid);
	strcat(pJid, "/zdd");
	JID jid(pJid);
	CString jidStr(jid.bare().c_str());
	j = new Client(jid, m_pwd);
	
}
void Message_Test::initFileSrc()
{
	rootPath = _T("C:\\Users\\IMGO");
	if (!PathIsDirectory(rootPath))
	{
		::CreateDirectory(rootPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
	}
	rootPath += _T("\\");
	chatLogPath = rootPath;
	chatLogPath += _T("chatLog");
	if (!PathIsDirectory(chatLogPath))
	{
		::CreateDirectory(chatLogPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
	}
	chatLogPath += _T("\\");
	headIconPath = rootPath;
	headIconPath += _T("headIcons");
	if (!PathIsDirectory(headIconPath))
	{
		::CreateDirectory(headIconPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
	}
	headIconPath += _T("\\");
	fileRecvPath = rootPath;
	fileRecvPath += _T("IMFileRecv");
	if (!PathIsDirectory(fileRecvPath))
	{
		::CreateDirectory(fileRecvPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
	}
	fileRecvPath += _T("\\");
}
UINT _GetMessage(LPVOID pParam);
void Message_Test::start()
{

	j->registerConnectionListener(this);
	j->registerMessageSessionHandler(this, 0);
	j->rosterManager()->registerRosterListener(this);
	j->disco()->setVersion("Message_Test", GLOOX_VERSION, "Linux");
	j->disco()->setIdentity("client", "bot");
	j->disco()->addFeature(XMLNS_CHAT_STATES);
	StringList ca;
	ca.push_back("/path/to/cacert.crt");
	j->setCACerts(ca);

	j->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);
	m_vManager = new VCardManager(j);


	m_server = new SOCKS5BytestreamServer(j->logInstance(), 5100, "127.0.0.1");
	ConnectionError le = ConnNoError;
	if ((le = m_server->listen()) != ConnNoError) {
		MessageBox(_T("socket error"));
		delete m_server;
		m_server = new SOCKS5BytestreamServer(j->logInstance(), 7778);
		if ((le = m_server->listen()) != ConnNoError)
			MessageBox(_T("socket error"));
	}
	//�����߳������Ӻͽ�����Ϣ
	f = new SIProfileFT(j, this);
	f->registerSOCKS5BytestreamServer(m_server);
	f->addStreamHost(j->jid(), "localhost", 6666);
	f->addStreamHost(JID("reflector.amessage.eu"), "reflector.amessage.eu", 6565);
	f->addStreamHost(JID("proxy.jabber.org"), "208.245.212.98", 7777);

	if (j->connect(false))
	{
		AfxBeginThread(_GetMessage, this, THREAD_PRIORITY_NORMAL);
	}
}
UINT _GetMessage(LPVOID pParam)
{
	ConnectionError ce = ConnNoError;
	ConnectionError se = ConnNoError;
	Message_Test * wdgloox = (Message_Test *)pParam;
	while (ce == ConnNoError)
	{
		ce = wdgloox->j->recv();
		std::list<Bytestream*>::iterator it = wdgloox->m_bs.begin();
		for (; it != wdgloox->m_bs.end(); ++it)
			(*it)->recv(100);
	}
//	wdgloox->j->connect();
	wdgloox->conFlag = false;
	return 0;
}
void Message_Test::onConnect()
{
//	conFlag = true;
}
void Message_Test::onDisconnect(ConnectionError e)
{
	conFlag = false;
}
bool Message_Test::onTLSConnect(const CertInfo& info)
{
//	Sleep(10000);
	return true;
}

/************************************************************************/
/*   ��Ϣ����                                                           */
/************************************************************************/
HANDLE hHandleMessageSemaphore;
void Message_Test::handleMessage(const Message& msg, MessageSession * /*session*/)
{
	WaitForSingleObject(hHandleMessageSemaphore, INFINITE);
	static int i;
	if (isSessionChanged)
	{
		isSessionChanged = false;
		for (i = 1; i < theApp.friendNum; i++)
		{
			if (theApp.friends[i].friendId == msg.from().bare().c_str())
			{
				sessionFriendId = i;
				break;
			}
		}
	}
	//����δ��֤
	if (i >= theApp.friendNum)
	{
		return;
	}
	


	m_messageEventFilter->raiseMessageEvent(MessageEventDisplayed);
#if defined( WIN32 ) || defined( _WIN32 )
	Sleep(200);
#else
	sleep(1);
#endif
	m_messageEventFilter->raiseMessageEvent(MessageEventComposing);
	m_chatStateFilter->setChatState(ChatStateComposing);
#if defined( WIN32 ) || defined( _WIN32 )
	Sleep(200);
#else
	sleep(2);
#endif
	//ʱ��
	
	if (msg.body() != "")
	{
		std::string recvTime;
		if(msg.when()!=0)
			recvTime = msg.when()->stamp();			//const string
		else
		{
		}
		//����Ϣ���ҵ�xml��ʽд��
		std::string re = "<immessage password:1231231231><immessagefromId>" + msg.from().bare()
			+ "</immessagefromId><immessagebody>" + msg.body()
			+ "</immessagebody></immessage password:1231231231>\n";

		//�����ݴ����Ի���
		//ת��
		static CString fromId;
		fromId = msg.from().bare().c_str();
		static CString xmlStr;	//����static��������Ϣ������
		char gb2312[64000];
		Utf8ToGB2312(re.c_str(), gb2312);
		xmlStr = gb2312;
		//��Test1Dlg������Ϣ  ���䵯�����촰��
		//�����ԭ����session���������ʾdata�� ���򴴽���tab
/*
		if (theApp.friends[sessionFriendId].f_receiveStr != "" && str != "")
			theApp.friends[sessionFriendId].f_receiveStr += "\r\n";*/
// 		Parser pp(this);    //�������this��ָ�Ľ�����Ϣ���࣬����ʵ����TagHandler�ӿ��е�//handleTag������
//		std::string msgXml = msg.tag()->xml();
// 		pp.feed(re);//������������ʱ���ͻ����handleTag�����ˡ�
		theApp.friends[sessionFriendId].f_receiveStr += xmlStr;
		::PostMessage(hwnd, WM_RECVDATA, (WPARAM)sessionFriendId, (LPARAM)(LPCTSTR)xmlStr);
	}
	ReleaseSemaphore(hHandleMessageSemaphore, 1, NULL);

}
void Message_Test::SendMessage(CString toId, CString str)		//��������
{
// 	if (str_name.Find("@") != -1)
// 		str_target = str_name + "/gloox";
// 	else
// 		str_target = str_name + "@" + str_server + "/gloox";

	//Unicodeת���ֽڣ�ʹ�����ĵ�CStringתΪchar*
	int n = str.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL);
	char * pStr = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, n, pStr, len, NULL, NULL);
	pStr[len] = '\0';   //���ֽ��ַ���'/0'����
	//CString ת char*
 	char* pToId = new char[200];
	sprintf(pToId, "%S", toId);

	char strTemp[64000];
	GB2312ToUtf8(pStr, strTemp);		//���ֽ�pStrתUTF-8

	//��������
	JID jidt(pToId);
	MessageSession *mes = new MessageSession(j, jidt);		//dispose����ɾ��
	SYSTEMTIME st;
	CString strDate, strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%2d-%2d"), st.wYear, st.wMonth, st.wDay);
	strTime.Format(_T("%2d:%2d:%2d"), st.wHour, st.wMinute, st.wSecond);

	DelayedDelivery *sendTime = new DelayedDelivery();
	sendTime->setStamp(CStringToChar(strTime));
	StanzaExtensionList sel = StanzaExtensionList();
	sel.push_back(sendTime);


	mes->send(strTemp, "No Subject", sel);
//	mes->send(strTemp);
	j->disposeMessageSession(mes);
	delete []pToId;
	delete []pStr;
}
void Message_Test::handleMessageEvent(const JID& from, MessageEventType event)
{
}
void Message_Test::handleChatState(const JID& from, ChatStateType state)
{
}
void Message_Test::handleMessageSession(MessageSession *session)
{
	printf("got new session\n");
	isSessionChanged = true;
//	conFlag = true;
//	isSessionChanged = true;
	// this example can handle only one session. so we get rid of the old session
	j->disposeMessageSession(m_session);
	m_session = session;
	m_session->registerMessageHandler(this);
	m_messageEventFilter = new MessageEventFilter(m_session);
	m_messageEventFilter->registerMessageEventHandler(this);
	m_chatStateFilter = new ChatStateFilter(m_session);
	m_chatStateFilter->registerChatStateHandler(this);


}
void Message_Test::handleLog(LogLevel level, LogArea area, const std::string& message)
{
}
/************************************************************************
* ���յ�����Ϣ����Ϊ�ļ�
* ������JID myId, JID fromId, CString message
*************************************************************************/
void Message_Test::msgSaveToFile(CString myId, CString fromId, CString message)
{
		CString strPath = _T("C:\\Users\\IMGO");
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}
		strPath += _T("\\chatLog");
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}
		strPath += _T("\\");
		strPath += myId;
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}
		strPath += _T("\\");
		strPath += fromId;
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
		}
		strPath += _T("\\chatLog.txt");
		char * pOldLocale = _strdup(setlocale(LC_CTYPE, NULL));
		setlocale(LC_CTYPE, "chs");
		CStdioFile file(strPath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
		file.SeekToEnd();
	//	char strtmp[64000];
	//	strcpy_s(strtmp, CStringToChar(message));
		file.WriteString(message);
//		file.Write(strtmp, strlen(strtmp));
		// ������Ϻ�,�ͷ���Դ
		setlocale(LC_CTYPE, pOldLocale);
		free(pOldLocale);
		file.Flush();
		file.Close();
}


/************************************************************************/
/*   ���ѹ���                                                           */
/************************************************************************/
bool isRosterOk = false;
//��ȡ�����б����ܽ����Դ��ͬ�����ظ�������,��������Դ�������ɾ����
void Message_Test::handleRoster(const Roster& roster)
{
//	printf("roster arriving\nitems:\n");

	it = roster.begin();
	itEnd = roster.end();
	//theApp.friendNum = 1;  ??
	for (; it != itEnd; ++it)
	{

		theApp.friends[theApp.friendNum].friendId = (*it).second->jidJID().bare().c_str();		//ID
		theApp.friends[theApp.friendNum].friendIdFull = (*it).second->jidJID().full().c_str();		//ID,���ɾ����������
//		theApp.friends[theApp.friendNum].nickName = (*it).second->name().c_str();		//������
		StringList g = (*it).second->groups();
		StringList::const_iterator it_g = g.begin();
		if (it_g != g.end())
		{
			theApp.friends[theApp.friendNum].friendGroup = (*it_g).c_str();		//��
		}
		else
		{
			theApp.friends[theApp.friendNum].friendGroup = "Pending Members";
		}
		theApp.friends[theApp.friendNum].friendStatus = 5;		//Ĭ�����ߣ� ���������״̬�����յ���Ϣ

		char noteName[100];
		Utf8ToGB2312((*it).second->name().c_str(), noteName);
		theApp.friends[theApp.friendNum].f_note = noteName;
		theApp.friendNum++;
	}
	isRosterOk = true;

}
HANDLE hHandleFriendSemaphore;
//bool hFriendInfoSemaphore = false;
//����״̬�л�
void Message_Test::handleRosterPresence(const RosterItem& item, const std::string& resource,
	Presence::PresenceType presence, const std::string& /*msg*/)
{
	WaitForSingleObject(hHandleFriendSemaphore, INFINITE);
	while (!isRosterOk)
	{
		Sleep(500);
	}
	static CString friendId;
	static int i;
	static int friendStatus;
	friendStatus = presence;
	friendId = item.jidJID().full().c_str();
	for (i = 1; i < theApp.friendNum; i++)
	{
		if (theApp.friends[i].friendId == friendId)
		{
			theApp.friends[i].friendStatus = presence;
			break;
		}
	}
	theApp.friends[i].friendStatus = presence;		//�Ա㱻��Ӻ��ѳɹ�ʱ��ȡ���Է�״̬
	Sleep(500);
	::PostMessage(hwnd, WM_RECVFRIEND, (WPARAM)i, (LPARAM)friendStatus);
	ReleaseSemaphore(hHandleFriendSemaphore, 1, NULL);
}
//�Է���Ӻ�������
bool Message_Test::handleSubscriptionRequest(const JID& jid, const std::string& /*msg*/)
{
	static CString jidStr;
	jidStr = jid.bare().c_str();
	static CString jidFull;
	jidFull = jid.full().c_str();
	for (int i = 1; i < theApp.friendNum; i++)
	{
		if (jidStr == theApp.friends[i].friendId)
		{
			static int f_index;
			f_index = i;
			::PostMessage(hwnd, WM_RECVSUBREQ, (WPARAM)(LPCTSTR)jidFull, (LPARAM)false);
			return true;
		}
	}


	CString msgBoxStr(jidFull);
	msgBoxStr += "���������Ϊ���ѣ��Ƿ�ͬ��";
	if (::MessageBox(NULL, msgBoxStr, _T("��Ӻ���"), MB_YESNO) == IDYES)
	{
		static int f_index;
		StringList groups;
		groups.push_back("Buddies");
		j->rosterManager()->subscribe(jid, (char*)(LPCTSTR)"", groups);

		::PostMessage(hwnd, WM_RECVSUBREQ, (WPARAM)(LPCTSTR)jidFull, (LPARAM)(LPCTSTR)true);
		return true;
	}
	return false;
}
//��Ӻ���
int Message_Test::WDAddFriend(const JID& jid, const std::string& /*msg*/)
{
	StringList groups;
	groups.push_back("Buddies");
	j->rosterManager()->subscribe(jid, (char*)(LPCTSTR)"", groups);
	theApp.friends[theApp.friendNum].friendId = jid.bare().c_str();
	theApp.friends[theApp.friendNum].friendIdFull = jid.full().c_str();
	theApp.friends[theApp.friendNum].friendGroup = "Buddies";
	theApp.r->getVcard(jid, theApp.friendNum);
	theApp.friends[theApp.friendNum++].friendStatus = 5;
	return theApp.friendNum - 1;
}
//ɾ������
void Message_Test::WDDelFriend(CString FullName)
{
	char* pFid = new char[200];
	sprintf(pFid, "%S", FullName);
	JID jid(pFid);
	delete[] pFid;
	j->rosterManager()->remove(jid);
}
//���ñ�ע
void Message_Test::setNoteName(const JID& jid, CString noteName)
{
	char strtmp[100];
 	strcpy_s(strtmp, CStringToChar(noteName));
 	j->rosterManager()->getRosterItem(jid)->setName(strtmp);
 	strcpy_s(strtmp, j->rosterManager()->getRosterItem(jid)->name().c_str());
	j->rosterManager()->synchronize();
}
//������ӳɹ�
void Message_Test::handleItemAdded(const JID& jid)
{
	isSessionChanged = true;
}
//�յ�������
void Message_Test::handleItemSubscribed(const JID& jid)
{
	isSessionChanged = true;
}
//�յ���ȡ������
void Message_Test::handleItemUnsubscribed(const JID& jid)
{
	isSessionChanged = true;
	//	j->rosterManager()->unsubscribe(jid);
}
//�յ���ɾ��
void Message_Test::handleItemRemoved(const JID& jid)
{
	isSessionChanged = true;
	//��ĳ���ڷ�����Ϣ������Ӻ����б�ɾ��
	j->rosterManager()->remove(jid);
	static CString jidFull;
	jidFull = jid.full().c_str();
	::PostMessage(hwnd, WM_RECVREMVED, (WPARAM)(LPCTSTR)jidFull, NULL);

}

//vcard
bool hFriendInfoMutexSemaphore = true;		//�ź���
bool hFriendInfoSemaphore = false;
void Message_Test::getVcard(const JID& jid, int findex)
{
	friendIndex = findex;
	m_count = 0;
	m_vManager->fetchVCard(jid, this);
	while (!hFriendInfoSemaphore)
	{
		Sleep(100);
	}
	hFriendInfoSemaphore = false;
	hFriendInfoMutexSemaphore = true;
}
void Message_Test::setVcard(const JID& jid, int findex, int setType)
{
	m_count = setType;
	m_vManager->fetchVCard(jid, this);
	while (!hFriendInfoSemaphore)
	{
		Sleep(100);
	}
	hFriendInfoSemaphore = false;
	hFriendInfoMutexSemaphore = true;
}
void Message_Test::handleVCard(const JID& jid, const VCard *v)
{
	while (!hFriendInfoMutexSemaphore)
	{
		Sleep(100);
	}
	hFriendInfoMutexSemaphore = false;
	if (!v)
	{
		hFriendInfoSemaphore = true;
		return;
	}
	int i = friendIndex;
	CString friendId(jid.bare().c_str());			//jid

	switch (m_count)
	{
	case 0:
	{
		VCard::TelephoneList::const_iterator itPhone = v->telephone().begin();		//�绰
		VCard::EmailList::const_iterator itEmail = v->emailAddresses().begin();		//����
		VCard::AddressList::const_iterator itAdress = v->addresses().begin();		//��ַ
		char strTemp[200];
		if (0 == i)
		{
			theApp.friends[i].friendId = friendId;
		}
		//��ȡ���ѵ���Ϣ
		if (friendId == theApp.friends[i].friendId)
		{
			if (itPhone != v->telephone().end())
			{
				theApp.friends[i].f_phoneNum = (*itPhone).number.c_str();		//�绰
			}
			if (itEmail != v->emailAddresses().end())
			{
				theApp.friends[i].f_email = (*itEmail).userid.c_str();		//����
			}
			if (itAdress != v->addresses().end())
			{
				Utf8ToGB2312((*itAdress).street.c_str(), strTemp);
				theApp.friends[i].f_address = strTemp;	//�ֵ���ַ
			}
			Utf8ToGB2312(v->formattedname().c_str(), strTemp);
			theApp.friends[i].f_name = strTemp;
			Utf8ToGB2312(v->nickname().c_str(), strTemp);
			theApp.friends[i].nickName = strTemp;
			Utf8ToGB2312(v->desc().c_str(), strTemp);
			theApp.friends[i].descInfo = strTemp;
			Utf8ToGB2312(v->title().c_str(), strTemp);
			theApp.friends[i].f_signature = strTemp;

			//ͷ��
			if (v->photo().binval.length() != 0)
			{
				CString pathTemp = friendId;
				CString strPath = _T("C:\\\\Users\\\\IMGO");
				if (!PathIsDirectory(strPath))
				{
					::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
				}
				strPath += _T("\\\\headIcons");
				if (!PathIsDirectory(strPath))
				{
					::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
				}
				strPath += _T("\\\\");
				strPath += pathTemp;
				if (!PathIsDirectory(strPath))
				{
					::CreateDirectory(strPath, NULL);//����Ŀ¼,���еĻ���Ӱ��  
				}
				strPath += _T("\\\\headIcon.bmp");

// 				long s;
// 				BYTE *buf = new BYTE[v->photo().binval.length() + 1];
// 				memcpy(buf, v->photo().binval.data(), v->photo().binval.length());
// 				BYTE* b = ConvertBMPToRGBBuffer(buf, 50, 50);
// 				BYTE* c = ConvertRGBToBMPBuffer(b, 50, 50, &s);
// 				SaveBMP(c, 50, 50, v->photo().binval.length() + 1, strPath);
				CFile file(strPath, CFile::modeCreate | CFile::modeWrite);
				file.Write(v->photo().binval.c_str(), v->photo().binval.length());
				file.Close();
				theApp.friends[i].headIconUrl = strPath;		//ͷ��
			}
		}
		break;
	}
	case 1:				//����ͷ��
	{
		VCard* vcard = new VCard(*v);		//Ҳ����delete
		CFile file;
		file.Open(theApp.friends[0].headIconUrl, CFile::modeRead | CFile::typeBinary);      //��ͼ���ļ�
		DWORD dwDataLen = file.GetLength();        //��ȡ�ļ��ĳ���
		BYTE *buf = new BYTE[dwDataLen+1];           //�������飬��������ͼ�������
		buf[dwDataLen] = '\0';
		file.Read(buf, dwDataLen);            //ͼ���ļ������ݱ�����������
		file.Close();                            //�ر��ļ�

		//ת��string
		std::string binval(reinterpret_cast<char const*>(buf), dwDataLen + 1);
		vcard->setPhoto("IMG", binval);
		m_vManager->storeVCard(vcard, this);
		if (buf) delete[] buf;
		break;
	}
	case 2:		//�ĸ�����Ϣ
	{
		VCard *vcard = new VCard(*v);
		char strtmp[1000];
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_name));
		vcard->setFormattedname(strtmp);
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].nickName));
		vcard->setNickname(strtmp);
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_phoneNum));
		vcard->addTelephone(strtmp, NULL);								//�޸Ĺ�addTelephone����
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_address));
		vcard->addAddress(strtmp, "app. 2", strtmp, "Springfield", "region", "123", "CHINA", VCard::AddrTypeHome);		//�޸Ĺ�addAddress����
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_email));
		vcard->addEmail(strtmp, NULL);									//�޸Ĺ�addEmail����
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_signature));
		vcard->setTitle(strtmp);		//����ǩ��
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].descInfo));
		vcard->setDesc(strtmp);		//��������
		m_vManager->storeVCard(vcard, this);
		::PostMessage(hwnd, WM_UPDATEINFO, NULL, NULL);
		break;
	}
	case 3:
		break;
	default:
		break;
	}
	hFriendInfoSemaphore = true;
}
void Message_Test::handleVCardResult(VCardContext context, const JID& jid,
	StanzaError se = StanzaErrorUndefined)
{
}

/************************************************************************/
/*   �ļ�����                                                           */
/************************************************************************/
void Message_Test::handleFTRequest(const JID& from, const JID& /*to*/, const std::string& sid,
	const std::string& name, long size, const std::string& hash,
	const std::string& date, const std::string& mimetype,
	const std::string& desc, int /*stypes*/)
{
/*
	printf("received ft request from %s: %s (%ld bytes, sid: %s). hash: %s, date: %s, mime-type: %s\n"
		"desc: %s\n",
		from.full().c_str(), name.c_str(), size, sid.c_str(), hash.c_str(), date.c_str(),
		mimetype.c_str(), desc.c_str());
*/
	char gb2312[200];
	Utf8ToGB2312(name.c_str(), gb2312);
	CString tipStr(from.bare().c_str());
	tipStr += _T("���������ļ�");
	tipStr += gb2312;
	tipStr.Format(_T("%s,��СΪ%ld"), tipStr, size);
	if (::MessageBox(NULL, tipStr, _T("�ļ�����"), MB_YESNO) == IDYES)
	{
		recvFile = myfileRecvPath;
		recvFile += gb2312;
		f->acceptFT(from, sid, SIProfileFT::FTTypeIBB);
	}
	f->declineFT(from, sid,	SIManager::RequestRejected,	"just testing");
}
void Message_Test::handleFTBytestream(Bytestream* bs)
{
	printf("received bytestream of type: %s", bs->type() == Bytestream::S5B ? "s5b" : "ibb");
	if (sendFileFlag)
	{
		sendFileFlag = false;
		m_bs2 = bs;
		m_bs2->registerBytestreamDataHandler(this);
		if (m_bs2->connect())
		{
			if (bs->type() == Bytestream::S5B)
				printf("ok! s5b connected to streamhost\n");
			else
				printf("ok! ibb sent request to remote entity\n");
		}
		return;
	}
	m_bs.push_back(bs);
	bs->registerBytestreamDataHandler(this);
	if (bs->connect())
	{
		if (bs->type() == Bytestream::S5B)
			printf("ok! s5b connected to streamhost\n");
		else
			printf("ok! ibb sent request to remote entity\n");
	}
}
//һ��ֻ�ܽ���һ���ļ�
void Message_Test::handleBytestreamData(Bytestream* /*s5b*/, const std::string& data)
{
//	printf("received %d bytes of data:\n%s\n", data.length(), data.c_str());
	CFile file(recvFile, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	file.SeekToEnd();
	char strtmp[64000];
	strcpy_s(strtmp, data.c_str());
	file.Write(strtmp, strlen(strtmp));
	file.Flush();
	file.Close();
}
void Message_Test::handleFTRequestError(const IQ& /*iq*/, const std::string& /*sid*/)
{
	sendFileFlag = false;
	MessageBox(_T("����/�����ļ�ʧ��"));
}

/************************************************************************/
/*   ע���û�                                                           */
/************************************************************************/
void Message_Test::handleRegistrationFields(const JID& from, int fields, std::string instructions)
{
// 	RegistrationFields vals;
// 	vals.username = m_cs[0];
// 	vals.password = m_cs[1];
// 	MessageBox(NULL, "handleRegistrationFields", "CWDGloox", MB_OK);
// 	m_reg->createAccount(fields, vals);
}
void Message_Test::handleAlreadyRegistered(const JID& from)
{}
void Message_Test::handleRegistrationResult(const JID& from, RegistrationResult regResult)
{}
void Message_Test::handleDataForm(const JID& from, const DataForm &form)
{}
void Message_Test::handleOOB(const JID& from, const OOB& oob)
{}


//������
char * Message_Test::CStringToChar(CString str)
{
	//Unicodeת���ֽڣ�ʹ�����ĵ�CStringתΪchar*
	int n = str.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL);
	char * pStr = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, n, pStr, len, NULL, NULL);
	pStr[len] = '\0';   //���ֽ��ַ���'/0'����
	//CString ת char*
	char strTemp[64000];
	GB2312ToUtf8(pStr, strTemp);		//���ֽ�pStrתUTF-8
	if (pStr) delete[]pStr;
	return strTemp;
}
static int Utf8ToGB2312(const char* utf8, char* gb2312)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, gb2312, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return len;
}
static int GB2312ToUtf8(const char* gb2312, char* utf8)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return len;
}


//���þ��
void Message_Test::SetFromHwnd(LPVOID lpParameter)
{
	hwnd = ((RECVPARAM2*)lpParameter)->hwnd;
}

//���ú���
JID Message_Test::CStringToJID(CString str)
{
	char* pFid = new char[200];
	sprintf(pFid, "%S", str);
	JID jid(pFid);
	delete []pFid;
	return jid;
}


BEGIN_MESSAGE_MAP(Message_Test, CWnd)
END_MESSAGE_MAP()



// Message_Test ��Ϣ�������


