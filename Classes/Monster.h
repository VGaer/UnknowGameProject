#ifndef _Monster_H_
#define _Monster_H_

#include "Entity.h"
#include "Player.h"
#include "AnimBase.h"

class Monster : Entity
{
public:
	// 属性初始化
	static Monster* create(const std::string& file);
	bool init(const std::string& file);
public:
	// 属性部分
	float hp;
	float damage;
	float moveSpeed;   // 行走一条边的时间
	float attackInter; // 攻击间隔
	float attackRange; // 攻击范围
public:
	// 命令部分
	void cmd_moveTo(Point);			// 进行移动
	void cmd_stop();				// 停止移动 
	bool cmd_hurt(float damage);	// 返回值表示是否死亡
	void cmd_attack();				// 进行攻击
private:
	Player* player;	
	AnimBase* animBase;             // 动作控制器
	void changeDirection();			// 改变移动方向
	bool checkRange();				// 是否在攻击范围
};

#endif