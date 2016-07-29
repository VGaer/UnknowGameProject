#include "BossManager.h"

BossManager::BossManager()
{

}



BossManager* BossManager::getInstance()
{
	static BossManager* instance = NULL;
	if (instance == NULL)
		instance = new BossManager();
	return instance;
}

Vector<BossABC*>& BossManager::getBossVec()
{
	return m_bossVector;
}

