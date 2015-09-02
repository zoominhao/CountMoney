#include "WebClient.h"

#include <thread>



#define DEFAULT_PORT 8080

//#define DEFAULT_ADDR "192.168.1.102"
#define DEFAULT_ADDR "10.66.237.62"

WebClient::WebClient()
{
	status = -1;
	m_CbObj = NULL;
}

WebClient::~WebClient()
{
	if (status == 0)
	{
		m_CbObj = NULL;
		close();
	}
		
}

void WebClient::close()
{
#ifdef _WIN32  
	closesocket(socket);
	WSACleanup();
#else
	::close(socket);
#endif  
	status = -1;
}

void WebClient::partExit()
{
#ifdef _WIN32  
	closesocket(socket);
	WSACleanup();
#else
	::close(socket);
#endif  
	status = -1;
}


void WebClient::getMessage(int socket)
{
	while (true)
	{
		FD_ZERO(&fdset);
		FD_SET(socket, &fdset);

		fd_set fd_pre = fdset;
		if (select(socket + 1, &fdset, NULL, NULL, NULL) == -1) {  // windows ignore the 1st param
			m_CbObj->onError("select error");
			break; // while
		}
		if (status == -1) // shutdown
			return;
		if (FD_ISSET(socket, &fdset)) {
			//char buf[128];
			char buf[32];
			mutex.lock();
			int ret = recv(socket, buf, sizeof(int), 0);
			mutex.unlock();

			if (ret == 0) {
				break; // while
			}
			else if (ret < 0) {
				if (m_CbObj != NULL)
				{
					m_CbObj->onError("recv error");
				}
				break; // while
			}
			switch (buf[0] - '0') {
			case EVENT_MONEY_REAL:
			{
				char direction = *buf;
				if (m_CbObj != NULL)
				{
					m_CbObj->onFight(EVENT_MONEY_REAL, buf[1] == '1');
				}
				break; //switch
			}
			case EVENT_NO_PAIR:
			{
				if (buf[1] == '1')   //自己发来的消息，配对不成功
				{
					m_CbObj->onWait();
				}
				if (buf[1] == '0')   //对方发来的消息，证明配对成功
				{
					m_CbObj->onMatch();
				}
				break;
			}
			case EVENT_LOSE:
			{
				if (buf[1] == '0')   //对方发来的消息
				{
					if (m_CbObj != NULL)
					{
						m_CbObj->onOpponentExit();
					}
				}
				break;
			}
			case EVENT_START:
			{
				if (buf[1] == '0')   //对方发来的消息
				{
					m_CbObj->onStart();
				}
				break;
			}
			default:
				m_CbObj->onError("unknow event");
				break; // switch
			}
		}
	}
	close();
}

bool WebClient::createSocket()
{
#ifdef _WIN32  
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		return false;
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return false;
	}
#endif  
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0)
	{
		return false;
	}
	return true;
}

bool WebClient::connect()
{
	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
#ifdef _WIN32  
	servaddr.sin_addr.S_un.S_addr = inet_addr(DEFAULT_ADDR);
#else  
	servaddr.sin_addr.s_addr = inet_addr(DEFAULT_ADDR);
#endif  
	servaddr.sin_port = htons(DEFAULT_PORT);

#ifdef _WIN32  
	if (::connect(socket, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		return false;
	}
#else
	// Set non-blocking 
	if (fcntl(socket, F_SETFL, O_NONBLOCK) < 0)
		return false;
	// Trying to connect with timeout 
	int res;
	fd_set myset;
	struct timeval tv;
	int valopt;
	socklen_t lon;

	res = ::connect(socket, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (res < 0) {
		if (errno != EINPROGRESS)
			return false;
		do {
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			FD_ZERO(&myset);
			FD_SET(socket, &myset);
			res = select(socket + 1, NULL, &myset, NULL, &tv);
			if (res < 0 && errno != EINTR) {
				return false;
			}
			else if (res > 0) {
				// Socket selected for write 
				lon = sizeof(int);
				if (getsockopt(socket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0)
					return false;
				// Check the value returned... 
				if (valopt)
					return false;
				break;
			}
			else
				return false;
		} while (1);
	}
	// Set to blocking mode again... 
	long arg;
	if ((arg = fcntl(socket, F_GETFL, NULL)) < 0)
		return false;
	arg &= (~O_NONBLOCK);
	if (fcntl(socket, F_SETFL, arg) < 0)
		return false;
#endif
	return true;
}

void WebClient::start()
{
	if (!createSocket()) {
		m_CbObj->onError("create socket error");
		return;
	}
	if (!connect()) {
		close();
		m_CbObj->onError("connection error");
		return;
	}
	std::thread thread_getMsg(&WebClient::getMessage, this, socket);
	thread_getMsg.detach();
	status = 0;
	return;
}

void WebClient::shutdown()
{
	status = -1;
	partExit();
}

void WebClient::pause()
{
	// send pause event
}

void WebClient::conti()
{
	// send conti event
}

void WebClient::sendCountEvent(MCFIGHT fight_event, bool toMe)
{
	if (status != 0)
		return;
	char buf[10];
	sprintf(buf, "%d%d", fight_event, toMe);
	send(buf, strlen(buf));
}

void WebClient::send(const char *data, int size)
{
	mutex.lock();
	int ret = ::send(socket, data, size, 0);
	mutex.unlock();
	if (ret < 0) {
		m_CbObj->onError("send error");
	}
}

void WebClient::registerMethod(WebClientMethod *CbObj)
{
	m_CbObj = CbObj;
}

void WebClient::unregisterMethod()
{
	m_CbObj = NULL;
}

void WebClient::shutdown2()
{
	status = -1;
}

