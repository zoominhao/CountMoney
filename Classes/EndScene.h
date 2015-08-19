#ifndef _END_SCENE_H_
#define _END_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class EndScene : public Layer
{
public:
	static Scene* createScene(RenderTexture* sqr, int score = 1000);
	virtual bool init();
	CREATE_FUNC(EndScene);

private:
	void restartButton(Ref *pSender, ui::Widget::TouchEventType type);
	void returnButton(Ref *pSender, ui::Widget::TouchEventType type);

};
#endif