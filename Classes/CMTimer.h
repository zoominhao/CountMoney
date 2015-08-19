#ifndef __CMTIMER_H__
#define __CMTIMER_H__

#include "cocos2d.h"
#include "Player.h"


USING_NS_CC;

class CMTimer : public Node
{
public:
	CMTimer();
	~CMTimer();
	//void createLabel(Vec2 pos, Player* pyr);
	void createLabel(Vec2 pos, Player* pyr, int sceneKind = 1);

	CREATE_FUNC(CMTimer);
	void setTotalTime(float totaltime){ m_totaltime = totaltime; }

	void startTimer();

private:
	void updatetime(float time);

	//时间到了的切换界面
	void switchScene();

private:
	Label* m_timerLabel;

	float  m_totaltime;
	float  m_timeLeft;
	bool   m_starter;
	bool   m_ender;

	Player*  m_player_status;
};

#endif // __CMTIMER_H__