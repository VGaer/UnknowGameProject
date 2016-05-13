#ifndef __RemoteSkill_H__
#define __RemoteSkill_H__

#include "Entity.h"
#include "cocos2d.h"
USING_NS_CC;

typedef enum
{
	enum_remoteskill_left = 1,
	enum_remoteskill_right,
	enum_remoteskill_up,
	enum_remoteskill_down,
	enum_remoteskill_none
};
//远程释放的技能类
class RemoteSkill : public Entity
{
public:
	RemoteSkill();
	~RemoteSkill();
	CREATE_FUNC(RemoteSkill);
	virtual bool init();
	void hide();
	void show();
	void SetStart(bool a);
	bool isSetedStart();
	void setSkillMove(Vec2 movespeed);
	void setSkillDir(int dir);
	int  getSkillDir();
private:
	int m_skillDir;
};

#endif