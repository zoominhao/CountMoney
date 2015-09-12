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

const std::string MCUtil::filename[24] = { "money/total_100.png", "money/total_fake_100.png", "money/single_100.png", "money/single_fake_100.png",
"money/total_tool1.png", "money/single_tool1.png", "money/total_tool2.png", "money/single_tool2.png", "money/total_tool3.png" ,"money/single_tool3.png", 
"money/total_tool4.png", "money/single_tool4.png", "money/total_tool5.png", "money/single_tool5.png", "money/total_tool6.png", "money/single_tool6.png",
"money/total_5.png", "money/single_5.png", "money/total_10.png", "money/single_10.png", "money/total_20.png", "money/single_20.png",
"money/total_50.png", "money/single_50.png" };

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

void MCUtil::displayCharacter()
{
	/** 在cocos2d-x中使用中文 **/
	//利用CCDictionary来读取xml  
/*
	CCDictionary *strings = CCDictionary::createWithContentsOfFile("fonts/strings.xml");
	//读取Hello键中的值 objectForKey根据key，获取对应的string  
	const char *hello = ((CCString*)strings->objectForKey("Hello")).c_str();
	//读取Info键中的值  
	const char *info = ((CCString*)strings->objectForKey("Info")).c_str();

	//显示  
	CCLabelTTF  *labelHello = CCLabelTTF::create(hello, "fonts/arial", 24);
	labelHello->setPosition(ccp(size.width / 2, size.height - 50));
	//this->addChild(labelHello, 1);

	CCLabelTTF *labelInfo = CCLabelTTF::create(info, "fonts/arial", 30);
	labelInfo->setPosition(ccp(size.width / 2, size.height - 100));
	//this->addChild(labelInfo, 1);*/
}

Color3B MCUtil::m_timerColor = Color3B(255.0, 255.0, 255.0);
Color3B MCUtil::m_targetColor = Color3B(255.0, 255.0, 255.0);