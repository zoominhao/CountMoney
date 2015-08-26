#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "CMTimer.h"
#include "Player.h"

USING_NS_CC;

class TellerScene : public Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(TellerScene);

private:
	void setBgImage();


	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);

	//callback
	void returnCallback(Ref* pSender);
	void pauseCallback(Ref* pSender);

	//add Cat
	void addCounter();

	void addTranshCan();

private:
	Vec2 _spos;
	Vec2 _curPos;

	bool m_count_flag;
	bool m_nextFake;
	bool m_curFake;
	bool m_needRand;

	bool m_isEmpty;
	//audio effect
	int m_effect_id;

	//¼ÆÊ±Æ÷
	CMTimer* m_cmTimer;
	//Íæ¼Ò
	Player*  m_player;
	//counter animation
	Sprite*  m_counter;
	//transh can
	Sprite*  m_transhCan;
};

#endif // __MAIN_SCENE_H__
