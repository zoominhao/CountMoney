#include "StartScene.h"
#include "MultiScene.h"
#include "AboutScene.h"
#include "SetScene.h"
#include "SingleScene.h"


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


	//给开始按钮添加事件监听  

	ui::Button *Btn_Single = dynamic_cast<ui::Button*>(uilayer->getChildByName("SingleCount"));
	Btn_Single->addTouchEventListener(CC_CALLBACK_2(StartScene::SingleScene, this));

	ui::Button *Btn_Double = dynamic_cast<ui::Button*>(uilayer->getChildByName("DoubleCount"));
	Btn_Double->addTouchEventListener(CC_CALLBACK_2(StartScene::MultiScene, this));

	ui::Button *Btn_No = dynamic_cast<ui::Button*>(uilayer->getChildByName("NoCount"));
	Btn_No->addTouchEventListener(CC_CALLBACK_2(StartScene::LeaveScene, this));



	ui::Button *Btn_Info = dynamic_cast<ui::Button*>(uilayer->getChildByName("Info"));
	Btn_Info->addTouchEventListener(CC_CALLBACK_2(StartScene::InfoScene, this));

	ui::Button *Btn_Set = dynamic_cast<ui::Button*>(uilayer->getChildByName("Set"));
	Btn_Set->addTouchEventListener(CC_CALLBACK_2(StartScene::SetScene, this));


	return true;
}




void StartScene::SingleScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto scene = SingleScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
}

void StartScene::MultiScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto scene = MultiScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
}

void StartScene::LeaveScene(Ref *pSender, ui::Widget::TouchEventType type)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	
}

void StartScene::InfoScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto scene = AboutScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
}

void StartScene::SetScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto scene = SetScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
}

