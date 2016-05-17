#include "AnimBase.h"

AnimBase::AnimBase(Entity* target)
{
	m_sprite = target->getSprite();
}

int AnimBase::getDirection(Point tarPos)
{
	Vec2 vec = tarPos - m_sprite->getPosition();
	if (vec.y > 0) return Dir_up;
	else if (vec.y < 0) return Dir_down;
	else
	{
		if (vec.x > 0) return Dir_right;
		else if (vec.x < 0) return Dir_left;
	}
}

void AnimBase::playMoveAnim(int direction)
{
	if (m_sprite == NULL || m_curDirection == direction) 
		return;
	if (direction == Dir_right)
		m_sprite->setScaleX(-1);
	else
		m_sprite->setScaleX(1);
	m_sprite->stopAllActions();
	m_sprite->runAction(m_moveAnimArray[direction]);
	m_curDirection = (AnimDirection)direction;
}

void AnimBase::playAttackAnim(int direction)
{
	if (m_sprite == NULL) 
		return;
	if (direction == Dir_right)
		m_sprite->setScaleX(-1);
	else
		m_sprite->setScaleX(1);
	m_sprite->stopAllActions();
	m_sprite->runAction(m_attaAnimArray[direction]);
}