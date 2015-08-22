#include "MCUtil.h"

MCDirection MCUtil::direction(Vec2 pos1, Vec2 pos2)
{
	if (pos1 == pos2)
	{
		return Stay;
	}
	if (std::abs(pos1.x - pos2.x) >= std::abs(pos1.y - pos2.y))   //ˮƽ����
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

const std::string MCUtil::filename[16] = { "money/total.jpg", "money/total_fake_100.jpg", "money/single_100.jpg", "money/single_fake_100.jpg",
"money/total_tool1.jpg", "money/single_tool1.jpg", "money/total_tool2.jpg", "money/single_tool2.jpg", "money/total_tool3.jpg" ,
"money/single_tool3.jpg", "money/total_tool4.jpg", "money/single_tool4.jpg", "money/single_5.jpg", "money/single_10.jpg", "money/single_20.jpg", "money/single_50.jpg" };

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
	/** ��cocos2d-x��ʹ������ **/
	//����CCDictionary����ȡxml  
/*
	CCDictionary *strings = CCDictionary::createWithContentsOfFile("fonts/strings.xml");
	//��ȡHello���е�ֵ objectForKey����key����ȡ��Ӧ��string  
	const char *hello = ((CCString*)strings->objectForKey("Hello")).c_str();
	//��ȡInfo���е�ֵ  
	const char *info = ((CCString*)strings->objectForKey("Info")).c_str();

	//��ʾ  
	CCLabelTTF  *labelHello = CCLabelTTF::create(hello, "fonts/arial", 24);
	labelHello->setPosition(ccp(size.width / 2, size.height - 50));
	//this->addChild(labelHello, 1);

	CCLabelTTF *labelInfo = CCLabelTTF::create(info, "fonts/arial", 30);
	labelInfo->setPosition(ccp(size.width / 2, size.height - 100));
	//this->addChild(labelInfo, 1);*/
}

