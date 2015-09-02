#ifndef _SINGLE_SCENE_H_
#define _SINGLE_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class SingleScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SingleScene);

private:
	void StartScene(Ref *pSender, ui::Widget::TouchEventType type);
	void DSScene(Ref *pSender, ui::Widget::TouchEventType type);
	void GYScene(Ref *pSender, ui::Widget::TouchEventType type);
	void WJScene(Ref *pSender, ui::Widget::TouchEventType type);

};
#endif