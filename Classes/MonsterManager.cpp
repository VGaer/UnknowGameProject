#include "MonsterManager.h"

MonsterManager* MonsterManager::getInstance()
{
	static MonsterManager* instance = NULL;
	if (instance == NULL)
		instance = new MonsterManager();
	return instance;
}

MonsterManager::MonsterManager()
{
	canMonsEnableAction = true;
}
Vector<Monster*>& MonsterManager::getMonsterVec()
{
	return m_monsterVector;
}

void MonsterManager::setMonsEnableAction(bool argv)
{
	if (argv == true)
	{ 
		if (canMonsEnableAction == false)
		{
			for (auto mons : m_monsterVector)
				mons->getStateMachine()->ChangeState(new Idle());
			canMonsEnableAction = true;
		}		
	}
	else
	{
		if (canMonsEnableAction == true)
		{
			for (auto mons : m_monsterVector)
			{
				mons->cmd_stop();
				mons->getStateMachine()->SetCurrState(NULL);				
			}				
			canMonsEnableAction = false;
		}
		
	}
}