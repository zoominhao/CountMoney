#include "EndlessEndScene.h"
#include "StartScene.h"
#include "EndlessScene.h"
#include "SimpleAudioEngine.h"
 

using namespace CocosDenshion;


Scene* EndlessEndScene::createScene(const char* resultstr)
{
	auto scene = Scene::create();
	auto layer = EndlessEndScene::create();
	scene->addChild(layer, 1);


	SimpleAudioEngine::sharedEngine()->stopAllEffects();


	auto scoreLabel = Label::createWithTTF(resultstr, "fonts/Marker Felt.ttf", 50);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//��ӵ÷�
	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 200);
	//scoreLabel->setColor(Color3B(255.0, 255.0, 255.0));
	scoreLabel->setColor(Color3B::GRAY);

	scene->addChild(scoreLabel, 1);

	return scene;
}

bool EndlessEndScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//����cocos studio�����Ľ���  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("End/EndUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x + visibleSize.width / 2 - uilayer->getContentSize().width / 2, 
		origin.y + visibleSize.height / 2 - uilayer->getContentSize().height / 2));
	this->addChild(uilayer);
	

	//����ʼ��ť����¼�����  

	ui::Button *Btn_Restart = dynamic_cast<ui::Button*>(uilayer->getChildByName("Restart"));
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(EndlessEndScene::restartButton, this));

	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(EndlessEndScene::returnButton, this));

	return true;
}

void EndlessEndScene::restartButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = EndlessScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void EndlessEndScene::returnButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}
