#include "MultiScene.h"
#include "MultiPauseScene.h"
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
	AudioControl::preLoad();

	//add background image
	setBgImage();

	//target num
	srand((unsigned)time(NULL));
	int x = rand() % 30;
	m_targetNum = x * 100 + 4000;
	char targetNumStr[30];
	sprintf(targetNumStr, "Target: %d", m_targetNum);
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

	addTranshCan();
	addCat();
	//播放背景音乐
	AudioControl::playBgMusic();

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
	m_isp1Empty = true;
	m_isP1Swapped = false;
	m_isP1Double = false;
	m_isP1Invincible = false;
	m_isP1Old = false;
	_touchP1ID = -1;


	m_p2NextMType = Real_100_S;
	m_p2CurMType = Real_100_S;
	m_p2NeedRand = true;
	m_isp2Empty = true;
	m_isP2Swapped = false;
	m_isP2Double = false;
	m_isP2Invincible = false;
	m_isP2Old = false;
	_touchP2ID = -1;

	initProArr();


	///////////////test////////////////////////
	WebClient::getinstance().start();
	//WebClient::getinstance().regidterFightCallback(handletest, this);
	///////////////test////////////////////////

	return true;
}

void MultiScene::setBgImage()
{
	auto sprite = Sprite::create("multi/multi_bg.png");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// position the sprite on the center of the screen
	sprite->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
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
			m_effect_id1 = AudioControl::playEffectMusic();

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
			m_effect_id2 = AudioControl::playEffectMusic();

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
				giveOpponent(UP, location, 1);
				m_player1->removeChildByName("up");
				m_player1->MoneySingle()->setName("up");
				m_p1NeedRand = true;
				break;

			case LEFT:
				if (m_isP1Swapped)
				{
					giveMyself(LEFT, location, 1);
				}
				else
				{
					throwTrashCan(LEFT, location, 1);
				}
				m_player1->removeChildByName("left");
				m_player1->MoneySingle()->setName("left");
				m_p1NeedRand = true;
				break;

			case RIGHT:
				if (m_isP1Swapped)
				{
					throwTrashCan(RIGHT, location, 1);
				}
				else
				{
					giveMyself(RIGHT, location, 1);
				}
				m_player1->removeChildByName("right");
				m_player1->MoneySingle()->setName("right");
				m_p1NeedRand = true;
				break;

			default:
				returnPos(location, 1);
				m_p1NeedRand = false;
				break;
			}
	
			_touchP1ID = -1;
			if (m_p1NeedRand)
				m_p1CurMType = m_p1NextMType;
			AudioControl::stopEffectMusic(m_effect_id1);
			m_player1->changeTotalMoneyLabel();

			if (m_isP1Invincible)
			{
				m_player1->MoneyTotal()->changeMoney(Real_100_T);
				m_p1CurMType = Real_100_S;
			}
		}


		if (touch->getID() == _touchP2ID)     //left player
		{
			switch (MCUtil::direction(_sP2Pos, location))
			{
			case DOWN:
				giveOpponent(DOWN, location, 2);
				m_player2->removeChildByName("down");
				m_player2->MoneySingle()->setName("down");
				m_p2NeedRand = true;
				break;

			case RIGHT:
				if (m_isP2Swapped)
				{
					giveMyself(RIGHT, location, 2);
				}
				else
				{
					throwTrashCan(RIGHT, location, 2);
				}
				m_player2->removeChildByName("right");
				m_player2->MoneySingle()->setName("right");
				m_p2NeedRand = true;
				break;

			case LEFT:
				if (m_isP2Swapped)
				{
					throwTrashCan(LEFT, location, 2);
				}
				else
				{
					giveMyself(LEFT, location, 2);
				}
				m_player2->removeChildByName("left");
				m_player2->MoneySingle()->setName("left");
				m_p2NeedRand = true;
				break;

			default:
				returnPos(location, 2);
				m_p2NeedRand = false;
				break;
			}
			/////test/////////////
			/*int event = EVENT_FIGHT;
			char dir = 0;
			char buf[20];
			memcpy(buf, &event, 4);
			memcpy(buf + 4, &dir, 1);
			WebClient::getinstance().send(std::string(buf));*/
		    /////test/////////////
			_touchP2ID = -1;
			if (m_p2NeedRand)
				m_p2CurMType = m_p2NextMType;
			AudioControl::stopEffectMusic(m_effect_id2);
			m_player2->changeTotalMoneyLabel();
			if (m_isP2Invincible)
			{
				m_player2->MoneyTotal()->changeMoney(Real_100_T);
				m_p2CurMType = Real_100_S;
			}
		}
		AudioControl::stopAllEffcts();
	}
}


/*
void MultiScene::handletest(int type, int direction, void *arg)
{
	MultiScene *pscene = (MultiScene *)arg;
	pscene->handle(type, direction);
}

void MultiScene::handle(int type, int direction)
{
	if (type == EVENT_FIGHT)
	{
		CCLOG("receive: %d, %d", type, direction);
	}
}*/

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
	if (whichPlayer == 1)  
	{
		label->setPosition(origin.x + visibleSize.width / 2 - 150, origin.y + visibleSize.height / 2 - 50);
	}
	else
	{
		label->setPosition(origin.x + visibleSize.width / 2 + 150, origin.y + visibleSize.height / 2 + 50);
		label->setRotation(180);
	}
	
	label->setColor(Color3B(0.0, 0.0, 0.0));

	this->addChild(label, 1);
}

void MultiScene::update(float dt)
{
	if (m_player2->totalMoneyNum() == m_targetNum && m_player2->totalMoneyNum() == m_targetNum)
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MultiScene::pauseCallback(cocos2d::Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height + 30);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = MultiPauseScene::createScene(renderTexture, m_player1->totalMoneyNum(), m_player2->totalMoneyNum());
	Director::sharedDirector()->pushScene(scene);
}

void MultiScene::addControlBtns(int whichPlayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto returnItem = MenuItemImage::create(
		"multi/icon_back.png",
		"multi/icon_back.png",
		CC_CALLBACK_1(MultiScene::returnCallback, this));


	auto pauseItem = MenuItemImage::create(
		"multi/icon_pause.png",
		"multi/icon_pause.png",
		CC_CALLBACK_1(MultiScene::pauseCallback, this));

	if (whichPlayer == 1)
	{
		returnItem->setPosition(origin.x + 60, origin.y + visibleSize.height / 2 - 60);
		pauseItem->setPosition(origin.x + visibleSize.width - 60, origin.y + visibleSize.height / 2 - 60);
	}
	else
	{
		returnItem->setPosition(origin.x + visibleSize.width - 60, origin.y + visibleSize.height / 2 + 60);
		returnItem->setRotation(180);
		pauseItem->setPosition(origin.x + 60, origin.y + visibleSize.height / 2 + 60);
	}
	

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, pauseItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void MultiScene::addTranshCan()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//transhcan 1
	m_transhCan1 = Sprite::create("trashCan.png");
	m_transhCan1->setScale(0.6);
	m_transhCan1->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height / 2 - 300));
	this->addChild(m_transhCan1, 1);

	//transhcan 2
	m_transhCan2 = Sprite::create("trashCan.png");
	m_transhCan2->setScale(0.6);
	m_transhCan2->setRotation(180);
	m_transhCan2->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height - 200));
	this->addChild(m_transhCan2, 1);
}


void MultiScene::addCat()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//cat 1
	m_cat1 = Sprite::create("cat_m.png");
	m_cat1->setScale(0.6);
	m_cat1->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2 - 300));
	this->addChild(m_cat1, 1);

	//cat 2
	m_cat2 = Sprite::create("cat_m.png");
	m_cat2->setScale(0.6);
	m_cat2->setRotation(180);
	m_cat2->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 200));
	this->addChild(m_cat2, 1);
}

void MultiScene::addTargetFrame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//cat 1
	m_cat1 = Sprite::create("cat_m.png");
	m_cat1->setScale(0.6);
	m_cat1->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2 - 300));
	this->addChild(m_cat1, 1);

	//cat 2
	m_cat2 = Sprite::create("cat_m.png");
	m_cat2->setScale(0.6);
	m_cat2->setRotation(180);
	m_cat2->setPosition(Vec2(origin.x + 100, origin.y + visibleSize.height - 200));
	this->addChild(m_cat2, 1);

/*
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_timerFrame = Sprite::create("multi/timer.png");
	m_timerFrame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75));
	this->addChild(m_timerFrame, 1);*/
}


void MultiScene::randNewSingleMoney(int whichPlayer)
{
	Player*  tmpPlayer = m_player1;
	Money_Type nextMType;
	bool isInInvincible = m_isP1Invincible;
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
		isInInvincible = m_isP2Invincible;
	}
	
	if (isInInvincible)
	{
		tmpPlayer->MoneyTotal()->changeMoney(Real_100_T);
		nextMType = Real_100_S;
	}
	else
	{
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
			tmpPlayer->MoneyTotal()->changeMoney(Fake_100_T);
			nextMType = Fake_100_S;
		}
		else
		{
			tmpPlayer->MoneyTotal()->changeMoney(Real_100_T);
			nextMType = Real_100_S;
		}
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
	m_pro[0] = 2;   //闪光弹
	m_pro[1] = 4;    //换位卡
	m_pro[2] = 6.5;  //破损卡
	m_pro[3] = 8.5;  //土豪卡
	m_pro[4] = 10;   //无敌卡
	m_pro[5] = 30;   //假钱
}

void MultiScene::giveOpponent(MCDirection direction, Vec2 location, int whichPlayer)
{
	Player* tmpPlayer = m_player1;
	Vec2 spos = _sP1Pos;
	Money_Type curType = m_p1CurMType;
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
		spos = _sP2Pos;
		curType = m_p2CurMType;
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

	switch (curType)
	{
	case Real_100_S:
		break;
	case Fake_100_S:
		break;
	case Tool_1_S:
		halfFlash(whichPlayer % 2 + 1);
		break;
	case Tool_2_S:
		changePos(whichPlayer % 2 + 1);
		break;
	case Tool_3_S:
		
		break;
	case Tool_4_S:
		triggerRich(whichPlayer % 2 + 1);
		break;
	case Tool_5_S:
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

	if (whichPlayer == 1 && m_isp1Empty)
	{
		m_transhCan1->setTexture("f_trashCan.png");
		m_isp1Empty = false;
	}
	if (whichPlayer == 2 && m_isp2Empty)
	{
		m_transhCan2->setTexture("f_trashCan.png");
		m_isp2Empty = false;
	}

}

void MultiScene::giveMyself(MCDirection direction, Vec2 location, int whichPlayer)
{
	Player* tmpPlayer = m_player1;
	Vec2 spos = _sP1Pos;
	Money_Type curType = m_p1CurMType;
	bool isDouble = m_isP1Double;
	if (whichPlayer == 2)
	{
		tmpPlayer = m_player2;
		spos = _sP2Pos;
		curType = m_p2CurMType;
		isDouble = m_isP2Double;
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

	switch (curType)
	{
	case Real_100_S:
		if (isDouble)
			tmpPlayer->addTotalMoney(200);	
		else
			tmpPlayer->addTotalMoney(100);
		break;
	case Fake_100_S:
		tmpPlayer->addFakeWrong(1);
		tmpPlayer->addTotalMoney(-200);
		break;
	case Tool_1_S:
		halfFlash(whichPlayer);
		break;
	case Tool_2_S:
		changePos(whichPlayer);
		break;
	case Tool_3_S:
		
		break;
	case Tool_4_S:
		triggerRich(whichPlayer);
		break;
	case Tool_5_S:
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

void MultiScene::halfFlash(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 pos;
	if (whichHalf == 1)
	{
		pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4);
	}
	else
	{
		pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 * 3);
	}

	CCSprite * sp = CCSprite::create("multi/blink.png");
	sp->setPosition(pos);
	this->addChild(sp, 2, "blink");
	//ActionInterval * fadeout = FadeOut::create(10.0f);
	ActionInterval * blink = CCBlink::create(2, 7);
	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("blink");
	});
	CCFiniteTimeAction * seq = CCSequence::create(blink, funcall, NULL);
	sp->runAction(seq);  
}

void MultiScene::changePos(int whichHalf)
{
	if (whichHalf == 1)
	{
		Vec2 transhPos = m_transhCan1->getPosition();
		Vec2 catPos = m_cat1->getPosition();
		m_transhCan1->setPosition(catPos);
		m_cat1->setPosition(transhPos);
		m_isP1Swapped = !m_isP1Swapped;

		CCActionInterval * transhBlink = CCBlink::create(1, 2);
		CCActionInterval * catBlink = CCBlink::create(1, 2);
		m_transhCan1->runAction(transhBlink);
		m_cat1->runAction(catBlink);
		
	}
	else
	{
		Vec2 transhPos = m_transhCan2->getPosition();
		Vec2 catPos = m_cat2->getPosition();
		m_transhCan2->setPosition(catPos);
		m_cat2->setPosition(transhPos);
		m_isP2Swapped = !m_isP2Swapped;

		CCActionInterval * transhBlink = CCBlink::create(1, 2);
		CCActionInterval * catBlink = CCBlink::create(1, 2);
		m_transhCan2->runAction(transhBlink);
		m_cat2->runAction(catBlink);


		/*CCActionInterval * delaytime = CCDelayTime::create(1);
		CCCallFunc * funcall = CCCallFunc::create([&](){
			m_transhCan2->setPosition(transhPos);
			m_cat2->setPosition(catPos);
			m_isP2Swapped = false;
		});
		CCFiniteTimeAction * seq = CCSequence::create(delaytime, funcall, NULL);
		m_transhCan2->runAction(seq);*/
	}
}

void MultiScene::triggerRich(int whichHalf)
{
	if (whichHalf == 1)
	{
		m_isP1Double = true;
		scheduleOnce(schedule_selector(MultiScene::updateRich1), 3.0f);
	}
	else
	{
		m_isP2Double = true;
		scheduleOnce(schedule_selector(MultiScene::updateRich2), 3.0f);
	}
}

void MultiScene::updateRich1(float time)
{
	m_isP1Double = false;
}

void MultiScene::updateRich2(float time)
{
	m_isP2Double = false;
}

void MultiScene::triggerInvincible(int whichHalf)
{
	if (whichHalf == 1)
	{
		m_isP1Invincible = true;
		scheduleOnce(schedule_selector(MultiScene::updateInvincible1), 3.0f);
	}
	else
	{
		m_isP2Invincible = true;
		scheduleOnce(schedule_selector(MultiScene::updateInvincible2), 3.0f);
	}
}

void MultiScene::updateInvincible1(float time)
{
	m_isP1Invincible = false;
	//CCLOG("P1 END!");
}

void MultiScene::updateInvincible2(float time)
{
	m_isP2Invincible = false;
	//CCLOG("P2 END!");
}




