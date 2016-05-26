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
#include "GameData.h"

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
	MonsData monsdata;
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
	bool isAttackedByProjectile; // 是否被投射物攻击
	Color3B m_monstercolor; //怪物颜色
	bool IstrackNoresult;    //追踪时记录怪物是否有寻路结果
	TimeCounter* m_bigskill_timecounter; //怪物大技能cd计时器
	TimeCounter* m_bigskill_attackNumsInter_timecounter;//怪物大技能多段攻击间隔计时器
	TimeCounter* m_remoteskill_timecounter; //怪物远程技能cd计时器
	TimeCounter* m_remoteskill_projectile_timecounter;//怪物远程投掷物生成间隔计时器
	TimeCounter* m_baseskill_timecounter; //怪物普通攻击技能cd计时器
	TimeCounter* m_baseskill_attackNumsInter_timecounter;//怪物普通攻击多段攻击间隔计时器
	bool checkInBigSkillRange();	//是否在大技能的攻击范围
	bool checkInRemoteSkillRange(); //是否在远程技能的攻击范围
	// -1代表没有技能
	float baseAttackRange;
	float bigSkillAttackRang;
	std::string m_curskill;//记录当前释放的技能
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