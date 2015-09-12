#include "StartScene.h"
#include "MultiScene.h"
#include "AboutScene.h"
#include "SetScene.h"
#include "SingleScene.h"
#include "OnlineScene.h"
#include "AudioControl.h"

#include "MCManual.h"
#include "MultiManualScene.h"
#include "RankScene.h"


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

	//setKeypadEnabled(true);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(StartScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	MCManual::readUserProfile();
	//给开始按钮添加事件监听  

	AudioControl::playBgMusic(LOGIN);

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

	ui::Button *Btn_DoubleManual = dynamic_cast<ui::Button*>(uilayer->getChildByName("Double_Manual"));
	Btn_DoubleManual->addTouchEventListener(CC_CALLBACK_2(StartScene::DManualScene, this));

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
		//auto scene = OnlineScene::createScene();
		//Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
		//MessageBox("coming soon ...","Info");
		MessageBox("即将上线，敬请期待 ...", "信息");
	}
	
}

void StartScene::RankScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = RankScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
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
	CCActionInterval * movedown = CCMoveBy::create(1.0f, ccp(-visibleSize.width / 2, -visibleSize.height / 2));
	sp1->runAction(movedown);

	CCSprite * sp2 = CCSprite::create("multi/P2.png");
	sp2->setPosition(origin.x, origin.y);
	this->addChild(sp2, 2, "vsup");
	CCActionInterval * moveup = CCMoveBy::create(1.0f, ccp(visibleSize.width / 2, visibleSize.height / 2));
	sp2->runAction(moveup);

	CCSprite * svs = CCSprite::create("multi/pvp.png");
	svs->setPosition(origin.x, origin.y + visibleSize.height / 2);
	this->addChild(svs, 2, "vscenter");
	CCActionInterval* moveby = CCMoveBy::create(1.0f, ccp(visibleSize.width / 2, 0));
	CCActionInterval * easeElasticOut = CCEaseElasticOut::create(moveby);

	CCActionInterval* rmdelay = CCDelayTime::create(1.0f);

	CCCallFunc * funcall = CCCallFunc::create([&](){
		this->removeChildByName("vsbg");
		this->removeChildByName("vsdown");
		this->removeChildByName("vsup");
		this->removeChildByName("vscenter");
		auto scene = MultiScene::createScene();
		Director::sharedDirector()->replaceScene(TransitionFadeUp::create(1.0f, scene));

	});
	CCFiniteTimeAction * seq = CCSequence::create(easeElasticOut, rmdelay, funcall, NULL);

	svs->runAction(seq);
	
}

void StartScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		CCDirector::sharedDirector()->end();
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	}
}

void StartScene::DManualScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto multiManuals = MultiManualScene::createScene();
		Director::sharedDirector()->replaceScene(multiManuals);
	}
}





