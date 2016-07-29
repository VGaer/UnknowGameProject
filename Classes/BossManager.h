#ifndef __BossManager_H__
#define __BossManager_H__

#include "CubeBoss.h"
#include "Entity.h"
#include "BossABC.h"

class BossManager
{
public:
	BossManager();
	static BossManager* getInstance();
public:
	// ��ȡ��������
	Vector<BossABC*>& getBossVec();
	//void setMonsEnableAction(bool);
private:
	//bool canMonsEnableAction;
	Vector<BossABC*> m_bossVector;
};

#endif