#ifndef _MonsterManager_H_
#define _MonsterManager_H_

#include "Monster.h"
class Monster;

class MonsterManager
{
public:
	static MonsterManager* getInstance();
public:
	// »ñÈ¡¹ÖÎïÈİÆ÷
	Vector<Monster*>& getMonsterVec();
private:
	Vector<Monster*> m_monsterVector;
};

#endif