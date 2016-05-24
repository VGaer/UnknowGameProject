#include "Projectile.h"
#include "MonsterManager.h"
#include "AnimationUtil.h"

bool Laser::init()
{
	dt = 0;
	auto animation = AnimationUtil::createWithSingleFrameName("laser", 0.1, -1);
	auto action = Animate::create(animation);
	auto sprite = Sprite::createWithSpriteFrameName("laser1.png");
	bindSprite(sprite);
	getSprite()->runAction(action);
	return true;
}

void Laser::update(float dt)
{
	// Åö×²¼ì²â
	Vector<Monster*> monsVec = MonsterManager::getInstance()->getMonsterVec();
	for (auto mons : monsVec)
	{
		if (mons->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			int i;
			for (i = collidedVector.size() - 1; i >= 0; i--)
			{
				if (collidedVector.at(i) == mons)
					break;
			}
			if (i < 0)
			{
				mons->cmd_hurt(attr_damage);
				mons->isAttackedByProjectile = true;
				collidedVector.pushBack(mons);
			}
		}
	}
	// ÎïÌåÒÆ¶¯
	Vec2 move(0, 0);
	if (attr_direction == em_up) 
		move.y = attr_moveSpeed;
	else if (attr_direction == em_down)
		move.y = -attr_moveSpeed;
	else if (attr_direction == em_left)
		move.x = -attr_moveSpeed;
	else if (attr_direction == em_right)
		move.x = attr_moveSpeed;
	Point afterPos(getPosition() + move);
	setPosition(afterPos);

	this->dt += dt;
	auto player = Player::getInstance();
	// ÅÐ¶ÏÊÇ·ñ×²µ½ÕÏ°­Îï
	if (!player->IsNot_CollidableTile(player->tiledCoordForPosition(afterPos)))
	{
		removeFromParent();
		return;
	}
	if (this->dt >= attr_duration)
		removeFromParent();
}