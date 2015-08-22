#include "AboutScene.h"
#include "StartScene.h"



Scene* AboutScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;
}

bool AboutScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("About/AboutUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x, origin.y));

	this->addChild(uilayer);


	//给开始按钮添加事件监听  
	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(AboutScene::StartScene, this));


	return true;
}




void AboutScene::StartScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto scene = StartScene::createScene();
	Director::sharedDirector()->replaceScene(scene);
}



