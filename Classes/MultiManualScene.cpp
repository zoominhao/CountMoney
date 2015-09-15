#include "MultiManualScene.h"
#include "StartScene.h"
#include "AudioControl.h"
#include "MCManual.h"

Scene* MultiManualScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MultiManualScene::create();
	scene->addChild(layer);
	return scene;
}

bool MultiManualScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sp1 = Sprite::create("multi/multi_manual1.png");
	auto sp2 = Sprite::create("multi/multi_manual2.png");
	auto sp3 = Sprite::create("multi/multi_manual3.png");
	sp1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	sp2->setPosition(origin.x + visibleSize.width * 3 / 2, origin.y + visibleSize.height / 2);
	sp3->setPosition(origin.x + visibleSize.width * 5 / 2, origin.y + visibleSize.height / 2);
	this->addSlidePage(sp1, 0);
	this->addSlidePage(sp2, 0);
	this->addSlidePage(sp3, 1);

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(MultiManualScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(MultiManualScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(MultiManualScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);


	return true;
}

bool MultiManualScene::onTouchBegan(Touch* touch, Event* event)
{
	SlidingLayer::onTouchBegan(touch, event);
	return true;
}

void MultiManualScene::onTouchMoved(Touch* touch, Event* event)
{
	SlidingLayer::onTouchMoved(touch, event);
}

void MultiManualScene::onTouchEnded(Touch* touch, Event* event)
{
	SlidingLayer::onTouchEnded(touch, event);
}

void MultiManualScene::btnCB(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = StartScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}
