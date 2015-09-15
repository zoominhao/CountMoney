#ifndef _MULTI_END_SCENE_H_
#define _MULTI_END_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class MultiEndScene : public Layer
{
public:
	static Scene* createScene(const char* resultstr);
	virtual bool init();
	CREATE_FUNC(MultiEndScene);


private:
	void restartButton(Ref *pSender, ui::Widget::TouchEventType type);
	void returnButton(Ref *pSender, ui::Widget::TouchEventType type);


	void addWinSprite(int whichplayer);

};
#endif