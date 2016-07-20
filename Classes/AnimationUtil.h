#ifndef __AnimationUtil_H__
#define __AnimationUtil_H__

#include "cocos2d.h"
USING_NS_CC;

class AnimationUtil
{
public:
	static Animation* createWithSingleFrameName(const char* name, float delay, int iLoops);
};

#endif