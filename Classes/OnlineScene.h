/*
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
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(OnlineScene);

	//联网响应事件
	virtual void onFight(MCFIGHT  fight_event, bool toMe);
	virtual void onOpponentExit();
	virtual void onMatch();
	virtual void onWait();
	virtual void onStart();
	virtual void onError(const std::string message);

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

	///联网
	void createVS();
	void changeMeVS();
	void changeUVS();
	void sendFight(MCFIGHT fight_type);
	void startConnect();
	void closeConnection();

	void timetest(float time);

private:
	Vec2 _spos;
	Vec2 _curPos;

	bool m_count_flag;
	bool m_timerStart;

	//audio effect
	int m_effect_id;
	//计时器
	CMTimer* m_cmTimer;
	//玩家
	Player*  m_player;

	Sprite*  m_timerFrame;

	//联网
	Label* m_meLabel;
	Label* m_uLabel;

	int    m_uScore;
	bool   m_connected;
};

#endif // __ONLINE_SCENE_H__
*/
