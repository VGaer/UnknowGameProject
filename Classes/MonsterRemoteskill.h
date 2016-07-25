#ifndef __MonsterRemoteskill_H__
#define __MonsterRemoteskill_H__

#include "Entity.h"
#include "Player.h"
#include "TimeCounter.h"

//怪物远程飞行技能
class MonsterRemoteskill : public Entity
{
public:
	static MonsterRemoteskill* createWithName_width_height_damage_duration_speed_diretion(std::string projectile,std::string projectileAnimate ,float width, float height, float damage, float duration, float speed, int direction);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction);
private:
	float m_damage;//技能伤害
	float m_duration; //技能持续时间
	float m_movespeed; //技能速度
	int m_direction; //投掷技能方向
	float m_dt; // 计时
	void update(float dt);
	Player* m_player;
	string name;
};

//怪物远程持续技能(移动一定距离后停下来的)
class MonsterRemoteskillDur : public Entity
{
public:
	static MonsterRemoteskillDur* createWithName_width_height_damage_duration_speed_diretion_attackNums_attackNumsInter_distance(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter,float distance);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter, float distance);
private:
	float m_damage;//技能伤害
	float m_duration; //技能持续时间
	float m_movespeed; //技能速度
	int m_direction; //技能方向
	int m_attckNums; //技能攻击次数
	float m_attckNumsInter; //多段攻击延迟
	float m_dt; // 计时技能是否结束
	TimeCounter* m_attackCounter;//计时多段攻击
	void update(float dt);
	Player* m_player;
	string name;
};

//怪物远程持续技能（不移动的，比如长长的一条激光）
class MonsterRemoteskillDur_NoMove : public Entity
{
public:
	static MonsterRemoteskillDur_NoMove* createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, std::string direction, int attackNums, float attckNumsInter);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration,std::string direction, int attackNums, float attckNumsInter);
	bool MyRectColli(Vec2 rect1_v0, Vec2 rect1_v1, Vec2 rect1_v2, Vec2 rect1_v3,
		Vec2 rect2_v0, Vec2 rect2_v1, Vec2 rect2_v2, Vec2 rect2_v3);//四点组成一个矩形，规定v0v1 平行 v2v3 ，v1v2平行v3v4
private:
	float m_damage;//技能伤害
	float m_duration; //技能持续时间
	std::string m_direction; //技能方向
	int m_attckNums; //技能攻击次数
	float m_attckNumsInter; //多段攻击延迟
	float m_dt; // 计时技能是否结束
	TimeCounter* m_attackCounter;//计时多段攻击
	void update(float dt);
	Player* m_player;
	string name;
};

#endif