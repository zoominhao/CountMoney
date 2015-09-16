#ifndef _ENDLESS_END_SCENE_H_
#define _ENDLESS_END_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "WebClient.h"

USING_NS_CC;

class EndlessEndScene : public Layer, public WebClientMethod
{
public:
	~EndlessEndScene();
	static Scene* createScene(const char* resultstr, int result);
	virtual bool init();
	CREATE_FUNC(EndlessEndScene);

	virtual void onFight(int type, bool toMe, int enemyScore){
		// TODO:
		//WebClient::getinstance().shutdown();
	}
	virtual void onClose(){
	}
	virtual void onPause(){
	}
	virtual void onContinue(){
	}
	virtual void onUpdateScore(){}
	virtual void onSendScore(bool levelup);
	virtual void onError(const std::string message);
	// name1,score1,name2,score2,...
	virtual void onQuery(GameMode mode, std::vector<std::string> result){}
	virtual void onStatusChanged(ConnectStatus status) {}

private:
	void restartButton(Ref *pSender, ui::Widget::TouchEventType type);
	void returnButton(Ref *pSender, ui::Widget::TouchEventType type);

	void updatePopDlg(float time);
	bool isPop;

	static int m_stage;

	ui::Button *Btn_Restart;
	ui::Button *Btn_Return;
};
#endif