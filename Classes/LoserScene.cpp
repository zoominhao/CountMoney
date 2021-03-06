#include "LoserScene.h"
#include "LoserPauseScene.h"
#include "SingleScene.h"
#include "MCUtil.h"
#include "MCManual.h"
#include "AudioControl.h"



Scene* LoserScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoserScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoserScene::init()
{
	
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto returnItem = MenuItemImage::create(
		"loser/icon_back.png",
		"loser/icon_back.png",
		CC_CALLBACK_1(LoserScene::returnCallback, this));

	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2));


	auto pauseItem = MenuItemImage::create(
		"loser/icon_pause.png",
		"loser/icon_pause.png",
		CC_CALLBACK_1(LoserScene::pauseCallback, this));

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

	addTimerFrame();

	//添加玩家，该场景为单人模式
	m_player = Player::create();
	m_player->createPlayer(1);
	m_player->setTotalMoneySpritePos(0, -20);
	//m_player->setTotalMoneyNumPos(-100, 10);
	this->addChild(m_player, 1);

	//添加计时器
	m_cmTimer = CMTimer::create();
	m_cmTimer->createLabel(Vec2(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - 73), m_player, 1);
	this->addChild(m_cmTimer, 2);


	//飘字
	m_flyword = FlyWord::create("flyword/+100.png", 1.0, Vec2(origin.x + visibleSize.width/2 - 130, origin.y + visibleSize.height / 2), 50);
	this->addChild(m_flyword, 2);

	//播放背景音乐
	//AudioControl::playBgMusic(LOSER);
	AudioControl::stopBGMusic();

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(LoserScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(LoserScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(LoserScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	if (MCManual::novice[0])
	{
		manualAct1();
	}
	else
	{
		readyGoAct();
	}

	//初始化
	m_count_flag = false;
	m_enabled = false;

	return true;
}


void LoserScene::pauseCallback(Ref* pSender)
{
	//遍历当前类的所有子节点信息，画入renderTexture中。  
	//这里类似截图。  
	Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height + 30);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = LoserPauseScene::createScene(renderTexture, m_player->totalMoneyNum());
	Director::sharedDirector()->pushScene(scene);

	AudioControl::stopBGMusic();
}

void LoserScene::returnCallback(Ref* pSender)
{
	AudioControl::stopBGMusic();
	auto scene = SingleScene::createScene();
	Director::getInstance()->replaceScene(TransitionZoomFlipX::create(0.5, scene, TransitionScene::Orientation::LEFT_OVER));
}

void LoserScene::setBgImage()
{
	// add "Main Scene" splash screen"
	auto sprite = Sprite::create("loser/loser_bg.png");
	//auto sprite = LayerColor::create(ccc4(0xff, 0xff, 0xff, 0xff), 768, 1024);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// position the sprite on the center of the screen
	sprite->setPosition(ccp(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
}

bool LoserScene::onTouchBegan(Touch* touch, Event* event)
{
	if (!m_enabled)
		return false;

	if (m_count_flag)
		return false;
	_spos = touch->getLocation();
	_curPos = _spos;
	if (m_player->MoneyTotal()->isOnMoney(_spos))
	{
		m_count_flag = true;
		m_effect_id = AudioControl::playCountEffect();
		m_player->addSingleMoneyLabel(false, "center", Vec2(12.0, 0.0));
	}

	return true;
}


void LoserScene::onTouchMoved(Touch* touch, Event* event)
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


void LoserScene::onTouchEnded(Touch* touch, Event* event)
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
		m_player->addTotalMoney(100);
		m_player->removeChildByName("up");
		m_player->MoneySingle()->setName("up");

		m_flyword->Flying();

		if (MCManual::novice[0])
		{
			m_enabled = false;
			manualAct2();
		}
		break;
	default:
		m_player->removeChild(m_player->MoneySingle());
		//AudioControl::stopEffectMusic(m_effect_id);
		return;
		break;
	}

	//m_player->removeChild(m_player->MoneySingle());
	//AudioControl::stopEffectMusic(m_effect_id);
	m_player->changeTotalMoneyLabel();
}


void LoserScene::addTimerFrame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_timerFrame = Sprite::create("loser/timer.png");
	m_timerFrame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75));
	this->addChild(m_timerFrame, 1);
}

void LoserScene::readyGoAct()
{

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto m_ready = Sprite::create();
	m_ready->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));
	m_ready->setScale(0.5);
	this->addChild(m_ready, 1, "ready");
	//将图片加载到精灵帧缓存池  
	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("ready/ready.plist", "ready/ready.png");
	//用一个列表保存所有的CCSpriteFrameCache  
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 5; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}
	//使用列表创建动画对象  
	Animation* animation = Animation::createWithSpriteFrames(frameArray, 0.4f, 1);

	//将动画包装成一个动作  
	Animate* act = Animate::create(animation);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("ready");
		AudioControl::playBgMusic(LOSER);
		m_enabled = true;
		m_cmTimer->startTimer();
	});
	CCFiniteTimeAction * seq = CCSequence::create(act, funcall, NULL);
	m_ready->runAction(seq);
	
	AudioControl::playReadyEffect();
}

void LoserScene::manualAct1()
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
		m_enabled = true;
	});

	CCFiniteTimeAction * seq = CCSequence::create(moveup, movedown, moveup, funcall, NULL);
	gesture->runAction(seq);
}

void LoserScene::manualAct2()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto manual_mask = Sprite::create("manual/mask.png");
	manual_mask->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(manual_mask, 5, "manual_mask");


	CCSprite* timer_frame = CCSprite::create("manual/redFrame.png");
	timer_frame->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75);
	timer_frame->setScale(1.1, 0.86);
	this->addChild(timer_frame, 5, "timer_frame");

	CCSprite* loser_tip1 = CCSprite::create("manual/loserTip1.png");
	loser_tip1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 220);
	this->addChild(loser_tip1, 5, "losertip1");

	scheduleOnce(schedule_selector(LoserScene::updateManualAct), 3.5f);
}

void LoserScene::updateManualAct(float time)
{
	this->removeChildByName("manual_mask");
	this->removeChildByName("timer_frame");
	this->removeChildByName("losertip1");
	MCManual::writeUserProfile(MANUAL_LOSER, false);
	readyGoAct();
}

