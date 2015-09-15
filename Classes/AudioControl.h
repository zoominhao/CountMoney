#ifndef __AUDIO_CONTROL_H__
#define __AUDIO_CONTROL_H__

/*
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
#endif*/

#define LOGIN_BGM "audio/login_BGM.mp3"
#define LOSER_BGM "audio/loser_BGM.mp3"
#define TELLER_BGM "audio/teller_BGM.mp3"
#define ENDLESS_BGM "audio/endless_BGM.mid"
#define DOUBLE_BGM "audio/pk_BGM.mp3"

#define COUNT_EFFECT "audio/count.mp3"
#define CLICK_EFFECT "audio/button_click.wav"
#define OVER_EFFECT "audio/gameover.mp3"
#define READY_EFFECT "audio/ready.mp3"

#define ENDLESS_WIN_EFFECT "audio/endless_win.mp3"
#define ENDLESS_LOSE_EFFECT "audio/endless_lose.mp3"

#define TOOL1_EFFECT "audio/tool1.mp3"
#define TOOL2_EFFECT "audio/tool2.mp3"
#define TOOL3_EFFECT "audio/tool3.mp3"
#define TOOL4_EFFECT "audio/tool4.mp3"
#define TOOL5_EFFECT "audio/tool5.mp3"
#define TOOL6_EFFECT "audio/tool6.mp3"

#define ENCOURAGE_EFFECT_COOL "audio/effcet_vo_cool.mp3"
#define ENCOURAGE_EFFECT_NICE "audio/effcet_vo_nice.mp3"
#define ENCOURAGE_EFFECT_GREAT "audio/effcet_vo_great.mp3"
#define ENCOURAGE_EFFECT_ACE "audio/effcet_vo_ace.mp3"
#define ENCOURAGE_EFFECT_EXCELLENT "audio/effcet_vo_excellent.mp3"
#define ENCOURAGE_EFFECT_AWESOME "audio/effcet_vo_awesome.mp3"


typedef enum 
{
	LOSER,
	TELLER,
	ENDLESS,
	PK_OFFLINE,
	PK_ONLINE,
	LOGIN,
} CMMODE;

typedef enum
{
	COOL,
	NICE,
	GREAT,
	ACE,
	EXCELLENT,
	AWESOME,
} ENCOURAGEMODE;

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
	static void playBgMusic(CMMODE mode);
	static int playCountEffect();
	static int playClickEffect();
	static int playOverEffect();
	static int playReadyEffect();

	static int playTool1Effect();
	static int playTool2Effect();
	static int playTool3Effect();
	static int playTool4Effect();
	static int playTool5Effect();
	static int playTool6Effect();

	static int playEncourageEffect(ENCOURAGEMODE mode);
	

	static int playEndlessEffect(bool flag);
	static void stopEffectMusic(int id);
	

	static void setMCEffectsVolume(float vol);
	static void setBGMusicVolume(float vol);

	static void stopMCEffects();
	static void stopBGMusic();

	static bool isEffectStopped;
	static bool isBGMusicStopped;
};

#endif