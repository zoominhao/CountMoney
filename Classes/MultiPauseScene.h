#ifndef _MULTI_PAUSE_SCENE_H_
#define _MULTI_PAUSE_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class MultiPauseScene : public Layer
{
public:
	static Scene* createScene(RenderTexture* sqr, int score1 = 1000, int score2 = 1000);
	virtual bool init();
	CREATE_FUNC(MultiPauseScene);

private:
	void continueCallback(Ref *pSender);
	//void restartCallback(Ref *pSender);


};
#endif