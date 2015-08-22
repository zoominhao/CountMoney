#include "MultiPauseScene.h"



Scene* MultiPauseScene::createScene(RenderTexture* sqr, int score1, int score2)
{
	auto scene = Scene::create();
	auto layer = MultiPauseScene::create();
	scene->addChild(layer, 1);

	char scoreStr[50];
	sprintf(scoreStr, "Player1: %d\t Player2: %d", score1, score2);
	auto scoreLabel = Label::createWithTTF(scoreStr, "fonts/Marker Felt.ttf", 50);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	scoreLabel->setColor(Color3B(255.0, 0.0, 255.0));
	scoreLabel->setRotation(90);
	scene->addChild(scoreLabel, 1);

	//并结束界面截图添加到GameEnd场景层中  
	//得到窗口的大小  
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	//Sprite *back_spr = Sprite::create("bg_pic.jpg");
	back_spr->setPosition(ccp(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 15)); //放置位置,这个相对于中心位置。  

	back_spr->setFlipY(true);            //翻转，因为UI坐标和OpenGL坐标不同  
	back_spr->setColor(Color3B::GRAY); //图片颜色变灰色  
	//back_spr->setColor(Color3B(255,0,0));
	scene->addChild(back_spr);

	return scene;
}

bool MultiPauseScene::init()
{
	if (!Layer::init())
		return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	auto continueItem = MenuItemImage::create(
		"start_icon1.png",
		"start_icon2.png",
		CC_CALLBACK_1(MultiPauseScene::continueCallback, this));

	continueItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - continueItem->getContentSize().width / 2,
		origin.y + visibleSize.height / 2));


	// create menu, it's an autorelease object
	auto menu = Menu::create(continueItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	return true;
}




void MultiPauseScene::continueCallback(Ref *pSender)
{
	Director::sharedDirector()->popScene();
}



