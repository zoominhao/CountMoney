#ifndef _ONLINE_END_SCENE_H_
#define _ONLINE_END_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class OnlineEndScene : public Layer
{
public:
	static Scene* createScene(const char* resultstr);
	virtual bool init();
	CREATE_FUNC(OnlineEndScene);


private:
	void restartButton(Ref *pSender, ui::Widget::TouchEventType type);
	void returnButton(Ref *pSender, ui::Widget::TouchEventType type);

};
#endif