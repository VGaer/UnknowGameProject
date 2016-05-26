#ifndef _SkillControl_H_
#define _SkillControl_H_

#include "Player.h"
#include "TimeCounter.h"

#define MAX_SKILL_NUM 1		// 未定



class Player;

enum SkillType
{
	skillType_laser = 0,
};

class SkillControl : public Node
{
public:
	// 初始化属性
	CREATE_FUNC(SkillControl);
	virtual bool init();
public:
	void useSkill(int id);  // 绑定快捷键可能用得上
	// 技能列表
	bool skill_laser();
private:
	// 玩家
	Player* m_player;
	// 技能计时器
	TimeCounter* skillCounter[MAX_SKILL_NUM];
};

#endif