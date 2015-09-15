#include "ZoomingWord.h"

ZoomingWord* ZoomingWord::create(const char *filename, Vec2 pos, const float sScale /*= 0.0f*/, const float eScale /*= 1.0f*/)
{
	ZoomingWord* ret = new ZoomingWord();
	//这样写更安全一些  
	if (ret && ret->init(filename, pos, sScale, eScale)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);  //安全删除  
	return nullptr;
}

bool ZoomingWord::init(const char *filename, Vec2 pos, const float sScale, const float eScale)
{
	if (!Sprite::init()){
		return false;
	}
	//初始化  
	m_sScale = sScale;
	m_eScale = eScale;
	m_panel = Sprite::create(filename);
	m_panel->setScale(sScale);
	this->addChild(m_panel);
	this->setPosition(pos);
	//初始化完成不可见  
	this->setVisible(false);

	return true;
}

void ZoomingWord::Zooming()
{
	ScaleTo* scaleact = ScaleTo::create(0.5f, m_eScale);
	CallFunc* callFunc = CallFunc::create(this, callfunc_selector(ZoomingWord::Zoomend));
	ActionInterval* act = Sequence::create(scaleact, callFunc, NULL);

	this->setVisible(true);
	this->runAction(act);
}


void ZoomingWord::Zoomend()
{
	//完成之后设置隐藏  
	this->setVisible(false);
	//回复原位  
	this->setScale(m_sScale);
}


