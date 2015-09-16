#pragma once

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
#include <vector>

#define EVENT_REGIST 0X00
#define EVENT_LOGIN  0X01

#define EVENT_UPLOAD_1    0X0100
#define EVENT_UPLOAD_2	  0X0200
#define EVENT_UPLOAD_3    0X0300
#define EVENT_UPLOAD_4    0X0400
#define EVENT_UPLOAD_LUP_MASK    0X1000
#define EVENT_UPLOAD_MASK 0XFF00

#define EVENT_QUERY_1	 0X010000
#define EVENT_QUERY_2	 0X020000
#define EVENT_QUERY_3	 0X030000
#define EVENT_QUERY_4	 0X040000
#define EVENT_QUERY_MASK 0XFF0000
#define EVENT_QUERY_SHIFT 16

#define EVENT_STATUS_SHIFT 28

#define EVENT_FIGHT      0X01000000
#define EVENT_WEAPON_1   0X02000000
#define EVENT_WEAPON_2   0X03000000
#define EVENT_WEAPON_3   0X04000000
#define EVENT_WEAPON_4   0X05000000
#define EVENT_FIGHT_MASK 0X0F000000
// TODO::
#define EVENT_STATUS_MASK 	 0XF0000000
#define EVENT_WAIT	 		 0X10000000
#define EVENT_READY		 	 0X20000000
#define EVENT_ENEMY_QUIT 	 0X30000000
#define EVENT_GAMEOVER	 	 0X40000000
#define EVENT_START	 	 	 0X50000000

enum GameMode
{
	DS = 0, GY, WJ, PK
};

//TODO:
enum ConnectStatus
{
	WAITING = 0, ENEMY_READY, ENEMY_QUIT, ENEMY_START = 4
};

class WebClientMethod
{
public:
	virtual ~WebClientMethod(){};

	virtual void onFight(int type, bool toMe, int enemyScore) = 0;
	virtual void onClose() = 0;
	virtual void onPause() = 0;
	virtual void onContinue() = 0;
	virtual void onQuery(GameMode mode, std::vector<std::string> result) = 0;
	virtual void onUpdateScore() = 0;
	virtual void onSendScore(bool levelup) = 0;
	virtual void onError(const std::string message) = 0;
	// TODO::
	virtual void onStatusChanged(ConnectStatus status) = 0;
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

	void registerMethod(WebClientMethod *method);
	void start();
	void shutdown();
	void pause();
	void conti();
	void query(enum GameMode mode);
	void sendScore(enum GameMode mode, int score);
	void updateScore(enum GameMode mode, std::string name, int score);
	void sendFightEvent(int type, bool toMe, int myScore);
	void sendGameOverEvent();
	void sendReadyEvent();
	void sendStartEvent();

private:
	WebClient();
	void getMessage(int socket);
	void send(const char *data, int size);
	void close();
	bool connect();

	bool createSocket();

	static WebClient *instance;
	int status;// 0: OK ; -1: close
	int socket;
	std::mutex mutex;
	fd_set fdset;
	WebClientMethod *method;
};

