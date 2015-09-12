#ifndef _RANK_SCENE_H_
#define _RANK_SCENE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "WebClient.h"

USING_NS_CC;

class RankScene : public Layer, public WebClientMethod
{
public:
	~RankScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(RankScene);
	virtual void onFight(int type, bool toMe){
	};
	virtual void onClose(){
		// TODO: onclose
	};
	virtual void onUpdateScore() {
		log("onSendScore");
	}
	virtual void onSendScore(bool levelup) {
		log("onSendScore");
		if (levelup)
			log("levelup");
	}
	virtual void onError(const std::string message);
	virtual void onPause(){};
	virtual void onContinue(){};
	// name1,score1,name2,score2,...
	virtual void onQuery(GameMode mode, std::vector<std::string> result);

	void startConnect(GameMode mode);
private:
	
	void setLabelContent(std::vector<std::string> &rankStrs);

	void StartScene(Ref *pSender, ui::Widget::TouchEventType type);
	void RankLoserScene(Ref *pSender, ui::Widget::TouchEventType type);
	void RankTellerScene(Ref *pSender, ui::Widget::TouchEventType type);
	void RankEndlessScene(Ref *pSender, ui::Widget::TouchEventType type);


	Label *m_label_name;
	Label *m_label_score;


	ui::Button *m_Btn_Loser;
	ui::Button *m_Btn_Teller;
	ui::Button *m_Btn_Endless;


};
#endif