#ifndef _MCUTIL_H_
#define _MCUTIL_H_

#include "cocos2d.h"

USING_NS_CC;

typedef enum{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	Stay,
} MCDirection;

class MCUtil
{
public:
	static MCDirection direction(Vec2 pos1, Vec2 pos2);
	static void displayCharacter();


	static const float m;
	static const std::string filename[18];

	static int random(int a, int b);
};

#endif