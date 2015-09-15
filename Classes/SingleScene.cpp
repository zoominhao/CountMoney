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

	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Single/SingleUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x + visibleSize.width / 2 - uilayer->getContentSize().width / 2,
		origin.y + visibleSize.height / 2 - uilayer->getContentSize().height / 2));

	this->addChild(uilayer);

	AudioControl::stopBGMusic();
	AudioControl::playBgMusic(LOGIN);

	//给开始按钮添加事件监听  
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

	AudioControl::stopBGMusic();
	AudioControl::playBgMusic(LOGIN);


	boatAnimation();

	return true;
}

void SingleScene::DSScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = LoserScene::createScene();
		Director::sharedDirector()->replaceScene(TransitionZoomFlipX::create(0.5, scene));
	}
}

void SingleScene::GYScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = TellerScene::createScene();
		Director::sharedDirector()->replaceScene(TransitionZoomFlipX::create(0.5, scene));
	}
}

void SingleScene::WJScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		AudioControl::playClickEffect();
		auto scene = EndlessScene::createScene();
		Director::sharedDirector()->replaceScene(TransitionZoomFlipX::create(0.5, scene));
	}
}

void SingleScene::StartScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		AudioControl::playClickEffect();
		auto scene = StartScene::createScene();
		Director::sharedDirector()->replaceScene(TransitionZoomFlipX::create(0.5, scene, TransitionScene::Orientation::LEFT_OVER));
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

void SingleScene::boatAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto boatSp = Sprite::create();
	boatSp->setPosition(origin.x + visibleSize.width / 2, origin.y + 100);
	boatSp->setName("boatsp");
	this->addChild(boatSp, 4);

	SpriteFrameCache *m_frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_frameCache->addSpriteFramesWithFile("Single/boat_animation.plist", "Single/boat_animation.png");
	Vector<SpriteFrame*> frameArray;
	for (unsigned int i = 1; i <= 2; i++)
	{
		SpriteFrame* frame = m_frameCache->spriteFrameByName(String::createWithFormat("boat_%d.png", i)->getCString());
		frameArray.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(frameArray);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.5f);

	Animate* boat_act = Animate::create(animation);

	boatSp->runAction(boat_act);
}





