#include "WebClient.h"
#include <iostream>

#include <stdio.h>
#include <thread>

#define DEFAULT_PORT 8080
//#define DEFAULT_ADDR "10.66.227.70"
//#define DEFAULT_ADDR "10.12.194.26"
//#define DEFAULT_ADDR "192.168.1.102"
#define DEFAULT_ADDR "10.66.237.210"


WebClient::WebClient()
{
	status = -1;
}

WebClient::~WebClient()
{
#ifdef _WIN32  
	closesocket(socket);
	WSACleanup();
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
		int ret = select(socket + 1, &fdset, NULL, NULL, NULL);  // windows ignore the 1st param
		if (ret == -1)
		{
			cocos2d::MessageBox("err select", "");
			return ;
		}
		if (FD_ISSET(socket, &fdset))
		{
			char buf[128];
			mutex.lock();
			int ret = recv(socket, buf, sizeof(int), 0);
			mutex.unlock();

			if (ret == 0)
			{
				//TODO:: close
				//printf("read_cb connection closed\n");
				//cocos2d::MessageBox("read_cb connection closed:", "");
				return;
			}
			else if (ret < 0)
			{
				//TODO:: error

				return;
			}

			int event = *(int *)buf;
			switch (event)
			{
			case EVENT_FIGHT:
			case EVENT_WEAPON_1:
			case EVENT_WEAPON_2:
			case EVENT_WEAPON_3:
			case EVENT_WEAPON_4:
			{
				int ret = recv(socket, buf, sizeof(char), 0);
				char direction = *buf;
				handle_fight_event(event, direction);
				break;
			}
			default:
				// TODO: error
				break;
			}
		}
	}
}

void WebClient::regidterFightCallback(void(*handleFightMsg)(int type, int direction, void *arg), void *arg)
{
	fight = handleFightMsg;
	fight_data = arg;
}

void WebClient::handle_fight_event(int type, char direction)
{
	fight(type, (int)direction, fight_data);
}

void WebClient::start()
{
	int ret;
#ifdef _WIN32  
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0) {
		cocos2d::MessageBox("@WSAStartup2", "DEBUG");
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return;
	}
#endif  
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket < 0)
	{
		fprintf(stderr, "socket error!\n"); 
		cocos2d::MessageBox("@2", "DEBUG"); 
		return;
	}
	struct sockaddr_in servaddr;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
#ifdef _WIN32  
	servaddr.sin_addr.S_un.S_addr = inet_addr(DEFAULT_ADDR);
#else  
	servaddr.sin_addr.s_addr = inet_addr(DEFAULT_ADDR);
#endif  
	servaddr.sin_port = htons(DEFAULT_PORT);
	ret = connect(socket, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (ret < 0) {
#ifdef _WIN32  
		closesocket(socket);
#else  
		close(socket);
#endif  
		cocos2d::MessageBox("@close socket", "DEBUG");
		return;
	}
	status = 0;
	std::thread thread_getMsg(&WebClient::getMessage, this, socket);
	thread_getMsg.detach();
	return;
}

void WebClient::send(const std::string& data)
{
	if (status != 0)
		return;
	mutex.lock();
	int ret = ::send(socket, data.c_str(), data.size(), 0);
	mutex.unlock();
	if (ret < 0) {
		//MessageBox("send error", "title");
	}
}