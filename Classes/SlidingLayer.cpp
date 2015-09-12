#include "SlidingLayer.h"

const int NEXT_SPRITE_SHOW = 50;    // 触摸点与滑动停止时点距离，如果超过这个值，我们认为是滑屏，没有超过则认为误操作，不做滑屏处理  



Scene* SlidingLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = SlidingLayer::create();
	scene->addChild(layer);
	return scene;
}

bool SlidingLayer::init()
{
	if (!Layer::init())
		return false;

	m_ptTouchDown.setPoint(-1, -1);
	m_nCurSprite = 0;
	m_bMoveLayer = false;

	return true;
}

bool SlidingLayer::onTouchBegan(Touch* touch, Event* event)
{
	m_ptTouchDown = touch->getLocation();    // 获取触摸点  
	m_bMoveLayer = false;
	return true;
}

void SlidingLayer::onTouchMoved(Touch* touch, Event* event)
{
	m_bMoveLayer = true;
	if (m_vecSprite.size() <= 0)               //没有需要划动显示的窗口
	{
		return;
	}

	int nCurSpriteTemp = m_nCurSprite;
	int nWillShowSpriteIndex = m_nCurSprite;    // 将要显示的精灵的索引，就是你左右滑动时，要显示的精灵  

	Point ptTouch = touch->getLocation();
	if (ptTouch.x > m_ptTouchDown.x)        // 向右滑动  
	{
		nWillShowSpriteIndex--;
	}
	else if (ptTouch.x < m_ptTouchDown.x)   // 向左滑动  
	{
		nWillShowSpriteIndex++;
	}
	nWillShowSpriteIndex = std::max(0, nWillShowSpriteIndex);
	nWillShowSpriteIndex = std::min(nWillShowSpriteIndex, (int)m_vecSprite.size() - 1);
	nCurSpriteTemp = std::max(0, nCurSpriteTemp);
	nCurSpriteTemp = std::min(nCurSpriteTemp, (int)m_vecSprite.size() - 1);

	// 当前页移动显示  
	int nDalta = ptTouch.x - m_ptTouchDown.x;            // 手指当前滑动到的触摸点与刚开始触摸的点的X差值  
	Sprite* pSprite = m_vecSprite[nCurSpriteTemp];

	Size size = CCDirector::sharedDirector()->getVisibleSize();
	if (nCurSpriteTemp == 0 && nDalta > 0 || nCurSpriteTemp == (int)m_vecSprite.size() - 1 && nDalta < 0)
	{
		return;
	}
	pSprite->setPosition(ccp(size.width / 2 + nDalta, size.height / 2));    // 移动当前页  

	if (nWillShowSpriteIndex != nCurSpriteTemp)    // 移动将要显示的页  
	{
		Sprite* pSpriteWillShow = m_vecSprite[nWillShowSpriteIndex];
		if (nWillShowSpriteIndex < nCurSpriteTemp)
		{
			pSpriteWillShow->setPosition(ccp(-size.width / 2 + nDalta, size.height / 2));    // 向右移动时，将要显示的页在屏幕左边，锚点在半屏之外  
		}
		else
		{
			pSpriteWillShow->setPosition(ccp(size.width * 3 / 2 + nDalta, size.height / 2)); // 向左移动时，将要显示的页在屏幕右边，锚点在3/2屏之外  
		}
	}
}

void SlidingLayer::onTouchEnded(Touch* touch, Event* event)
{
	if (m_vecSprite.size() == 0)
	{
		return;
	}

	CCPoint ptEnd = touch->getLocation();
	if (ptEnd.x > m_ptTouchDown.x)          // 向右滑  
	{
		if (ptEnd.x - m_ptTouchDown.x >= NEXT_SPRITE_SHOW)
		{
			m_nCurSprite--;    // 当前页变为前一页  
		}
	}
	else if (ptEnd.x < m_ptTouchDown.x)     // 向左滑  
	{
		if (m_ptTouchDown.x - ptEnd.x >= NEXT_SPRITE_SHOW)
		{
			m_nCurSprite++;    // 当前页变为下一页  
		}
	}
	m_nCurSprite = std::min(m_nCurSprite, (int)m_vecSprite.size() - 1);
	m_nCurSprite = std::max(0, m_nCurSprite);

	if (m_vecSprite.size() != 0)
	{
		Sprite* pSprite = m_vecSprite[m_nCurSprite];
		Size size = CCDirector::sharedDirector()->getVisibleSize();
		pSprite->setPosition(ccp(size.width / 2, size.height / 2));    // 将当前页显示在屏幕中间  

		// 为了使左右两个精灵不对当前页构成显示影响，将左右精灵都移动到很远很远的地方去  
		if (m_nCurSprite >= 1)
		{
			pSprite = m_vecSprite[m_nCurSprite - 1];
			pSprite->setPosition(ccp(-10000, 0));
		}
		if (m_nCurSprite + 1 <= (int)m_vecSprite.size() - 1)
		{
			pSprite = m_vecSprite[m_nCurSprite + 1];
			pSprite->setPosition(ccp(-10000, 0));
		}
	}
}

void SlidingLayer::addSlidePage(Sprite* pSprite, int btn_type)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//添加按钮
	if (btn_type == 0)
	{
		ui::Button *btn = ui::Button::create("manual/skip_manual.png");
		btn->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + 50));
		btn->setScale(0.8);
		btn->addTouchEventListener(CC_CALLBACK_2(SlidingLayer::btnCB, this));
		pSprite->addChild(btn);
	}
	else if (btn_type == 1)
	{
		ui::Button *btn = ui::Button::create("manual/finish_manual.png");
		btn->setPosition(Vec2(origin.x + visibleSize.width - 150, origin.y + 50));
		btn->setScale(0.8);
		btn->addTouchEventListener(CC_CALLBACK_2(SlidingLayer::btnCB, this));
		pSprite->addChild(btn);
	}
	m_vecSprite.push_back(pSprite);
	this->addChild(pSprite);
}

void SlidingLayer::btnCB(Ref *pSender, ui::Widget::TouchEventType type)
{

}


