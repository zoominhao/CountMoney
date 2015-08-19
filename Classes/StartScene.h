#ifndef _START_SCENE_H_
#define _START_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class StartScene : public Layer
{
public:
	static Scene* createScene(); 
	virtual bool init();
	CREATE_FUNC(StartScene);

	void enter_tellerScene(Ref* ref);
	void enter_loserScene(Ref* ref);
	void enter_multiScene(Ref* ref);

	void game_over(Ref* ref);
};
#endif