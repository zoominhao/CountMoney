#include "CMTimer.h"
#include "LoserEndScene.h"
#include "TellerEndScene.h"
#include "EndlessEndScene.h"
#include "OnlineEndScene.h"
#include "WebClient.h"
#include "AudioControl.h"
#include "MCUtil.h"


CMTimer::CMTimer() :m_timerLabel(NULL), m_totaltime(10.0), m_starter(true), m_ender(false)
{
	m_timeLeft = m_totaltime;
	m_allowed = true;
}

CMTimer::~CMTimer()
{
	
}

void CMTimer::createLabel(Vec2 pos, Player* pyr, int sceneMode)
{
	char totalTimeStr[10];
	sprintf(totalTimeStr, "%4.2f", m_totaltime);
	m_timerLabel = Label::createWithTTF(totalTimeStr, "fonts/DTLNobelT-Bold.otf", 50);
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_timerLabel->setPosition(pos);
	//m_timerLabel->setRotation(90);                               //放置在竖直白线上的，所以旋转90度
	m_timerLabel->setColor(MCUtil::m_timerColor);

	m_player_status = pyr;
	m_sceneMode = sceneMode;
	this->addChild(m_timerLabel, 2);
}

void CMTimer::updatetime(float time)
{
	m_timeLeft -= 0.01;
	if (m_timeLeft < 0.00)
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
	if (m_starter && m_allowed)
	{
		//m_totaltime = 10.0;
		m_starter = false;
		this->schedule(schedule_selector(CMTimer::updatetime), 0.01f);
	}
}

void CMTimer::stopTimer()
{
	if (!m_starter)
	{
		m_starter = true;
		this->unschedule(schedule_selector(CMTimer::updatetime));
	}
}

void CMTimer::switchScene()
{
	char displayStr[100];
	Scene* scene;
	if (m_sceneMode == 1)
	{
		AudioControl::stopBGMusic();
		sprintf(displayStr, "%d", m_player_status->totalMoneyNum());
		scene = LoserEndScene::createScene(displayStr, m_player_status->totalMoneyNum());
		AudioControl::playOverEffect();
	}
	else if (m_sceneMode == 2)
	{
		AudioControl::stopBGMusic();
		sprintf(displayStr, "%d", m_player_status->totalMoneyNum());
		scene = TellerEndScene::createScene(displayStr, m_player_status->totalMoneyNum());
		AudioControl::playOverEffect();
	}
	else if (m_sceneMode == 3)
	{
		AudioControl::stopBGMusic();
		sprintf(displayStr, "%d ", m_player_status->stageNum() + 1);
		scene = EndlessEndScene::createScene(displayStr, m_player_status->stageNum() + 1);
		AudioControl::playEndlessEffect(false);
	}
	else if (m_sceneMode == 4)
	{
	#ifdef _WIN32  
		Sleep(100);
	#else  
		sleep(0.1);
	#endif  
		
		AudioControl::stopBGMusic();
		if (m_player_status->Win() == 1)
		{
			scene = OnlineEndScene::createScene("1");
		}
		else if (m_player_status->Win() == -1)
		{
			scene = OnlineEndScene::createScene("-1");
		}
		else
		{
			scene = OnlineEndScene::createScene("0");
		}
	}
	 

	Director::getInstance()->replaceScene(scene);
}


