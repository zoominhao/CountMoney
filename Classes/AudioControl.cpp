#include "AudioControl.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

bool AudioControl::isEffectStopped = false;
bool AudioControl::isBGMusicStopped = false;

void AudioControl::preLoad()
{
	//预加载音乐和音效并设置音量
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(BGMUSIC_FILE).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(EFFECT_FILE).c_str());
}


void AudioControl::playBgMusic()
{
	if (isBGMusicStopped)
		return;
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(BGMUSIC_FILE).c_str(), true);
	//SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	/*CCLOG("b:%d", SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() - 0.4f);
	CCLOG("a:%d",SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());*/
}

int AudioControl::playEffectMusic()
{
	if (isEffectStopped)
		return -1;
	int id = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(EFFECT_FILE).c_str(), false);
	//SimpleAudioEngine::sharedEngine()->stopAllEffects();
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
	return id;
}

void AudioControl::stopEffectMusic(int id)
{
	SimpleAudioEngine::sharedEngine()->stopEffect(id);
}

void AudioControl::stopAllEffcts()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
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
