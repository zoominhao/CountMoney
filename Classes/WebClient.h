#ifndef __WEB_CLIENT_H__
#define __WEB_CLIENT_H__

#ifdef _WIN32  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#pragma comment(lib, "ws2_32.lib")  
#else  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <fcntl.h>
#include <unistd.h>
#endif  

#include <string>
#include <mutex>


/*
typedef enum{
	EVENT_REGIST,
	EVENT_LOGIN,
	EVENT_UPLOAD_1,
	EVENT_UPLOAD_2,
	EVENT_QUERY_1,
	EVENT_QUERY_2
} MCUSER;*/

typedef enum{
	EVENT_TOOL_1,
	EVENT_TOOL_2,
	EVENT_TOOL_3,
	EVENT_TOOL_4,
	EVENT_MONEY_REAL,
	EVENT_MONEY_FAKE,
	EVENT_NO_PAIR,
	EVENT_LOSE,
	EVENT_START,
} MCFIGHT;


class WebClientMethod
{
public:
	virtual ~WebClientMethod(){};

	virtual void onFight(MCFIGHT  fight_event, bool toMe) = 0;
	virtual void onOpponentExit() = 0;
	virtual void onMatch() = 0;
	virtual void onWait() = 0;
	virtual void onStart() = 0;
	virtual void onError(const std::string message) = 0;
};

class WebClient
{
public:
	static WebClient &getinstance()
	{
		static WebClient instance;
		return instance;
	}

	~WebClient();

	void registerMethod(WebClientMethod* CbObj);
	void unregisterMethod();
	void start();
	void shutdown();
	void shutdown2();
	void pause();
	void conti();
	void sendCountEvent(MCFIGHT fight_event, bool toMe = false);
	

private:
	WebClient();
	void getMessage(int socket);
	void send(const char *data, int size);
	void close();
	void partExit();
	bool connect();


	bool createSocket();

	static WebClient *instance;
	int status;      // 0: OK ; -1: close
	int socket;
	std::mutex mutex;
	fd_set fdset;
	WebClientMethod* m_CbObj;
};

#endif // __WEB_CLIENT_H__