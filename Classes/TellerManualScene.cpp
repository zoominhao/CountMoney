#include "TellerManualScene.h"
#include "TellerScene.h"
#include "AudioControl.h"
#include "MCManual.h"

Scene* TellerManualScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TellerManualScene::create();
	scene->addChild(layer);
	return scene;
}

bool TellerManualScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sp1 = Sprite::create("teller/teller_manual.png");
	sp1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addSlidePage(sp1, 1);

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(TellerManualScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(TellerManualScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(TellerManualScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);


	return true;
}

bool TellerManualScene::onTouchBegan(Touch* touch, Event* event)
{
	SlidingLayer::onTouchBegan(touch, event);
	return true;
}

void TellerManualScene::onTouchMoved(Touch* touch, Event* event)
{
	SlidingLayer::onTouchMoved(touch, event);
}

void TellerManualScene::onTouchEnded(Touch* touch, Event* event)
{
	SlidingLayer::onTouchEnded(touch, event);
}

void TellerManualScene::btnCB(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//SlidingLayer::btnCB(pSender, type);
		AudioControl::playClickEffect();
		auto scene = TellerScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
		MCManual::writeUserProfile(MANUAL_TELLER, false);
	}
}
