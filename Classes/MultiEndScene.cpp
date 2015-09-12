#include "MultiEndScene.h"
#include "StartScene.h"
#include "MultiScene.h"
#include "SimpleAudioEngine.h"
 

using namespace CocosDenshion;


Scene* MultiEndScene::createScene(const char* resultstr)
{
	auto scene = Scene::create();
	auto layer = MultiEndScene::create();
	scene->addChild(layer, 1);


	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Sprite *scoreSp;
	if (strcmp(resultstr, "0") == 0)
		scoreSp = Sprite::create("multi/tie.png");
	else if (strcmp(resultstr, "1") == 0)
		scoreSp = Sprite::create("multi/player1win.png");
	else
		scoreSp = Sprite::create("multi/player2win.png");

	scoreSp->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 200);
	scene->addChild(scoreSp, 1);
	/*auto scoreLabel = Label::createWithTTF(resultstr, "fonts/Marker Felt.ttf", 24);
	//添加得分
	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height  - 200);
	//scoreLabel->setColor(Color3B(255.0, 255.0, 255.0));
	scoreLabel->setColor(Color3B::GRAY);

	scene->addChild(scoreLabel, 1);*/

	return scene;
}

bool MultiEndScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("End/EndUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x + visibleSize.width / 2 - uilayer->getContentSize().width / 2, 
		origin.y + visibleSize.height / 2 - uilayer->getContentSize().height / 2));
	this->addChild(uilayer);
	

	//给开始按钮添加事件监听  

	ui::Button *Btn_Restart = dynamic_cast<ui::Button*>(uilayer->getChildByName("Restart"));
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(MultiEndScene::restartButton, this));

	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(MultiEndScene::returnButton, this));

	return true;
}

void MultiEndScene::restartButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = MultiScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void MultiEndScene::returnButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
/*
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		CCLOG("Button Down");//按钮按下  
		break;
	case ui::Widget::TouchEventType::MOVED:
		CCLOG("Button Down and Mouse move");//按钮按下移动  
		break;
	case ui::Widget::TouchEventType::ENDED:
		CCLOG("Button Up");//放开按钮  
		
		break;
	case ui::Widget::TouchEventType::CANCELED:
		CCLOG("Button Cancel");//取消按钮  
		break;
	default:
		break;
	}*/
}
