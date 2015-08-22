#ifndef _TELLER_PAUSE_SCENE_H_
#define _TELLER_PAUSE_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class TellerPauseScene : public Layer
{
public:
	static Scene* createScene(RenderTexture* sqr, int score = 1000);
	virtual bool init();
	CREATE_FUNC(TellerPauseScene);

private:
	void continueCallback(Ref *pSender);
	void restartCallback(Ref *pSender);


};
#endif