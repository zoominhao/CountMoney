#ifndef __LOSER_SCENE_H__
#define __LOSER_SCENE_H__

#include "cocos2d.h"
#include "CMTimer.h"
#include "Player.h"
#include "FlyWord.h"


USING_NS_CC;

class LoserScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoserScene);
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

	void readyGoAct();

	void manualAct1();
	void manualAct2();
	void updateManualAct(float time);

private:
	Vec2 _spos;
	Vec2 _curPos;

	bool m_count_flag;

	//audio effect
	int m_effect_id;
	//¼ÆÊ±Æ÷
	CMTimer* m_cmTimer;
	//Íæ¼Ò
	Player*  m_player;
	
	Sprite*  m_timerFrame;

	bool     m_enabled;

	FlyWord *m_flyword;
};

#endif // __LOSER_SCENE_H__
