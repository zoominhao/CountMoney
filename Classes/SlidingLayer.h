#ifndef _SLIDING_LAYER_H_  
#define _SLIDING_LAYER_H_  

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////  
class SlidingLayer : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(SlidingLayer);
	

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	void addSlidePage(Sprite* pSprite, int btn_type);           // ������ڻ�����ʾ�ľ���  
	bool IsMoveLayer()  { return m_bMoveLayer; } // �����ж��Ƿ���ָ������  

	virtual void btnCB(Ref *pSender, ui::Widget::TouchEventType type);

private:
	Point m_ptTouchDown;      // �״ΰ��µĴ�����  

	typedef std::vector<Sprite*> VEC_SPRITE;
	VEC_SPRITE m_vecSprite;     // ���鼯��  

	int m_nCurSprite;           // ��ǰ��ʾ�ľ���  

	bool m_bMoveLayer;          // �ƶ���  
};
#endif 