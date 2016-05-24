#ifndef _Projectile_H_
#define _Projectile_H_

#include "cocos2d.h"
#include "Monster.h"
#include "SkillControl.h"
USING_NS_CC;

class Laser :Entity
{
public:
	// 初始化
	CREATE_FUNC(Laser);
	virtual bool init();
private:
	// 属性列表	
	float attr_damage;
	float attr_moveSpeed;
	float attr_duration;
	int attr_direction;
	Vector<Monster*> collidedVector;	// 碰撞清单
	// 定时器
	float dt;
	void update(float dt);
	friend class SkillControl;
};

#endif