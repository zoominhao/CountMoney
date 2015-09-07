#include "LoserManualScene.h"
#include "LoserScene.h"
#include "AudioControl.h"
#include "MCManual.h"

Scene* LoserManualScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoserManualScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoserManualScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sp1 = Sprite::create("loser/Loser_Manual.png");
	sp1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addSlidePage(sp1, 1);

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(LoserManualScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(LoserManualScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(LoserManualScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);


	return true;
}

bool LoserManualScene::onTouchBegan(Touch* touch, Event* event)
{
	SlidingLayer::onTouchBegan(touch, event);
	return true;
}

void LoserManualScene::onTouchMoved(Touch* touch, Event* event)
{
	SlidingLayer::onTouchMoved(touch, event);
}

void LoserManualScene::onTouchEnded(Touch* touch, Event* event)
{
	SlidingLayer::onTouchEnded(touch, event);
}

void LoserManualScene::btnCB(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//SlidingLayer::btnCB(pSender, type);
		AudioControl::playClickEffect();
		MCManual::writeUserProfile(MANUAL_LOSER, false);
		auto scene = LoserScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}
