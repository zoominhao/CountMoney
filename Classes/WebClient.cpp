#include "WebClient.h"

#include <thread>
#include <sstream>


#define DEFAULT_PORT 8080
//#define DEFAULT_ADDR "10.66.227.70"
#define DEFAULT_ADDR "112.74.212.113"
//#define DEFAULT_ADDR "192.168.1.102"
//#define DEFAULT_ADDR "10.66.237.210"

WebClient::WebClient()
{
	status = -1;
	method = NULL;
}

WebClient::~WebClient()
{
	if (status == 0)
		close();
}

void WebClient::close()
{
#ifdef _WIN32  
	closesocket(socket);
	WSACleanup();
#else
	::close(socket);
#endif  
	method->onClose();
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
			method->onError("select error");
			break; // while
		}
		if (status == -1) // shutdown
			return;
		if (FD_ISSET(socket, &fdset)) {
			char buf[512];
			mutex.lock();
			int ret = recv(socket, buf, 4, 0);
			mutex.unlock();

			if (ret == 0) {
				break; // while
			}
			else if (ret < 0) {
				method->onError("recv error");
				break; // while
			}

			int event = *(int *)buf;
			if (event & EVENT_FIGHT_MASK) {
				int ret = recv(socket, buf, 1, 0);
				char direction = *buf;
				method->onFight(event, (direction == 0) ? false : true);
			}
			else if (event & EVENT_UPLOAD_MASK) {
				recv(socket, buf, 1, 0);
				if (event & EVENT_UPLOAD_LUP_MASK)
					method->onUpdateScore();
				else {
					bool islevelup = (buf[0] == 0x10);
					method->onSendScore(islevelup);
				}
			}
			else if (event & EVENT_QUERY_MASK) {
				recv(socket, buf, 1, 0); // err
				recv(socket, buf, 4, 0); // size
				int size = *(int *)buf;
				recv(socket, buf, size, 0);
				std::string name, score;
				std::vector<std::string> result;
				char *str = buf;
				// number#name#score#name#score#...#
				int pos = strcspn(str, "#");
				buf[pos] = '\0';
				int number;
				//scanf(buf, "%d", &number); 
				std::stringstream iss(buf);
				iss >> number;

				str = buf + pos + 1;
				for (int i = 0; i < number; i++) {
					pos = strcspn(str, "#");
					name.assign(str, pos);
					str = str + pos + 1;
					pos = strcspn(str, "#");
					score.assign(str, pos);
					str = str + pos + 1;
					result.push_back(name);
					result.push_back(score);
				}
				method->onQuery(GameMode((event >> EVENT_QUERY_SHIFT) - 1), result);
			}
			else
				method->onError("unknow event");
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
		method->onError("create socket error");
		return;
	}
	if (!connect()) {
		close();
		method->onError("connection error");
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
	close();
}

void WebClient::pause()
{
	// send pause event
}

void WebClient::conti()
{
	// send conti event
}

void WebClient::query(enum GameMode mode)
{
	if (status != 0)
		return;
	int type;
	switch (mode) {
	case DS:
		type = EVENT_QUERY_1;
		break;
	case GY:
		type = EVENT_QUERY_2;
		break;
	case WJ:
		type = EVENT_QUERY_3;
		break;
	case PK:
		type = EVENT_QUERY_4;
		break;
	default:
		method->onError("mode error");
		break;
	}
	char buf[16];
	memcpy(buf, &type, 4);
	send((char *)&buf, 4);
}

void WebClient::sendFightEvent(int type, bool toMe)
{
	if (status != 0)
		return;
	char buf[8];
	memcpy(buf, &type, 4);
	char diretion = toMe ? 1 : 0;
	memcpy(buf + 4, &diretion, 1);
	send((char *)&buf, 5);
}

void WebClient::sendScore(enum GameMode mode, int score)
{
	if (status != 0)
		return;
	int type;
	switch (mode) {
	case DS:
		type = EVENT_UPLOAD_1;
		break;
	case GY:
		type = EVENT_UPLOAD_2;
		break;
	case WJ:
		type = EVENT_UPLOAD_3;
		break;
	case PK:
		type = EVENT_UPLOAD_4;
		break;
	default:
		method->onError("mode error");
		break;
	}
	char buf[16];
	memcpy(buf, &type, 4);
	memcpy(buf + 4, &score, 4);
	send((char *)&buf, 8);
}

void WebClient::updateScore(enum GameMode mode, std::string name, int score)
{
	if (status != 0)
		return;
	int type;
	switch (mode) {
	case DS:
		type = EVENT_UPLOAD_1 | EVENT_UPLOAD_LUP_MASK;
		break;
	case GY:
		type = EVENT_UPLOAD_2 | EVENT_UPLOAD_LUP_MASK;
		break;
	case WJ:
		type = EVENT_UPLOAD_3 | EVENT_UPLOAD_LUP_MASK;
		break;
	case PK:
		type = EVENT_UPLOAD_4 | EVENT_UPLOAD_LUP_MASK;
		break;
	default:
		method->onError("mode error");
		break;
	}
	char buf[512];
	memcpy(buf, &type, 4);
	memcpy(buf + 4, &score, 4);
	int size = name.size();
	memcpy(buf + 8, &size, 4);
	memcpy(buf + 12, name.c_str(), size);
	send((char *)&buf, 12 + size);
}

void WebClient::send(const char *data, int size)
{
	mutex.lock();
	int ret = ::send(socket, data, size, 0);
	mutex.unlock();
	if (ret < 0) {
		method->onError("send error");
	}
}

void WebClient::registerMethod(WebClientMethod *method)
{
	this->method = method;
}
