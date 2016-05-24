#ifndef _SkillControl_H_
#define _SkillControl_H_

#include "Player.h"
#include "TimeCounter.h"

#define MAX_SKILL_NUM 5		// 未定

#define SKILL_LASER_CD 2

class Player;

enum SkillType
{
	skillType_laser = 0,
};

class SkillControl
{
public:
	// 初始化属性
	SkillControl(Player* player);
public:
	void useSkill(int id);  // 绑定快捷键可能用得上
	// 技能列表
	void skill_laser();
private:
	// 玩家
	Player* m_player;
	// 技能计时器
	TimeCounter skillCounter[MAX_SKILL_NUM];
};

#endif