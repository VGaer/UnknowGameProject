#ifndef _Monster_H_
#define _Monster_H_

#include "Entity.h"
#include "Player.h"
#include "FindPath.h"
#include "AnimBase.h"
#include "StateMachine.h"

class FindPath;
class StateMachine;

class Monster : public Entity
{
public:
	// 属性初始化
	static Monster* create(const std::string& name);
	bool init(const std::string& name);
public:
	// 属性部分
	std::string name;  // 名字
	float hp;		   // 生命值
	float damage;	   // 攻击力
	float moveSpeed;   // 行走一条边的时间
	float attackInter; // 攻击间隔
	float attackRange; // 攻击范围
	float eyeRange;    // 视野半径
	// 对外接口
	AnimBase* getAnimBase();
	StateMachine* getStateMachine();
	Player* getPlayer();
public:
	// 命令部分
	void cmd_moveTo(Point tarPos);	// 进行移动
	void cmd_stop();                // 静止
	bool cmd_hurt(float damage);	// 返回值表示是否死亡
	void cmd_attack();				// 进行攻击	
	bool checkInEyeRange();			// 是否在视野范围内
	bool checkInAttaRange();		// 是否在攻击范围内
private:
	Player* player;	
	AnimBase* animBase;             // 动作控制器
	FindPath* findPath;             // 自动寻路
	StateMachine* stateMachine;		// 状态机
	void update(float);				// 更新状态机
};

#endif