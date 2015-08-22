#include "MultiScene.h"
#include "MultiPauseScene.h"
#include "StartScene.h"
#include "MCUtil.h"
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
	m_player1->setTotalMoneySpritePos(0, -100);
	m_player1->setTotalMoneyNumPos(-50, visibleSize.height / 2 - 70);
	this->addChild(m_player1, 1);

	m_player2 = Player::create();
	m_player2->createPlayer(2);
	m_player2->MoneyTotal()->MoneySprite()->setScale(0.5);
	m_player2->setTotalMoneySpritePos(0, visibleSize.width - 65);
	m_player2->setTotalMoneyNumPos(-200, visibleSize.height / 2 + 30);
	m_player2->setTotalMoneyNumRot(180);
	this->addChild(m_player2, 1);

	addTranshCan();
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
	m_p1NextFake = false;
	m_p1CurFake = false;
	m_p1NeedRand = true;
	m_isp1Empty = true;
	_touchP1ID = -1;

	m_p2NextFake = false;
	m_p2CurFake = false;
	m_p2NeedRand = true;
	m_isp2Empty = true;
	_touchP2ID = -1;

	return true;
}

void MultiScene::setBgImage()
{
	auto sprite = Sprite::create("background_multi.png");

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
				m_player1->addSingleMoneyLabel(m_p1CurFake, "center", Vec2(0, -100));
				m_player1->MoneySingle()->MoneySprite()->setScale(0.5);
				int x = rand() % 100;
				if (x > 70)
				{
					m_player1->MoneyTotal()->changeMoney(Fake_100_T);
					m_p1NextFake = true;
				}
				else
				{
					m_player1->MoneyTotal()->changeMoney(Real_100_T);
					m_p1NextFake = false;
				}
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
				m_player2->addSingleMoneyLabel(m_p2CurFake, "center", Vec2(0, 702));
				m_player2->MoneySingle()->MoneySprite()->setScale(0.5);
				int x = rand() % 100;
				if (x > 70)
				{
					m_player2->MoneyTotal()->changeMoney(Fake_100_T);
					m_p2NextFake = true;
				}
				else
				{
					m_player2->MoneyTotal()->changeMoney(Real_100_T);
					m_p2NextFake = false;
				}
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
				m_player1->MoneySingle()->MoneySprite()->setName("up");
				if (location.y >= 512)
					location.y = 512;
				m_player1->MoneySingle()->moneyFly(0.0, 200.0 - (location.y - _sP1Pos.y)*0.5, 0.1);
				if (m_p1CurFake)   //出现假钞
				{
					m_player1->addFakeWrong(1);
					m_player1->addTotalMoney(-200);
					//m_player->changeFakeWrongLabel();
				}
				else
				{
					m_player1->addTotalMoney(100);
				}
				m_p1NeedRand = true;
				break;

			case LEFT:
				m_player1->MoneySingle()->MoneySprite()->setName("left");
				m_player1->MoneySingle()->moneyFakeFly(-110.0, 0.0, 0.1);

				if (m_isp1Empty)
				{
					m_transhCan1->setTexture("f_trashCan.png");
					m_isp1Empty = false;
				}
				m_p1NeedRand = true;
				break;

			default:
				m_p1NeedRand = false;
				break;
			}
	
			_touchP1ID = -1;
			if (m_p1NeedRand)
				m_p1CurFake = m_p1NextFake;
			AudioControl::stopEffectMusic(m_effect_id1);
			m_player1->changeTotalMoneyLabel();
			
		}


		if (touch->getID() == _touchP2ID)     //left player
		{
			switch (MCUtil::direction(_sP2Pos, location))
			{
			case DOWN:
				m_player2->MoneySingle()->MoneySprite()->setName("down");
				if (location.y <= 512)
					location.y = 512;
				m_player2->MoneySingle()->moneyFly(0.0, -200.0 - (location.y - _sP2Pos.y)*0.5, 0.1);
				if (m_p2CurFake)   //出现假钞
				{
					m_player2->addFakeWrong(1);
					m_player2->addTotalMoney(-200);
					//m_player->changeFakeWrongLabel();
				}
				else
				{
					m_player2->addTotalMoney(100);
				}
				m_p2NeedRand = true;
				break;

			case RIGHT:
				m_player2->MoneySingle()->MoneySprite()->setName("right");
				m_player2->MoneySingle()->moneyFakeFly(110.0, 0.0, 0.1);

				if (m_isp2Empty)
				{
					m_transhCan2->setTexture("f_trashCan.png");
					m_isp2Empty = false;
				}
				m_p2NeedRand = true;
				break;

			default:
				m_p2NeedRand = false;
				break;
			}

			_touchP2ID = -1;
			if (m_p2NeedRand)
				m_p2CurFake = m_p2NextFake;
			AudioControl::stopEffectMusic(m_effect_id2);
			m_player2->changeTotalMoneyLabel();
			
		}

		AudioControl::stopAllEffcts();

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
		"return_icon1.png",
		"return_icon2.png",
		CC_CALLBACK_1(MultiScene::returnCallback, this));


	auto pauseItem = MenuItemImage::create(
		"pause_icon1.png",
		"pause_icon2.png",
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
