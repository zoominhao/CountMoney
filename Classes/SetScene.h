#ifndef _SET_SCENE_H_
#define _SET_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

class SetScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SetScene);

private:
	void returnCallback(Ref *pSender, ui::Widget::TouchEventType type);
	void infoScene(Ref *pSender, ui::Widget::TouchEventType type);

	void effectCallback(Ref *pSender, ui::CheckBoxEventType type);
	void BGMCallback(Ref *pSender, ui::CheckBoxEventType type);

};
#endif