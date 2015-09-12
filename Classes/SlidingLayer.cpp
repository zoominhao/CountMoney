#include "SlidingLayer.h"

const int NEXT_SPRITE_SHOW = 50;    // �������뻬��ֹͣʱ����룬����������ֵ��������Ϊ�ǻ�����û�г�������Ϊ�������������������  



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
	m_ptTouchDown = touch->getLocation();    // ��ȡ������  
	m_bMoveLayer = false;
	return true;
}

void SlidingLayer::onTouchMoved(Touch* touch, Event* event)
{
	m_bMoveLayer = true;
	if (m_vecSprite.size() <= 0)               //û����Ҫ������ʾ�Ĵ���
	{
		return;
	}

	int nCurSpriteTemp = m_nCurSprite;
	int nWillShowSpriteIndex = m_nCurSprite;    // ��Ҫ��ʾ�ľ�������������������һ���ʱ��Ҫ��ʾ�ľ���  

	Point ptTouch = touch->getLocation();
	if (ptTouch.x > m_ptTouchDown.x)        // ���һ���  
	{
		nWillShowSpriteIndex--;
	}
	else if (ptTouch.x < m_ptTouchDown.x)   // ���󻬶�  
	{
		nWillShowSpriteIndex++;
	}
	nWillShowSpriteIndex = std::max(0, nWillShowSpriteIndex);
	nWillShowSpriteIndex = std::min(nWillShowSpriteIndex, (int)m_vecSprite.size() - 1);
	nCurSpriteTemp = std::max(0, nCurSpriteTemp);
	nCurSpriteTemp = std::min(nCurSpriteTemp, (int)m_vecSprite.size() - 1);

	// ��ǰҳ�ƶ���ʾ  
	int nDalta = ptTouch.x - m_ptTouchDown.x;            // ��ָ��ǰ�������Ĵ�������տ�ʼ�����ĵ��X��ֵ  
	Sprite* pSprite = m_vecSprite[nCurSpriteTemp];

	Size size = CCDirector::sharedDirector()->getVisibleSize();
	if (nCurSpriteTemp == 0 && nDalta > 0 || nCurSpriteTemp == (int)m_vecSprite.size() - 1 && nDalta < 0)
	{
		return;
	}
	pSprite->setPosition(ccp(size.width / 2 + nDalta, size.height / 2));    // �ƶ���ǰҳ  

	if (nWillShowSpriteIndex != nCurSpriteTemp)    // �ƶ���Ҫ��ʾ��ҳ  
	{
		Sprite* pSpriteWillShow = m_vecSprite[nWillShowSpriteIndex];
		if (nWillShowSpriteIndex < nCurSpriteTemp)
		{
			pSpriteWillShow->setPosition(ccp(-size.width / 2 + nDalta, size.height / 2));    // �����ƶ�ʱ����Ҫ��ʾ��ҳ����Ļ��ߣ�ê���ڰ���֮��  
		}
		else
		{
			pSpriteWillShow->setPosition(ccp(size.width * 3 / 2 + nDalta, size.height / 2)); // �����ƶ�ʱ����Ҫ��ʾ��ҳ����Ļ�ұߣ�ê����3/2��֮��  
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
	if (ptEnd.x > m_ptTouchDown.x)          // ���һ�  
	{
		if (ptEnd.x - m_ptTouchDown.x >= NEXT_SPRITE_SHOW)
		{
			m_nCurSprite--;    // ��ǰҳ��Ϊǰһҳ  
		}
	}
	else if (ptEnd.x < m_ptTouchDown.x)     // ����  
	{
		if (m_ptTouchDown.x - ptEnd.x >= NEXT_SPRITE_SHOW)
		{
			m_nCurSprite++;    // ��ǰҳ��Ϊ��һҳ  
		}
	}
	m_nCurSprite = std::min(m_nCurSprite, (int)m_vecSprite.size() - 1);
	m_nCurSprite = std::max(0, m_nCurSprite);

	if (m_vecSprite.size() != 0)
	{
		Sprite* pSprite = m_vecSprite[m_nCurSprite];
		Size size = CCDirector::sharedDirector()->getVisibleSize();
		pSprite->setPosition(ccp(size.width / 2, size.height / 2));    // ����ǰҳ��ʾ����Ļ�м�  

		// Ϊ��ʹ�����������鲻�Ե�ǰҳ������ʾӰ�죬�����Ҿ��鶼�ƶ�����Զ��Զ�ĵط�ȥ  
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
	//��Ӱ�ť
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


