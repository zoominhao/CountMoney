#include "EndlessManualScene.h"
#include "SingleScene.h"
#include "AudioControl.h"
#include "MCManual.h"

Scene* EndlessManualScene::createScene()
{
	auto scene = Scene::create();
	auto layer = EndlessManualScene::create();
	scene->addChild(layer);
	return scene;
}

bool EndlessManualScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sp1 = Sprite::create("endless/endless_manual.png");
	sp1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addSlidePage(sp1, 1);

	//注册单点触屏事件
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(false);

	touchlistenter->onTouchBegan = CC_CALLBACK_2(EndlessManualScene::onTouchBegan, this);
	touchlistenter->onTouchEnded = CC_CALLBACK_2(EndlessManualScene::onTouchEnded, this);
	touchlistenter->onTouchMoved = CC_CALLBACK_2(EndlessManualScene::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);


	return true;
}

bool EndlessManualScene::onTouchBegan(Touch* touch, Event* event)
{
	SlidingLayer::onTouchBegan(touch, event);
	return true;
}

void EndlessManualScene::onTouchMoved(Touch* touch, Event* event)
{
	SlidingLayer::onTouchMoved(touch, event);
}

void EndlessManualScene::onTouchEnded(Touch* touch, Event* event)
{
	SlidingLayer::onTouchEnded(touch, event);
}

void EndlessManualScene::btnCB(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//SlidingLayer::btnCB(pSender, type);
		AudioControl::playClickEffect();
		auto scene = SingleScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
		//MCManual::writeUserProfile(MANUAL_ENDLESS, false);
	}
}
