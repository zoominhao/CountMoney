#ifndef _POP_CHOICE_SCENE_H_
#define _POP_CHOICE_SCENE_H_


#include "cocos2d.h"
USING_NS_CC;

class PopChoiceDlg : public LayerColor
{
public:
	static Scene * createScene();
	virtual bool init();
	CREATE_FUNC(PopChoiceDlg);

		void setTitle(const char* titleStr);    //设置对话框的title
		void setContent(const char* contentStr);  //设置对话框的文本内容

private:
	virtual bool onTouchBegan(Touch* touch, Event* event);

	void yesButton(Ref* pSender);
	void noButton(Ref* pSender);

	void addBgImg();  
	void addTitle();
	void addContent();

	Size     m_size;    //m_size代表的是对话框背景的大小
	Sprite * m_bgSprite;	//对话框的背景精灵
	LabelBMFont * m_title;
	LabelTTF * m_content;
};
#endif
