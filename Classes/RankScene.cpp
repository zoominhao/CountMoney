#include "RankScene.h"
#include "StartScene.h"
#include "LoserScene.h"
#include "LoserPauseScene.h"
#include "SingleScene.h"
#include "MCUtil.h"
#include "AudioControl.h"
#include <sstream>


Scene* RankScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RankScene::create();
	scene->addChild(layer);


	return scene;
}

RankScene::~RankScene()
{
	WebClient::getinstance().shutdown();
}

bool RankScene::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Rank/RankUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x, origin.y));

	this->addChild(uilayer);

	//给开始按钮添加事件监听  
	m_Btn_Loser = dynamic_cast<ui::Button*>(uilayer->getChildByName("Rank_Loser"));
	m_Btn_Loser->addTouchEventListener(CC_CALLBACK_2(RankScene::RankLoserScene, this));
	m_Btn_Loser->setVisible(false);

	m_Btn_Teller = dynamic_cast<ui::Button*>(uilayer->getChildByName("Rank_Teller"));
	m_Btn_Teller->addTouchEventListener(CC_CALLBACK_2(RankScene::RankTellerScene, this));

	m_Btn_Endless = dynamic_cast<ui::Button*>(uilayer->getChildByName("Rank_Endless"));
	m_Btn_Endless->addTouchEventListener(CC_CALLBACK_2(RankScene::RankEndlessScene, this));

	ui::Button *Rank_Esc_btn = dynamic_cast<ui::Button*>(uilayer->getChildByName("Rank_Esc_btn"));
	Rank_Esc_btn->addTouchEventListener(CC_CALLBACK_2(RankScene::StartScene, this));

	
	
	m_label_name = Label::createWithTTF("", "fonts/arial.ttf", 24);
	m_label_name->setVerticalAlignment(TextVAlignment::TOP);
	m_label_name->setLineHeight(65);
	m_label_name->setTextColor(Color4B(0, 0, 0, 255));
	m_label_name->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2 - 50));

	this->addChild(m_label_name, 1);


	m_label_score = Label::createWithTTF("", "fonts/arial.ttf", 24);
	m_label_score->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
	m_label_score->setLineHeight(65);
	m_label_score->setTextColor(Color4B(0, 0, 0, 255));
	m_label_score->setPosition(Vec2(origin.x + visibleSize.width / 2 + 130,
		origin.y + visibleSize.height / 2 - 50));

	this->addChild(m_label_score, 1);

	WebClient::getinstance().registerMethod(this);
	WebClient::getinstance().start();
	WebClient::getinstance().query(DS);

	return true;
}




void RankScene::StartScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		auto scene = StartScene::createScene();
		Director::sharedDirector()->replaceScene(scene);
	}
}

void RankScene::RankLoserScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_Btn_Loser->setVisible(false);
		m_Btn_Teller->setVisible(true);
		m_Btn_Endless->setVisible(true);
		WebClient::getinstance().query(DS);
	}
}

void RankScene::RankTellerScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_Btn_Loser->setVisible(true);
		m_Btn_Teller->setVisible(false);
		m_Btn_Endless->setVisible(true);
		WebClient::getinstance().query(GY);
	}
}

void RankScene::RankEndlessScene(Ref *pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		m_Btn_Loser->setVisible(true);
		m_Btn_Teller->setVisible(true);
		m_Btn_Endless->setVisible(false);
		WebClient::getinstance().query(WJ);
	}
}


void RankScene::onQuery(GameMode mode, std::vector<std::string> result) {
	//comment log
	/*int size = result.size();
	log("onQuery mode = %d", mode);
	for (int i = 0; i < size; i += 2) {
		log("name:%s\tscore:%s", result[i].c_str(), result[i + 1].c_str());
	}*/
	setLabelContent(result);
}

void RankScene::setLabelContent(std::vector<std::string> &rankStrs)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	std::string namestr = "";
	std::string scorestr = "";
	int rankStrsSize = 20 - rankStrs.size();
	for (int i = 0; i < rankStrsSize; i++)
	{
		rankStrs.push_back("");
	}
	for (int i = 0; i < rankStrs.size(); i = i + 2)
	{
		namestr.append(rankStrs[i]);
		namestr.append("\r\n");
		scorestr.append(rankStrs[i + 1]);
		scorestr.append("\r\n");
	}
	

	m_label_name->setString(namestr);
	m_label_score->setString(scorestr);

}

void RankScene::onError(const std::string message){
	// TODO: 在这里加网络异常messagebox处理提示,只要message不为空，则提示用户网络异常
	//log("onError: ");
	//log(message.c_str());
	//MessageBox(message.c_str(), "Info");
};

void RankScene::startConnect(GameMode mode)
{
	WebClient::getinstance().registerMethod(this);
	WebClient::getinstance().start();

	switch (mode)
	{
	case DS:
		m_Btn_Loser->setVisible(false);
		m_Btn_Teller->setVisible(true);
		m_Btn_Endless->setVisible(true);
		WebClient::getinstance().query(DS);
		break;
	case GY:
		m_Btn_Loser->setVisible(true);
		m_Btn_Teller->setVisible(false);
		m_Btn_Endless->setVisible(true);
		WebClient::getinstance().query(GY);
		break;
	case WJ:
		m_Btn_Loser->setVisible(true);
		m_Btn_Teller->setVisible(true);
		m_Btn_Endless->setVisible(false);
		WebClient::getinstance().query(WJ);
		break;
	case PK:
		break;
	default:
		break;
	}
	{
	}
}
