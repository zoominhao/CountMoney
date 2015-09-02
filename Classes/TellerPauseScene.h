#ifndef _TELLER_PAUSE_SCENE_H_
#define _TELLER_PAUSE_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class TellerPauseScene : public Layer
{
public:
	static Scene* createScene(RenderTexture* sqr, int score = 1000);
	virtual bool init();
	CREATE_FUNC(TellerPauseScene);

private:
	void continueCallback(Ref *pSender, ui::Widget::TouchEventType type);
	void restartCallback(Ref *pSender, ui::Widget::TouchEventType type);
	void returnCallback(Ref *pSender, ui::Widget::TouchEventType type);


};
#endif