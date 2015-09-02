#include "EndlessPauseScene.h"
#include "EndlessScene.h"
#include "AudioControl.h"
#include "SingleScene.h"


Scene* EndlessPauseScene::createScene(RenderTexture* sqr, int score)
{
	auto scene = Scene::create();
	auto layer = EndlessPauseScene::create();
	scene->addChild(layer, 1);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	/*char scoreStr[20];
	sprintf(scoreStr, "%d", score);
	auto scoreLabel = Label::createWithTTF(scoreStr, "fonts/Marker Felt.ttf", 70);
	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 80);
	scoreLabel->setColor(Color3B(255.0, 0.0, 255.0));
	scene->addChild(scoreLabel, 1);*/

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

bool EndlessPauseScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("pause/PauseUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x, origin.y));

	this->addChild(uilayer);

	//给开始按钮添加事件监听  
	ui::Button *Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(EndlessPauseScene::returnCallback, this));

	ui::Button *Btn_Continue = dynamic_cast<ui::Button*>(uilayer->getChildByName("Continue"));
	Btn_Continue->addTouchEventListener(CC_CALLBACK_2(EndlessPauseScene::continueCallback, this));

	ui::Button *Btn_Restart = dynamic_cast<ui::Button*>(uilayer->getChildByName("Restart"));
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(EndlessPauseScene::restartCallback, this));

	return true;
}


void EndlessPauseScene::continueCallback(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->popScene();
		AudioControl::playBgMusic(ENDLESS);
	}
}


void EndlessPauseScene::restartCallback(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = EndlessScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void EndlessPauseScene::returnCallback(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = SingleScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}



