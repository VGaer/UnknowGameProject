#include "AnimationUtil.h"

Animation* AnimationUtil::createWithSingleFrameName(const char* name, float delay, int iLoops)
{
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();

	Vector<SpriteFrame*>frameVec;
	int index = 1;
	do
	{
		String* framename = String::createWithFormat("%s%d.png", name, index++);
		SpriteFrame* spriteframe = cache->getSpriteFrameByName(framename->getCString());
		if (spriteframe == NULL)
			break;
		frameVec.pushBack(spriteframe);
	} while (true);
	Animation* animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(delay);
	animation->setLoops(iLoops);
	animation->setRestoreOriginalFrame(true);

	return animation;
}