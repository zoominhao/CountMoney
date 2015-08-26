#include "LoserScene.h"
#include "LoserPauseScene.h"
#include "SingleScene.h"
#include "MCUtil.h"
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
		"return_icon1.png",
		"return_icon2.png",
		CC_CALLBACK_1(LoserScene::returnCallback, this));

	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2));


	auto pauseItem = MenuItemImage::create(
		"pause_icon1.png",
		"pause_icon2.png",
		CC_CALLBACK_1(LoserScene::pauseCallback, this));

	pauseItem->setPosition(Vec2(origin.x + visibleSize.width - pauseItem->getContentSize().width / 2,
		origin.y + visibleSize.height - pauseItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, pauseItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//preload audio
	AudioControl::preLoad();

	//add background image
	setBgImage();

	//add cat
	addCat();

	//添加玩家，该场景为单人模式
	m_player = Player::create();
	m_player->createPlayer(1);
	this->addChild(m_player, 1);

	//添加计时器
	m_cmTimer = CMTimer::create();
	m_cmTimer->createLabel(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 80), m_player, 1);
	this->addChild(m_cmTimer, 2);

	//播放背景音乐
	AudioControl::playBgMusic();

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(LoserScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(LoserScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(LoserScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);


	//初始化
	m_count_flag = false;

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
}

void LoserScene::returnCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	auto scene = SingleScene::createScene();
	Director::getInstance()->replaceScene(scene);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void LoserScene::setBgImage()
{
	// add "Main Scene" splash screen"
	//auto sprite = Sprite::create("bg_pic.jpg");
	auto sprite = LayerColor::create(ccc4(0xff, 0xff, 0xff, 0xff), 768, 1024);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// position the sprite on the center of the screen
	sprite->setPosition(ccp(origin.x + visibleSize.width / 2 - sprite->getContentSize().width / 2,
		origin.y + visibleSize.height / 2 - sprite->getContentSize().height / 2));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
}

bool LoserScene::onTouchBegan(Touch* touch, Event* event)
{
	_spos = touch->getLocation();
	_curPos = _spos;
	if (m_player->MoneyTotal()->isOnMoney(_spos))
	{
		m_count_flag = true;
		m_cmTimer->startTimer();
		m_effect_id = AudioControl::playEffectMusic();
		m_player->addSingleMoneyLabel(false, "center");
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
		break;
	default:
		m_player->removeChild(m_player->MoneySingle());
		AudioControl::stopEffectMusic(m_effect_id);
		return;
		break;
	}

	//m_player->removeChild(m_player->MoneySingle());
	AudioControl::stopEffectMusic(m_effect_id);
	m_player->changeTotalMoneyLabel();
}

void LoserScene::addCat()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_cat = Sprite::create("cat.png");
	m_cat->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200));
	this->addChild(m_cat, 1);
}


