#ifndef _SET_SCENE_H_
#define _SET_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class SetScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SetScene);

private:
	void returnCallback(Ref* pSender);

	void setBgImage();

	bool isBGMusicStopped;
	bool isEffectStopped;

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);


	Sprite* bgMusicBtn;
	Sprite* effectBtn;
};
#endif