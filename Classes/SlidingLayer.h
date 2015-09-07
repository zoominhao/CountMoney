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
	void addSlidePage(Sprite* pSprite, int btn_type);           // 添加用于滑屏显示的精灵  
	bool IsMoveLayer()  { return m_bMoveLayer; } // 用于判断是否手指滑动了  

	virtual void btnCB(Ref *pSender, ui::Widget::TouchEventType type);

private:
	Point m_ptTouchDown;      // 首次按下的触摸点  

	typedef std::vector<Sprite*> VEC_SPRITE;
	VEC_SPRITE m_vecSprite;     // 精灵集合  

	int m_nCurSprite;           // 当前显示的精灵  

	bool m_bMoveLayer;          // 移动层  
};
#endif 