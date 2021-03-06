#include "MultiScene.h"
#include "StartScene.h"
#include "AudioControl.h"
#include "MultiEndScene.h"
#include "MCManual.h"

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
	//int x = rand() % 30;
	//m_targetNum = x * 100 + 5000;
	/*if (MCManual::novice[3])
	{
		m_targetNum = 7000;
	}*/
	m_targetNum = 7000;

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
	addPlayerName2();



	//飘字
	m_flyWord[0][0] = FlyWord::create("flyword/+100.png", 0.5, Vec2(origin.x + visibleSize.width/2 - 100, origin.y + visibleSize.height / 2 - 300), 25);
	this->addChild(m_flyWord[0][0], 2);
	m_flyWord[0][1] = FlyWord::create("flyword/+200.png", 0.5, Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 - 300), 25);
	this->addChild(m_flyWord[0][1], 2);
	m_flyWord[0][2] = FlyWord::create("flyword/-300.png", 0.5, Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 - 300), 25);
	this->addChild(m_flyWord[0][2], 2);
	m_flyWord[0][3] = FlyWord::create("flyword/-500.png", 0.5, Vec2(origin.x + visibleSize.width / 2 - 100, origin.y + visibleSize.height / 2 - 300), 25);
	this->addChild(m_flyWord[0][3], 2);

	m_flyWord[1][0] = FlyWord::create("flyword/+100.png", 0.5, Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 + 300), -25);
	m_flyWord[1][0]->setRotation(180);
	this->addChild(m_flyWord[1][0], 2);
	m_flyWord[1][1] = FlyWord::create("flyword/+200.png", 0.5, Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 + 300), -25);
	m_flyWord[1][1]->setRotation(180);
	this->addChild(m_flyWord[1][1], 2);
	m_flyWord[1][2] = FlyWord::create("flyword/-300.png", 0.5, Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 + 300), -25);
	m_flyWord[1][2]->setRotation(180);
	this->addChild(m_flyWord[1][2], 2);
	m_flyWord[1][3] = FlyWord::create("flyword/-500.png", 0.5, Vec2(origin.x + visibleSize.width / 2 + 100, origin.y + visibleSize.height / 2 + 300), -25);
	this->addChild(m_flyWord[1][3], 2);

	//鼓励文字
	for (int i = 0; i < 6; ++i)
	{
		m_zoomingWord[0][i] = ZoomingWord::create(String::createWithFormat("encourage/encourage_%d.png", i + 1)->getCString(),
			Vec2(origin.x + visibleSize.width / 2 - 200, origin.y + visibleSize.height / 2 - 200));
		m_zoomingWord[0][i]->setScale(0.8);
		this->addChild(m_zoomingWord[0][i], 2);

		m_zoomingWord[1][i] = ZoomingWord::create(String::createWithFormat("encourage/encourage_%d.png", i + 1)->getCString(),
			Vec2(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height / 2 + 200));
		m_zoomingWord[1][i]->setScale(0.8);
		m_zoomingWord[1][i]->setRotation(180);
		this->addChild(m_zoomingWord[1][i], 2);
	}

	m_p1GoodCount = 0;
	m_p2GoodCount = 0;

	//播放背景音乐
	AudioControl::stopBGMusic();
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

	m_stageCount = 0;
	if (MCManual::novice[3])
	{
		manualAct1();
	}
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
}


void MultiScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
	if (MCManual::novice[3] && m_stageCount == 0)
		return;

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
			if (MCManual::novice[3])
			{
				return;
			}
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
				if (MCManual::novice[3] && m_stageCount > 6)
				{
					returnPos(location, 1);
					m_p1NeedRand = false;
				}
				else
				{
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
						if (MCManual::novice[3] && m_stageCount == 1)
						{
							m_stageCount = 0;
							manualAct2();

						}
						else if (MCManual::novice[3] && m_stageCount == 2)
						{
							m_stageCount = 0;
							manualAct3();
						}
						else if (MCManual::novice[3] && m_stageCount == 5)
						{
							m_stageCount = 0;
							manualAct4();
						}
						else
						{
							m_stageCount++;
						}
						m_p1NeedRand = true;
					}
				}
				break;
			case RIGHT:
				if (MCManual::novice[3] && m_stageCount < 6)
				{
					returnPos(location, 1);
					m_p1NeedRand = false;
				}
				else
				{
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

						if (MCManual::novice[3] && m_stageCount == 7)
						{
							m_stageCount = 0;
							manualAct5();
						}
						m_p1NeedRand = true;
					}
				}
				break;
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
	
	auto label = Label::createWithTTF(str, "fonts/DTLNobelT-Bold.otf", 35);
	auto sprite = Sprite::create("multi/multi_target.png");
	if (whichPlayer == 1)  
	{
		label->setPosition(origin.x + visibleSize.width / 2 - 120, origin.y + visibleSize.height / 2 - 40);
		sprite->setPosition(origin.x + visibleSize.width / 2 - 235, origin.y + visibleSize.height / 2 - 40);
	}
	else
	{
		label->setPosition(origin.x + visibleSize.width / 2 + 120, origin.y + visibleSize.height / 2 + 40);
		label->setRotation(180);
		sprite->setPosition(origin.x + visibleSize.width / 2 + 235, origin.y + visibleSize.height / 2 + 40);
		sprite->setRotation(180);
	}
	
	label->setColor(MCUtil::m_targetColor);

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


	m_p1Label1 = Label::createWithTTF(scoreStr1, "fonts/DTLNobelT-Bold.otf", 30);
	m_p1Label2 = Label::createWithTTF(scoreStr2, "fonts/DTLNobelT-Bold.otf", 30);
	m_p1Label1->setPosition(origin.x + visibleSize.width / 2 + 190, origin.y + visibleSize.height / 2 - 40);
	m_p1Label2->setPosition(origin.x + visibleSize.width / 2 + 280, origin.y + visibleSize.height / 2 - 40);

	m_p2Label1 = Label::createWithTTF(scoreStr1, "fonts/DTLNobelT-Bold.otf", 30);
	m_p2Label2 = Label::createWithTTF(scoreStr2, "fonts/DTLNobelT-Bold.otf", 30);
	m_p2Label1->setRotation(180);
	m_p2Label2->setRotation(180);
	m_p2Label1->setPosition(origin.x + visibleSize.width / 2 - 280, origin.y + visibleSize.height / 2 + 40);
	m_p2Label2->setPosition(origin.x + visibleSize.width / 2 - 190, origin.y + visibleSize.height / 2 + 40);

	m_p1Label1->setColor(Color3B(255, 255, 255));
	m_p1Label2->setColor(Color3B(255, 255, 255));
	m_p2Label1->setColor(Color3B(255, 255, 255));
	m_p2Label2->setColor(Color3B(255, 255, 255));

	Label *m_label3 = Label::createWithTTF(":", "fonts/DTLNobelT-Bold.otf", 40);
	m_label3->setPosition(origin.x + visibleSize.width / 2 + 235, origin.y + visibleSize.height / 2 - 40);
	Label *m_label4 = Label::createWithTTF(":", "fonts/DTLNobelT-Bold.otf", 40);
	m_label4->setPosition(origin.x + visibleSize.width / 2 - 235, origin.y + visibleSize.height / 2 + 40);
	m_label3->setColor(Color3B(255, 255, 255));
	m_label4->setColor(Color3B(255, 255, 255));


	this->addChild(m_p1Label1, 1);
	this->addChild(m_p1Label2, 1);
	this->addChild(m_p2Label1, 1);
	this->addChild(m_p2Label2, 1);
	this->addChild(m_label3, 2);
	this->addChild(m_label4, 2);

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
		auto scene = MultiEndScene::createScene("0");
		Director::sharedDirector()->pushScene(scene);
		this->unscheduleUpdate();
	}
	if (m_player1->totalMoneyNum() >= m_targetNum)
	{
		auto scene = MultiEndScene::createScene("1");
		Director::sharedDirector()->pushScene(scene);
		this->unscheduleUpdate();
	}

	if (m_player2->totalMoneyNum() >= m_targetNum)
	{
		auto scene = MultiEndScene::createScene("-1");
		Director::sharedDirector()->pushScene(scene);
		this->unscheduleUpdate();
	}
}

void MultiScene::returnCallback(cocos2d::Ref* pSender)
{
	AudioControl::stopBGMusic();
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(CCTransitionZoomFlipX::create(0.5, scene, TransitionScene::Orientation::LEFT_OVER));
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
	m_gateWay1->setScale(1);
	m_gateWay1->setRotation(0);
	m_gateWay1->setPosition(Vec2(origin.x + visibleSize.width - 90, origin.y + visibleSize.height / 2 - 350));
	this->addChild(m_gateWay1, 1);


	//gateway 2
	m_gateWay2 = Sprite::create("multi/gateway.png");
	m_gateWay2->setScale(1);
	m_gateWay2->setRotation(180);
	m_gateWay2->setPosition(Vec2(origin.x + 90, origin.y + visibleSize.height - 150));
	this->addChild(m_gateWay2, 1);
}

void MultiScene::addWallet()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//wallet 1
	m_wallet1 = Sprite::create("multi/person.png");
	m_wallet1->setScale(1);
	m_wallet1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));	
	this->addChild(m_wallet1, 0);

	Sprite *m_icon1 = Sprite::create("multi/person_icon.png");
	m_icon1->setPosition(Vec2(origin.x + visibleSize.width / 2 + 120, origin.y + visibleSize.height / 2 - 40));
	this->addChild(m_icon1, 1);

	Sprite *m_icon2 = Sprite::create("multi/duck_icon.png");
	m_icon2->setPosition(Vec2(origin.x + visibleSize.width / 2 + 350, origin.y + visibleSize.height / 2 - 40));
	this->addChild(m_icon2, 1);

	//wallet 2
	m_wallet2 = Sprite::create("multi/duck.png");
	m_wallet2->setScale(1);
	m_wallet2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 90));
	this->addChild(m_wallet2, 0);

	Sprite *m_icon3 = Sprite::create("multi/duck_icon.png");
	m_icon3->setPosition(Vec2(origin.x + visibleSize.width / 2 - 120, origin.y + visibleSize.height / 2 + 40));
	m_icon3->setRotation(180);
	this->addChild(m_icon3, 1);

	Sprite *m_icon4 = Sprite::create("multi/person_icon.png");
	m_icon4->setPosition(Vec2(origin.x + visibleSize.width / 2 - 350, origin.y + visibleSize.height / 2 + 40));
	m_icon4->setRotation(180);
	this->addChild(m_icon4, 1);
	
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
	if (MCManual::novice[3])
	{
		if (m_stageCount == 1)
		{
			x = m_pro[4];  // 财神
		}
		else if (m_stageCount < 5 && m_stageCount > 1)
		{
			x = 99;
		}
		else if (m_stageCount == 5)
		{
			x = m_pro[1];    // 换位
		}

	}

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
	m_pro[0] = 3;   //迷雾
	m_pro[1] = 5;    //换位
	m_pro[2] = 8;  //加重
	m_pro[3] = 19;  //穷神
	m_pro[4] = 21;   //双倍
	m_pro[5] = 23;   //免疫
	m_pro[6] = 23;   //假钱
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

	if (curType == Real_100_S || curType == Tool_5_S || curType == Tool_6_S || ((curType == Tool_2_S) && m_p1Status[1]))
	{
		if (whichPlayer == 1)
			m_p1GoodCount = 0;
		if (whichPlayer == 2)
			m_p2GoodCount = 0;
	}


	if (curType == Tool_1_S || curType == Tool_3_S || curType == Tool_4_S || ((curType == Tool_2_S) && !m_p1Status[1]))
	{
		if (whichPlayer == 1)
			m_p1GoodCount++;
		if (whichPlayer == 2)
			m_p2GoodCount++;

		encourageEffect(whichPlayer);
	}

	triggerFlash(whichPlayer);


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
			AudioControl::playTool1Effect();
			halfSmoke(whichPlayer % 2 + 1);
		}
		break;
	case Tool_2_S:
		if (!isInvincible)
		{
			AudioControl::playTool2Effect();
			changePos(whichPlayer % 2 + 1);
		}
		break;
	case Tool_3_S:
		if (!isInvincible)
		{
			AudioControl::playTool3Effect();
			increaseWeight(whichPlayer % 2 + 1);
		}
		break;
	case Tool_4_S:
		if (!isInvincible)
		{
			AudioControl::playTool4Effect();
			triggerPoor(whichPlayer % 2 + 1, PoorOtherNum);
		}
		break;
	case Tool_5_S:
		AudioControl::playTool5Effect();
		triggerRich(whichPlayer % 2 + 1);
		break;
	case Tool_6_S:
		AudioControl::playTool6Effect();
		triggerInvincible(whichPlayer % 2 + 1);
		break;

	default:
		break;
	}

}

void MultiScene::triggerFlash(int whichPlayer)
{
	if (whichPlayer == 1)
	{
		m_gateWay1->setTexture("multi/gateway_light.png");
		ActionInterval* delaytime = DelayTime::create(0.2f);
		CallFunc * funcall = CallFunc::create([&](){
			m_gateWay1->setTexture("multi/gateway.png");
		});
		FiniteTimeAction * seq = Sequence::create(delaytime, funcall, NULL);
		m_gateWay1->runAction(seq);
	}
	if (whichPlayer == 2)
	{
		m_gateWay2->setTexture("multi/gateway_light.png");
		ActionInterval* delaytime = DelayTime::create(0.2f);
		CallFunc * funcall = CallFunc::create([&](){
			m_gateWay2->setTexture("multi/gateway.png");
		});
		FiniteTimeAction * seq = Sequence::create(delaytime, funcall, NULL);
		m_gateWay2->runAction(seq);
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

	if (curType == Real_100_S || curType == Tool_5_S || curType == Tool_6_S || ((curType == Tool_2_S) && m_p1Status[1]))
	{
		if (whichPlayer == 1)
			m_p1GoodCount++;
		if (whichPlayer == 2)
			m_p2GoodCount++;

		encourageEffect(whichPlayer);
	}

	if (curType == Tool_1_S || curType == Tool_3_S || curType == Tool_4_S || ((curType == Tool_2_S) && !m_p1Status[1]))
	{
		if (whichPlayer == 1)
			m_p1GoodCount = 0;
		if (whichPlayer == 2)
			m_p2GoodCount = 0;
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
		{
			m_flyWord[whichPlayer - 1][1]->Flying();
			tmpPlayer->addTotalMoney(200);
		}
		else
		{
			m_flyWord[whichPlayer - 1][0]->Flying();
			tmpPlayer->addTotalMoney(100);
		}
			
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
			AudioControl::playTool1Effect();
			halfSmoke(whichPlayer);
		}
		break;
	case Tool_2_S:
		if (!isInvincible)
		{
			AudioControl::playTool2Effect();
			changePos(whichPlayer);
		}
		break;
	case Tool_3_S:
		if (!isInvincible)
		{
			AudioControl::playTool3Effect();
			increaseWeight(whichPlayer);
		}
		break;
	case Tool_4_S:
		if (!isInvincible)
		{
			AudioControl::playTool4Effect();
			triggerPoor(whichPlayer, PoorMeNum);
		}
		break;
	case Tool_5_S:
		AudioControl::playTool5Effect();
		triggerRich(whichPlayer);
		break;
	case Tool_6_S:
		AudioControl::playTool6Effect();
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

	//添加效果
	smokeEffect(whichHalf);

	if (whichHalf == 1 && !m_p1Status[0])
	{
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
	this->removeChildByName("tool1_1");
	m_p1Status[0] = false;
	m_occupiedP1[m_p1Occupied[0]] = false;
	m_p1Occupied[0] = -1;
}

void MultiScene::updateSmoke2(float time)
{
	this->removeChildByName("tool1_2");
	m_p2Status[0] = false;
	m_occupiedP2[m_p2Occupied[0]] = false;
	m_p2Occupied[0] = -1;
}

void MultiScene::changePos(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
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

		m_p1Status[1] = !m_p1Status[1];
		if (m_p1Status[1])
		{
			m_gateWay1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 110));
			m_wallet1->setPosition(Vec2(origin.x + visibleSize.width - 120, origin.y + visibleSize.height / 2 - 350));
		}
		else
		{
			m_gateWay1->setPosition(Vec2(origin.x + visibleSize.width - 90, origin.y + visibleSize.height / 2 - 350));
			m_wallet1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
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

		m_p2Status[1] = !m_p2Status[1];
		if (m_p2Status[1])
		{
			m_gateWay2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 120));
			m_gateWay2->setRotation(180);
			m_wallet2->setPosition(Vec2(origin.x + 120, origin.y + visibleSize.height - 150));
		}
		else
		{
			m_gateWay2->setPosition(Vec2(origin.x + 90, origin.y + visibleSize.height - 150));
			m_gateWay2->setRotation(180);
			m_wallet2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 90));
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
		Vec2 pos = Vec2(origin.x + visibleSize.width / 2 + 20, origin.y + sp->getContentSize().height / 2 - 10);
		sp->setPosition(pos);
		sp->setRotation(90);
		sp->setScale(0.8);
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
		Vec2 pos = Vec2(origin.x + visibleSize.width / 2 - 20, origin.y + visibleSize.height - sp->getContentSize().height / 2 + 10);
		sp->setPosition(pos);
		sp->setRotation(270);
		sp->setScale(0.8);
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

void MultiScene::triggerPoor(int whichHalf, int minusScore)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1 && !m_p1Status[3])
	{
		//添加效果
		poorAnimation(1);
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

		if (minusScore == -300)
			m_flyWord[0][2]->Flying();
		else if (minusScore == -500)
			m_flyWord[0][3]->Flying();
		
		m_player1->addTotalMoney(minusScore);
		m_player1->changeTotalMoneyLabel();
		changeCurScore(1);
		scheduleOnce(schedule_selector(MultiScene::updatePoor1), 1.0f);

	}
	if (whichHalf == 2 && !m_p2Status[3])
	{
		//添加效果
		poorAnimation(2);
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
		if (minusScore == -300)
			m_flyWord[1][2]->Flying();
		else if (minusScore == -500)
			m_flyWord[1][3]->Flying();

		m_player2->addTotalMoney(minusScore);
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
		//添加效果
		richAnimation(1);
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
		//添加效果
		richAnimation(2);
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
		//添加效果
		invincibleAnimation(1);
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

		scheduleOnce(schedule_selector(MultiScene::updateInvincible1), 4.0f);

	}
	if (whichHalf == 2 && !m_p2Status[5])
	{
		//添加效果
		invincibleAnimation(2);
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
		scheduleOnce(schedule_selector(MultiScene::updateInvincible2), 4.0f);
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

void MultiScene::addPlayerName()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto pname1 = Label::createWithTTF("player1", "fonts/DTLNobelT-Bold.otf", 40);
	auto pname2 = Label::createWithTTF("player2", "fonts/DTLNobelT-Bold.otf", 40);

	pname1->setPosition(origin.x + 60, origin.y + 40);
	pname2->setPosition(origin.x + visibleSize.width - 60, origin.y + visibleSize.height - 40);
	pname2->setRotation(180);

	pname1->setColor(Color3B(151.0, 36.0, 9.0));
	pname2->setColor(Color3B(39.0, 93.0, 139.0));


	this->addChild(pname1, 1);
	this->addChild(pname2, 1);
}

void MultiScene::addPlayerName2()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto pname1 = Sprite::create("multi/player1.png");
	pname1->setPosition(origin.x + 60, origin.y + 40);
	this->addChild(pname1, 1);

	auto pnameicon1 = Sprite::create("multi/person_icon.png");
	pnameicon1->setPosition(origin.x + 155, origin.y + 40);
	this->addChild(pnameicon1, 1);


	auto pname2 = Sprite::create("multi/player2.png");
	pname2->setPosition(origin.x + visibleSize.width - 60, origin.y + visibleSize.height - 40);
	this->addChild(pname2, 1);

	auto pnameicon2 = Sprite::create("multi/duck_icon.png");
	pnameicon2->setPosition(origin.x + visibleSize.width - 150, origin.y + visibleSize.height - 40);
	pnameicon2->setRotation(180);
	this->addChild(pnameicon2, 1);
}

void MultiScene::manualAct1()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");


	CCSprite* gesture = CCSprite::create("manual/gesture.png");
	gesture->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8);
	gesture->setScale(0.6);
	this->addChild(gesture, 5, "gesture");

	CCSprite* up_count = CCSprite::create("manual/up_count.png");
	up_count->setPosition(origin.x + visibleSize.width / 2 + 120, origin.y + visibleSize.height / 4);
	this->addChild(up_count, 5, "up_count");

	CCActionInterval * moveup = CCMoveBy::create(1.0f, ccp(0, 100));
	CCActionInterval * movedown = CCMoveBy::create(0.1f, ccp(0, -100));

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("gesture");
		this->removeChildByName("up_count");
		m_stageCount = 1;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveup, movedown, moveup, funcall, NULL);
	gesture->runAction(seq);
}

void MultiScene::manualAct2()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");

	auto multi_tip1 = Sprite::create("manual/multiTip1.png");
	multi_tip1->setPosition(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 250);
	this->addChild(multi_tip1, 5, "multitip1");

	CCSprite* gesture = CCSprite::create("manual/gesture.png");
	gesture->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8);
	gesture->setScale(0.6);
	this->addChild(gesture, 5, "gesture");

	CCActionInterval * moveup = CCMoveBy::create(1.0f, ccp(0, 100));
	CCActionInterval * movedown = CCMoveBy::create(0.1f, ccp(0, -100));

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("gesture");
		this->removeChildByName("multitip1");
		m_stageCount = 2;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveup, movedown, moveup, funcall, NULL);
	gesture->runAction(seq);
}

void MultiScene::manualAct3()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");

	auto multi_tip2 = Sprite::create("manual/multiTip2.png");
	multi_tip2->setPosition(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 150);
	this->addChild(multi_tip2, 5, "multitip2");

	CCSprite* tool_cm = CCSprite::create("multi/tool5.png");
	tool_cm->setPosition(origin.x + 80, origin.y + visibleSize.height / 2 - 250);
	this->addChild(tool_cm, 5, "tool_cm");

	CCActionInterval * scaleTo = CCScaleTo::create(3.0, 0.3, 0.3);
	
	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("tool_cm");
		this->removeChildByName("multitip2");
		m_stageCount = 3;
	});

	CCFiniteTimeAction * seq = CCSequence::create(scaleTo, funcall, NULL);
	tool_cm->runAction(seq);
}

void MultiScene::manualAct4()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");

	auto multi_tip3 = Sprite::create("manual/multiTip3.png");
	multi_tip3->setPosition(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 150);
	this->addChild(multi_tip3, 5, "multitip3");

	CCSprite* gesture = CCSprite::create("manual/gesture.png");
	gesture->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8);
	gesture->setScale(0.6);
	this->addChild(gesture, 5, "gesture");

	CCActionInterval* moveright = CCMoveBy::create(1.5f, ccp(130, 0));
	CCActionInterval* moveleft = CCMoveBy::create(0.2f, ccp(-130, 0));

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("gesture");
		this->removeChildByName("multitip3");
		m_stageCount = 7;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveright, moveleft, moveright, funcall, NULL);
	gesture->runAction(seq);
}

void MultiScene::manualAct5()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");

	auto multi_tip4 = Sprite::create("manual/multiTip4.png");
	multi_tip4->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 200);
	this->addChild(multi_tip4, 5, "multitip4");

	scheduleOnce(schedule_selector(MultiScene::updateManualAct1), 3.0f);
}

void MultiScene::updateManualAct1(float time)
{
	this->removeChildByName("multitip4");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	CCSprite* target_frame = CCSprite::create("manual/redFrame.png");
	target_frame->setPosition(origin.x + visibleSize.width / 2 - 180, origin.y + visibleSize.height / 2 - 40);
	target_frame->setScale(1.0, 0.4);
	this->addChild(target_frame, 5, "target_frame");

	auto multi_tip5 = Sprite::create("manual/multiTip5.png");
	multi_tip5->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 250);
	this->addChild(multi_tip5, 5, "multitip5");

	scheduleOnce(schedule_selector(MultiScene::updateManualAct2), 3.0f);
}

void MultiScene::updateManualAct2(float time)
{
	this->removeChildByName("multitip5");
	this->removeChildByName("target_frame");
	MCManual::writeUserProfile(MANUAL_MULTI, false);
	auto scene = MultiScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5f, scene, false));
}

void MultiScene::smokeEffect(int whichHalf)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (whichHalf == 1)
	{
		auto smokeLayer1 = Sprite::create("multi/cloud1.png");
		smokeLayer1->setPosition(origin.x + visibleSize.width / 2 + 5, origin.y + smokeLayer1->getContentSize().height / 2);
		this->addChild(smokeLayer1, 2, "smokeLayer1_1");

		auto smokeLayer2 = Sprite::create("multi/cloud2.png");
		smokeLayer2->setPosition(origin.x + visibleSize.width / 2 + 5, origin.y + smokeLayer1->getContentSize().height / 2);
		this->addChild(smokeLayer2, 2, "smokeLayer1_2");

		auto smokeLayer3 = Sprite::create("multi/cloud3.png");
		smokeLayer3->setPosition(origin.x + visibleSize.width / 2 + 5, origin.y + smokeLayer1->getContentSize().height / 2);
		this->addChild(smokeLayer3, 2, "smokeLayer1_3");

		CCActionInterval* moveleft = CCMoveBy::create(2.5f, ccp(-30, 0));
		CCActionInterval* fadeout = CCFadeOut::create(0.5f);

		CCCallFunc * funcall = CCCallFunc::create([&](){
			this->removeChildByName("smokeLayer1_1");
			this->removeChildByName("smokeLayer1_2");
			this->removeChildByName("smokeLayer1_3");
			m_stageCount = 7;
		});

		CCFiniteTimeAction * seq = CCSequence::create(moveleft, fadeout, funcall, NULL);

		smokeLayer2->runAction(seq);
	}
	if (whichHalf == 2)
	{
		auto smokeLayer1 = Sprite::create("multi/cloud1.png");
		smokeLayer1->setPosition(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - smokeLayer1->getContentSize().height / 2 - 1);
		smokeLayer1->setRotation(180);
		this->addChild(smokeLayer1, 2, "smokeLayer2_1");

		auto smokeLayer2 = Sprite::create("multi/cloud2.png");
		smokeLayer2->setPosition(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - smokeLayer1->getContentSize().height / 2 - 1);
		smokeLayer2->setRotation(180);
		this->addChild(smokeLayer2, 2, "smokeLayer2_2");

		auto smokeLayer3 = Sprite::create("multi/cloud3.png");
		smokeLayer3->setPosition(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - smokeLayer1->getContentSize().height / 2 - 1);
		smokeLayer3->setRotation(180);
		this->addChild(smokeLayer3, 2, "smokeLayer2_3");

		CCActionInterval* moveright = CCMoveBy::create(2.5f, ccp(30, 0));
		CCActionInterval* fadeout = CCFadeOut::create(0.5f);

		CCCallFunc * funcall = CCCallFunc::create([&](){
			this->removeChildByName("smokeLayer2_1");
			this->removeChildByName("smokeLayer2_2");
			this->removeChildByName("smokeLayer2_3");
			m_stageCount = 7;
		});

		CCFiniteTimeAction * seq = CCSequence::create(moveright, fadeout, funcall, NULL);

		smokeLayer2->runAction(seq);
	}

}

void MultiScene::poorAnimation(int whichPlayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto poorSp = Sprite::create();
	if (whichPlayer == 1)
	{
		poorSp->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 250));
		poorSp->setName("poorsp");
	}
	else
	{
		poorSp->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 250));
		poorSp->setRotation(180);
		poorSp->setName("poorsp");
	}
	this->addChild(poorSp, 1);


	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("multi/poor_animation.plist", "multi/poor_animation.png");
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 12; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("poor_%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(1);             
	animation->setDelayPerUnit(0.083f);

	Animate* act = Animate::create(animation);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("poorsp");
	});

	CCFiniteTimeAction * seq = CCSequence::create(act, funcall, NULL);
	poorSp->runAction(seq);
}

void MultiScene::invincibleAnimation(int whichPlayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto invincibleSp = Sprite::create();
	auto invincibleLightSp = Sprite::create("multi/engel_light.png");
	auto invincibTipSp = Sprite::create("multi/invic_tip.png");

	if (whichPlayer == 1)
	{
		invincibleSp->setPosition(Vec2(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height / 2 - 250));
		invincibleSp->setScale(0.8);
		invincibleLightSp->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 335));
		invincibleLightSp->setScale(0.73, 0.9);
		invincibTipSp->setPosition(Vec2(origin.x + visibleSize.width / 2 + 80, origin.y + visibleSize.height / 2 - 250));
		invincibTipSp->setScale(0.8);
	}
	else
	{
		invincibleSp->setPosition(Vec2(origin.x + visibleSize.width / 2 + 50, origin.y + visibleSize.height / 2 + 250));
		invincibleSp->setRotation(180);
		invincibleSp->setScale(0.8);
		invincibleLightSp->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 335));
		invincibleLightSp->setScale(0.73, 0.9);
		invincibleLightSp->setRotation(180);
		invincibTipSp->setPosition(Vec2(origin.x + visibleSize.width / 2 - 80, origin.y + visibleSize.height / 2 + 250));
		invincibTipSp->setRotation(180);
		invincibTipSp->setScale(0.8);
	}

	invincibleSp->setName("invinciblesp");
	invincibleLightSp->setName("invincibleLightsp");
	invincibTipSp->setName("invincibTipsp");

	this->addChild(invincibleSp, 1);
	this->addChild(invincibleLightSp, 1);
	this->addChild(invincibTipSp, 1);

	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("multi/invincible.plist", "multi/invincible.png");
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 2; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("invincible_%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(10);
	animation->setDelayPerUnit(0.2f);

	Animate* act = Animate::create(animation);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("invinciblesp");
		this->removeChildByName("invincibleLightsp");
		this->removeChildByName("invincibTipsp");
	});

	CCFiniteTimeAction * seq = CCSequence::create(act, funcall, NULL);
	invincibleSp->runAction(seq);
}

void MultiScene::richAnimation(int whichPlayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto richSp = Sprite::create();
	auto richNumSp = Sprite::create("multi/rich_rate.png");
	richSp->setScale(0.5);
	richNumSp->setScale(0.8);
	if (whichPlayer == 1)
	{
		richSp->setPosition(Vec2(origin.x + visibleSize.width / 2 - 30, origin.y + visibleSize.height / 2 - 250));
		richNumSp->setPosition(Vec2(origin.x + visibleSize.width / 2 + 30, origin.y + visibleSize.height / 2 - 250));
	}
	else
	{
		richSp->setPosition(Vec2(origin.x + visibleSize.width / 2 + 30, origin.y + visibleSize.height / 2 + 250));
		richNumSp->setPosition(Vec2(origin.x + visibleSize.width / 2 - 30, origin.y + visibleSize.height / 2 + 250));
		richSp->setRotation(180);
		richNumSp->setRotation(180);
	}

	richSp->setName("richsp");
	richNumSp->setName("richNumsp");

	this->addChild(richSp, 1);
	this->addChild(richNumSp, 1);


	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("multi/rich_animation.plist", "multi/rich_animation.png");
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 13; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("rich_%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(2);
	animation->setDelayPerUnit(0.154f);

	Animate* act = Animate::create(animation);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("richsp");
		this->removeChildByName("richNumsp");
	});

	CCFiniteTimeAction * seq = CCSequence::create(act, funcall, NULL);
	richSp->runAction(seq);
}

void MultiScene::encourageEffect(int whichPlayer)
{
	int goodCount = m_p1GoodCount;
	if (whichPlayer == 2)
		goodCount = m_p2GoodCount;

	if (goodCount == 4)
	{
		AudioControl::playEncourageEffect(COOL);
		m_zoomingWord[whichPlayer-1][0]->Zooming();
	}
	else if (goodCount == 7)
	{
		AudioControl::playEncourageEffect(NICE);
		m_zoomingWord[whichPlayer - 1][1]->Zooming();
	}
	else if (goodCount == 10)
	{
		AudioControl::playEncourageEffect(GREAT);
		m_zoomingWord[whichPlayer - 1][2]->Zooming();
	}
	else if (goodCount == 13)
	{
		AudioControl::playEncourageEffect(ACE);
		m_zoomingWord[whichPlayer - 1][3]->Zooming();
	}
	else if (goodCount == 16)
	{
		AudioControl::playEncourageEffect(EXCELLENT);
		m_zoomingWord[whichPlayer - 1][4]->Zooming();
	}
	else if (goodCount % 4 == 0 && goodCount > 16)
	{
		AudioControl::playEncourageEffect(AWESOME);
		m_zoomingWord[whichPlayer - 1][5]->Zooming();
	}

}
