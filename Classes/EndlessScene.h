#ifndef __ENDLESS_SCENE_H__
#define __ENDLESS_SCENE_H__

#include "cocos2d.h"
#include "CMTimer.h"
#include "Player.h"

USING_NS_CC;

class EndlessScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(EndlessScene);

private:
	void setBgImage();

	//callback
	void returnCallback(Ref* pSender);
	void pauseCallback(Ref* pSender);

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);

	//add cat animation
	void addCatAnimation();

	void addTranshCan();

	void addTargetNumLabel();
	void addStageLabel();

	void randNewSingleMoney();
	void initProArr();

	//add timer frame
	void addTimerFrame();

	void playStageEffect(int level, int target, float timelimit);
	void removeStageEffect(float time);


	//manual
	void manualAct1();
	void manualAct2();
	void manualAct3();
	void manualAct4();
	void updateManualAct1(float time);
	void updateManualAct2(float time);

private:
	Vec2 _spos;
	Vec2 _curPos;

	bool m_count_flag;
	bool m_needRand;
	Money_Type m_moneyType;
	Money_Type m_nextMoneyType;

	bool m_isEmpty;

	int		m_stage;               //当前关数
	float	m_time_per_count;      //?

	//audio effect
	int m_effect_id;
	//计时器
	CMTimer* m_cmTimer;
	//玩家
	Player*  m_player;

	Sprite*  m_cat;

	//transh can
	Sprite*  m_transhCan;

	int m_targetNum;
	Label*  m_targetLabel;
	Label*  m_stageLabel;

	float m_pro[5][5];

	Sprite*  m_timerFrame;
	bool     m_enabled;

	//manual
	int      m_stageCount;
};

#endif // __ENDLESS_SCENE_H__
