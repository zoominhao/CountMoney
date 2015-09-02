#ifndef _LOSER_PAUSE_SCENE_H_
#define _LOSER_PAUSE_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class LoserPauseScene : public Layer
{
public:
	static Scene* createScene(RenderTexture* sqr, int score = 1000);
	virtual bool init();
	CREATE_FUNC(LoserPauseScene);

private:
	void continueCallback(Ref *pSender, ui::Widget::TouchEventType type);
	void restartCallback(Ref *pSender, ui::Widget::TouchEventType type);
	void returnCallback(Ref *pSender, ui::Widget::TouchEventType type);


};
#endif