#include "OnlineScene.h"
#include "StartScene.h"
#include "MCUtil.h"
#include "AudioControl.h"
#include "PopChoiceDlg.h"


OnlineScene::~OnlineScene()
{
	//WebClient::getinstance().shutdown();
}


Scene* OnlineScene::createScene()
{
	auto scene = Scene::create();
	auto layer = OnlineScene::create();
	scene->addChild(layer);
	return scene;
}

bool OnlineScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	auto returnItem = MenuItemImage::create(
		"online/icon_back.png",
		"online/icon_back.png",
		CC_CALLBACK_1(OnlineScene::returnCallback, this));

	returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width / 2,
		origin.y + visibleSize.height - returnItem->getContentSize().height / 2));


	/*auto pauseItem = MenuItemImage::create(
		"online/icon_pause.png",
		"online/icon_pause.png",
		CC_CALLBACK_1(OnlineScene::pauseCallback, this));

	pauseItem->setPosition(Vec2(origin.x + visibleSize.width - pauseItem->getContentSize().width / 2,
		origin.y + visibleSize.height - pauseItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(returnItem, pauseItem, NULL);*/
	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//vs band
	createVS();
	m_connected = false;
	m_uScore = 0;

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
	m_cmTimer->createLabel(Vec2(origin.x + visibleSize.width / 2 - 5, origin.y + visibleSize.height - 73), m_player, 4);
	this->addChild(m_cmTimer, 2);

	

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(OnlineScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(OnlineScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(OnlineScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	startConnect();

	//播放背景音乐
	AudioControl::playBgMusic(PK_ONLINE);
	//初始化
	m_count_flag = false;
	m_timerStart = false;

	return true;
}


void OnlineScene::pauseCallback(Ref* pSender)
{
	//遍历当前类的所有子节点信息，画入renderTexture中。  
	//这里类似截图。  
	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture *renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height + 30);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = OnlinePauseScene::createScene(renderTexture, m_player->totalMoneyNum());
	Director::sharedDirector()->pushScene(scene);*/
}

void OnlineScene::returnCallback(Ref* pSender)
{
	AudioControl::stopBGMusic();
	WebClient::getinstance().shutdown();
	auto scene = StartScene::createScene();
	Director::getInstance()->replaceScene(CCTransitionZoomFlipX::create(0.5, scene, TransitionScene::Orientation::LEFT_OVER));
}

void OnlineScene::setBgImage()
{
	// add "Main Scene" splash screen"
	auto sprite = Sprite::create("online/online_bg.png");
	//auto sprite = LayerColor::create(ccc4(0xff, 0xff, 0xff, 0xff), 768, 1024);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// position the sprite on the center of the screen
	sprite->setPosition(ccp(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
}

bool OnlineScene::onTouchBegan(Touch* touch, Event* event)
{
	if (m_count_flag)
		return false;
	_spos = touch->getLocation();
	_curPos = _spos;
	if (m_player->MoneyTotal()->isOnMoney(_spos))
	{
		if (m_player->allowed())
		{
			m_count_flag = true;
			if (!m_timerStart)
			{
				m_timerStart = true;
				m_cmTimer->startTimer();
				sendStartEvent();
			}
			m_effect_id = AudioControl::playCountEffect();
			m_player->addSingleMoneyLabel(false, "center", Vec2(12.0, 0.0));
		}
	}

	return true;
}


void OnlineScene::onTouchMoved(Touch* touch, Event* event)
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


void OnlineScene::onTouchEnded(Touch* touch, Event* event)
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
	
		sendFightEvent(EVENT_FIGHT, false);

		break;
	default:
		m_player->removeChild(m_player->MoneySingle());
		return;
		break;
	}

	m_player->changeTotalMoneyLabel();
	changeMeVS();
}


void OnlineScene::addTimerFrame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_timerFrame = Sprite::create("loser/timer.png");
	m_timerFrame->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 75));
	this->addChild(m_timerFrame, 1);
}

void OnlineScene::createVS()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//myself
	m_meLabel = Label::createWithTTF("0", "fonts/DTLNobelT-Bold.otf", 40);
	m_meLabel->setPosition(origin.x + visibleSize.width * 0.5 - 100, origin.y + visibleSize.height * 0.5 - 50);
	m_meLabel->setColor(Color3B(106.0, 57.0, 6.0));
	this->addChild(m_meLabel, 3);


	//opponent
	m_uLabel = Label::createWithTTF("0", "fonts/DTLNobelT-Bold.otf", 40);
	m_uLabel->setPosition(origin.x + visibleSize.width * 0.5 + 100, origin.y + visibleSize.height * 0.5 - 50);
	m_uLabel->setColor(Color3B(106.0, 57.0, 6.0));
	this->addChild(m_uLabel, 3);

	//vs band
	auto vsband = Sprite::create("online/vs.png");
	vsband->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));
	this->addChild(vsband, 3);
}

void OnlineScene::changeMeVS()
{
	//myself
	char meMoneyStr[20];
	sprintf(meMoneyStr, "%d", m_player->totalMoneyNum());
	m_meLabel->setString(meMoneyStr);	
}


void OnlineScene::changeUVS()
{
	//opponent
	char uMoneyStr[20];
	sprintf(uMoneyStr, "%d", m_uScore);
	m_uLabel->setString(uMoneyStr);
}

void OnlineScene::startConnect()
{
	WebClient::getinstance().registerMethod(this);
	WebClient::getinstance().start();
	WebClient::getinstance().sendReadyEvent();
}

void OnlineScene::onStatusChanged(ConnectStatus status)
{
	switch (status)
	{
	case WAITING:
		m_cmTimer->setAllowed(false);
		m_player->setAllowed(false);
		MessageBox("Waiting...", "Info");
		//PopChoiceDlg* dlg = PopChoiceDlg::create();
		//this->addChild(dlg, 2);
		break;
	case ENEMY_READY:
		if (!m_connected)
		{
			m_connected = true;
			MessageBox("Match Successful!", "Info");
			m_cmTimer->setAllowed(true);
			m_player->setAllowed(true);
		}
		break;
	case ENEMY_QUIT:
		m_connected = false;
		m_cmTimer->setAllowed(false);
		m_cmTimer->stopTimer();
		if (m_cmTimer->timeLeft() > 0.5f)
		{
			m_player->setAllowed(false);
			MessageBox("Opponent Lost!", "Info");
		}
		break;
	case ENEMY_START:
		m_cmTimer->startTimer();
		m_timerStart = true;
		break;
	default:
		break;
	}
}


void OnlineScene::sendFightEvent(int type, bool toMe)
{
	WebClient::getinstance().sendFightEvent(type, toMe, m_player->totalMoneyNum());
}

void OnlineScene::onFight(int type, bool toMe, int enemyScore)
{
	switch (type)
	{
		case EVENT_FIGHT:
		{
			m_uScore = enemyScore;
			m_player->setWin(enemyScore);
			changeUVS();
			break;
		}
		default:
			break;
	}
}

void OnlineScene::sendStartEvent()
{
	WebClient::getinstance().sendStartEvent();
}

void OnlineScene::readyGoAct()
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
		//m_enabled = true;
		m_cmTimer->startTimer();
	});
	CCFiniteTimeAction * seq = CCSequence::create(act, funcall, NULL);
	m_ready->runAction(seq);

	AudioControl::playReadyEffect();
}

