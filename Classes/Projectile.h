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
	static const int attr_inter = 3; //攻击cd
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

class Fire : Entity
{
public:
	// 初始化
	CREATE_FUNC(Fire);
	virtual bool init();
	static const int attr_inter = 3; //攻击cd
private:
	// 属性列表	
	float attr_damage;
	float attr_moveSpeed;
	float attr_duration;
	int attr_direction;
	bool isBombing;
	void bomb();
	Vector<Monster*> collidedVector;	// 碰撞清单
	// 定时器
	float dt;
	void update(float dt);
	friend class SkillControl;
};

#endif