#ifndef __MonsterRemoteskill_H__
#define __MonsterRemoteskill_H__

#include "Entity.h"
#include "Player.h"

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
};

#endif