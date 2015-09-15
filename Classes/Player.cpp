#include "Player.h"


Player::Player() :m_totalMoneyNumLabel(NULL), m_fakeWrongNumLabel(NULL), m_totalMoneyNum(0), m_fakeWrongNum(0)
{
	m_allowed = true;
}

Player::~Player()
{

}

void Player::reset()
{
	m_totalMoneyNum = 0;
	m_fakeWrongNum = 0;
	m_stageNum = 0;
	changeTotalMoneyLabel();
	this->removeChild(m_money_single);
}

void Player::createPlayer(int mode)
{
	
	addTotalMoneyLabel();
	displayTotalMoney();
	addScoreFrame();
	/*if (mode == 2)
		displayFakeWrong();*/
	m_totalMoneyNum = 0;
	m_fakeWrongNum = 0;
	m_stageNum = 0;
	m_oppScore = 0;
}

void Player::displayTotalMoney()
{
	m_totalMoneyNumLabel = Label::createWithTTF("score: 0", "fonts/DTLNobelT-Bold.otf", 40);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_totalMoneyNumLabel->setPosition(origin.x + visibleSize.width * 0.5, origin.y + 65);
	m_totalMoneyNumLabel->setColor(Color3B(106.0, 57.0, 6.0));

	this->addChild(m_totalMoneyNumLabel, 3);
}

void Player::displayFakeWrong()
{
	m_fakeWrongNumLabel = Label::createWithTTF("0", "fonts/DTLNobelT-Bold.otf", 24);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_fakeWrongNumLabel->setPosition(origin.x + visibleSize.width * 0.75, origin.y + 20);
	m_fakeWrongNumLabel->setColor(Color3B(255.0, 0.0, 0.0));

	this->addChild(m_fakeWrongNumLabel, 1);
}

void Player::addTotalMoney(int deta)
{
	if (!m_allowed)
		return;
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
	char totalMoneyStr[20];
	sprintf(totalMoneyStr, "score: %d", m_totalMoneyNum);
	m_totalMoneyNumLabel->setString(totalMoneyStr);
}

void Player::addFakeWrong(int deta)
{
	if (!m_allowed)
		return;
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
	if (!m_allowed)
		return;
	//add single money
	//if (this->getChildByName(rmname))
	//	this->removeChildByName(rmname, true);
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
	if (!m_allowed)
		return;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 curpos = Vec2(visibleSize.width / 2 + origin.x + pos.x, origin.y + pos.y);

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

void Player::addScoreFrame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_scoreFrame = Sprite::create("score.png");
	m_scoreFrame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 70));
	m_scoreFrame->setScale(0.67);
	this->addChild(m_scoreFrame, 2);
}

void Player::setScoreFramePos(float detax, float detay)
{
	m_scoreFrame->setPosition(m_scoreFrame->getPositionX() + detax, m_scoreFrame->getPositionY() + detay);
}

void Player::setWin(int oppScore)
{
	m_oppScore = oppScore;
}

int Player::Win()
{
	if (m_totalMoneyNum > m_oppScore)
	{
		return 1;
	}
	else if (m_totalMoneyNum < m_oppScore)
	{
		return  -1;
	}
	else
	{
		return  0;
	}
}
