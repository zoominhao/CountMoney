#include "OnlineEndScene.h"
#include "StartScene.h"
#include "OnlineScene.h"
#include "SimpleAudioEngine.h"
 

using namespace CocosDenshion;


Scene* OnlineEndScene::createScene(const char* resultstr)
{
	auto scene = Scene::create();
	auto layer = OnlineEndScene::create();
	scene->addChild(layer, 1);

	SimpleAudioEngine::sharedEngine()->stopAllEffects();

	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加得分
/*
	auto scoreLabel = Label::createWithTTF(resultstr, "fonts/DTLNobelT-Bold.otf", 50);
	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 200);
	//scoreLabel->setColor(Color3B(255.0, 255.0, 255.0));
	scoreLabel->setColor(Color3B::GRAY);
	layer->addChild(scoreLabel, 1);*/

	if (strcmp(resultstr, "0") == 0)
		layer->addWinSprite(1);
	else if (strcmp(resultstr, "1") == 0)
		layer->addWinSprite(1);
	else
		layer->addWinSprite(2);

	WebClient::getinstance().shutdown();

	return scene;
}

bool OnlineEndScene::init()
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
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(OnlineEndScene::restartButton, this));

	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(OnlineEndScene::returnButton, this));

	return true;
}

void OnlineEndScene::restartButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = OnlineScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void OnlineEndScene::returnButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void OnlineEndScene::addWinSprite(int whichplayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (whichplayer == 1)
	{
		auto p1 = Sprite::create("multi/boy_win.png");
		p1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100);
		this->addChild(p1, 1);
	
	}
	if (whichplayer == 2)
	{
		auto p1 = Sprite::create("multi/boy_lose.png");
		p1->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50);
		this->addChild(p1, 1);
	}
}
