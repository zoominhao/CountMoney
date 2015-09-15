#include "AudioControl.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool AudioControl::isEffectStopped = false;
bool AudioControl::isBGMusicStopped = false;

void AudioControl::preLoad()
{
	//Ô¤¼ÓÔØÒôÀÖºÍÒôÐ§²¢ÉèÖÃÒôÁ¿
	//±³¾°ÒôÀÖ
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(LOSER_BGM).c_str());
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(TELLER_BGM).c_str());
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENDLESS_BGM).c_str());
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(DOUBLE_BGM).c_str());
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(LOGIN_BGM).c_str());
	//ÒôÐ§
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(COUNT_EFFECT).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(CLICK_EFFECT).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(OVER_EFFECT).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(READY_EFFECT).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENDLESS_WIN_EFFECT).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENDLESS_LOSE_EFFECT).c_str());

}


void AudioControl::playBgMusic(CMMODE mode)
{
	if (isBGMusicStopped)
		return;
	switch (mode)
	{
	case LOSER:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(LOSER_BGM).c_str(), true);
		break;

	case TELLER:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(TELLER_BGM).c_str(), true);
		break;

	case ENDLESS:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENDLESS_BGM).c_str(), true);
		break;

	case PK_OFFLINE:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(DOUBLE_BGM).c_str(), true);
		break;

	case PK_ONLINE:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(DOUBLE_BGM).c_str(), true);
		break;
	case LOGIN:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(LOGIN_BGM).c_str(), true);
		break;
	default:
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(LOSER_BGM).c_str(), true);
		break;
	}
	
	//SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	/*CCLOG("b:%d", SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() - 0.4f);
	CCLOG("a:%d",SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());*/
}

int AudioControl::playCountEffect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(COUNT_EFFECT).c_str(), false);
	//SimpleAudioEngine::sharedEngine()->stopAllEffects();
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
	return id;
}

int AudioControl::playClickEffect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(CLICK_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playOverEffect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(OVER_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playReadyEffect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(READY_EFFECT).c_str(), false);
	return id;
}



int AudioControl::playEndlessEffect(bool flag)
{
	if (isEffectStopped)
		return -1;
	int id;
	if (flag)
	{
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENDLESS_WIN_EFFECT).c_str(), false);
	}
	else
	{
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENDLESS_LOSE_EFFECT).c_str(), false);
	}

	return id;
}



void AudioControl::stopEffectMusic(int id)
{
	SimpleAudioEngine::sharedEngine()->stopEffect(id);
}


void AudioControl::setMCEffectsVolume(float vol)
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(vol);
}

void AudioControl::setBGMusicVolume(float vol)
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(vol);
}

void AudioControl::stopMCEffects()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void AudioControl::stopBGMusic()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

int AudioControl::playTool1Effect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(TOOL1_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playTool2Effect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(TOOL2_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playTool3Effect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(TOOL3_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playTool4Effect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(TOOL4_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playTool5Effect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(TOOL5_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playTool6Effect()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(TOOL6_EFFECT).c_str(), false);
	return id;
}

int AudioControl::playEncourageEffect(ENCOURAGEMODE mode)
{
	if (isEffectStopped)
		return -1;
	int id = -1;
	switch (mode)
	{
	case COOL:
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENCOURAGE_EFFECT_COOL).c_str(), false);
		break;

	case NICE:
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENCOURAGE_EFFECT_NICE).c_str(), false);
		break;

	case GREAT:
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENCOURAGE_EFFECT_GREAT).c_str(), false);
		break;

	case ACE:
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENCOURAGE_EFFECT_ACE).c_str(), false);
		break;

	case EXCELLENT:
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENCOURAGE_EFFECT_EXCELLENT).c_str(), false);
		break;
	case AWESOME:
		id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(ENCOURAGE_EFFECT_AWESOME).c_str(), false);
		break;
	default:
		break;
	}
	return id;
}
