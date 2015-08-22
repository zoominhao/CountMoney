#include "Player.h"


Player::Player() :m_totalMoneyNumLabel(NULL), m_fakeWrongNumLabel(NULL), m_totalMoneyNum(0), m_fakeWrongNum(0)
{

}

Player::~Player()
{

}


void Player::createPlayer(int mode)
{
	
	addTotalMoneyLabel();
	displayTotalMoney();
	/*if (mode == 2)
		displayFakeWrong();*/
	m_totalMoneyNum = 0;
	m_fakeWrongNum = 0;
}

void Player::displayTotalMoney()
{
	m_totalMoneyNumLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 24);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_totalMoneyNumLabel->setPosition(origin.x + visibleSize.width * 0.67, origin.y + 20);
	m_totalMoneyNumLabel->setColor(Color3B(0.0, 0.0, 0.0));

	this->addChild(m_totalMoneyNumLabel, 1);
}

void Player::displayFakeWrong()
{
	m_fakeWrongNumLabel = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 24);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_fakeWrongNumLabel->setPosition(origin.x + visibleSize.width * 0.75, origin.y + 20);
	m_fakeWrongNumLabel->setColor(Color3B(255.0, 0.0, 0.0));

	this->addChild(m_fakeWrongNumLabel, 1);
}

void Player::addTotalMoney(int deta)
{
	m_totalMoneyNum += deta;
	if (m_totalMoneyNum < 0)
		m_totalMoneyNum = 0;
}

void Player::addTotalMoneyLabel()
{
	//add total money
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_money_total = Money::create();
	m_money_total->createSprite(Vec2(visibleSize.width / 2 + origin.x, origin.y), Real_100_T);
	this->addChild(m_money_total, 1);
}

void Player::changeTotalMoneyLabel()
{
	char totalMoneyStr[10];
	sprintf(totalMoneyStr, "%d", m_totalMoneyNum);
	m_totalMoneyNumLabel->setString(totalMoneyStr);
}

void Player::addFakeWrong(int deta)
{
	m_fakeWrongNum += deta;
}

void Player::changeFakeWrongLabel()
{
	char fakeWrongStr[10];
	sprintf(fakeWrongStr, "%d", m_fakeWrongNum);
	m_fakeWrongNumLabel->setString(fakeWrongStr);
}

void Player::addSingleMoneyLabel(bool isFake, const char* name, Vec2 pos)
{
	//add single money
	
	/*if (this->getChildByName(name))
		this->removeChildByName(name, true);*/
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	Vec2 curpos = Vec2(visibleSize.width / 2 + origin.x + pos.x, origin.y + pos.y);

	m_money_single = Money::create();
	if (isFake)
		m_money_single->createSprite(curpos, Fake_100_S);
	else
		m_money_single->createSprite(curpos, Real_100_S);
	this->addChild(m_money_single, 1, name);
}

void Player::addSingleMoneyMLabel(Money_Type moneyType, const char* name, Vec2 pos)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 curpos = Vec2(visibleSize.width + pos.x, visibleSize.height / 2 + origin.y + pos.y);

	m_money_single = Money::create();
	m_money_single->createSprite(curpos, moneyType);

	this->addChild(m_money_single, 1, name);
}


void Player::setSingleMoneyName(const char* name /*= ""*/)
{
	m_money_single->setName(name);
}

void Player::setTotalMoneySpritePos(float detax, float detay)
{
	m_money_total->setPos(detax, detay);
}

void Player::setSingleMoneySpritePos(float detax, float detay)
{
	m_money_single->setPos(detax, detay);
}

void Player::setTotalMoneyNumPos(float detax, float detay)
{
	m_totalMoneyNumLabel->setPosition(m_totalMoneyNumLabel->getPositionX() + detax, m_totalMoneyNumLabel->getPositionY() + detay);
}

void Player::setFakeWrongNumPos(float detax, float detay)
{
	m_fakeWrongNumLabel->setPosition(m_fakeWrongNumLabel->getPositionX() + detax, m_fakeWrongNumLabel->getPositionY() + detay);
}

void Player::setTotalMoneyNumRot(float detaRot)
{
	m_totalMoneyNumLabel->setRotation(detaRot);
}


