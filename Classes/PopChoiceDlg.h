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

		void setTitle(const char* titleStr);    //���öԻ����title
		void setContent(const char* contentStr);  //���öԻ�����ı�����

private:
	virtual bool onTouchBegan(Touch* touch, Event* event);

	void yesButton(Ref* pSender);
	void noButton(Ref* pSender);

	void addBgImg();  
	void addTitle();
	void addContent();

	Size     m_size;    //m_size������ǶԻ��򱳾��Ĵ�С
	Sprite * m_bgSprite;	//�Ի���ı�������
	LabelBMFont * m_title;
	LabelTTF * m_content;
};
#endif
