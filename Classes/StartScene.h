#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class StartScene : public Layer
{
public:
	static Scene* createScene(); 
	virtual bool init();
	CREATE_FUNC(StartScene);


	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

private:
	void SingleScene(Ref *pSender, ui::Widget::TouchEventType type);
	void MultiScene(Ref *pSender, ui::Widget::TouchEventType type);
	void NetScene(Ref *pSender, ui::Widget::TouchEventType type);

	void RankScene(Ref *pSender, ui::Widget::TouchEventType type);
	void SetScene(Ref *pSender, ui::Widget::TouchEventType type);


	void DManualScene(Ref *pSender, ui::Widget::TouchEventType type);

	void multiOpenAct();

	void openSceneAnimation();

};
#endif