#ifndef __AUDIO_CONTROL_H__
#define __AUDIO_CONTROL_H__

#if   (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define EFFECT_FILE  "count.wav" //"effect2.ogg"  
#elif (CC_TARCET_PLATFORM == CC_PLATFORM_MARMALADE)
#define EFFECT_FILE  "count.wav" //"effect1.raw"
#else
#define EFFECT_FILE  "count.wav" //"effect1.wav"
#endif

#if (CC_TARGET_PLATEFOM == CC_PLATFORM_WIN32)
#define  BGMUSIC_FILE  "bg_music.wav" //"music.mid"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#define BGMUSIC_FILE   "bg_music.wav" //"background.ogg"
#else
#define BGMUSIC_FILE   "bg_music.wav" //"background.mp3"
#endif

#include "cocos2d.h"
#include "CMTimer.h"
#include "Player.h"

USING_NS_CC;

class AudioControl
{
public:
	AudioControl();
	~AudioControl();
	static void preLoad();
	static void playBgMusic();
	static int playEffectMusic();
	static void stopEffectMusic(int id);
	static void stopAllEffcts();

	static void setMCEffectsVolume(float vol);
	static void setBGMusicVolume(float vol);

	static void stopMCEffects();
	static void stopBGMusic();

	static bool isEffectStopped;
	static bool isBGMusicStopped;
};

#endif