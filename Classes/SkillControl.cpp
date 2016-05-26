#include "SkillControl.h"
#include "Projectile.h"

bool SkillControl::init()
{
	m_player = Player::getInstance();
	for (int i = 0; i < MAX_SKILL_NUM; i++)
	{
		skillCounter[i] = TimeCounter::create();
		addChild(skillCounter[i]);
	}
	return true;
}

void SkillControl::useSkill(int id)
{

}

bool SkillControl::skill_laser()
{
	float time = skillCounter[skillType_laser]->getCurTime();
	// 判断是否在cd范围内
	if (time > 0 && time < Laser::attr_inter)
		return false;
	skillCounter[skillType_laser]->start();
	auto laser = Laser::create();
	laser->setAnchorPoint(Vec2(0, 0.5));
	// 数值设定
	laser->attr_damage = 10;
	laser->attr_moveSpeed = 8;
	laser->attr_duration = 3.5;
	laser->attr_direction = m_player->getPlayerDir();
	auto map = m_player->getParent();
	map->addChild(laser, map->getChildren().size());
	Point pos = m_player->getPosition();
	Point size = m_player->getContentSize();
	switch (laser->attr_direction)
	{
	case em_up:
		pos.y += size.y * m_player->getAnchorPoint().y;
		laser->setRotation(-90);
		break;
	case em_down:
		pos.y -= size.y * m_player->getAnchorPoint().y;
		laser->setRotation(90);
		break;
	case em_left:
		pos.x -= size.x * m_player->getAnchorPoint().x;
		laser->setRotation(180);
		break;
	case em_right:
		pos.x += size.x * m_player->getAnchorPoint().x;
		break;
	default:
		break;
	}
	laser->setPosition(pos);
	laser->scheduleUpdate();

	return true;
}