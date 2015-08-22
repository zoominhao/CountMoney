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

	void update(float dt);

private:
	void setBgImage();

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);

	//callback
	void addControlBtns(int whichPlayer);
	void addTargetNumLabel(int whichPlayer, const char* str);

	void returnCallback(Ref* pSender);
	void pauseCallback(Ref* pSender);


	void addTranshCan();

private:
	Vec2 _sP1Pos;
	Vec2 _sP2Pos;

	Vec2 _curP1Pos;
	Vec2 _curP2Pos;

	int _touchP1ID;
	int _touchP2ID;

	int m_targetNum;

	bool m_p1NextFake;
	bool m_p2NextFake;
	bool m_p1CurFake;
	bool m_p2CurFake;
	bool m_p1NeedRand;
	bool m_p2NeedRand;

	bool m_isp1Empty;
	bool m_isp2Empty;

	//audio effect
	int m_effect_id1;
	int m_effect_id2;

	//Íæ¼Ò
	Player*  m_player1;
	Player*  m_player2;

	//transh can
	Sprite*  m_transhCan1;
	Sprite*  m_transhCan2;

};
#endif