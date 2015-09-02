#include "SetScene.h"
#include "StartScene.h"
#include "AudioControl.h"
#include "AboutScene.h"

USING_NS_CC;


Scene* SetScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SetScene::create();
	scene->addChild(layer);
	return scene;
}

bool SetScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Set/SetUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x, origin.y));

	this->addChild(uilayer);

	//给开始按钮添加事件监听  
	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(SetScene::returnCallback, this));

	ui::Button *Btn_Info = dynamic_cast<ui::Button*>(uilayer->getChildByName("Info"));
	Btn_Info->addTouchEventListener(CC_CALLBACK_2(SetScene::infoScene, this));

	ui::CheckBox *Check_Effect = dynamic_cast<ui::CheckBox*>(uilayer->getChildByName("Effect"));
	Check_Effect->addEventListenerCheckBox(this, ui::SEL_SelectedStateEvent(&SetScene::effectCallback));

	ui::CheckBox *Check_BGM = dynamic_cast<ui::CheckBox*>(uilayer->getChildByName("BGM"));
	Check_BGM->addEventListenerCheckBox(this, ui::SEL_SelectedStateEvent(&SetScene::BGMCallback));

	if (AudioControl::isEffectStopped)
	{
		Check_Effect->setSelectedState(true);
	}
	if (AudioControl::isBGMusicStopped)
	{
		Check_BGM->setSelectedState(true);
	}

	return true;
}



void SetScene::returnCallback(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void SetScene::infoScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = AboutScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void SetScene::effectCallback(Ref *pSender, ui::CheckBoxEventType type)
{
	AudioControl::isEffectStopped = !AudioControl::isEffectStopped;
	if (AudioControl::isEffectStopped)
	{
		AudioControl::stopMCEffects();
	}
}

void SetScene::BGMCallback(Ref *pSender, ui::CheckBoxEventType type)
{
	AudioControl::isBGMusicStopped = !AudioControl::isBGMusicStopped;
	if (AudioControl::isBGMusicStopped)
	{
		AudioControl::stopBGMusic();
	}
}



