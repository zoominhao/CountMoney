#include "FlyWord.h"

FlyWord* FlyWord::create(const char *filename, const float spscale, Vec2 begin, float dis)
{
	FlyWord* ret = new FlyWord();
	//����д����ȫһЩ  
	if (ret && ret->init(filename, spscale, begin, dis)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);  //��ȫɾ��  
	return nullptr;
}

bool FlyWord::init(const char *filename, const float spscale, Vec2 begin, float dis)
{
	if (!Sprite::init()){
		return false;
	}
	//��ʼ��  
	m_begin = begin;
	m_panel = Sprite::create(filename);
	m_panel->setScale(spscale);
	this->addChild(m_panel);
	this->setPosition(begin);
	//��ʼ����ɲ��ɼ�  
	this->setVisible(false);
	m_dis = dis;
	return true;
}

void FlyWord::Flying()
{
	MoveBy* moveact = MoveBy::create(0.5f, CCPointMake(0, m_dis));     //0.5�������ƶ�70  

	//�����ص�����������Ʈ�����  
	CallFunc* callFunc = CallFunc::create(this, callfunc_selector(FlyWord::Flyend));
	//������������  
	ActionInterval* act = Sequence::create(moveact, callFunc, NULL);
	//���ÿɼ���  
	this->setVisible(true);
	this->runAction(act);
}

void FlyWord::Flyend()
{
	//���֮����������  
	this->setVisible(false);
	//�ظ�ԭλ  
	this->setPosition(m_begin);
}

