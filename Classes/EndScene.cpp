#include "EndScene.h"
#include "StartScene.h"
#include "TellerScene.h"
#include "SimpleAudioEngine.h"
 

using namespace CocosDenshion;


Scene* EndScene::createScene(RenderTexture* sqr, int score)
{
	auto scene = Scene::create();
	auto layer = EndScene::create();
	scene->addChild(layer, 1);

	SimpleAudioEngine::sharedEngine()->stopAllEffects();

	char scoreStr[20];
	sprintf(scoreStr, "%d", score);
	auto scoreLabel = Label::createWithTTF(scoreStr, "fonts/Marker Felt.ttf", 24);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加得分
	scoreLabel->setPosition(origin.x + visibleSize.width / 3, origin.y + 20);
	scoreLabel->setColor(Color3B(255.0, 255.0, 255.0));

	scene->addChild(scoreLabel, 1);


	//并结束界面截图添加到GameEnd场景层中  
	//得到窗口的大小  
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	//Sprite *back_spr = Sprite::create("bg_pic.jpg");
	back_spr->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2 + 15)); //放置位置,这个相对于中心位置。  
	back_spr->setFlipY(true);            //翻转，因为UI坐标和OpenGL坐标不同  
	back_spr->setColor(Color3B::GRAY); //图片颜色变灰色  
	//back_spr->setColor(Color3B(255,0,0));
	scene->addChild(back_spr);

	return scene;
}

bool EndScene::init()
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
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(EndScene::restartButton, this));

	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(EndScene::returnButton, this));

	return true;
}

void EndScene::restartButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//auto scene = TellerScene::createScene();
		//Director::getInstance()->replaceScene(scene);

		//Director::sharedDirector()->popScene();
	}
}

void EndScene::returnButton(Ref *pSender, ui::Widget::TouchEventType type)
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
