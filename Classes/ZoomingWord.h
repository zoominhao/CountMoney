#ifndef _ZOOMING_WORD_H_
#define _ZOOMING_WORD_H_

#include "cocos2d.h"

USING_NS_CC;

class ZoomingWord: public Sprite
{
public:
	static ZoomingWord* create(const char *filename, Vec2 pos, const float sScale = 0.01f, const float eScale = 100.0f);
	virtual bool init(const char *filename, Vec2 pos, const float sScale, const float eScale);

	void Zooming();

private:
	void Zoomend();


	float m_sScale;
	float m_eScale;
	Sprite* m_panel;

};
#endif