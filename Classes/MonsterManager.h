#ifndef _MonsterManager_H_
#define _MonsterManager_H_

#include "Monster.h"
class Monster;

class MonsterManager
{
public:
	MonsterManager();
	static MonsterManager* getInstance();
public:
	// ��ȡ��������
	Vector<Monster*>& getMonsterVec();
	void setMonsEnableAction(bool);
private:
	bool canMonsEnableAction;
	Vector<Monster*> m_monsterVector;
};

#endif