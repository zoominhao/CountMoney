#include "CMTimer.h"
#include "LoserEndScene.h"
#include "TellerEndScene.h"


CMTimer::CMTimer() :m_timerLabel(NULL), m_totaltime(10.0), m_starter(true), m_ender(false)
{
	m_timeLeft = m_totaltime;
}

CMTimer::~CMTimer()
{
	
}

void CMTimer::createLabel(Vec2 pos, Player* pyr, int sceneMode)
{
	char totalTimeStr[10];
	sprintf(totalTimeStr, "%4.2f", m_totaltime);
	m_timerLabel = Label::createWithTTF(totalTimeStr, "fonts/Marker Felt.ttf", 60);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_timerLabel->setPosition(pos);
	//m_timerLabel->setRotation(90);                               //放置在竖直白线上的，所以旋转90度
	m_timerLabel->setColor(Color3B(0.0, 0.0, 0.0));

	m_player_status = pyr;
	m_sceneMode = sceneMode;
	this->addChild(m_timerLabel, 2);
}

void CMTimer::updatetime(float time)
{
	m_timeLeft -= 0.01;
	if (m_timeLeft <= 0)
	{
		m_ender = true;
		unschedule(schedule_selector(CMTimer::updatetime));
		m_timerLabel->setString("0.00");
		switchScene();
	}
	else
	{
		char timeLeftStr[10];
		sprintf(timeLeftStr, "%4.2f", m_timeLeft);
		m_timerLabel->setString(timeLeftStr);
	}
}

void CMTimer::startTimer()
{
	if (m_starter)
	{
		m_totaltime = 10.0;
		m_starter = false;
		schedule(schedule_selector(CMTimer::updatetime), 0.01f);
	}
}

void CMTimer::switchScene()
{
	char scoreStr[20];
	sprintf(scoreStr, "%d", m_player_status->totalMoneyNum());
	Scene* scene;
	if (m_sceneMode == 1)
	{
		scene = LoserEndScene::createScene(scoreStr);
	}
	else
	{
		scene = TellerEndScene::createScene(scoreStr);
	}
	 

	Director::getInstance()->replaceScene(scene);
}
