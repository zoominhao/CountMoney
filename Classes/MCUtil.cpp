#include "MCUtil.h"

MCDirection MCUtil::direction(Vec2 pos1, Vec2 pos2)
{
	if (pos1 == pos2)
	{
		return Stay;
	}
	if (std::abs(pos1.x - pos2.x) >= std::abs(pos1.y - pos2.y))   //水平方向
	{
		if (pos2.x > pos1.x)
			return RIGHT;
		else
			return LEFT;
	}
	else
	{
		if (pos2.y > pos1.y)
			return UP;
		else
			return DOWN;
	}
}

const float MCUtil::m = 3.4;

const std::string MCUtil::filename[12] = { "money/total.jpg", "money/total_fake_100.jpg", "money/single_100.jpg", "money/single_fake_100.jpg",
"money/total_tool1.jpg", "money/single_tool1.jpg", "money/total_tool2.jpg", "money/single_tool2.jpg", "money/total_tool3.jpg" ,
"money/single_tool3.jpg", "money/total_tool4.jpg", "money/single_tool4.jpg"};

int MCUtil::random(int a, int b)
{
	int k = 0;
	int sum = 0;
	while (pow(2, k) < b)
	{
		sum += (random(0, 1) << k);
		//rand()/2
		k++;
	}
	return sum;
}

