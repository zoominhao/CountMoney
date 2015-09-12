#include "SingleScene.h"
#include "StartScene.h"
#include "LoserScene.h"
#include "TellerScene.h"
#include "EndlessScene.h"
#include "AudioControl.h"
#include "MCManual.h"
#include "LoserManualScene.h"
#include "TellerManualScene.h"
#include "EndlessManualScene.h"



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

	//����cocos studio�����Ľ���  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Single/SingleUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x + visibleSize.width / 2 - uilayer->getContentSize().width / 2,
		origin.y + visibleSize.height / 2 - uilayer->getContentSize().height / 2));

	this->addChild(uilayer);

	AudioControl::stopBGMusic();
	AudioControl::playBgMusic(LOGIN);

	//����ʼ��ť����¼�����  
	ui::Button *Btn_Double = dynamic_cast<ui::Button*>(uilayer->getChildByName("DoubleMode"));
	Btn_Double->addTouchEventListener(CC_CALLBACK_2(SingleScene::StartScene, this));

	ui::Button *Btn_DS = dynamic_cast<ui::Button*>(uilayer->getChildByName("DSMode"));
	Btn_DS->addTouchEventListener(CC_CALLBACK_2(SingleScene::DSScene, this));

	ui::Button *Btn_GY = dynamic_cast<ui::Button*>(uilayer->getChildByName("GYMode"));
	Btn_GY->addTouchEventListener(CC_CALLBACK_2(SingleScene::GYScene, this));

	ui::Button *Btn_WJ = dynamic_cast<ui::Button*>(uilayer->getChildByName("WJMode"));
	Btn_WJ->addTouchEventListener(CC_CALLBACK_2(SingleScene::WJScene, this));

	//manual
	ui::Button *Btn_DS_Manual = dynamic_cast<ui::Button*>(uilayer->getChildByName("Loser_Manual"));
	Btn_DS_Manual->addTouchEventListener(CC_CALLBACK_2(SingleScene::DSManualScene, this));

	ui::Button *Btn_GY_Manual = dynamic_cast<ui::Button*>(uilayer->getChildByName("Teller_Manual"));
	Btn_GY_Manual->addTouchEventListener(CC_CALLBACK_2(SingleScene::GYManualScene, this));

	ui::Button *Btn_WJ_Manual = dynamic_cast<ui::Button*>(uilayer->getChildByName("Endless_Manual"));
	Btn_WJ_Manual->addTouchEventListener(CC_CALLBACK_2(SingleScene::WJManualScene, this));


	return true;
}

void SingleScene::DSScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = LoserScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::GYScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = TellerScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::WJScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		AudioControl::playClickEffect();
		auto scene = EndlessScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::StartScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = StartScene::createScene();
		Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0, scene, false));
	}
}

void SingleScene::DSManualScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = LoserManualScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void SingleScene::GYManualScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = TellerManualScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void SingleScene::WJManualScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = EndlessManualScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}





