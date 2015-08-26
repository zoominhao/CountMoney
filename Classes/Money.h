#ifndef __MONEY_H__
#define __MONEY_H__

#include "cocos2d.h"
USING_NS_CC;

typedef enum{
	Real_100_T,
	Fake_100_T,
	Real_100_S,
	Fake_100_S,
	Tool_1_T,
	Tool_1_S,
	Tool_2_T,
	Tool_2_S,
	Tool_3_T,
	Tool_3_S,
	Tool_4_T,
	Tool_4_S,
	Tool_5_T,
	Tool_5_S,
	Real_5_S,
	Real_10_S,
	Real_20_S,
	Real_50_S
}Money_Type;

class Money: public Node
{
public:
	Money();
	~Money();
	void createSprite(Vec2 pos, Money_Type mtype);
	CREATE_FUNC(Money);

	void changeMoney(Money_Type mtype);

	bool isOnMoney(Vec2 point){ return m_MoneySprite->boundingBox().containsPoint(point); }

	void moneyFly(float detax, float detay, float detat);
	void moneyFakeFly(float detax, float detay, float detat);
	void moneyMove(float x, float y, float detat);

	void setPos(float detax, float detay);
	Sprite* MoneySprite(){ return m_MoneySprite; }
private:
	const char* getMoneyImg(Money_Type mtype);

private:
	Sprite* m_MoneySprite;
};

#endif // __MONEY_H__