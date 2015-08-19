#include "MultiScene.h"
#include "StartScene.h"
#include "MCUtil.h"
#include "AudioControl.h"

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

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MultiScene::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//preload audio
	AudioControl::preLoad();

	// add title
	addTitle(origin.x + visibleSize.width / 2, origin.y + visibleSize.height);

	//add background image
	setBgImage(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	//
	addTargetNumLabel();
	//添加玩家，该场景为双人模式
	m_player1 = Player::create();
	m_player1->createPlayer(2);
	this->addChild(m_player1, 1);

	m_player2 = Player::create();
	m_player2->createPlayer(2);
	//m_player2->setPositionX(m_player2->getPositionX() - 380);
	m_player2->setTotalMoneySpritePos(-380, 0);
	m_player2->setTotalMoneyNumPos(-200, 0);
	m_player2->setFakeWrongNumPos(-200, 0);
	this->addChild(m_player2, 1);


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
	m_p1count_flag = false;
	m_p1IsFake = false;
	_touchP1ID = -1;

	m_p2count_flag = false;
	m_p2IsFake = false;
	_touchP2ID = -1;

	srand((unsigned)time(NULL));

	return true;
}



void MultiScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	//Director::getInstance()->end();
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MultiScene::setBgImage(float px, float py)
{
	// add "Main Scene" splash screen"
	auto sprite = Sprite::create("bg_pic.jpg");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(px, py));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
}

void MultiScene::addTitle(float px, float py)
{
	// add a label shows "Count Money"
	// create and initialize a label
	auto label = Label::createWithTTF("Count Money", "fonts/Marker Felt.ttf", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(px, py - label->getContentSize().height));
	label->setColor(Color3B(0.0, 0.0, 0.0));
	// add the label as a child to this layer
	this->addChild(label, 1);
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
			_touchP1ID = touch->getID();
			m_effect_id1 = AudioControl::playEffectMusic();
		}
		if (_touchP2ID == -1 && m_player2->MoneyTotal()->isOnMoney(location))
		{
			_sP2Pos = location;
			_touchP2ID = touch->getID();
			m_effect_id2 = AudioControl::playEffectMusic();
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


		bool flag1 = true;
		if (touch->getID() == _touchP1ID)          //right player
		{
			switch (MCUtil::direction(_sP1Pos, location))
			{
			case LEFT:
				m_player1->addSingleMoneyLabel(m_p1IsFake, "left1");
				m_player1->MoneySingle()->moneyFly(-100.0, 0.0, 0.1);
				break;

			case UP:
				m_player1->addSingleMoneyLabel(m_p1IsFake, "up1");
				m_player1->MoneySingle()->moneyFly(0.0, 150.0, 0.1);
				if (m_p1IsFake)   //出现假钞
				{
					m_player1->addFakeWrong(1);
					m_player1->changeFakeWrongLabel();
				}
				else
				{
					m_player1->addTotalMoney(100);
				}
				break;

			default:
				m_player1->addSingleMoneyLabel(m_p1IsFake);
				m_player1->removeChild(m_player1->MoneySingle());
				flag1 = false;
				break;
			}
			AudioControl::stopEffectMusic(m_effect_id1);
			_touchP1ID = -1;
			if (flag1)
			{
				int x = rand() % 100;
				if (x > 50)
				{
					m_player1->MoneyTotal()->changeMoney(Fake_100_T);
					m_p1IsFake = true;
				}
				else
				{
					m_player1->MoneyTotal()->changeMoney(Real_100_T);
					m_p1IsFake = false;
				}
				m_player1->changeTotalMoneyLabel();
			}
		}

		bool flag2 = true;
		if (touch->getID() == _touchP2ID)     //left player
		{
			switch (MCUtil::direction(_sP2Pos, location))
			{
			case RIGHT:
				m_player2->addSingleMoneyLabel(m_p2IsFake, "right2", Vec2(-380, 0));
				m_player2->MoneySingle()->moneyFly(100.0, 0.0, 0.1);
				break;

			case UP:
				m_player2->addSingleMoneyLabel(m_p2IsFake, "up2", Vec2(-380, 0));
				m_player2->MoneySingle()->moneyFly(0.0, 150.0, 0.1);
				if (m_p2IsFake)   //出现假钞
				{
					m_player2->addFakeWrong(1);
					m_player2->changeFakeWrongLabel();
				}
				else
				{
					m_player2->addTotalMoney(100);
				}
				break;

			default:
				m_player2->addSingleMoneyLabel(m_p2IsFake);
				m_player2->removeChild(m_player2->MoneySingle());
				flag2 = false;
				break;
			}

			AudioControl::stopEffectMusic(m_effect_id2);
			_touchP2ID = -1;
			if (flag2)
			{
				int x = rand() % 100;
				if (x > 50)
				{
					m_player2->MoneyTotal()->changeMoney(Fake_100_T);
					m_p2IsFake = true;
				}
				else
				{
					m_player2->MoneyTotal()->changeMoney(Real_100_T);
					m_p2IsFake = false;
				}
				m_player2->changeTotalMoneyLabel();
			}
		}

		AudioControl::stopAllEffcts();

	}
}

void MultiScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{

}


void MultiScene::addTargetNumLabel()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//srand((unsigned)GetCurrentTime());
	//srand((unsigned)time(0));
	int x = rand() % 30;
	m_targetNum = x * 100 + 4000;
	//m_targetNum = 500;
	char targetNumStr[30];
	sprintf(targetNumStr, "Target: %d", m_targetNum);
	auto label = Label::createWithTTF(targetNumStr, "fonts/Marker Felt.ttf", 24);
	label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8);
	label->setColor(Color3B(0.0, 0.0, 0.0));

	this->addChild(label, 1);
}

void MultiScene::update(float dt)
{
	if (m_player2->totalMoneyNum() == m_targetNum && m_player2->totalMoneyNum() == m_targetNum)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto label = Label::createWithTTF("TIE", "fonts/Marker Felt.ttf", 24);
		label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2);
		label->setColor(Color3B(0.0, 0.0, 0.0));
		this->addChild(label, 1);
		this->unscheduleUpdate();
	}
	if (m_player1->totalMoneyNum() == m_targetNum)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto label = Label::createWithTTF("Player1 Win", "fonts/Marker Felt.ttf", 24);
		label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2);
		label->setColor(Color3B(0.0, 0.0, 0.0));
		this->addChild(label, 1);
		this->unscheduleUpdate();
	}

	if (m_player2->totalMoneyNum() == m_targetNum)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();
		auto label = Label::createWithTTF("Player2 Win", "fonts/Marker Felt.ttf", 24);
		label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2);
		label->setColor(Color3B(0.0, 0.0, 0.0));
		this->addChild(label, 1);
		this->unscheduleUpdate();
	}
}