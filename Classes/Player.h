#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Money.h"
USING_NS_CC;

class Player : public Node
{
public:
	Player();
	~Player();

	void createPlayer(int mode);
	CREATE_FUNC(Player);

	void addTotalMoney(int deta);
	void changeTotalMoneyLabel();

	void addFakeWrong(int deta);
	void changeFakeWrongLabel();

	void addTotalMoneyLabel();
	void addSingleMoneyLabel(bool isFake, const char* name = "center", Vec2 pos = Vec2(0, 0));
	void addSingleMoneyMLabel(Money_Type moneyType, const char* name = "center", Vec2 pos = Vec2(0, 0));
	void setSingleMoneyName(const char* name = "");

	Money* MoneyTotal(){ return m_money_total; }
	Money* MoneySingle(){ return m_money_single; }
	Sprite* ScoreFrame(){ return m_scoreFrame; }

	int totalMoneyNum() { return m_totalMoneyNum; }
	int fakeWrongNum() { return m_fakeWrongNum; }

	void setTotalMoneySpritePos(float detax, float detay);
	void setSingleMoneySpritePos(float detax, float detay);
	void setScoreFramePos(float detax, float detay);
	void setScoreFontSize(float size){ m_totalMoneyNumLabel->setSystemFontSize(size); }
	
	void setTotalMoneyNumPos(float detax, float detay);
	void setFakeWrongNumPos(float detax, float detay);

	void setTotalMoneyNumRot(float detaRot);

	void setStageNum(int stageNum){ m_stageNum = stageNum; }
	int stageNum(){ return m_stageNum; }

	void reset();
	void setAllowed(bool flag){ m_allowed = flag; }
	bool allowed(){ return m_allowed; }

	void setWin(int oppScore);
	int Win();

private:
	void displayTotalMoney();
	void displayFakeWrong();
	void addScoreFrame();


private:
	int m_totalMoneyNum;
	int m_fakeWrongNum;

	Label* m_totalMoneyNumLabel;
	Label* m_fakeWrongNumLabel;

	Money*   m_money_total;
	Money*   m_money_single;

	Sprite*  m_scoreFrame;
	
	int    m_stageNum;
	bool   m_allowed;


	int   m_oppScore;
};

#endif // __PLAYER_H__