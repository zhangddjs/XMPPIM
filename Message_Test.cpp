// Message_Test.cpp : 实现文件
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
/*   预处理                                                             */
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
		::CreateDirectory(rootPath, NULL);//创建目录,已有的话不影响  
	}
	rootPath += _T("\\");
	chatLogPath = rootPath;
	chatLogPath += _T("chatLog");
	if (!PathIsDirectory(chatLogPath))
	{
		::CreateDirectory(chatLogPath, NULL);//创建目录,已有的话不影响  
	}
	chatLogPath += _T("\\");
	headIconPath = rootPath;
	headIconPath += _T("headIcons");
	if (!PathIsDirectory(headIconPath))
	{
		::CreateDirectory(headIconPath, NULL);//创建目录,已有的话不影响  
	}
	headIconPath += _T("\\");
	fileRecvPath = rootPath;
	fileRecvPath += _T("IMFileRecv");
	if (!PathIsDirectory(fileRecvPath))
	{
		::CreateDirectory(fileRecvPath, NULL);//创建目录,已有的话不影响  
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
	//调用线程来链接和接收消息
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
/*   消息处理                                                           */
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
	//好友未认证
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
	//时间
	
	if (msg.body() != "")
	{
		std::string recvTime;
		if(msg.when()!=0)
			recvTime = msg.when()->stamp();			//const string
		else
		{
		}
		//将消息以我的xml格式写入
		std::string re = "<immessage password:1231231231><immessagefromId>" + msg.from().bare()
			+ "</immessagefromId><immessagebody>" + msg.body()
			+ "</immessagebody></immessage password:1231231231>\n";

		//将数据传给对话框
		//转码
		static CString fromId;
		fromId = msg.from().bare().c_str();
		static CString xmlStr;	//不用static，发送消息会乱码
		char gb2312[64000];
		Utf8ToGB2312(re.c_str(), gb2312);
		xmlStr = gb2312;
		//向Test1Dlg发送消息  让其弹出聊天窗口
		//如果是原来的session，则继续显示data， 否则创建新tab
/*
		if (theApp.friends[sessionFriendId].f_receiveStr != "" && str != "")
			theApp.friends[sessionFriendId].f_receiveStr += "\r\n";*/
// 		Parser pp(this);    //我这里的this是指的接收消息的类，其中实现了TagHandler接口中的//handleTag方法。
//		std::string msgXml = msg.tag()->xml();
// 		pp.feed(re);//当调用这个语句时，就会调用handleTag方法了。
		theApp.friends[sessionFriendId].f_receiveStr += xmlStr;
		::PostMessage(hwnd, WM_RECVDATA, (WPARAM)sessionFriendId, (LPARAM)(LPCTSTR)xmlStr);
	}
	ReleaseSemaphore(hHandleMessageSemaphore, 1, NULL);

}
void Message_Test::SendMessage(CString toId, CString str)		//发送数据
{
// 	if (str_name.Find("@") != -1)
// 		str_target = str_name + "/gloox";
// 	else
// 		str_target = str_name + "@" + str_server + "/gloox";

	//Unicode转宽字节：使带中文的CString转为char*
	int n = str.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL);
	char * pStr = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, n, pStr, len, NULL, NULL);
	pStr[len] = '\0';   //多字节字符以'/0'结束
	//CString 转 char*
 	char* pToId = new char[200];
	sprintf(pToId, "%S", toId);

	char strTemp[64000];
	GB2312ToUtf8(pStr, strTemp);		//多字节pStr转UTF-8

	//发送数据
	JID jidt(pToId);
	MessageSession *mes = new MessageSession(j, jidt);		//dispose后不用删除
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
* 将收到的消息保存为文件
* 参数：JID myId, JID fromId, CString message
*************************************************************************/
void Message_Test::msgSaveToFile(CString myId, CString fromId, CString message)
{
		CString strPath = _T("C:\\Users\\IMGO");
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
		}
		strPath += _T("\\chatLog");
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
		}
		strPath += _T("\\");
		strPath += myId;
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
		}
		strPath += _T("\\");
		strPath += fromId;
		if (!PathIsDirectory(strPath))
		{
			::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
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
		// 处理完毕后,释放资源
		setlocale(LC_CTYPE, pOldLocale);
		free(pOldLocale);
		file.Flush();
		file.Close();
}


/************************************************************************/
/*   好友管理                                                           */
/************************************************************************/
bool isRosterOk = false;
//读取好友列表，不能解决资源不同出现重复的问题,而且有资源的情况又删不掉
void Message_Test::handleRoster(const Roster& roster)
{
//	printf("roster arriving\nitems:\n");

	it = roster.begin();
	itEnd = roster.end();
	//theApp.friendNum = 1;  ??
	for (; it != itEnd; ++it)
	{

		theApp.friends[theApp.friendNum].friendId = (*it).second->jidJID().bare().c_str();		//ID
		theApp.friends[theApp.friendNum].friendIdFull = (*it).second->jidJID().full().c_str();		//ID,解决删不掉的问题
//		theApp.friends[theApp.friendNum].nickName = (*it).second->name().c_str();		//别名？
		StringList g = (*it).second->groups();
		StringList::const_iterator it_g = g.begin();
		if (it_g != g.end())
		{
			theApp.friends[theApp.friendNum].friendGroup = (*it_g).c_str();		//组
		}
		else
		{
			theApp.friends[theApp.friendNum].friendGroup = "Pending Members";
		}
		theApp.friends[theApp.friendNum].friendStatus = 5;		//默认下线？ 如果是在线状态将会收到消息

		char noteName[100];
		Utf8ToGB2312((*it).second->name().c_str(), noteName);
		theApp.friends[theApp.friendNum].f_note = noteName;
		theApp.friendNum++;
	}
	isRosterOk = true;

}
HANDLE hHandleFriendSemaphore;
//bool hFriendInfoSemaphore = false;
//好友状态切换
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
	theApp.friends[i].friendStatus = presence;		//以便被添加好友成功时获取到对方状态
	Sleep(500);
	::PostMessage(hwnd, WM_RECVFRIEND, (WPARAM)i, (LPARAM)friendStatus);
	ReleaseSemaphore(hHandleFriendSemaphore, 1, NULL);
}
//对方添加好友请求
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
	msgBoxStr += "请求添加您为好友，是否同意";
	if (::MessageBox(NULL, msgBoxStr, _T("添加好友"), MB_YESNO) == IDYES)
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
//添加好友
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
//删除好友
void Message_Test::WDDelFriend(CString FullName)
{
	char* pFid = new char[200];
	sprintf(pFid, "%S", FullName);
	JID jid(pFid);
	delete[] pFid;
	j->rosterManager()->remove(jid);
}
//设置备注
void Message_Test::setNoteName(const JID& jid, CString noteName)
{
	char strtmp[100];
 	strcpy_s(strtmp, CStringToChar(noteName));
 	j->rosterManager()->getRosterItem(jid)->setName(strtmp);
 	strcpy_s(strtmp, j->rosterManager()->getRosterItem(jid)->name().c_str());
	j->rosterManager()->synchronize();
}
//好友添加成功
void Message_Test::handleItemAdded(const JID& jid)
{
	isSessionChanged = true;
}
//收到被订阅
void Message_Test::handleItemSubscribed(const JID& jid)
{
	isSessionChanged = true;
}
//收到被取消订阅
void Message_Test::handleItemUnsubscribed(const JID& jid)
{
	isSessionChanged = true;
	//	j->rosterManager()->unsubscribe(jid);
}
//收到被删除
void Message_Test::handleItemRemoved(const JID& jid)
{
	isSessionChanged = true;
	//向某窗口发送消息，让其从好友列表删除
	j->rosterManager()->remove(jid);
	static CString jidFull;
	jidFull = jid.full().c_str();
	::PostMessage(hwnd, WM_RECVREMVED, (WPARAM)(LPCTSTR)jidFull, NULL);

}

//vcard
bool hFriendInfoMutexSemaphore = true;		//信号量
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
		VCard::TelephoneList::const_iterator itPhone = v->telephone().begin();		//电话
		VCard::EmailList::const_iterator itEmail = v->emailAddresses().begin();		//邮箱
		VCard::AddressList::const_iterator itAdress = v->addresses().begin();		//地址
		char strTemp[200];
		if (0 == i)
		{
			theApp.friends[i].friendId = friendId;
		}
		//获取朋友的信息
		if (friendId == theApp.friends[i].friendId)
		{
			if (itPhone != v->telephone().end())
			{
				theApp.friends[i].f_phoneNum = (*itPhone).number.c_str();		//电话
			}
			if (itEmail != v->emailAddresses().end())
			{
				theApp.friends[i].f_email = (*itEmail).userid.c_str();		//邮箱
			}
			if (itAdress != v->addresses().end())
			{
				Utf8ToGB2312((*itAdress).street.c_str(), strTemp);
				theApp.friends[i].f_address = strTemp;	//街道地址
			}
			Utf8ToGB2312(v->formattedname().c_str(), strTemp);
			theApp.friends[i].f_name = strTemp;
			Utf8ToGB2312(v->nickname().c_str(), strTemp);
			theApp.friends[i].nickName = strTemp;
			Utf8ToGB2312(v->desc().c_str(), strTemp);
			theApp.friends[i].descInfo = strTemp;
			Utf8ToGB2312(v->title().c_str(), strTemp);
			theApp.friends[i].f_signature = strTemp;

			//头像
			if (v->photo().binval.length() != 0)
			{
				CString pathTemp = friendId;
				CString strPath = _T("C:\\\\Users\\\\IMGO");
				if (!PathIsDirectory(strPath))
				{
					::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
				}
				strPath += _T("\\\\headIcons");
				if (!PathIsDirectory(strPath))
				{
					::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
				}
				strPath += _T("\\\\");
				strPath += pathTemp;
				if (!PathIsDirectory(strPath))
				{
					::CreateDirectory(strPath, NULL);//创建目录,已有的话不影响  
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
				theApp.friends[i].headIconUrl = strPath;		//头像
			}
		}
		break;
	}
	case 1:				//设置头像
	{
		VCard* vcard = new VCard(*v);		//也不用delete
		CFile file;
		file.Open(theApp.friends[0].headIconUrl, CFile::modeRead | CFile::typeBinary);      //打开图像文件
		DWORD dwDataLen = file.GetLength();        //获取文件的长度
		BYTE *buf = new BYTE[dwDataLen+1];           //创建数组，用来保存图像的数据
		buf[dwDataLen] = '\0';
		file.Read(buf, dwDataLen);            //图像文件的数据保存在数组中
		file.Close();                            //关闭文件

		//转成string
		std::string binval(reinterpret_cast<char const*>(buf), dwDataLen + 1);
		vcard->setPhoto("IMG", binval);
		m_vManager->storeVCard(vcard, this);
		if (buf) delete[] buf;
		break;
	}
	case 2:		//改个人信息
	{
		VCard *vcard = new VCard(*v);
		char strtmp[1000];
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_name));
		vcard->setFormattedname(strtmp);
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].nickName));
		vcard->setNickname(strtmp);
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_phoneNum));
		vcard->addTelephone(strtmp, NULL);								//修改过addTelephone函数
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_address));
		vcard->addAddress(strtmp, "app. 2", strtmp, "Springfield", "region", "123", "CHINA", VCard::AddrTypeHome);		//修改过addAddress函数
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_email));
		vcard->addEmail(strtmp, NULL);									//修改过addEmail函数
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].f_signature));
		vcard->setTitle(strtmp);		//设置签名
		strcpy_s(strtmp, CStringToChar(theApp.friends[0].descInfo));
		vcard->setDesc(strtmp);		//设置描述
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
/*   文件接收                                                           */
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
	tipStr += _T("向您发送文件");
	tipStr += gb2312;
	tipStr.Format(_T("%s,大小为%ld"), tipStr, size);
	if (::MessageBox(NULL, tipStr, _T("文件接收"), MB_YESNO) == IDYES)
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
//一次只能接收一个文件
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
	MessageBox(_T("发送/接受文件失败"));
}

/************************************************************************/
/*   注册用户                                                           */
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


//乱码解决
char * Message_Test::CStringToChar(CString str)
{
	//Unicode转宽字节：使带中文的CString转为char*
	int n = str.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, str, n, NULL, 0, NULL, NULL);
	char * pStr = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, n, pStr, len, NULL, NULL);
	pStr[len] = '\0';   //多字节字符以'/0'结束
	//CString 转 char*
	char strTemp[64000];
	GB2312ToUtf8(pStr, strTemp);		//多字节pStr转UTF-8
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


//设置句柄
void Message_Test::SetFromHwnd(LPVOID lpParameter)
{
	hwnd = ((RECVPARAM2*)lpParameter)->hwnd;
}

//常用函数
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



// Message_Test 消息处理程序


