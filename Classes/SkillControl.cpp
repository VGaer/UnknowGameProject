#include "SkillControl.h"
#include "Projectile.h"

SkillControl::SkillControl(Player* player)
{
	m_player = player;
	for (int i = 0; i < MAX_SKILL_NUM; i++)
	{
		skillCounter[i].init();
	}
}

void SkillControl::useSkill(int id)
{
	
}

void SkillControl::skill_laser()
{
	float time = skillCounter[skillType_laser].getCurTime();
	//log("%f", time);
	skillCounter[skillType_laser].start();
	// �ж��Ƿ���cd��Χ��
	if (time > 0 && time < SKILL_LASER_CD)
		return;
	auto laser = Laser::create();
	// ��ֵ�趨
	laser->attr_damage = 10;
	laser->attr_moveSpeed = 3;
	laser->attr_duration = 3.5;
	laser->attr_direction = m_player->getPlayerDir();
	auto map = m_player->getParent();
	map->addChild(laser, map->getChildren().size());
	Point pos = m_player->getPosition();
	Point size = m_player->getContentSize();
	switch (laser->attr_direction)
	{
	case em_up:
		pos.y += size.y / 2;
		laser->setRotation(-90);
		break;
	case em_down:
		pos.y -= size.y / 2;
		laser->setRotation(90);
		break;
	case em_left:
		pos.x -= size.x / 2;
		laser->setRotation(180);
		break;
	case em_right:
		pos.x += size.x / 2;
		break;
	default:
		break;
	}
	laser->setPosition(pos);
	laser->scheduleUpdate();
}