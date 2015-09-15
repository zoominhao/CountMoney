#ifndef _FLY_WORD_H_
#define _FLY_WORD_H_

#include "cocos2d.h"

USING_NS_CC;

class FlyWord: public Sprite
{
public:
	static FlyWord* create(const char *filename, const float spscale, Vec2 begin, float dis);
	virtual bool init(const char *filename, const float spscale, Vec2 begin, float dis);

	void Flying();

private:
	void Flyend();


	Vec2 m_begin;
	Sprite* m_panel;
	float m_dis;
};
#endif