#ifndef _MonsterManager_H_
#define _MonsterManager_H_

#include "Monster.h"
class Monster;

class MonsterManager
{
public:
	static MonsterManager* getInstance();
	MonsterManager();
public:
	// »ñÈ¡¹ÖÎïÈİÆ÷
	Vector<Monster*>& getMonsterVec();
	void setMonsEnableAction(bool);
private:
	bool canMonsEnableAction;
	Vector<Monster*> m_monsterVector;
};

#endif