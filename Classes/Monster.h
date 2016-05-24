#ifndef __Monster_H__
#define __Monster_H__

#include "Entity.h"
#include "Player.h"
#include "FindPath.h"
#include "AnimBase.h"
#include "StateMachine.h"
#include "TimeCounter.h"
#include "cocos2d.h"
#include <vector>

class FindPath;
class StateMachine;

typedef enum 
{
	enum_MonsterIdle = 0,
	enum_MonsterPatrol,
	enum_MonsterAttack,
	enum_MonsterTrack,
	enum_MonsterAttacked
};

class AnimBase;

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
	float patrolRange; // 巡逻半径
	float eyeRange;    // 视野半径
	float perceptionRange; //感知半径
	float attackedrestoretimes; //硬直时间
	float beforeattacktimes;      //前摇时间
	float attackAnimatetimePer; //攻击动画播放时间
	// 对外接口
	AnimBase* getAnimBase();
	StateMachine* getStateMachine();
	Player* getPlayer();
	void bindPlayer(Player* player);//绑定主角
public:
	// 命令部分
	void cmd_moveTo(Point tarPos);	// 进行移动
	void cmd_stop();                // 静止
	bool cmd_hurt(float damage);	// 返回值表示是否死亡
	void cmd_attack();				// 进行攻击	
	bool checkInEyeRange();			// 是否在视野范围内
	bool checkInPerceptionRange();  // 是否在感知区域
	bool checkInAttaRange();		// 是否在攻击范围内	
public:
	void setMachineState(int i);
	int checkInSectorRange();   //扇形视野区域
	int getCurMachineState();
	void setMonsterParent(TMXTiledMap* parent);
	int getMonster_magnification();
	bool Is_firstFindplayer_Track;
	void setvecPatrolpoint();
	~Monster();
	std::vector<Vec2> getPatrolpointvec();
	TimeCounter* m_timecounter;
	float duration;
	bool IspatrolEnd;//巡逻完成
	int index; //巡逻点记录
	bool IsattackedByPlayer();	//是否被主角攻击
	Color3B m_monstercolor; //怪物颜色
	bool Isrestartbaseattack;	//记录怪物是否重新普通攻击
	Vec2 prePosition;    //追踪时记录怪物前一次的位置
private:
	Player* player;
	AnimBase* animBase;             // 动作控制器
	FindPath* findPath;             // 自动寻路
	StateMachine* stateMachine;		// 状态机
	void update(float);				// 更新状态机
	int m_CurMachineState;								// 状态机的状态
	TMXTiledMap* m_parrent;		//
	int m_magnification;//怪物精灵放大倍数
	std::vector<Vec2> vecPatrolpoint; //巡逻点向量
};
#endif