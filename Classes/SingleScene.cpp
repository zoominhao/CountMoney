#include "SingleScene.h"
#include "StartScene.h"
#include "LoserScene.h"
#include "TellerScene.h"
#include "EndlessScene.h"



Scene* SingleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SingleScene::create();
	scene->addChild(layer);
	return scene;
}

bool SingleScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Single/SingleUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x + visibleSize.width / 2 - uilayer->getContentSize().width / 2,
		origin.y + visibleSize.height / 2 - uilayer->getContentSize().height / 2));

	this->addChild(uilayer);


	//给开始按钮添加事件监听  

	ui::Button *Btn_DS = dynamic_cast<ui::Button*>(uilayer->getChildByName("DSMode"));
	Btn_DS->addTouchEventListener(CC_CALLBACK_2(SingleScene::DSScene, this));

	ui::Button *Btn_GY = dynamic_cast<ui::Button*>(uilayer->getChildByName("GYMode"));
	Btn_GY->addTouchEventListener(CC_CALLBACK_2(SingleScene::GYScene, this));


	ui::Button *Btn_WJ = dynamic_cast<ui::Button*>(uilayer->getChildByName("WJMode"));
	Btn_WJ->addTouchEventListener(CC_CALLBACK_2(SingleScene::WJScene, this));

	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(SingleScene::StartScene, this));

	return true;
}




void SingleScene::DSScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = LoserScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::GYScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = TellerScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::WJScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = EndlessScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::StartScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}


