#include "MonsterManager.h"

MonsterManager* MonsterManager::getInstance()
{
	static MonsterManager* instance = NULL;
	if (instance == NULL)
		instance = new MonsterManager();
	return instance;	
}

Vector<Monster*>& MonsterManager::getMonsterVec()
{
	return m_monsterVector;
}