#ifndef __BossManager_H__
#define __BossManager_H__

#include "CubeBoss.h"


class BossManager
{
public:
	BossManager();
	static BossManager* getInstance();
public:
	// ��ȡ��������
	Vector<Node*>& getBossVec();
	//void setMonsEnableAction(bool);
private:
	//bool canMonsEnableAction;
	Vector<Node*> m_bossVector;
};

#endif