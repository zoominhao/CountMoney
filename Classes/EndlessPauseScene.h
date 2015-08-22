#ifndef _ENDLESS_PAUSE_SCENE_H_
#define _ENDLESS_PAUSE_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class EndlessPauseScene : public Layer
{
public:
	static Scene* createScene(RenderTexture* sqr, int score = 1000);
	virtual bool init();
	CREATE_FUNC(EndlessPauseScene);

private:
	void continueCallback(Ref *pSender);
	void restartCallback(Ref *pSender);


};
#endif