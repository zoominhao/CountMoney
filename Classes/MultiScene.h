#ifndef _MULTI_SCENE_H_
#define _MULTI_SCENE_H_

#include "cocos2d.h"
#include "Player.h"
#include "MCUtil.h"
#include "WebClient.h"
#include "WebClient.h"

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
	void addCat();


	//add target frame
	void addTargetFrame();

	//��ӵ���
	void randNewSingleMoney(int whichPlayer);
	void initProArr();


	void giveOpponent(MCDirection direction, Vec2 location, int whichPlayer);
	void giveMyself(MCDirection direction, Vec2 location, int whichPlayer);
	void throwTrashCan(MCDirection direction, Vec2 location, int whichPlayer);
	void returnPos(Vec2 location, int whichPlayer);

	//scene effect
	void halfFlash(int whichHalf);   //���ⵯ

	void changePos(int whichHalf);    //��λ��

	void triggerRich(int whichHalf);   //������

	void triggerInvincible(int whichHalf);  //�޵п�

	void updateRich1(float time);  
	void updateRich2(float time);
	void updateInvincible1(float time);
	void updateInvincible2(float time);


	//test////////////////////////////
	//static void handletest(int type, int direction, void *arg);
	//void handle(int type, int direction);

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

	bool m_isp1Empty;
	bool m_isp2Empty;

	//audio effect
	int m_effect_id1;
	int m_effect_id2;

	//���
	Player*  m_player1;
	Player*  m_player2;

	//transh can
	Sprite*  m_transhCan1;
	Sprite*  m_transhCan2;
	Sprite*  m_cat1;
	Sprite*  m_cat2;


	float m_pro[6];

	//�Ƿ񽻻�λ��
	bool m_isP1Swapped;
	bool m_isP2Swapped;

	//�Ƿ�����
	bool m_isP1Old;
	bool m_isP2Old;

	//�Ƿ��������
	bool m_isP1Double;
	bool m_isP2Double;

	//�Ƿ�����޵�״̬
	bool m_isP1Invincible;
	bool m_isP2Invincible;

};
#endif