#include "MCManual.h"



bool MCManual::novice[4] = { false, false, false, false };
const std::string MCManual::mode_name[4] = { "Loser", "Teller", "Endless", "Multi" };
bool MCManual::m_isFirstTime = true;

void MCManual::readUserProfile()
{
	if (MCManual::m_isFirstTime)
		MCManual::m_isFirstTime = false;
	else
		return;

	CCDictionary* pDict;
	if (!CCFileUtils::sharedFileUtils()->isFileExist(CCFileUtils::sharedFileUtils()->getWritablePath() + "conf.xml"))
	{
		pDict = CCDictionary::createWithContentsOfFile("conf.xml");
		pDict->writeToFile((CCFileUtils::sharedFileUtils()->getWritablePath() + "conf.xml").c_str());
	}
	else
	{
		pDict = CCDictionary::createWithContentsOfFile((CCFileUtils::sharedFileUtils()->getWritablePath() + "conf.xml").c_str());
	}

	for (int i = 0; i < 4; i++)
	{
		if(strcmp(((CCString*)pDict->valueForKey(mode_name[i]))->getCString(), "true") == 0)
			MCManual::novice[i] = true;
		else
			MCManual::novice[i] = false;
	}
}

void MCManual::writeUserProfile(MCManualType manualType, bool flag)
{

	MCManual::novice[manualType] = flag;
	CCDictionary* pDict = CCDictionary::createWithContentsOfFile((CCFileUtils::sharedFileUtils()->getWritablePath() + "conf.xml").c_str());
	for (size_t i = 0; i < 4; i++)
	{
		std::string tmp = "false";
		if (novice[i])
			tmp = "true";
		CCString* pValue = CCString::create(tmp);
		pDict->setObject(pValue, mode_name[i]);
	}

	pDict->writeToFile((CCFileUtils::sharedFileUtils()->getWritablePath() + "conf.xml").c_str());
}


