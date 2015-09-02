#include "MultiScene.h"
#include "StartScene.h"
#include "AudioControl.h"
#include "MultiEndScene.h"

#include <time.h>

USING_NS_CC;

Scene* MultiScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MultiScene::create();
	scene->addChild(layer);
	return scene;
}

bool MultiScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	addControlBtns(1);
	addControlBtns(2);

	//preload audio
	//AudioControl::preLoad();

	//add background image
	setBgImage();

	//target num
	srand((unsigned)time(NULL));
	int x = rand() % 30;
	m_targetNum = x * 100 + 5000;
	char targetNumStr[30];
	sprintf(targetNumStr, " %d", m_targetNum);
	addTargetNumLabel(1, targetNumStr);
	addTargetNumLabel(2, targetNumStr);

	

	//添加玩家，该场景为双人模式
	m_player1 = Player::create();
	m_player1->createPlayer(2);
	m_player1->MoneyTotal()->MoneySprite()->setScale(0.5);
	m_player1->setTotalMoneySpritePos(0, -130);    
	m_player1->setTotalMoneyNumPos(0,  -20);
	m_player1->setScoreFontSize(20);
	m_player1->ScoreFrame()->setPosition(visibleSize.width / 2, origin.y + 50);
	m_player1->ScoreFrame()->setScale(0.335);
	this->addChild(m_player1, 1);

	m_player2 = Player::create();
	m_player2->createPlayer(2);
	m_player2->MoneyTotal()->MoneySprite()->setScale(0.5);
	m_player2->MoneyTotal()->MoneySprite()->setRotation(270);
	m_player2->setTotalMoneySpritePos(0, visibleSize.width - 95);  
	m_player2->setTotalMoneyNumPos(0, visibleSize.height - 115);
	m_player2->setScoreFontSize(20);
	m_player2->setTotalMoneyNumRot(180);
	m_player2->ScoreFrame()->setPosition(visibleSize.width / 2, visibleSize.height - 50);
	m_player2->ScoreFrame()->setRotation(180);
	m_player2->ScoreFrame()->setScale(0.335);
	this->addChild(m_player2, 1);

	addWallet();
	addGateWay();

	addCurScore();

	//播放背景音乐
	AudioControl::playBgMusic(PK_OFFLINE);

	//注册多点触屏事件
	auto mutiTouchlistener = EventListenerTouchAllAtOnce::create();
	mutiTouchlistener->onTouchesBegan = CC_CALLBACK_2(MultiScene::onTouchesBegan, this);
	mutiTouchlistener->onTouchesMoved = CC_CALLBACK_2(MultiScene::onTouchesMoved, this);
	mutiTouchlistener->onTouchesEnded = CC_CALLBACK_2(MultiScene::onTouchesEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mutiTouchlistener, this);
	_eventDispatcher->setPriority(mutiTouchlistener, 1);

	//注册帧事件
	this->scheduleUpdate();


	//初始化
	m_p1NextMType = Real_100_S;
	m_p1CurMType = Real_100_S;
	m_p1NeedRand = true;
	_touchP1ID = -1;


	m_p2NextMType = Real_100_S;
	m_p2CurMType = Real_100_S;
	m_p2NeedRand = true;
	_touchP2ID = -1;

	initProArr();
	initStatus();

	return true;
}

void MultiScene::setBgImage()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//bg
	auto bgSprite = Sprite::create("multi/multi_bg.png");
	bgSprite->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bgSprite, 0);

	//line
	auto lineSprite = Sprite::create("multi/line.png");
	lineSprite->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(lineSprite, 0);

	//vs
	auto vsSprite = Sprite::create("multi/VS.png");
	vsSprite->setScale(1.5f);
	vsSprite->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(vsSprite, 0);

}


void MultiScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto location = touch->getLocation();
		if (_touchP1ID != -1 && _touchP2ID != -1)
			break;

		if (_touchP1ID == -1 && m_player1->MoneyTotal()->isOnMoney(location))
		{
			_sP1Pos = location;
			_curP1Pos = _sP1Pos;
			_touchP1ID = touch->getID();
			m_effect_id1 = AudioControl::playCountEffect();

			if (m_p1NeedRand)
			{
				m_player1->addSingleMoneyMLabel(m_p1CurMType, "center", Vec2(5, -110));
				m_player1->MoneySingle()->MoneySprite()->setScale(0.5);
				randNewSingleMoney(1);
			}

		}
		if (_touchP2ID == -1 && m_player2->MoneyTotal()->isOnMoney(location))
		{
			_sP2Pos = location;
			_curP2Pos = _sP2Pos;
			_touchP2ID = touch->getID();
			m_effect_id2 = AudioControl::playCountEffect();

			if (m_p2NeedRand)
			{
				m_player2->addSingleMoneyMLabel(m_p2CurMType, "center", Vec2(-5, 693));
				m_player2->MoneySingle()->MoneySprite()->setRotation(270);
				m_player2->MoneySingle()->MoneySprite()->setScale(0.5);
				randNewSingleMoney(2);
			}
		}

	}

}

void MultiScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
	if (_touchP1ID == -1 && _touchP2ID == -1)
		return;

	for (auto &item : touches)
	{
		auto touch = item;
		auto location = touch->getLocation();


		if (touch->getID() == _touchP1ID)          //right player
		{
			switch (MCUtil::direction(_sP1Pos, location))
			{
			case UP:
				if (m_p1Status[2] && std::abs(_sP1Pos.y - location.y) < WeightDis)  //加重状态
				{
					returnPos(location, 1);
					m_p1NeedRand = false;
				}
				else
				{
					if (m_p1Status[1])
					{
						giveOpponent(UP, location, 1);
					}
					else
					{
						giveMyself(UP, location, 1);
					}
					m_player1->removeChildByName("up");
					m_player1->MoneySingle()->setName("up");
					m_p1NeedRand = true;
				}
				break;
			case RIGHT:
				if (m_p1Status[2] && std::abs(_sP1Pos.x - location.x) < WeightDis)  //加重状态
				{
					returnPos(location, 1);
					m_p1NeedRand = false;
				}
				else
				{
					if (m_p1Status[1])
					{
						giveMyself(RIGHT, location, 1);
					}
					else
					{
						giveOpponent(RIGHT, location, 1);
					}
					m_player1->removeChildByName("right");
					m_player1->MoneySingle()->setName("right");
					m_p1NeedRand = true;
					break;
				}
			default:
				returnPos(location, 1);
				m_p1NeedRand = false;
				break;
			}

			_touchP1ID = -1;
			if (m_p1NeedRand)
				m_p1CurMType = m_p1NextMType;
			//AudioControl::stopEffectMusic(m_effect_id1);
			m_player1->changeTotalMoneyLabel();
			changeCurScore(1);
		}


		if (touch->getID() == _touchP2ID)     //left player
		{
			switch (MCUtil::direction(_sP2Pos, location))
			{
			case DOWN:
				if (m_p2Status[2] && std::abs(_sP2Pos.y - location.y) < WeightDis)  //加重状态
				{
					returnPos(location, 2);
					m_p2NeedRand = false;
				}
				else
				{
					if (m_p2Status[1])
					{
						giveOpponent(DOWN, location, 2);
					}
					else
					{
						giveMyself(DOWN, location, 2);
					}
					m_player2->removeChildByName("down");
					m_player2->MoneySingle()->setName("down");
					m_p2NeedRand = true;
				}
				
				break;

			case LEFT:
				if (m_p2Status[2] && std::abs(_sP2Pos.x - location.x) < WeightDis)  //加重状态
				{
					returnPos(location, 2);
					m_p2NeedRand = false;
				}
				else
				{
					if (m_p2Status[1])
					{
						giveMyself(LEFT, location, 2);
					}
					else
					{
						giveOpponent(LEFT, location, 2);
					}
					m_player2->removeChildByName("left");
					m_player2->MoneySingle()->setName("left");
					m_p2NeedRand = true;
				}
				
				break;

			default:
				returnPos(location, 2);
				m_p2NeedRand = false;
				break;
			}

			_touchP2ID = -1;
			if (m_p2NeedRand)
				m_p2CurMType = m_p2NextMType;
			//AudioControl::stopEffectMusic(m_effect_id2);
			m_player2->changeTotalMoneyLabel();
			changeCurScore(2);

			if (m_p2Status[5])
			{
				m_player2->MoneyTotal()->changeMoney(Real_100_T);
				m_p2CurMType = Real_100_S;
			}
		}
		//AudioControl::stopMCEffects();
	}
}

void MultiScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		auto location = touch->getLocation();
		if (touch->getID() == _touchP1ID)
		{
			if (location.y <= _sP1Pos.y)
				location.y = _sP1Pos.y;
			
			if (location.y >= 512)
				location.y = 512;
	
			m_player1->MoneySingle()->MoneySprite()->setPosition(m_player1->MoneySingle()->MoneySprite()->getPositionX(),
				m_player1->MoneySingle()->MoneySprite()->getPositionY() + (location.y - _curP1Pos.y)*0.5);
			_curP1Pos = location;
		}
		if (touch->getID() == _touchP2ID)
		{
			if (location.y >= _sP2Pos.y)
				location.y = _sP2Pos.y;
			if (location.y <= 512)
				location.y = 512;

			m_player2->MoneySingle()->MoneySprite()->setPosition(m_player2->MoneySingle()->MoneySprite()->getPositionX(),
				m_player2->MoneySingle()->MoneySprite()->getPositionY() + (location.y - _curP2Pos.y)*0.5);
			_curP2Pos = location;
		}
	}
}


void MultiScene::addTargetNumLabel(int whichPlayer, const char* str)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	auto label = Label::createWithTTF(str, "fonts/Marker Felt.ttf", 40);
	auto sprite = Sprite::create("multi/target.png");
	if (whichPlayer == 1)  
	{
		label->setPosition(origin.x + visibleSize.width / 2 + 250, origin.y + visibleSize.height / 2 - 75);
		sprite->setPosition(origin.x + visibleSize.width / 2 + 230, origin.y + visibleSize.height / 2 - 70);
	}
	else
	{
		label->setPosition(origin.x + visibleSize.width / 2 - 250, origin.y + visibleSize.height / 2 + 75);
		label->setRotation(180);
		sprite->setPosition(origin.x + visibleSize.width / 2 - 230, origin.y + visibleSize.height / 2 + 70);
		sprite->setRotation(180);
	}
	
	label->setColor(Color3B(0.0, 0.0, 0.0));

	this->addChild(sprite, 1);
	this->addChild(label, 1);
}

void MultiScene::addCurScore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	char scoreStr1[10], scoreStr2[10];
	sprintf(scoreStr1, "%d", m_player1->totalMoneyNum());
	sprintf(scoreStr2, "%d", m_player2->totalMoneyNum());
	

	m_p1Label1 = Label::createWithTTF(scoreStr1, "fonts/Marker Felt.ttf", 40);
	m_p1Label2 = Label::createWithTTF(scoreStr2, "fonts/Marker Felt.ttf", 30);
	m_p1Label1->setPosition(origin.x + visibleSize.width / 2 - 60, origin.y + visibleSize.height / 2 - 40);
	m_p1Label2->setPosition(origin.x + visibleSize.width / 2 + 60, origin.y + visibleSize.height / 2 - 40);
	
	m_p2Label1 = Label::createWithTTF(scoreStr1, "fonts/Marker Felt.ttf", 30);
	m_p2Label2 = Label::createWithTTF(scoreStr2, "fonts/Marker Felt.ttf", 40);
	m_p2Label1->setRotation(180);
	m_p2Label2->setRotation(180);
	m_p2Label1->setPosition(origin.x + visibleSize.width / 2 - 60, origin.y + visibleSize.height / 2 + 40);
	m_p2Label2->setPosition(origin.x + visibleSize.width / 2 + 60, origin.y + visibleSize.height / 2 + 40);

	m_p1Label1->setColor(Color3B(151.0, 36.0, 9.0));
	m_p1Label2->setColor(Color3B(39.0, 93.0, 139.0));
	m_p2Label1->setColor(Color3B(151.0, 36.0, 9.0));
	m_p2Label2->setColor(Color3B(39.0, 93.0, 139.0));
	

	this->addChild(m_p1Label1, 1);
	this->addChild(m_p1Label2, 1);
	this->addChild(m_p2Label1, 1);
	this->addChild(m_p2Label2, 1);
}

void MultiScene::changeCurScore(int whichPlayer)
{
	char scoreStr[10];
	if (whichPlayer == 1)
	{
		sprintf(scoreStr, "%d", m_player1->totalMoneyNum());
		m_p1Label1->setString(scoreStr);
		m_p2Label1->setString(scoreStr);
	}
	else
	{
		sprintf(scoreStr, "%d", m_player2->totalMoneyNum());
		m_p1Label2->setString(scoreStr);
		m_p2Label2->setString(scoreStr);
	}
}


void MultiScene::update(float dt)
{
	if (m_player1->totalMoneyNum() == m_targetNum && m_player2->totalMoneyNum() == m_targetNum)
	{
		auto scene = MultiEndScene::createScene("TIE");
		Director::sharedDirector()->pushScene(scene);
		this->unscheduleUpdate();
	}
	if (m_player1->totalMoneyNum() == m_targetNum)
	{
		auto scene = MultiEndScene::createScene("Player1 Win");
		Director::sharedDirector()->pushScene(scene);
		this->unscheduleUpdate();
	}

	if (m_player2->totalMoneyNum() == m_targetNum)
	{
		auto scene = MultiEndScene::createScene("Player2 Win");
		Director::sharedDirector()->pushScene(scene);
		this->unscheduleUpdate();
	}
}

void MultiScene::returnCallback(cocos2d::Ref* pSender)
{
	AudioControl::stopBGMusic();
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void MultiScene::pauseCallback(cocos2d::Ref* pSender)
{
	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height + 30);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = MultiPauseScene::createScene(renderTexture, m_player1->totalMoneyNum(), m_player2->totalMoneyNum());
	Director::sharedDirector()->pushScene(scene);

	AudioControl::stopBGMusic();*/
}

void MultiScene::addControlBtns(int whichPlayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto returnItem = MenuItemImage::create(
		"multi/icon_back.png",
		"multi/icon_back.png",
		CC_CALLBACK_1(MultiScene::returnCallback, this));


	/*auto pauseItem = MenuItemImage::create(
		"multi/icon_pause.png",
		"multi/icon_pause.png",
		CC_CALLBACK_1(MultiScene::pauseCallback, this));*/

	if (whichPlayer == 1)
	{
		returnItem->setPosition(origin.x + 60, origin.y + visibleSize.height / 2 - 60);
		//pauseItem->setPosition(origin.x + visibleSize.width - 60, origin.y + visibleSize.height / 2 - 60);
	}
	else
	{
		returnItem->setPosition(origin.x + visibleSize.width - 60, origin.y + visibleSize.height / 2 + 60);
		returnItem->setRotation(180);
		//pauseItem->setPosition(origin.x + 60, origin.y + visibleSize.height / 2 + 60);
	}
	

	// create menu, it's an autorelease object
	//auto menu = Menu::create(returnItem, pauseItem, NULL);
	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void MultiScene::addGateWay()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//gateway 1
	m_gateWay1 = Sprite::create("multi/gateway.png");
	m_gateWay1->setScale(0.7);
	m_gateWay1->setRotation(180);
	m_gateWay1->setPosition(Vec2(origin.x + visibleSize.width - 60, origin.y + visibleSize.height / 2 - 350));
	this->addChild(m_gateWay1, 1);
	

	//gateway 2
	m_gateWay2 = Sprite::create("multi/gateway.png");
	m_gateWay2->setScale(0.7);
	m_gateWay2->setPosition(Vec2(origin.x + 60, origin.y + visibleSize.height - 150));
	this->addChild(m_gateWay2, 1);
}

void MultiScene::addWallet()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//wallet 1
	m_wallet1 = Sprite::create("multi/wallet.png");
	m_wallet1->setRotation(90);
	m_wallet1->setScale(0.7);
	m_wallet1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 120));
	this->addChild(m_wallet1, 0);


	//wallet 2
	m_wallet2 = Sprite::create("multi/wallet.png");
	m_wallet2->setRotation(270);
	m_wallet2->setScale(0.7);
	m_wallet2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 120));
	this->addChild(m_wallet2, 0);
	
}

void MultiScene::randNewSingleMoney(int whichPlayer)
{
	Player*  tmpPlayer = m_player1;
	Money_Type nextMType;
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
	}
	
	int x = rand() % 100;
	if (x <= m_pro[0])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Tool_1_T);
		nextMType = Tool_1_S;
	}
	else if (x <= m_pro[1])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Tool_2_T);
		nextMType = Tool_2_S;
	}
	else if (x <= m_pro[2])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Tool_3_T);
		nextMType = Tool_3_S;
	}
	else if (x <= m_pro[3])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Tool_4_T);
		nextMType = Tool_4_S;
	}
	else if (x <= m_pro[4])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Tool_5_T);
		nextMType = Tool_5_S;
	}
	else if (x <= m_pro[5])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Tool_6_T);
		nextMType = Tool_6_S;
	}
	else if (x <= m_pro[6])
	{
		tmpPlayer->MoneyTotal()->changeMoney(Fake_100_T);
		nextMType = Fake_100_S;
	}
	else
	{
		tmpPlayer->MoneyTotal()->changeMoney(Real_100_T);
		nextMType = Real_100_S;
	}
	
	if (whichPlayer == 1)
	{
		m_p1NextMType = nextMType;
	}
	else
	{
		m_p2NextMType = nextMType;
	}
}

void MultiScene::initProArr()
{
	m_pro[0] = 2;   //迷雾
	m_pro[1] = 4;    //换位
	m_pro[2] = 6;  //加重
	m_pro[3] = 8;  //穷神
	m_pro[4] = 10;   //双倍
	m_pro[5] = 12;   //免疫
	m_pro[6] = 12;   //假钱
}

void MultiScene::giveOpponent(MCDirection direction, Vec2 location, int whichPlayer)
{
	Player* tmpPlayer = m_player1;
	Vec2 spos = _sP1Pos;
	Money_Type curType = m_p1CurMType;
	bool isInvincible = m_p2Status[5];
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
		spos = _sP2Pos;
		curType = m_p2CurMType;
		isInvincible = m_p1Status[5];
	}

	switch (direction)
	{
	case UP:
		//防止玩家1，也即向上划钱到界面另一半,其实玩家2是不会有up操作的
		if (location.y >= 512)
			location.y = 512;
		//飘钱效果
		tmpPlayer->MoneySingle()->moneyFakeFly(0.0, 250.0 - (location.y - spos.y)*0.5, 0.1);
		break;
	case DOWN:
		//防止玩家2，也即向上划钱到界面另一半,其实玩家1是不会有down操作的
		if (location.y <= 512)
			location.y = 512;
		tmpPlayer->MoneySingle()->moneyFakeFly(0.0, -250.0 - (location.y - spos.y)*0.5, 0.1);
		break;
	case LEFT:
		tmpPlayer->MoneySingle()->moneyHFakeFly(-270.0, 0.0, 0.1);
		break;
	case RIGHT:
		tmpPlayer->MoneySingle()->moneyHFakeFly(270.0, 0.0, 0.1);
		break;
	default:
		break;
	}

	switch (curType)
	{
	case Real_100_S:
		break;
	case Fake_100_S:
		break;
	case Tool_1_S:
		if (!isInvincible)
		{
			AudioControl::playBufferEffect();
			halfSmoke(whichPlayer % 2 + 1);
		}
		break;
	case Tool_2_S:
		if (!isInvincible)
		{
			AudioControl::playDebufferEffect();
			changePos(whichPlayer % 2 + 1);
		}
		break;
	case Tool_3_S:
		if (!isInvincible)
		{
			AudioControl::playBufferEffect();
			increaseWeight(whichPlayer % 2 + 1);
		}
		break;
	case Tool_4_S:
		if (!isInvincible)
		{
			AudioControl::playDebufferEffect();
			triggerPoor(whichPlayer % 2 + 1);
		}
		break;
	case Tool_5_S:
		AudioControl::playBufferEffect();
		triggerRich(whichPlayer % 2 + 1);
		break;
	case Tool_6_S:
		AudioControl::playBufferEffect();
		triggerInvincible(whichPlayer % 2 + 1);
		break;

	default:
		break;
	}

}


void MultiScene::throwTrashCan(MCDirection direction, Vec2 location, int whichPlayer)
{
	Player* tmpPlayer = m_player1;
	Vec2 spos = _sP1Pos;
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
		spos = _sP2Pos;
	}

	switch (direction)
	{
	case UP:
		//防止玩家1，也即向上划钱到界面另一半,其实玩家2是不会有up操作的
		if (location.y >= 512)
			location.y = 512;
		//飘钱效果
		tmpPlayer->MoneySingle()->moneyFakeFly(0.0, 200.0 - (location.y - spos.y)*0.5, 0.1);
		break;
	case DOWN:
		//防止玩家2，也即向上划钱到界面另一半,其实玩家1是不会有down操作的
		if (location.y <= 512)
			location.y = 512;
		tmpPlayer->MoneySingle()->moneyFakeFly(0.0, -200.0 - (location.y - spos.y)*0.5, 0.1);
		break;
	case LEFT:
		tmpPlayer->MoneySingle()->moneyFakeFly(-110.0, 0.0, 0.1);
		break;
	case RIGHT:
		tmpPlayer->MoneySingle()->moneyFakeFly(110.0, 0.0, 0.1);
		break;
	default:
		break;
	}

}

void MultiScene::giveMyself(MCDirection direction, Vec2 location, int whichPlayer)
{
	Player* tmpPlayer = m_player1;
	Vec2 spos = _sP1Pos;
	Money_Type curType = m_p1CurMType;
	bool isDouble = m_p1Status[4];
	bool isInvincible = m_p1Status[5];
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
		spos = _sP2Pos;
		curType = m_p2CurMType;
		isDouble = m_p2Status[4];
		isInvincible = m_p2Status[5];
	}

	switch (direction)
	{
	case UP:
		//防止玩家1，也即向上划钱到界面另一半,其实玩家2是不会有up操作的
		if (location.y >= 512)
			location.y = 512;
		//飘钱效果
		tmpPlayer->MoneySingle()->moneyFakeFly(0.0, 250.0 - (location.y - spos.y)*0.5, 0.1);
		break;
	case DOWN:
		//防止玩家2，也即向上划钱到界面另一半,其实玩家1是不会有down操作的
		if (location.y <= 512)
			location.y = 512;
		tmpPlayer->MoneySingle()->moneyFakeFly(0.0, -250.0 - (location.y - spos.y)*0.5, 0.1);
		break;
	case LEFT:
		tmpPlayer->MoneySingle()->moneyHFakeFly(-270.0, 0.0, 0.1);
		break;
	case RIGHT:
		tmpPlayer->MoneySingle()->moneyHFakeFly(270.0, 0.0, 0.1);
		break;
	default:
		break;
	}


	switch (curType)
	{
	case Real_100_S:
		if (isDouble)
			tmpPlayer->addTotalMoney(200);	
		else
			tmpPlayer->addTotalMoney(100);
		break;
	case Fake_100_S:
		if (!isInvincible)
		{
			tmpPlayer->addFakeWrong(1);
			tmpPlayer->addTotalMoney(-200);
		}
		break;
	case Tool_1_S:
		if (!isInvincible)
		{
			AudioControl::playBufferEffect();
			halfSmoke(whichPlayer);
		}
		break;
	case Tool_2_S:
		if (!isInvincible)
		{
			AudioControl::playDebufferEffect();
			changePos(whichPlayer);
		}
		break;
	case Tool_3_S:
		AudioControl::playBufferEffect();
		increaseWeight(whichPlayer); 
		break;
	case Tool_4_S:
		if (!isInvincible)
		{
			AudioControl::playDebufferEffect();
			triggerPoor(whichPlayer);
		}
		break;
	case Tool_5_S:
		AudioControl::playBufferEffect();
		triggerRich(whichPlayer);
		break;
	case Tool_6_S:
		AudioControl::playBufferEffect();
		triggerInvincible(whichPlayer);
		break;

	default:
		break;
	}
}

void MultiScene::returnPos(Vec2 location, int whichPlayer)
{
	if (whichPlayer == 1 && location.y - _sP1Pos.y > 0)
	{
		m_player1->MoneySingle()->MoneySprite()->setPositionY(m_player1->MoneySingle()->MoneySprite()->getPositionY() - (location.y - _sP1Pos.y)*0.5);
	}
	if (whichPlayer == 2 && location.y - _sP2Pos.y < 0)
	{
		m_player2->MoneySingle()->MoneySprite()->setPositionY(m_player2->MoneySingle()->MoneySprite()->getPositionY() - (location.y - _sP2Pos.y)*0.5);
	}
}

void MultiScene::halfSmoke(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1 && !m_p1Status[0])
	{
		//添加效果
		CCSprite * sp = CCSprite::create("multi/smoke_effect.png");
		Vec2 pos = Vec2(origin.x + visibleSize.width / 2 + 5, origin.y + sp->getContentSize().height / 2);
		sp->setPosition(pos);
		sp->setRotation(90);
		sp->setScale(0.5);		
		this->addChild(sp, 2, "smokeeffect1");

		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP1[i])
			{
				m_p1Occupied[0] = i;
				m_occupiedP1[i] = true;
				break;
			}
		}
		auto sprite1 = Sprite::create(m_status_file[0]);
		sprite1->setPosition(m_p1StatusPos[m_p1Occupied[0]]);
		sprite1->setScale(0.3);
		this->addChild(sprite1, 1, "tool1_1");


		m_p1Status[0] = true;
		scheduleOnce(schedule_selector(MultiScene::updateSmoke1), 3.0f);
		
	}
	if (whichHalf == 2 && !m_p2Status[0])
	{
		//添加效果
		CCSprite * sp = CCSprite::create("multi/smoke_effect.png");
		Vec2 pos = Vec2(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - sp->getContentSize().height / 2 - 1);
		sp->setPosition(pos);
		sp->setRotation(270);
		sp->setScale(0.5);
		this->addChild(sp, 2, "smokeeffect2");

		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP2[i])
			{ 
				m_p2Occupied[0] = i;
				m_occupiedP2[i] = true;
				break;
			}
		}
		auto sprite2 = Sprite::create(m_status_file[0]);
		sprite2->setPosition(m_p2StatusPos[m_p2Occupied[0]]);
		sprite2->setRotation(180);
		sprite2->setScale(0.3);
		this->addChild(sprite2, 1, "tool1_2");

		m_p2Status[0] = true;
		scheduleOnce(schedule_selector(MultiScene::updateSmoke2), 3.0f);
	}
}

void MultiScene::updateSmoke1(float time)
{
	this->removeChildByName("smokeeffect1");
	this->removeChildByName("tool1_1");
	m_p1Status[0] = false;
	m_occupiedP1[m_p1Occupied[0]] = false;
	m_p1Occupied[0] = -1;
}

void MultiScene::updateSmoke2(float time)
{
	this->removeChildByName("smokeeffect2");
	this->removeChildByName("tool1_2");
	m_p2Status[0] = false;
	m_occupiedP2[m_p2Occupied[0]] = false;
	m_p2Occupied[0] = -1;
}

void MultiScene::changePos(int whichHalf)
{

	if (whichHalf == 1)
	{
		if (m_p1Status[1])
		{
			//去除状态
			this->removeChildByName("tool2_1");
			m_occupiedP1[m_p1Occupied[1]] = false;
			m_p1Occupied[1] = -1;
		}
		else
		{
			//添加状态
			for (int i = 0; i < 6; i++)
			{
				if (!m_occupiedP1[i])
				{
					m_p1Occupied[1] = i;
					m_occupiedP1[i] = true;
					break;
				}
			}
			auto sprite1 = Sprite::create(m_status_file[1]);
			sprite1->setPosition(m_p1StatusPos[m_p1Occupied[1]]);
			sprite1->setScale(0.3);
			this->addChild(sprite1, 1, "tool2_1");
		}
		Vec2 gateWayPos = m_gateWay1->getPosition();
		Vec2 walletPos = m_wallet1->getPosition();
		m_gateWay1->setPosition(walletPos);
		m_wallet1->setPosition(gateWayPos);
		m_p1Status[1] = !m_p1Status[1];
		if (m_p1Status[1])
		{
			m_gateWay1->setRotation(90);
			m_wallet1->setRotation(180);
		}
		else
		{
			m_gateWay1->setRotation(180);
			m_wallet1->setRotation(90);
		}
		
		CCActionInterval * gateWayBlink = CCBlink::create(1, 2);
		CCActionInterval * walletBlink = CCBlink::create(1, 2);
		m_gateWay1->runAction(gateWayBlink);
		m_wallet1->runAction(walletBlink);
		
	}
	else
	{
		if (m_p2Status[1])
		{
			//去除状态
			this->removeChildByName("tool2_2");
			m_occupiedP2[m_p2Occupied[1]] = false;
			m_p2Occupied[1] = -1;
		}
		else
		{
			//添加状态
			for (int i = 0; i < 6; i++)
			{
				if (!m_occupiedP2[i])
				{
					m_p2Occupied[1] = i;
					m_occupiedP2[i] = true;
					break;
				}
			}
			auto sprite2 = Sprite::create(m_status_file[1]);
			sprite2->setPosition(m_p2StatusPos[m_p2Occupied[1]]);
			sprite2->setScale(0.3);
			this->addChild(sprite2, 1, "tool2_2");
		}

		Vec2 gateWayPos = m_gateWay2->getPosition();
		Vec2 walletPos = m_wallet2->getPosition();
		m_gateWay2->setPosition(walletPos);
		m_wallet2->setPosition(gateWayPos);
		m_p2Status[1] = !m_p2Status[1];
		if (m_p2Status[1])
		{
			m_gateWay2->setRotation(270);
			m_wallet2->setRotation(0);
		}
		else
		{
			m_gateWay2->setRotation(0);
			m_wallet2->setRotation(270);
		}

		CCActionInterval * gateWayBlink = CCBlink::create(1, 2);
		CCActionInterval * walletBlink = CCBlink::create(1, 2);
		m_gateWay2->runAction(gateWayBlink);
		m_wallet2->runAction(walletBlink);
	}
}

void MultiScene::increaseWeight(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1 && !m_p1Status[2])
	{
		//添加效果
		CCSprite * sp = CCSprite::create("multi/weight_effect.png");
		Vec2 pos = Vec2(origin.x + visibleSize.width / 2 + 5, origin.y + sp->getContentSize().height / 2);
		sp->setPosition(pos);
		sp->setRotation(90);
		sp->setScale(0.5);
		this->addChild(sp, 2, "weighteffect1");

		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP1[i])
			{
				m_p1Occupied[2] = i;
				m_occupiedP1[i] = true;
				break;
			}
		}
		auto sprite1 = Sprite::create(m_status_file[2]);
		sprite1->setPosition(m_p1StatusPos[m_p1Occupied[2]]);
		sprite1->setScale(0.3);
		this->addChild(sprite1, 1, "tool3_1");


		m_p1Status[2] = true;
		scheduleOnce(schedule_selector(MultiScene::updateWeight1), 3.0f);

	}
	if (whichHalf == 2 && !m_p2Status[2])
	{
		//添加效果
		CCSprite * sp = CCSprite::create("multi/weight_effect.png");
		Vec2 pos = Vec2(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - sp->getContentSize().height / 2 - 1);
		sp->setPosition(pos);
		sp->setRotation(270);
		sp->setScale(0.5);
		this->addChild(sp, 2, "weighteffect2");

		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP2[i])
			{
				m_p2Occupied[2] = i;
				m_occupiedP2[i] = true;
				break;
			}
		}
		auto sprite2 = Sprite::create(m_status_file[2]);
		sprite2->setPosition(m_p2StatusPos[m_p2Occupied[2]]);
		sprite2->setRotation(180);
		sprite2->setScale(0.3);
		this->addChild(sprite2, 1, "tool3_2");

		m_p2Status[2] = true;
		scheduleOnce(schedule_selector(MultiScene::updateWeight2), 3.0f);
	}
}

void MultiScene::updateWeight1(float time)
{
	this->removeChildByName("weighteffect1");
	this->removeChildByName("tool3_1");
	m_p1Status[2] = false;
	m_occupiedP1[m_p1Occupied[2]] = false;
	m_p1Occupied[2] = -1;
}

void MultiScene::updateWeight2(float time)
{
	this->removeChildByName("weighteffect2");
	this->removeChildByName("tool3_2");
	m_p2Status[2] = false;
	m_occupiedP2[m_p2Occupied[2]] = false;
	m_p2Occupied[2] = -1;
}

void MultiScene::triggerPoor(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1 && !m_p1Status[3])
	{
		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP1[i])
			{
				m_p1Occupied[3] = i;
				m_occupiedP1[i] = true;
				break;
			}
		}
		auto sprite1 = Sprite::create(m_status_file[3]);
		sprite1->setPosition(m_p1StatusPos[m_p1Occupied[3]]);
		sprite1->setScale(0.3);
		this->addChild(sprite1, 1, "tool4_1");
		m_p1Status[3] = true;

		m_player1->addTotalMoney(PoorNum);
		m_player1->changeTotalMoneyLabel();
		changeCurScore(1);
		scheduleOnce(schedule_selector(MultiScene::updatePoor1), 1.0f);

	}
	if (whichHalf == 2 && !m_p2Status[3])
	{
		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP2[i])
			{
				m_p2Occupied[3] = i;
				m_occupiedP2[i] = true;
				break;
			}
		}
		auto sprite2 = Sprite::create(m_status_file[3]);
		sprite2->setPosition(m_p2StatusPos[m_p2Occupied[3]]);
		sprite2->setRotation(180);
		sprite2->setScale(0.3);
		this->addChild(sprite2, 1, "tool4_2");

		m_p2Status[3] = true;
		m_player2->addTotalMoney(PoorNum);
		m_player2->changeTotalMoneyLabel();
		changeCurScore(2);
		scheduleOnce(schedule_selector(MultiScene::updatePoor2), 1.0f);
	}
}

void MultiScene::updatePoor1(float time)
{
	this->removeChildByName("tool4_1");
	m_p1Status[3] = false;
	m_occupiedP1[m_p1Occupied[3]] = false;
	m_p1Occupied[3] = -1;
}

void MultiScene::updatePoor2(float time)
{
	this->removeChildByName("tool4_2");
	m_p2Status[3] = false;
	m_occupiedP2[m_p2Occupied[3]] = false;
	m_p2Occupied[3] = -1;
}

void MultiScene::triggerRich(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1 && !m_p1Status[4])
	{
		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP1[i])
			{
				m_p1Occupied[4] = i;
				m_occupiedP1[i] = true;
				break;
			}
		}
		auto sprite1 = Sprite::create(m_status_file[4]);
		sprite1->setPosition(m_p1StatusPos[m_p1Occupied[4]]);
		sprite1->setScale(0.3);
		this->addChild(sprite1, 1, "tool5_1");
		m_p1Status[4] = true;

		scheduleOnce(schedule_selector(MultiScene::updateRich1), 4.0f);

	}
	if (whichHalf == 2 && !m_p2Status[4])
	{
		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP2[i])
			{
				m_p2Occupied[4] = i;
				m_occupiedP2[i] = true;
				break;
			}
		}
		auto sprite2 = Sprite::create(m_status_file[4]);
		sprite2->setPosition(m_p2StatusPos[m_p2Occupied[4]]);
		sprite2->setRotation(180);
		sprite2->setScale(0.3);
		this->addChild(sprite2, 1, "tool5_2");

		m_p2Status[4] = true;
		scheduleOnce(schedule_selector(MultiScene::updateRich2), 4.0f);
	}
}

void MultiScene::updateRich1(float time)
{
	this->removeChildByName("tool5_1");
	m_p1Status[4] = false;
	m_occupiedP1[m_p1Occupied[4]] = false;
	m_p1Occupied[4] = -1;
}

void MultiScene::updateRich2(float time)
{
	this->removeChildByName("tool5_2");
	m_p2Status[4] = false;
	m_occupiedP2[m_p2Occupied[4]] = false;
	m_p2Occupied[4] = -1;
}

void MultiScene::triggerInvincible(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1 && !m_p1Status[5])
	{
		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP1[i])
			{
				m_p1Occupied[5] = i;
				m_occupiedP1[i] = true;
				break;
			}
		}
		auto sprite1 = Sprite::create(m_status_file[5]);
		sprite1->setPosition(m_p1StatusPos[m_p1Occupied[5]]);
		sprite1->setScale(0.3);
		this->addChild(sprite1, 1, "tool6_1");
		m_p1Status[5] = true;

		scheduleOnce(schedule_selector(MultiScene::updateInvincible1), 6.0f);

	}
	if (whichHalf == 2 && !m_p2Status[5])
	{
		//添加状态
		for (int i = 0; i < 6; i++)
		{
			if (!m_occupiedP2[i])
			{
				m_p2Occupied[5] = i;
				m_occupiedP2[i] = true;
				break;
			}
		}
		auto sprite2 = Sprite::create(m_status_file[5]);
		sprite2->setPosition(m_p2StatusPos[m_p2Occupied[5]]);
		sprite2->setRotation(180);
		sprite2->setScale(0.3);
		this->addChild(sprite2, 1, "tool6_2");

		m_p2Status[5] = true;
		scheduleOnce(schedule_selector(MultiScene::updateInvincible2), 6.0f);
	}
}

void MultiScene::updateInvincible1(float time)
{
	this->removeChildByName("tool6_1");
	m_p1Status[5] = false;
	m_occupiedP1[m_p1Occupied[5]] = false;
	m_p1Occupied[5] = -1;
}

void MultiScene::updateInvincible2(float time)
{
	this->removeChildByName("tool6_2");
	m_p2Status[5] = false;
	m_occupiedP2[m_p2Occupied[5]] = false;
	m_p2Occupied[5] = -1;
}

void MultiScene::initStatus()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	for (int i = 0; i < 6; ++i)
	{
		m_p1Status[i] = false;
		m_p2Status[i] = false;
		m_p1Occupied[i] = -1;
		m_p2Occupied[i] = -1;
		m_occupiedP1[i] = false;
		m_occupiedP2[i] = false;

		m_p1StatusPos[i] = Vec2(origin.x + 80 + (i % 2) * 70, origin.y + visibleSize.height / 2 - 250 - (i / 2) * 70);
		m_p2StatusPos[i] = Vec2(origin.x + +visibleSize.width - 80 - (i % 2) * 70, origin.y + visibleSize.height / 2 + 250 + (i / 2) * 70);
	}

	m_status_file[0] = "multi/tool1.png";
	m_status_file[1] = "multi/tool2.png";
	m_status_file[2] = "multi/tool3.png";
	m_status_file[3] = "multi/tool4.png";
	m_status_file[4] = "multi/tool5.png";
	m_status_file[5] = "multi/tool6.png";

	/*for (int i = 0; i < 6; ++i)
	{
		auto sprite1 = Sprite::create("multi/tool1.png");
		sprite1->setPosition(m_p1StatusPos[i]);
		sprite1->setScale(0.3);
		this->addChild(sprite1, 1);

		auto sprite2 = Sprite::create("multi/tool1.png");
		sprite2->setPosition(m_p2StatusPos[i]);
		sprite2->setRotation(180);
		sprite2->setScale(0.3);
		this->addChild(sprite2, 1);
	}*/
}




