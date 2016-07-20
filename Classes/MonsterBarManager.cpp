#include "MonsterBarManager.h"

MonsterBarManager* MonsterBarManager::getInstance()
{
	static MonsterBarManager* monsterbarmanager = NULL;
	if (monsterbarmanager == NULL)
		monsterbarmanager = new MonsterBarManager();
	return monsterbarmanager;
}

Vector<Sprite*>& MonsterBarManager::getmonsterBarVec()
{
	return monsterSprBarVec;
}
