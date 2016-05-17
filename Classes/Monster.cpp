#include "Monster.h"
#include "GameData.h"

Monster* Monster::create(const std::string& name)
{
	auto unit = new Monster();
	if (unit && unit->init(name)){
		unit->autorelease();
	}
	else{
		CC_SAFE_DELETE(unit);
		unit = NULL;
	}
	return unit;
}

bool Monster::init(const std::string& name)
{
	auto data = GameData::getInstance()->getDataFromMonsData(name);
	this->name = data->name;
	hp = data->hp;
	damage = data->damage;
	moveSpeed = data->moveSpeed;
	attackInter = data->attackInter;
	attackRange = data->attackRange;
	eyeRange = data->eyeRange;
	bindSprite(Sprite::create(data->imagePath));
	findPath = new FindPath();
	return true;
}

AnimBase* Monster::getAnimBase()
{
	return animBase;
}

void Monster::cmd_moveTo(Point tarPos)
{
	findPath->run(getPosition(), tarPos);
}

void Monster::cmd_stop()
{
	int curDir = animBase->getCurDirection();
	animBase->playStanAnim(curDir);
}

bool Monster::cmd_hurt(float damage)
{
	hp -= damage;
	if (hp <= 0)
		return true;
	return false;
}

void Monster::cmd_attack()
{
	// 获取攻击方向
	int tarDir = animBase->getDirectionByTargetPos(player->getPosition());
	animBase->playAttaAnim(tarDir);

	// .. 玩家受伤逻辑 ..   
}

bool Monster::checkInEyeRange()
{
	if (player != NULL)
	{
		float distance = (player->getPosition() - getPosition()).getLength();
		if (distance <= eyeRange)
			return true;
	}
	return false;
}

bool Monster::checkInAttaRange()
{
	if (player != NULL)
	{
		float distance = (player->getPosition() - getPosition()).getLength();
		if (distance <= attackRange)
			return true;
	}
	return false;
}