#ifndef _LOSER_END_SCENE_H_
#define _LOSER_END_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "WebClient.h"

USING_NS_CC;

class LoserEndScene : public Layer, public WebClientMethod
{
public:
	~LoserEndScene();
	static Scene* createScene(const char* resultstr, int result);
	virtual bool init();
	CREATE_FUNC(LoserEndScene);

	virtual void onFight(int type, bool toMe){
		// TODO:
		//WebClient::getinstance().shutdown();
	};
	virtual void onClose(){
		// TODO: onclose
	};
	virtual void onUpdateScore(){};
	virtual void onSendScore(bool levelup);
	virtual void onError(const std::string message);
	virtual void onPause(){};
	virtual void onContinue(){};
	// name1,score1,name2,score2,...
	virtual void onQuery(GameMode mode, std::vector<std::string> result)
	{

	}


private:
	void restartButton(Ref *pSender, ui::Widget::TouchEventType type);
	void returnButton(Ref *pSender, ui::Widget::TouchEventType type);

	void updatePopDlg(float time);
	bool isPop;

	static int m_score;

	ui::Button *Btn_Restart;
	ui::Button *Btn_Return;
		
};
#endif