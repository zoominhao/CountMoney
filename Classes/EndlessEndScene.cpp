#include "EndlessEndScene.h"
#include "StartScene.h"
#include "EndlessScene.h"
#include "SimpleAudioEngine.h"
#include "PopInputDlg.h"
 

using namespace CocosDenshion;

int EndlessEndScene::m_stage = 0;

EndlessEndScene::~EndlessEndScene()
{
	WebClient::getinstance().shutdown();
}

Scene* EndlessEndScene::createScene(const char* resultstr, int result)
{
	auto scene = Scene::create();
	auto layer = EndlessEndScene::create();
	scene->addChild(layer, 1);


	SimpleAudioEngine::sharedEngine()->stopAllEffects();


	auto scoreLabel = Label::createWithTTF(resultstr, "fonts/Marker Felt.ttf", 50);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加得分
	scoreLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 200);
	//scoreLabel->setColor(Color3B(255.0, 255.0, 255.0));
	scoreLabel->setColor(Color3B::GRAY);

	scene->addChild(scoreLabel, 1);

	m_stage = result;
	WebClient::getinstance().sendScore(WJ, m_stage);

	return scene;
}

bool EndlessEndScene::init()
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

	Btn_Restart = dynamic_cast<ui::Button*>(uilayer->getChildByName("Restart"));
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(EndlessEndScene::restartButton, this));
	Btn_Restart->setEnabled(false);

	Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(EndlessEndScene::returnButton, this));
	Btn_Return->setEnabled(false);

	WebClient::getinstance().registerMethod(this);
	WebClient::getinstance().start();
	isPop = false;
	scheduleOnce(schedule_selector(EndlessEndScene::updatePopDlg), 0.1f);

	return true;
}

void EndlessEndScene::restartButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = EndlessScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void EndlessEndScene::returnButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void EndlessEndScene::onSendScore(bool levelup) {
	//log("onSendScore");
	if (levelup)
	{
		//CCLOG("true");
		isPop = levelup;
	}

}


void EndlessEndScene::onError(const std::string message){
	// TODO: 在这里加网络异常messagebox处理提示,只要message不为空，则提示用户网络异常
	//log("onError: ");
	//log(message.c_str());
	MessageBox(message.c_str(), "Info");
};

void EndlessEndScene::updatePopDlg(float time)
{
	if (isPop)
	{
		isPop = false;
		auto scene = PopInputDlg::createScene(WJ, m_stage);
		this->addChild(scene);
	}

	Btn_Restart->setEnabled(true);
	Btn_Return->setEnabled(true);
}