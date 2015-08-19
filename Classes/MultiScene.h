#ifndef _MULTI_SCENE_H_
#define _MULTI_SCENE_H_

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class MultiScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MultiScene);

	void menuCloseCallback(cocos2d::Ref* pSender);
	void update(float dt);

private:
	void setBgImage(float px, float py);
	void addTitle(float px, float py);
	void addTargetNumLabel();
	


	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);

private:
	Vec2 _sP1Pos;
	Vec2 _sP2Pos;

	int _touchP1ID;
	int _touchP2ID;

	int m_targetNum;

	bool m_p1count_flag;
	bool m_p2count_flag;

	bool m_p1IsFake;
	bool m_p2IsFake;



	//audio effect
	int m_effect_id1;
	int m_effect_id2;

	//Íæ¼Ò
	Player*  m_player1;
	Player*  m_player2;
};
#endif