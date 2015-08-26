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

	int totalMoneyNum() { return m_totalMoneyNum; }
	int fakeWrongNum() { return m_fakeWrongNum; }

	void setTotalMoneySpritePos(float detax, float detay);
	void setSingleMoneySpritePos(float detax, float detay);
	
	void setTotalMoneyNumPos(float detax, float detay);
	void setFakeWrongNumPos(float detax, float detay);

	void setTotalMoneyNumRot(float detaRot);

	void setStageNum(int stageNum){ m_stageNum = stageNum; }
	int stageNum(){ return m_stageNum; }

private:
	void displayTotalMoney();
	void displayFakeWrong();

private:
	int m_totalMoneyNum;
	int m_fakeWrongNum;

	Label* m_totalMoneyNumLabel;
	Label* m_fakeWrongNumLabel;

	Money*   m_money_total;
	Money*   m_money_single;
	
	int m_stageNum;
};

#endif // __PLAYER_H__