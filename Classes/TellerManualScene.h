#ifndef _TELLER_MANUAL_SCENE_H_
#define _TELLER_MANUAL_SCENE_H_

#include "cocos2d.h"
#include "SlidingLayer.h"

USING_NS_CC;

class TellerManualScene : public SlidingLayer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(TellerManualScene);

private:

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	virtual void btnCB(Ref *pSender, ui::Widget::TouchEventType type);

};

#endif