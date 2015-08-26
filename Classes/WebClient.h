#pragma once

#ifdef _WIN32  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#pragma comment(lib, "ws2_32.lib")  
#else  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#endif  

#include <string>
#include <mutex>

// for test
#include "cocos2d.h"

#define EVENT_REGIST 0X00
#define EVENT_LOGIN  0X01

#define EVENT_UPLOAD_1 0X0100
#define EVENT_UPLOAD_1 0X0200
#define EVENT_UPLOAD_1 0X0300

#define EVENT_QUERY_1 0X010000
#define EVENT_QUERY_2 0X020000
#define EVENT_QUERY_3 0X030000

#define EVENT_WEAPON_1 0X01000000
#define EVENT_WEAPON_2 0X02000000
#define EVENT_WEAPON_3 0X03000000
#define EVENT_WEAPON_4 0X04000000

#define EVENT_FIGHT 0X10000000


class WebClient
{
public:
	static WebClient &getinstance()
	{
		static WebClient instance;
		return instance;
	}
	
	~WebClient();

	void test(cocos2d::Label *l);
	// for test
	cocos2d::Label *label;

	void start();
	void send(const std::string& data);

	void regidterFightCallback(void(*handleFightMsg)(int type, int direction, void *arg), void *arg);
private:
	WebClient();
	void getMessage(int socket);
	void handle_fight_event(int type, char direction);

	void(*fight)(int type, int direction, void *arg);
	void *fight_data;

	static WebClient *instance;
	int status;// 0: OK ; -1: NULL
	int socket;
	std::mutex mutex;
	fd_set fdset;
};

