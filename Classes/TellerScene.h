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
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(TellerScene);

private:
	void setBgImage(float px, float py);
	void addTitle(float px, float py);


	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);

private:
	Vec2 _spos;

	bool m_count_flag;
	bool m_isFake;

	//audio effect
	int m_effect_id;
	//¼ÆÊ±Æ÷
	CMTimer* m_cmTimer;
	//Íæ¼Ò
	Player*  m_player;
};

#endif // __MAIN_SCENE_H__
