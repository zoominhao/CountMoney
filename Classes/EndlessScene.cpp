#include "EndlessScene.h"
#include "SingleScene.h"
#include "MCUtil.h"
#include "AudioControl.h"
#include "EndLessEndScene.h"
#include "EndLessPauseScene.h"

#include<math.h>

USING_NS_CC;

Scene* EndlessScene::createScene()
{

	auto scene = Scene::create();
	auto layer = EndlessScene::create();
	scene->addChild(layer);
	return scene;
}


bool EndlessScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto returnItem = MenuItemImage::create(
		"endless/icon_back.png",
		"endless/icon_back.png",
		CC_CALLBACK_1(EndlessScene::returnCallback, this));

	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2));


	auto pauseItem = MenuItemImage::create(
		"endless/icon_pause.png",
		"endless/icon_pause.png",
		CC_CALLBACK_1(EndlessScene::pauseCallback, this));

	pauseItem->setPosition(Vec2(origin.x + visibleSize.width - pauseItem->getContentSize().width / 2,
		origin.y + visibleSize.height - pauseItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, pauseItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//preload audio
	//AudioControl::preLoad();

	//add background image
	setBgImage();
	//addCatAnimation();
	addTranshCan();
	addTimerFrame();

	//添加玩家，该场景为单人模式
	m_player = Player::create();
	m_player->createPlayer(4);
	m_player->setTotalMoneySpritePos(0, -20);
	this->addChild(m_player, 1);

	//添加计时器
	m_cmTimer = CMTimer::create();
	m_cmTimer->setTotalTime(5);
	m_cmTimer->createLabel(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 80), m_player, 3);
	this->addChild(m_cmTimer, 2);


	//播放背景音乐
	AudioControl::playBgMusic(ENDLESS);

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(EndlessScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(EndlessScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(EndlessScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	//初始化
	m_count_flag = false;
	m_moneyType = Real_100_S;
	m_nextMoneyType = Real_100_S;
	m_needRand = true;

	m_isEmpty = true;

	initProArr();

	m_stage = 0;
	m_time_per_count = 1.0f;

	srand((unsigned)time(NULL));
	addTargetNumLabel();
	addStageLabel();

	return true;
}




void EndlessScene::setBgImage()
{
	//auto sprite = LayerColor::create(ccc4(0xff, 0xff, 0xff, 0xff), 768, 1024);
	auto sprite = Sprite::create("endless/endless_bg.png");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	sprite->setPosition(ccp(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	this->addChild(sprite, 0);
}


bool EndlessScene::onTouchBegan(Touch* touch, Event* event)
{
	if (m_count_flag)
		return false;
	
	_spos = touch->getLocation();
	_curPos = _spos;
	if (m_player->MoneyTotal()->isOnMoney(_spos))
	{
		m_count_flag = true;
		m_cmTimer->startTimer();
		m_effect_id = AudioControl::playCountEffect();
		if (m_needRand)
		{
			m_player->addSingleMoneyMLabel(m_moneyType, "center", Vec2(12.0, 0.0));
			randNewSingleMoney();
		}
	}
	return true;
}

void EndlessScene::onTouchEnded(Touch* touch, Event* event)
{
	if (m_count_flag)
		m_count_flag = false;
	else
		return;


	auto pos = touch->getLocation();
	switch (MCUtil::direction(_spos, pos))
	{
	case UP:
		m_player->MoneySingle()->moneyFly(0.0, 400.0 - (pos.y - _spos.y)*0.5, 0.1);
		switch (m_moneyType)
		{
			case Real_100_S:
				m_player->addTotalMoney(100);
				break;
			case Real_5_S:
				m_player->addTotalMoney(5);
				break;
			case Real_10_S:
				m_player->addTotalMoney(10);
				break;
			case Real_20_S:
				m_player->addTotalMoney(20);
				break;
			case Real_50_S:
				m_player->addTotalMoney(50);
				break;
			default:
				break;
		}
		if (m_player->totalMoneyNum() == m_targetNum)
		{ 
			AudioControl::playEndlessEffect(true);
			m_stage++;
			m_targetNum = 5 * (25 + 10 * m_stage + rand() % 11 - 5);
			char targetNumStr[30];
			sprintf(targetNumStr, "Target: %d", m_targetNum);
			m_targetLabel->setString(targetNumStr);
			m_player->setStageNum(m_stage);
			char stageStr[30];
			sprintf(stageStr, "Stage: %d", m_stage + 1);
			m_stageLabel->setString(stageStr);

			m_player->addTotalMoney(-1 * m_targetNum);
			m_player->changeTotalMoneyLabel();


			if (m_stage < 10)
				m_cmTimer->setTotalTime(floor(m_time_per_count * (0.154 * m_stage + 4.6157)));
			else
				m_cmTimer->setTotalTime(floor(m_time_per_count * (0.154 * m_stage + 4.6157) - (m_stage - 9) * 0.1));
		}
		else if (m_player->totalMoneyNum() > m_targetNum)
		{
			AudioControl::stopBGMusic();
			char stageNumStr[100];
			sprintf(stageNumStr, "Fail In Stage %d", m_stage + 1);
			auto scene = EndlessEndScene::createScene(stageNumStr);
			Director::getInstance()->replaceScene(scene);
			AudioControl::playEndlessEffect(false);
/*
	#ifdef _WIN32  
			Sleep(100);
	#else  
			sleep(0.1);
	#endif  */
		}
		m_player->removeChildByName("up");
		m_player->MoneySingle()->setName("up");
		m_needRand = true;
		break;
	case RIGHT:
		//m_player->MoneySingle()->MoneySprite()->setName("right");
		m_player->MoneySingle()->moneyHFakeFly(220.0, 0.0, 0.1);
		if (m_isEmpty)
		{
			m_transhCan->setTexture("f_trashCan.png");
			m_isEmpty = false;
		}
		m_player->removeChildByName("right");
		m_player->MoneySingle()->setName("right");
		m_needRand = true;
		break;
	default:
		if (pos.y - _spos.y > 0)
		{
			m_player->MoneySingle()->MoneySprite()->setPositionY(m_player->MoneySingle()->MoneySprite()->getPositionY() - (pos.y - _spos.y)*0.5);
		}
		
		m_needRand = false;
		break;
	}

	if (m_needRand)
		m_moneyType = m_nextMoneyType;
	//AudioControl::stopEffectMusic(m_effect_id);
	m_player->changeTotalMoneyLabel();
}

void EndlessScene::onTouchMoved(Touch* touch, Event* event)
{
	if (!m_count_flag)
		return;
	auto pos = touch->getLocation();
	if (pos.y <= _spos.y)
		pos.y = _spos.y;
	m_player->MoneySingle()->MoneySprite()->setPosition(m_player->MoneySingle()->MoneySprite()->getPositionX(),
		m_player->MoneySingle()->MoneySprite()->getPositionY() + (pos.y - _curPos.y)*0.5);
	_curPos = pos;
}

void EndlessScene::returnCallback(Ref* pSender)
{
	AudioControl::stopBGMusic();
	auto scene = SingleScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void EndlessScene::pauseCallback(Ref* pSender)
{
	//遍历当前类的所有子节点信息，画入renderTexture中。  
	//这里类似截图。  
	Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height + 30);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = EndlessPauseScene::createScene(renderTexture, m_player->totalMoneyNum());
	Director::sharedDirector()->pushScene(scene);

	AudioControl::stopBGMusic();
}

void EndlessScene::addCatAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_cat = Sprite::create();
	m_cat->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));
	this->addChild(m_cat, 1);
	//将图片加载到精灵帧缓存池  
	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("endless/catanimation.plist", "endless/catanimation.png");
	//用一个列表保存所有的CCSpriteFrameCache  
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 9; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}
	//使用列表创建动画对象  
	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);            //表示无限循环播放  
	animation->setDelayPerUnit(0.1f);     //每两张图片的时间隔，图片数目越少，间隔最小就越小  

	//将动画包装成一个动作  
	Animate* act = Animate::create(animation);
	m_cat->runAction(act);
}

void EndlessScene::addTargetNumLabel()
{
	int x = rand() % 30;
	m_targetNum = 5 * (25 + rand() % 11 - 5);
	char targetNumStr[30];
	sprintf(targetNumStr, "Target: %d", m_targetNum);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_targetLabel = Label::createWithTTF(targetNumStr, "fonts/Marker Felt.ttf", 50);
	m_targetLabel->setPosition(origin.x + visibleSize.width / 2 - 200, origin.y + visibleSize.height - 200);
	m_targetLabel->setColor(Color3B(0.0, 0.0, 0.0));
	this->addChild(m_targetLabel, 1);
}

void EndlessScene::addStageLabel()
{
	char stageStr[30];
	sprintf(stageStr, "Current Stage: %d", m_stage + 1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_stageLabel = Label::createWithTTF(stageStr, "fonts/Marker Felt.ttf", 50);
	m_stageLabel->setPosition(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height - 200);
	m_stageLabel->setColor(Color3B(0.0, 0.0, 0.0));
	this->addChild(m_stageLabel, 1);
}

void EndlessScene::randNewSingleMoney()
{
	int rowIndex = 0;
	if (m_targetNum - m_player->totalMoneyNum() >= 100)
		rowIndex = 0;
	else if (m_targetNum - m_player->totalMoneyNum() >= 50)
		rowIndex = 1;
	else if (m_targetNum - m_player->totalMoneyNum() >= 20)
		rowIndex = 2;
	else if (m_targetNum - m_player->totalMoneyNum() >= 10)
		rowIndex = 3;
	else
		rowIndex = 4;
	int x = rand() % 100;
	if (x <= m_pro[rowIndex][0])
	{
		m_player->MoneyTotal()->changeMoney(Real_100_T);
		m_nextMoneyType = Real_100_S;
	}
	else if (x <= m_pro[rowIndex][1])
	{
		m_player->MoneyTotal()->changeMoney(Real_50_T);
		m_nextMoneyType = Real_50_S;
	}
	else if (x <= m_pro[rowIndex][2])
	{
		m_player->MoneyTotal()->changeMoney(Real_20_T);
		m_nextMoneyType = Real_20_S;
	}
	else if (x <= m_pro[rowIndex][3])
	{
		m_player->MoneyTotal()->changeMoney(Real_10_T);
		m_nextMoneyType = Real_10_S;
	}
	else
	{
		m_player->MoneyTotal()->changeMoney(Real_5_T);
		m_nextMoneyType = Real_5_S;
	}
}

void EndlessScene::initProArr()
{
	float tmpArr[5][5] = { { 80, 85, 90, 95, 100 }, { 30, 70, 80, 90, 100 }, { 20, 40, 90, 95, 100 },
	{ 15, 30, 45, 95, 100 }, { 12.5, 25, 37.5, 50, 100 } };

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			m_pro[i][j] = tmpArr[i][j];
		}
	}
}

void EndlessScene::addTranshCan()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_transhCan = Sprite::create("trashCan.png");
	m_transhCan->setScale(0.6);
	m_transhCan->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2 - 300));
	this->addChild(m_transhCan, 1);
}

void EndlessScene::addTimerFrame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_timerFrame = Sprite::create("endless/timer.png");
	m_timerFrame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75));
	this->addChild(m_timerFrame, 1);
}







