#include "FlyWord.h"

FlyWord* FlyWord::create(const char *filename, const float spscale, Vec2 begin, float dis)
{
	FlyWord* ret = new FlyWord();
	//这样写更安全一些  
	if (ret && ret->init(filename, spscale, begin, dis)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);  //安全删除  
	return nullptr;
}

bool FlyWord::init(const char *filename, const float spscale, Vec2 begin, float dis)
{
	if (!Sprite::init()){
		return false;
	}
	//初始化  
	m_begin = begin;
	m_panel = Sprite::create(filename);
	m_panel->setScale(spscale);
	this->addChild(m_panel);
	this->setPosition(begin);
	//初始化完成不可见  
	this->setVisible(false);
	m_dis = dis;
	return true;
}

void FlyWord::Flying()
{
	MoveBy* moveact = MoveBy::create(0.5f, CCPointMake(0, m_dis));     //0.5秒向上移动70  

	//创建回调动作，文字飘动完后  
	CallFunc* callFunc = CallFunc::create(this, callfunc_selector(FlyWord::Flyend));
	//创建连续动作  
	ActionInterval* act = Sequence::create(moveact, callFunc, NULL);
	//设置可见性  
	this->setVisible(true);
	this->runAction(act);
}

void FlyWord::Flyend()
{
	//完成之后设置隐藏  
	this->setVisible(false);
	//回复原位  
	this->setPosition(m_begin);
}

