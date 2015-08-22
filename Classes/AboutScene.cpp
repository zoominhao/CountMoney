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


	//����cocos studio�����Ľ���  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("About/AboutUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x, origin.y));

	this->addChild(uilayer);


	//����ʼ��ť����¼�����  
	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(AboutScene::StartScene, this));


	return true;
}




void AboutScene::StartScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	auto scene = StartScene::createScene();
	Director::sharedDirector()->replaceScene(scene);
}



