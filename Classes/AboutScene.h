#ifndef _ABOUT_SCENE_H_
#define _ABOUT_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class AboutScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(AboutScene);

private:
	void StartScene(Ref *pSender, ui::Widget::TouchEventType type);


};
#endif