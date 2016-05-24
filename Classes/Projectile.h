#ifndef _Projectile_H_
#define _Projectile_H_

#include "cocos2d.h"
#include "Monster.h"
#include "SkillControl.h"
USING_NS_CC;

class Laser :Entity
{
public:
	// ��ʼ��
	CREATE_FUNC(Laser);
	virtual bool init();
private:
	// �����б�	
	float attr_damage;
	float attr_moveSpeed;
	float attr_duration;
	int attr_direction;
	Vector<Monster*> collidedVector;	// ��ײ�嵥
	// ��ʱ��
	float dt;
	void update(float dt);
	friend class SkillControl;
};

#endif