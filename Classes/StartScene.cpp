#include "StartScene.h"
#include "TellerScene.h"
#include "LoserScene.h"
#include "MultiScene.h"
#include "MCUtil.h"

USING_NS_CC;

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
	//添加进入游戏和关闭游戏的按钮
	auto menuitem_loser = MenuItemImage::create("Loser_Scene.png", "Loser_Scene.png", CC_CALLBACK_1(StartScene::enter_loserScene, this));
	menuitem_loser->setPosition(visibleSize.width / 2, 380);

	auto menuitem_teller = MenuItemImage::create("Teller_Scene.png", "Teller_Scene.png", CC_CALLBACK_1(StartScene::enter_tellerScene, this));
	menuitem_teller->setPosition(visibleSize.width / 2, 300);

	auto menuitem_multi = MenuItemImage::create("Multi_Scene.png", "Multi_Scene.png", CC_CALLBACK_1(StartScene::enter_multiScene, this));
	menuitem_multi->setPosition(visibleSize.width / 2, 220);

	auto menuitem_end = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(StartScene::game_over, this));
	menuitem_end->setPosition(visibleSize.width / 2, 100);

	auto menu = Menu::create(menuitem_teller, menuitem_loser, menuitem_multi, menuitem_end, NULL);
	menu->setPosition(Vect::ZERO);
	this->addChild(menu,1);


	auto sprite = Sprite::create("start.jpg");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 0);

	return true;
}

void StartScene::enter_tellerScene(Ref* ref)
{
	auto scene = TellerScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(2, scene, false));
}

void StartScene::enter_loserScene(Ref* ref)
{
	auto scene = LoserScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(2, scene, false));
}

void StartScene::enter_multiScene(Ref* ref)
{
	auto scene = MultiScene::createScene();
	Director::sharedDirector()->replaceScene(CCTransitionPageTurn::create(2, scene, false));
}


void StartScene::game_over(Ref* ref)
{
	Director::getInstance()->end();
}

