#ifndef __MonsterBarManager_H__
#define __MonsterBarManager_H__

#include "cocos2d.h"
USING_NS_CC;

class MonsterBarManager
{
public:
	static MonsterBarManager* getInstance();
	Vector<Sprite*>& getmonsterBarVec();
private:
	Vector<Sprite*> monsterSprBarVec;
};

#endif 
