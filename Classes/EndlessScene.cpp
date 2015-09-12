#include "EndlessScene.h"
#include "SingleScene.h"
#include "MCUtil.h"
#include "AudioControl.h"
#include "EndLessEndScene.h"
#include "EndLessPauseScene.h"

#include "MCManual.h"

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
	AudioControl::stopBGMusic();
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
	m_enabled = true;

	m_stageCount = 0;

	if (MCManual::novice[2])
	{ 
		manualAct1();
	}
	

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
	if (!m_enabled || (MCManual::novice[2] && m_stageCount == 0))
		return false;
	if (m_count_flag)
		return false;
	
	_spos = touch->getLocation();
	_curPos = _spos;
	if (m_player->MoneyTotal()->isOnMoney(_spos))
	{
		m_count_flag = true;
		if (!MCManual::novice[2])
		{
			m_cmTimer->startTimer();
		}
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
		if (MCManual::novice[2] && m_stageCount == 2)
		{
			if (pos.y - _spos.y > 0)
			{
				m_player->MoneySingle()->MoneySprite()->setPositionY(m_player->MoneySingle()->MoneySprite()->getPositionY() - (pos.y - _spos.y)*0.5);
			}
			m_needRand = false;
		}
		else
		{
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
				m_stage++;         //过关
				m_targetNum = 5 * (25 + 10 * m_stage + rand() % 11 - 5);

				float timeLimit;
				if (m_stage < 10)
					timeLimit = floor(m_time_per_count * (0.154 * m_stage + 4.6157));
				else
					timeLimit = floor(m_time_per_count * (0.154 * m_stage + 4.6157) - (m_stage - 9) * 0.1);
				playStageEffect(m_stage, m_targetNum, timeLimit);
			}
			else if (m_player->totalMoneyNum() > m_targetNum)
			{
				AudioControl::stopBGMusic();
				char stageNumStr[100];
				sprintf(stageNumStr, "Fail In Level %d", m_stage + 1);
				auto scene = EndlessEndScene::createScene(stageNumStr, m_stage + 1);
				Director::getInstance()->replaceScene(scene);
				AudioControl::playEndlessEffect(false);
			}
			m_player->removeChildByName("up");
			m_player->MoneySingle()->setName("up");

			if (MCManual::novice[2] && m_stageCount == 1)
			{
				m_stageCount = 0;
				manualAct2();
			}
			if (MCManual::novice[2] && m_stageCount == 3)
			{
				manualAct4();
			}

			m_needRand = true;
		}
		
		break;
	case RIGHT:
		if (MCManual::novice[2] && m_stageCount != 2)
		{
			if (pos.y - _spos.y > 0)
			{
				m_player->MoneySingle()->MoneySprite()->setPositionY(m_player->MoneySingle()->MoneySprite()->getPositionY() - (pos.y - _spos.y)*0.5);
			}
			m_needRand = false;
		}
		else
		{
			m_player->MoneySingle()->moneyHFakeFly(220.0, 0.0, 0.1);
			if (m_isEmpty)
			{
				m_transhCan->setTexture("f_trashCan.png");
				m_isEmpty = false;
			}
			m_player->removeChildByName("right");
			m_player->MoneySingle()->setName("right");

			if (MCManual::novice[2] && m_stageCount == 2)
			{
				m_stageCount = 0;
				manualAct3();
			}

			m_needRand = true;
		}
		
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
	if (MCManual::novice[2])
	{
		m_targetNum = 110;
	}
	char targetNumStr[30];
	sprintf(targetNumStr, "Target: %d", m_targetNum);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_targetLabel = Label::createWithTTF(targetNumStr, "fonts/Marker Felt.ttf", 50);
	m_targetLabel->setPosition(origin.x + visibleSize.width / 2 - 200, origin.y + visibleSize.height - 200);
	m_targetLabel->setColor(MCUtil::m_targetColor);
	this->addChild(m_targetLabel, 1);
}

void EndlessScene::addStageLabel()
{
	char stageStr[30];
	sprintf(stageStr, "Level: %d", m_stage + 1);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_stageLabel = Label::createWithTTF(stageStr, "fonts/Marker Felt.ttf", 50);
	m_stageLabel->setPosition(origin.x + visibleSize.width / 2 + 200, origin.y + visibleSize.height - 200);
	m_stageLabel->setColor(MCUtil::m_targetColor);
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

	if (MCManual::novice[2])
	{
		if (m_stageCount == 1)
		{
			x = m_pro[rowIndex][2];  // 20
		}
		else if (m_stageCount == 2)
		{
			x = m_pro[rowIndex][3];  // 10
		}
	}

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

void EndlessScene::playStageEffect(int level, int target, float timelimit)
{
	m_enabled = false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto levelBg = Sprite::create("endless/levelSlide.png");
	levelBg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));


	char levelStr[10];
	sprintf(levelStr, "%d", level);
	auto levelLabel = Label::createWithTTF(levelStr, "fonts/Marker Felt.ttf", 40);
	levelLabel->setPosition(origin.x + visibleSize.width / 2 - 40, origin.y + visibleSize.height / 2 - 50);
	levelLabel->setColor(Color3B(67.0, 72.0, 88.0));
	levelBg->addChild(levelLabel);


	char targetStr[10];
	sprintf(targetStr, "%d", target);
	auto targetLabel = Label::createWithTTF(targetStr, "fonts/Marker Felt.ttf", 40);
	targetLabel->setPosition(origin.x + visibleSize.width / 2 - 70, origin.y + visibleSize.height / 2 - 210);
	targetLabel->setColor(Color3B(67.0, 72.0, 88.0));
	levelBg->addChild(targetLabel);

	char tlStr[10];
	sprintf(tlStr, "%4.2f", timelimit);
	auto tlLabel = Label::createWithTTF(tlStr, "fonts/Marker Felt.ttf", 40);
	tlLabel->setPosition(origin.x + visibleSize.width / 2 - 130, origin.y + visibleSize.height / 2 - 290);
	tlLabel->setColor(Color3B(67.0, 72.0, 88.0));
	levelBg->addChild(tlLabel);

	m_cmTimer->stopTimer();
	m_cmTimer->setTotalTime(timelimit);

	this->addChild(levelBg, 3, "levelbg");

	levelBg->setScale(0.0f);	
	CCActionInterval *scaleto = CCScaleTo::create(0.5f, 1.0f);
	CCActionInterval *delaytime = CCDelayTime::create(1.0f);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("levelbg");
		char targetNumStr[30];
		sprintf(targetNumStr, "Target: %d", m_targetNum);
		m_targetLabel->setString(targetNumStr);
		m_player->setStageNum(m_stage);
		char stageStr[30];
		sprintf(stageStr, "Level: %d", m_stage + 1);
		m_stageLabel->setString(stageStr);

		m_player->addTotalMoney(-1 * m_targetNum);
		m_player->changeTotalMoneyLabel();

		m_cmTimer->startTimer();
		m_enabled = true;

	});
	CCFiniteTimeAction * seq = CCSequence::create(scaleto, delaytime, funcall, NULL);

	levelBg->runAction(seq);

	//scheduleOnce(schedule_selector(EndlessScene::removeStageEffect), 1.0f);
}

void EndlessScene::removeStageEffect(float time)
{
	this->removeChildByName("levelbg");
	char targetNumStr[30];
	sprintf(targetNumStr, "Target: %d", m_targetNum);
	m_targetLabel->setString(targetNumStr);
	m_player->setStageNum(m_stage);
	char stageStr[30];
	sprintf(stageStr, "Level: %d", m_stage + 1);
	m_stageLabel->setString(stageStr);

	m_player->addTotalMoney(-1 * m_targetNum);
	m_player->changeTotalMoneyLabel();

	m_cmTimer->startTimer();
}

void EndlessScene::manualAct1()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");


	CCSprite* gesture = CCSprite::create("manual/gesture.png");
	gesture->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 - 50);
	this->addChild(gesture, 5, "gesture");

	CCSprite* up_count = CCSprite::create("manual/up_count.png");
	up_count->setPosition(origin.x + visibleSize.width / 2 + 120, origin.y + visibleSize.height / 4 + 170);
	this->addChild(up_count, 5, "up_count");

	CCActionInterval * moveup = CCMoveBy::create(1.1f, ccp(0, 200));
	CCActionInterval * movedown = CCMoveBy::create(0.15f, ccp(0, -200));

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("gesture");
		this->removeChildByName("up_count");
		m_stageCount = 1;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveup, movedown, moveup, funcall, NULL);
	gesture->runAction(seq);
}

void EndlessScene::manualAct2()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");


	CCSprite* target_frame = CCSprite::create("manual/redFrame.png");
	target_frame->setPosition(origin.x + 180, origin.y + visibleSize.height - 200);
	target_frame->setScale(1.1, 0.7);
	this->addChild(target_frame, 5, "target_frame");

	CCSprite* endless_tip1 = CCSprite::create("manual/endlessTip1.png");
	endless_tip1->setPosition(origin.x + visibleSize.width / 2 - 50, origin.y + visibleSize.height - 300);
	this->addChild(endless_tip1, 5, "endlesstip1");

	CCSprite* endless_tip2 = CCSprite::create("manual/endlessTip2.png");
	endless_tip2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(endless_tip2, 5, "endlesstip2");

	scheduleOnce(schedule_selector(EndlessScene::updateManualAct1), 1.0f);
}

void EndlessScene::manualAct3()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");


	CCSprite* endless_tip3 = CCSprite::create("manual/endlessTip3.png");
	endless_tip3->setPosition(origin.x + visibleSize.width / 2 , origin.y + visibleSize.height / 2);
	this->addChild(endless_tip3, 5, "endlesstip3");

	CCSprite* gesture = CCSprite::create("manual/gesture.png");
	gesture->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 - 50);
	this->addChild(gesture, 5, "gesture");

	CCActionInterval * moveup = CCMoveBy::create(1.5f, ccp(0, 200));
	CCActionInterval * movedown = CCMoveBy::create(0.2f, ccp(0, -200));

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("gesture");
		this->removeChildByName("endlesstip3");
		m_stageCount = 3;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveup, movedown, moveup, funcall, NULL);
	gesture->runAction(seq);

}

void EndlessScene::updateManualAct1(float time)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CCSprite* gesture = CCSprite::create("manual/gesture.png");
	gesture->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 4 - 50);
	this->addChild(gesture, 5, "gesture");

	CCActionInterval* moveright = CCMoveBy::create(1.5f, ccp(150, 0));
	CCActionInterval* moveleft = CCMoveBy::create(0.2f, ccp(-150, 0));

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("manual_mask");
		this->removeChildByName("gesture");
		this->removeChildByName("endlesstip1");
		this->removeChildByName("endlesstip2");
		this->removeChildByName("target_frame");
		m_stageCount = 2;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveright, moveleft, moveright, funcall, NULL);
	gesture->runAction(seq);
	
}


void EndlessScene::manualAct4()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//引导结束的提示
	CCSprite* endless_tip4 = CCSprite::create("manual/endlessTip4.png");
	endless_tip4->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 160);
	this->addChild(endless_tip4, 5, "endlesstip4");

	scheduleOnce(schedule_selector(EndlessScene::updateManualAct2), 2.0f);
}




void EndlessScene::updateManualAct2(float time)
{
	this->removeChildByName("endlesstip4");
	MCManual::writeUserProfile(MANUAL_ENDLESS, false);
}





