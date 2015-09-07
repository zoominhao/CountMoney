#ifndef _MULTI_SCENE_H_
#define _MULTI_SCENE_H_

#include "cocos2d.h"
#include "Player.h"
#include "MCUtil.h"

#define WeightDis 250
#define PoorNum  -300

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

	void addCurScore();
	void changeCurScore(int whichPlayer);

	void addStatus();

	void addPlayerName();

	void returnCallback(Ref* pSender);
	void pauseCallback(Ref* pSender);


	void addGateWay();
	void addWallet();



	//添加道具
	void randNewSingleMoney(int whichPlayer);
	void initProArr();


	void giveOpponent(MCDirection direction, Vec2 location, int whichPlayer);
	void giveMyself(MCDirection direction, Vec2 location, int whichPlayer);
	void throwTrashCan(MCDirection direction, Vec2 location, int whichPlayer);
	void returnPos(Vec2 location, int whichPlayer);

	//scene effect
	void halfSmoke(int whichHalf);   //迷雾
	void updateSmoke1(float time);
	void updateSmoke2(float time);

	void changePos(int whichHalf);    //换位


	void increaseWeight(int whichHalf);    //加重
	void updateWeight1(float time);
	void updateWeight2(float time);
	
	void triggerPoor(int whichHalf);   //穷神
	void updatePoor1(float time);
	void updatePoor2(float time);

	void triggerRich(int whichHalf);   //土豪卡
	void updateRich1(float time);
	void updateRich2(float time);

	void triggerInvincible(int whichHalf);  //无敌卡
	void updateInvincible1(float time);
	void updateInvincible2(float time);

	void initStatus();

private:
	Vec2 _sP1Pos;
	Vec2 _sP2Pos;

	Vec2 _curP1Pos;
	Vec2 _curP2Pos;

	int _touchP1ID;
	int _touchP2ID;

	int m_targetNum;

	Money_Type m_p1NextMType;
	Money_Type m_p2NextMType;
	Money_Type m_p1CurMType;
	Money_Type m_p2CurMType;

	bool m_p1NeedRand;
	bool m_p2NeedRand;


	//audio effect
	int m_effect_id1;
	int m_effect_id2;

	//玩家
	Player*  m_player1;
	Player*  m_player2;

	//
	Sprite*  m_gateWay1;
	Sprite*  m_gateWay2;
	Sprite*  m_wallet1;
	Sprite*  m_wallet2;

	//记分
	Label* m_p1Label1;
	Label* m_p1Label2;
	Label* m_p2Label1;
	Label* m_p2Label2;

	float m_pro[7];


	//状态栏
	bool m_p1Status[6];
	bool m_p2Status[6];
	int m_p1Occupied[6];
	int m_p2Occupied[6];
	bool m_occupiedP1[6];
	bool m_occupiedP2[6];


	Vec2 m_p1StatusPos[6];
	Vec2 m_p2StatusPos[6];

	std::string m_status_file[6];

};
#endif