#include "StartScene.h"
#include "MultiScene.h"
#include "AboutScene.h"
#include "SetScene.h"
#include "SingleScene.h"
#include "OnlineScene.h"
#include "AudioControl.h"


Scene* StartScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Start/StartUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x + visibleSize.width / 2 - uilayer->getContentSize().width / 2,
		origin.y + visibleSize.height / 2 - uilayer->getContentSize().height / 2));


	this->addChild(uilayer);

	//preload audio
	AudioControl::preLoad();

	//给开始按钮添加事件监听  

	ui::Button *Btn_Single = dynamic_cast<ui::Button*>(uilayer->getChildByName("SingleCount"));
	Btn_Single->addTouchEventListener(CC_CALLBACK_2(StartScene::SingleScene, this));

	ui::Button *Btn_Double = dynamic_cast<ui::Button*>(uilayer->getChildByName("DoubleCount"));
	Btn_Double->addTouchEventListener(CC_CALLBACK_2(StartScene::MultiScene, this));

	ui::Button *Btn_Net = dynamic_cast<ui::Button*>(uilayer->getChildByName("NetCount"));
	Btn_Net->addTouchEventListener(CC_CALLBACK_2(StartScene::NetScene, this));

	ui::Button *Btn_Rank = dynamic_cast<ui::Button*>(uilayer->getChildByName("Rank"));
	Btn_Rank->addTouchEventListener(CC_CALLBACK_2(StartScene::RankScene, this));

	ui::Button *Btn_Set = dynamic_cast<ui::Button*>(uilayer->getChildByName("Set"));
	Btn_Set->addTouchEventListener(CC_CALLBACK_2(StartScene::SetScene, this));

	return true;
}




void StartScene::SingleScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = SingleScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void StartScene::MultiScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		multiOpenAct();
	}
}

void StartScene::NetScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = OnlineScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
	
}

void StartScene::RankScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = AboutScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void StartScene::SetScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = SetScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void StartScene::multiOpenAct()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//白底
	Layer *layer = LayerColor::create(ccc4(0xff, 0xff, 0xff, 0xf0), visibleSize.width, visibleSize.height);
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



