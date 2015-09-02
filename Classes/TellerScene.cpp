#include "TellerScene.h"
#include "TellerPauseScene.h"
#include "SingleScene.h"
#include "MCUtil.h"
#include "AudioControl.h"

#include <iostream>

USING_NS_CC;

Scene* TellerScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = TellerScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TellerScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto returnItem = MenuItemImage::create(
		"teller/icon_back.png",
		"teller/icon_back.png",
		CC_CALLBACK_1(TellerScene::returnCallback, this));

	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2));


	auto pauseItem = MenuItemImage::create(
		"teller/icon_pause.png",
		"teller/icon_pause.png",
		CC_CALLBACK_1(TellerScene::pauseCallback, this));

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

	//
	addTranshCan();
	addTimerFrame();

	//�����ң��ó���Ϊ����ģʽ
	m_player = Player::create();
	m_player->createPlayer(2);
	m_player->setTotalMoneySpritePos(0, -20);
	this->addChild(m_player, 1);

	//��Ӽ�ʱ��
	m_cmTimer = CMTimer::create();
	m_cmTimer->createLabel(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 80), m_player, 2);
	this->addChild(m_cmTimer, 2);
	
	//ready go
	AudioControl::playReadyEffect();

    //���ű�������
	//AudioControl::playBgMusic(TELLER);

	//ע�ᵥ�㴥���¼�
	auto touchlistenter = EventListenerTouchOneByOne::create();   
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(TellerScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(TellerScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(TellerScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	//��ʼ��
	m_count_flag = false;
	m_curFake = false;
	m_nextFake = false;
	m_isEmpty = true;
	m_needRand = true;
	m_enabled = false;

	srand((unsigned)time(NULL));
	readyGoAct();

    return true;
}

void TellerScene::setBgImage()
{
	auto sprite = Sprite::create("teller/teller_bg.png");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// position the sprite on the center of the screen
	sprite->setPosition(ccp(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
}


bool TellerScene::onTouchBegan(Touch* touch, Event* event)
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
		//m_cmTimer->startTimer();
		m_effect_id = AudioControl::playCountEffect();
		if (m_needRand)
		{
			m_player->addSingleMoneyLabel(m_curFake, "center", Vec2(12.0, 0.0));
			int x = rand() % 100;
			if (x > 70)
			{
				m_player->MoneyTotal()->changeMoney(Fake_100_T);
				m_nextFake = true;
			}
			else
			{
				m_player->MoneyTotal()->changeMoney(Real_100_T);
				m_nextFake = false;
			}
		}
	}
	return true;
}

void TellerScene::onTouchMoved(Touch* touch, Event* event)
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

void TellerScene::onTouchEnded(Touch* touch, Event* event)
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
		if (m_curFake)   //���ּٳ�
		{
			m_player->addFakeWrong(1);
			m_player->addTotalMoney(-200);
			//m_player->changeFakeWrongLabel();
		}
		else
		{
			m_player->addTotalMoney(100);
		}

		m_player->removeChildByName("up");
		m_player->MoneySingle()->setName("up");
		m_needRand = true;
		break;
	case RIGHT:
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
		m_curFake = m_nextFake;
	//AudioControl::stopEffectMusic(m_effect_id);
	m_player->changeTotalMoneyLabel();
}


void TellerScene::returnCallback(Ref* pSender)
{
	AudioControl::stopBGMusic();
	auto scene = SingleScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void TellerScene::pauseCallback(Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height + 30);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = TellerPauseScene::createScene(renderTexture, m_player->totalMoneyNum());
	Director::sharedDirector()->pushScene(scene);

	AudioControl::stopBGMusic();
}


void TellerScene::addTranshCan()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_transhCan = Sprite::create("trashCan.png");
	m_transhCan->setScale(0.6);
	m_transhCan->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2 - 300));
	this->addChild(m_transhCan, 1);
}

void TellerScene::addTimerFrame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_timerFrame = Sprite::create("teller/timer.png");
	m_timerFrame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75));
	this->addChild(m_timerFrame, 1);
}

void TellerScene::readyGoAct()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto m_ready = Sprite::create();
	m_ready->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));
	m_ready->setScale(0.5);
	this->addChild(m_ready, 1, "ready");
	//��ͼƬ���ص�����֡�����  
	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("ready/ready.plist", "ready/ready.png");
	//��һ���б������е�CCSpriteFrameCache  
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 5; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}
	//ʹ���б�����������  
	Animation* animation = Animation::createWithSpriteFrames(frameArray, 0.4f, 1);

	//��������װ��һ������  
	Animate* act = Animate::create(animation);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("ready");
		AudioControl::playBgMusic(TELLER);
		m_enabled = true;
		m_cmTimer->startTimer();
	});
	CCFiniteTimeAction * seq = CCSequence::create(act, funcall, NULL);
	m_ready->runAction(seq);

	AudioControl::playReadyEffect();
}
