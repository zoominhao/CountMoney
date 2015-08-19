#include "AudioControl.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

void AudioControl::preLoad()
{
	//预加载音乐和音效并设置音量
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(BGMUSIC_FILE).c_str());
	SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(EFFECT_FILE).c_str());
}


void AudioControl::playBgMusic()
{
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename(BGMUSIC_FILE).c_str(), true);
	//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
	//SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
	/*CCLOG("b:%d", SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() - 0.4f);
	CCLOG("a:%d",SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume());*/
}

int AudioControl::playEffectMusic()
{
	return SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(EFFECT_FILE).c_str(), true);
}

void AudioControl::stopEffectMusic(int id)
{
	SimpleAudioEngine::sharedEngine()->stopEffect(id);
}

void AudioControl::stopAllEffcts()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}
