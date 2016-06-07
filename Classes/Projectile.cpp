#include "Projectile.h"
#include "MonsterManager.h"
#include "AnimationUtil.h"

bool Laser::init()
{
	dt = 0;
	attr_damage = 0;
	attr_moveSpeed = 0;
	attr_duration = 0;
	attr_direction = 0;
	auto animation = AnimationUtil::createWithSingleFrameName("laser", 0.1, -1);
	auto action = Animate::create(animation);
	auto sprite = Sprite::createWithSpriteFrameName("laser1.png");
	bindSprite(sprite);
	getSprite()->runAction(action);
	return true;
}

void Laser::update(float dt)
{
	// 碰撞检测
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
	// 物体移动
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
	// 判断是否撞到障碍物
	if (!player->IsNot_CollidableTile(player->tiledCoordForPosition(afterPos)))
	{
		removeFromParent();
		return;
	}
	if (this->dt >= attr_duration)
		removeFromParent();
}

bool Fire::init()
{
	dt = 0;
	attr_damage = 0;
	attr_moveSpeed = 0;
	attr_duration = 0;
	attr_direction = 0;
	isBombing = false;
	auto animation = AnimationUtil::createWithSingleFrameName("fire", 0.1, -1);
	auto action = Animate::create(animation);
	auto sprite = Sprite::createWithSpriteFrameName("fire1.png");
	bindSprite(sprite);
	getSprite()->runAction(action);
	return true;
}

void Fire::update(float dt)
{
	this->dt += dt;
	// 如果物体没有爆炸，继续移动
	if (isBombing)
	{
		unschedule(schedule_selector(Fire::update));
		return;
	}
	if (this->dt >= attr_duration)
	{
		removeFromParent();
		return;
	}
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
}

void Fire::collideUpdate(float dt)
{
	// 碰撞检测
	Vector<Monster*> monsVec = MonsterManager::getInstance()->getMonsterVec();
	for (auto mons : monsVec)
	{
		if (mons->getBoundingBox().intersectsRect(getBoundingBox()))
		{
			if (!isBombing)
				bomb();
			mons->cmd_hurt(attr_damage);
			mons->isAttackedByProjectile = true;
			collidedVector.pushBack(mons);
		}
	}
	// 判断是否撞到障碍物
	auto player = Player::getInstance();
	if (!player->IsNot_CollidableTile(player->tiledCoordForPosition(getPosition())))
	{
		if (!isBombing)
			bomb();
	}
}

void Fire::bomb()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/fireboom.wav");
	isBombing = true;
	getSprite()->stopAllActions();
	auto bomb = AnimationUtil::createWithSingleFrameName("bomb", 0.1, 1);
	auto bombAnim = Animate::create(bomb);
	auto removeFunc = CallFunc::create([=](){
		this->removeFromParent();
	});
	getSprite()->runAction(Sequence::create(bombAnim, removeFunc, NULL));
}