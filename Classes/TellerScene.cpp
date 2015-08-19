#include "TellerScene.h"
#include "StartScene.h"
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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
										   CC_CALLBACK_1(TellerScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//preload audio
	AudioControl::preLoad();

    // add title
	addTitle(origin.x + visibleSize.width / 2, origin.y + visibleSize.height);

	//add background image
	setBgImage(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	//添加玩家，该场景为单人模式
	m_player = Player::create();
	m_player->createPlayer(2);
	this->addChild(m_player, 1);

	//添加计时器
	m_cmTimer = CMTimer::create();
	m_cmTimer->createLabel(Vec2(visibleSize.width / 2 + origin.x + 170, visibleSize.height / 2 + origin.y), m_player, 2);
	this->addChild(m_cmTimer, 2);
	
    //播放背景音乐
	AudioControl::playBgMusic();

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();   
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(TellerScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(TellerScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(TellerScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	//初始化
	m_count_flag = false;
	m_isFake = false;

	srand((unsigned)time(NULL));

    return true;
}


void TellerScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    //Director::getInstance()->end();
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void TellerScene::setBgImage(float px, float py)
{
	// add "Main Scene" splash screen"
	auto sprite = Sprite::create("bg_pic.jpg");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(px, py));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
}


void TellerScene::addTitle(float px, float py)
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

bool TellerScene::onTouchBegan(Touch* touch, Event* event)
{
	_spos = touch->getLocation();
	if (m_player->MoneyTotal()->isOnMoney(_spos))
	{
		m_count_flag = true;
		m_cmTimer->startTimer();
		m_effect_id = AudioControl::playEffectMusic();
	}
	return true;
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
	case LEFT:
		m_player->addSingleMoneyLabel(m_isFake, "left");
		m_player->MoneySingle()->moneyFly(-300.0, 0.0, 0.1);
		if (m_isFake)   //出现假钞
		{
			m_player->addFakeWrong(1);
			m_player->changeFakeWrongLabel();
		}
		else
		{
			m_player->addTotalMoney(100);
		}
		
		break;
/*
	case RIGHT:
		m_money_single->moneyMove(50, 0, 0.1);
		this->removeChild(m_money_single);
		SimpleAudioEngine::sharedEngine()->stopEffect(m_effctId);
		return;
		break;*/
	case UP:
		m_player->addSingleMoneyLabel(m_isFake, "up");
		m_player->MoneySingle()->moneyFly(0.0, 150.0, 0.1);
		break;
	/*case DOWN:
		m_money_single->moneyMove(0, -25, 0.1);
		this->removeChild(m_money_single);
		SimpleAudioEngine::sharedEngine()->stopEffect(m_effctId);
		return;
		break;*/
	default:
		m_player->addSingleMoneyLabel(m_isFake);
		m_player->removeChild(m_player->MoneySingle());
		AudioControl::stopEffectMusic(m_effect_id);
		return;
		break;
	}
	
	
	AudioControl::stopEffectMusic(m_effect_id);

	int x = rand() % 100;
	//CCLOG("%d", x);
	if (x > 50)
	{
		m_player->MoneyTotal()->changeMoney(Fake_100_T);
		m_isFake = true;
	}
	else
	{
		m_player->MoneyTotal()->changeMoney(Real_100_T);
		m_isFake = false;
	}
	m_player->changeTotalMoneyLabel();
}

void TellerScene::onTouchMoved(Touch* touch, Event* event)
{
	/*auto pos = touch->getLocation();
	cu_pos = pos;
	auto vatu = sp2->getPhysicsBody()->getVelocity();

	if (vatu.x >= 0.0f)
	{
	sp2->getPhysicsBody()->setVelocity(vatu + Vect(20, 0));
	}
	else
	{
	sp2->getPhysicsBody()->setVelocity(vatu + Vect(-20, 0));
	}

	log("%f---\n", sp2->getPhysicsBody()->getLinearDamping());*/

/*
	int random(int a, int b){
		int k = 0;
		int sum = 0;
		while (pow(2, k) < b)
		{
			sum += (random(0, 1) << k);
			//rand()/2
			k++；
		}
	}*/
}






