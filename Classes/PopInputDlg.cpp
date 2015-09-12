#include "PopInputDlg.h"
#include "RankScene.h"


GameMode PopInputDlg::m_gmode = DS;
int PopInputDlg::m_score = 0;

Scene* PopInputDlg::createScene(GameMode gmode, int score)
{
	auto scene = Scene::create();
	auto layer = PopInputDlg::create();
	scene->addChild(layer);

	m_gmode = gmode;
	m_score = score;
	return scene;
}

bool PopInputDlg::init()
{
	if (!LayerColor::initWithColor(ccc4(25, 25, 25, 125))) 
		return false;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	//加载cocos studio制作的界面  
	auto uilayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("inputname/InputNameUi_1.ExportJson");
	uilayer->setPosition(ccp(origin.x, origin.y - 50));

	this->addChild(uilayer);

	//给开始按钮添加事件监听  
	ui::Button *Btn_Input_Esc = dynamic_cast<ui::Button*>(uilayer->getChildByName("Input_esc"));
	Btn_Input_Esc->addTouchEventListener(CC_CALLBACK_2(PopInputDlg::onEscButton, this));

	//获得输入框
	m_Input_Name_Text = dynamic_cast<ui::TextField*>(uilayer->getChildByName("InputName"));


	//保证是模态对话框
	auto touchlistenter = EventListenerTouchOneByOne::create();
	touchlistenter->setSwallowTouches(true);
	touchlistenter->onTouchBegan = CC_CALLBACK_2(PopInputDlg::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistenter, this);

	return true;
}

void PopInputDlg::onEscButton(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string username = m_Input_Name_Text->getStringValue();
		if (trim(username).empty() || trim(username)==" ")
		{
			MessageBox("姓名不能为空", "提示");
		}
		else if (checkCharacter(username.c_str()))
		{
			MessageBox("请输入RTX的英文名", "提示");
		}
		else
		{
			WebClient::getinstance().updateScore(m_gmode, username, m_score);
			/*auto scene = RankScene::createScene();
			Director::getInstance()->replaceScene(scene);*/
			this->removeFromParentAndCleanup(true);
		}
	}
}


bool PopInputDlg::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

bool PopInputDlg::checkCharacter(const char* username)
{
	while (*username)
	{
		if ((*username) & 0x80){
			//是汉字
			return true;
		}
		username++;
	}
	return false;
}

std::string& PopInputDlg::trim(std::string& s) {
	if (s.empty()) {
	    return s;
	}

	std::string::iterator c;
	// Erase whitespace before the string
	for (c = s.begin(); c != s.end() && iswspace(*c++););
	     s.erase(s.begin(), --c);
	
	// Erase whitespace after the string
	for (c = s.end(); c != s.begin() && iswspace(*--c););
	    s.erase(++c, s.end());
	
	return s;
}