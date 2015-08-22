#include "EndlessPauseScene.h"
#include "EndlessScene.h"



Scene* EndlessPauseScene::createScene(RenderTexture* sqr, int score)
{
	auto scene = Scene::create();
	auto layer = EndlessPauseScene::create();
	scene->addChild(layer, 1);

	char scoreStr[20];
	sprintf(scoreStr, "%d", score);
	auto scoreLabel = Label::createWithTTF(scoreStr, "fonts/Marker Felt.ttf", 70);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 80);
	scoreLabel->setColor(Color3B(255.0, 0.0, 255.0));

	scene->addChild(scoreLabel, 1);

	//�����������ͼ��ӵ�GameEnd��������  
	//�õ����ڵĴ�С  
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	//Sprite *back_spr = Sprite::create("bg_pic.jpg");
	back_spr->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2 + 15)); //����λ��,������������λ�á�  
	back_spr->setFlipY(true);            //��ת����ΪUI�����OpenGL���겻ͬ  
	back_spr->setColor(Color3B::GRAY); //ͼƬ��ɫ���ɫ  
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


	auto continueItem = MenuItemImage::create(
		"start_icon1.png",
		"start_icon2.png",
		CC_CALLBACK_1(EndlessPauseScene::continueCallback, this));

	continueItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - continueItem->getContentSize().width / 2,
		origin.y + visibleSize.height / 2));


	auto restartItem = MenuItemImage::create(
		"restart_icon1.png",
		"restart_icon2.png",
		CC_CALLBACK_1(EndlessPauseScene::restartCallback, this));

	restartItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + restartItem->getContentSize().width / 2,
		origin.y + visibleSize.height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(continueItem, restartItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	return true;
}




void EndlessPauseScene::continueCallback(Ref *pSender)
{
	Director::sharedDirector()->popScene();
}

void EndlessPauseScene::restartCallback(Ref *pSender)
{
	auto scene = EndlessScene::createScene();
	Director::getInstance()->replaceScene(scene);
}



