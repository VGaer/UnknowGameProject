#include "NpcManager.h"

NpcManager* NpcManager::getInstance()
{
	static NpcManager* instance = NULL;
	if (instance == NULL)
		instance = new NpcManager();
	return instance;
}

Vector<NPC*>& NpcManager::getNpcsVec()
{
	return m_npcVector;
}