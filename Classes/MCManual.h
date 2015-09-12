#ifndef _MCMANUAL_H_
#define _MCMANUAL_H_

#include "cocos2d.h"

USING_NS_CC;

typedef enum{
	MANUAL_LOSER,
	MANUAL_TELLER,
	MANUAL_ENDLESS,
	MANUAL_MULTI,
} MCManualType;

class MCManual
{
public:
	static void readUserProfile();
	static void writeUserProfile(MCManualType manualType, bool flag);
	static bool novice[4];
	static const std::string mode_name[4];

private:
	static bool m_isFirstTime;

};

//	MCManual::writeUserProfile(MANUAL_LOSRE, false);

#endif