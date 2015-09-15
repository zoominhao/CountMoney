#include "TellerEndScene.h"
#include "StartScene.h"
#include "TellerScene.h"
#include "SimpleAudioEngine.h"
#include "PopInputDlg.h"
 

using namespace CocosDenshion;

int TellerEndScene::m_score = 0;

TellerEndScene::~TellerEndScene()
{
	WebClient::getinstance().shutdown();
}

Scene* TellerEndScene::createScene(const char* resultstr, int result)
{
	auto scene = Scene::create();
	auto layer = TellerEndScene::create();
	scene->addChild(layer, 1);


	SimpleAudioEngine::sharedEngine()->stopAllEffects();


	auto scoreLabel = Label::createWithTTF(resultstr, "fonts/DTLNobelT-Bold.otf", 50);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加得分
	scoreLabel->setPosition(origin.x + visibleSize.width / 2 - 10, origin.y + visibleSize.height / 2 + 50);
	scoreLabel->setColor(Color3B::WHITE);

	layer->addChild(scoreLabel, 1);

	m_score = result;
	WebClient::getinstance().sendScore(GY, m_score);

	return scene;
}

bool TellerEndScene::init()
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
	Btn_Restart->addTouchEventListener(CC_CALLBACK_2(TellerEndScene::restartButton, this));
	Btn_Restart->setEnabled(false);

	Btn_Return = dynamic_cast<ui::Button*>(uilayer->getChildByName("Return"));
	Btn_Return->addTouchEventListener(CC_CALLBACK_2(TellerEndScene::returnButton, this));
	Btn_Return->setEnabled(false);

	WebClient::getinstance().registerMethod(this);
	WebClient::getinstance().start();
	isPop = false;
	scheduleOnce(schedule_selector(TellerEndScene::updatePopDlg), 0.1f);

	return true;
}

void TellerEndScene::restartButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = TellerScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}

}

void TellerEndScene::returnButton(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
}

void TellerEndScene::onSendScore(bool levelup) {
	//log("onSendScore");
	if (levelup)
	{
		//CCLOG("true");
		isPop = levelup;
	}

}


void TellerEndScene::onError(const std::string message){
	// TODO: 在这里加网络异常messagebox处理提示,只要message不为空，则提示用户网络异常
	//log("onError: ");
	//log(message.c_str());
	MessageBox(message.c_str(), "Info");
};

void TellerEndScene::updatePopDlg(float time)
{
	if (isPop)
	{
		isPop = false;
		auto poplayer = PopInputDlg::createLayer(GY, m_score);
		this->addChild(poplayer, 2);
	}

	Btn_Restart->setEnabled(true);
	Btn_Return->setEnabled(true);

}
