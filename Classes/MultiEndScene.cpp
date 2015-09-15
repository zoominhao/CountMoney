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

	if (strcmp(resultstr, "0") == 0)
		layer->addWinSprite(1);
	else if (strcmp(resultstr, "1") == 0)
		layer->addWinSprite(1);
	else
		layer->addWinSprite(2);

	return scene;
}

bool MultiEndScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("multiEnd/EndUi_1.ExportJson");
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
}

void MultiEndScene::addWinSprite(int whichplayer)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (whichplayer == 1)
	{
		auto p1 = Sprite::create("multi/boy_win.png");
		p1->setPosition(origin.x + visibleSize.width / 2, origin.y + 420);
		this->addChild(p1, 1);

		auto p2 = Sprite::create("multi/duck_lose.png");
		p2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 250);
		this->addChild(p2, 1);

		auto u1 = Sprite::create("multi/uwin.png");
		u1->setPosition(origin.x + visibleSize.width / 2 + 40, origin.y + 240);
		this->addChild(u1, 1);

		auto u2 = Sprite::create("multi/ulose.png");
		u2->setRotation(180);
		u2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100);
		this->addChild(u2, 1);
	}
	if (whichplayer == 2)
	{
		auto p1 = Sprite::create("multi/boy_lose.png");
		p1->setPosition(origin.x + visibleSize.width / 2, origin.y + 400);
		this->addChild(p1, 1);

		auto p2 = Sprite::create("multi/duck_win.png");
		p2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 250);
		this->addChild(p2, 1);

		auto u1 = Sprite::create("multi/ulose.png");
		u1->setPosition(origin.x + visibleSize.width / 2, origin.y + 250);
		this->addChild(u1, 1);

		auto u2 = Sprite::create("multi/uwin.png");
		u2->setRotation(180);
		u2->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100);
		this->addChild(u2, 1);
	}
}
