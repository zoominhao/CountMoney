#include "MultiManualScene.h"
#include "MultiScene.h"
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
		//SlidingLayer::btnCB(pSender, type);
		AudioControl::playClickEffect();
		MCManual::writeUserProfile(MANUAL_MULTI, false);
		multiOpenAct();
	}
}

void MultiManualScene::multiOpenAct()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//白底
	Layer *layer = LayerColor::create(ccc4(0xff, 0xff, 0xff, 0xff), visibleSize.width, visibleSize.height);
	this->addChild(layer, 2, "vsbg");

	CCSprite* sp1 = CCSprite::create("multi/P1.png");
	sp1->setPosition(origin.x + visibleSize.width, origin.y + visibleSize.height);
	this->addChild(sp1, 2, "vsdown");
	CCActionInterval * movedown = CCMoveBy::create(0.5f, ccp(-visibleSize.width / 2, -visibleSize.height / 2));
	sp1->runAction(movedown);

	CCSprite * sp2 = CCSprite::create("multi/P2.png");
	sp2->setPosition(origin.x, origin.y);
	this->addChild(sp2, 2, "vsup");
	CCActionInterval * moveup = CCMoveBy::create(0.5f, ccp(visibleSize.width / 2, visibleSize.height / 2));
	sp2->runAction(moveup);

	CCSprite * svs = CCSprite::create("multi/pvp.png");
	svs->setPosition(origin.x, origin.y + visibleSize.height / 2);
	this->addChild(svs, 2, "vscenter");
	CCActionInterval* moveby = CCMoveBy::create(0.5f, ccp(visibleSize.width / 2, 0));
	CCActionInterval * easeElasticOut = CCEaseElasticOut::create(moveby);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("vsbg");
		this->removeChildByName("vsdown");
		this->removeChildByName("vsup");
		this->removeChildByName("vscenter");
		auto scene = MultiScene::createScene();
		Director::sharedDirector()->replaceScene(TransitionFadeUp::create(0.5f, scene));

	});
	CCFiniteTimeAction * seq = CCSequence::create(easeElasticOut, funcall, NULL);

	svs->runAction(seq);
}
