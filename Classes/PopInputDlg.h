#ifndef _POP_INPUT_SCENE_H_
#define _POP_INPUT_SCENE_H_


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LoserEndScene.h"

USING_NS_CC;

class PopInputDlg : public LayerColor
{
public:
	static Layer * createLayer(GameMode gmode, int score);
	virtual bool init();
	CREATE_FUNC(PopInputDlg);

private:
	virtual bool onTouchBegan(Touch* touch, Event* event);

	void onEscButton(Ref* pSender, ui::Widget::TouchEventType type);

	ui::TextField *m_Input_Name_Text;

	static int m_score;
	static GameMode m_gmode;

	bool checkCharacter(const char* username);
	std::string& trim(std::string& s);

};
#endif


/*
auto scene = PopInputDlg::createScene();
this->addChild(scene);*/