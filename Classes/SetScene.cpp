#include "SetScene.h"
#include "StartScene.h"
#include "AudioControl.h"

USING_NS_CC;


Scene* SetScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SetScene::create();
	scene->addChild(layer);
	return scene;
}

bool SetScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	setBgImage();

	//添加返回按钮
	auto returnItem = MenuItemImage::create(
		"return_icon1.png",
		"return_icon2.png",
		CC_CALLBACK_1(SetScene::returnCallback, this));

	returnItem->setPosition(origin.x + 60, origin.y + visibleSize.height - 60);

	auto menu = Menu::create(returnItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	if (AudioControl::isBGMusicStopped)
		bgMusicBtn = Sprite::create("Set/off.jpg");
	else
		bgMusicBtn = Sprite::create("Set/on.jpg");
	bgMusicBtn->setPosition(origin.x + visibleSize.width / 2 + 40, origin.y + visibleSize.height / 2 + 100);
	this->addChild(bgMusicBtn, 1);

	if (AudioControl::isEffectStopped)
		effectBtn = Sprite::create("Set/off.jpg");
	else
		effectBtn = Sprite::create("Set/on.jpg");
	effectBtn->setPosition(origin.x + visibleSize.width / 2 + 40, origin.y + visibleSize.height / 2 - 50);
	this->addChild(effectBtn, 1);


	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(true);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(SetScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(SetScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(SetScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	isBGMusicStopped = false;
	isEffectStopped = false;

	return true;
}



bool SetScene::onTouchBegan(Touch* touch, Event* event)
{
	CCLOG("touched");
	if (bgMusicBtn->getBoundingBox().containsPoint(touch->getLocation()))
	{
		AudioControl::isBGMusicStopped = !AudioControl::isBGMusicStopped;
		if (AudioControl::isBGMusicStopped)
		{
			bgMusicBtn->setTexture("Set/off.jpg");
			AudioControl::stopBGMusic();
		}
		else
		{
			bgMusicBtn->setTexture("Set/on.jpg");
		}
	}

	if (effectBtn->getBoundingBox().containsPoint(touch->getLocation()))
	{
		AudioControl::isEffectStopped = !AudioControl::isEffectStopped;
		if (AudioControl::isEffectStopped)
		{
			effectBtn->setTexture("Set/off.jpg");
			AudioControl::stopMCEffects();
		}
		else
		{
			effectBtn->setTexture("Set/on.jpg");
		}
	}

	return true;
	
}

void SetScene::onTouchMoved(Touch* touch, Event* event)
{

}

void SetScene::onTouchEnded(Touch* touch, Event* event)
{

}

void SetScene::setBgImage()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = Sprite::create("Set/background_sw.png");
	sprite->setPosition(ccp(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));

	this->addChild(sprite, 0);
}

void SetScene::returnCallback(Ref* pSender)
{
	auto scene = StartScene::createScene();
	Director::sharedDirector()->replaceScene(scene);
}



