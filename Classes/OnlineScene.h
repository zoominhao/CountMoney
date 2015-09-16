#ifndef __ONLINE_SCENE_H__
#define __ONLINE_SCENE_H__

#include "cocos2d.h"
#include "CMTimer.h"
#include "Player.h"
#include "WebClient.h"


USING_NS_CC;

class OnlineScene : public Layer, public WebClientMethod
{
public:
	~OnlineScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(OnlineScene);

	virtual void onFight(int type, bool toMe, int enemyScore);
	virtual void onClose(){};
	virtual void onUpdateScore() {}
	virtual void onSendScore(bool levelup) {}
	virtual void onError(const std::string message){};
	virtual void onPause(){};
	virtual void onContinue(){};
	// name1,score1,name2,score2,...
	virtual void onQuery(GameMode mode, std::vector<std::string> result){};
	virtual void onStatusChanged(ConnectStatus status);


private:
	void setBgImage();


	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//callback
	void returnCallback(Ref* pSender);
	void pauseCallback(Ref* pSender);

	//add timer frame
	void addTimerFrame();

	///����
	void createVS();
	void changeMeVS();
	void changeUVS();
	void startConnect();

	void sendFightEvent(int type, bool toMe);
	void sendStartEvent();

	//
	void readyGoAct();


private:
	Vec2 _spos;
	Vec2 _curPos;

	bool m_count_flag;
	bool m_timerStart;

	//audio effect
	int m_effect_id;
	//��ʱ��
	CMTimer* m_cmTimer;
	//���
	Player*  m_player;

	Sprite*  m_timerFrame;

	//����
	Label* m_meLabel;
	Label* m_uLabel;

	int    m_uScore;
	bool   m_connected;
};

#endif // __ONLINE_SCENE_H__
