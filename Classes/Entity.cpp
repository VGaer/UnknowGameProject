#include "Entity.h"

void Entity::bindSprite(Sprite* sprite)
{
	m_sprite = sprite;
	m_sprite->setPosition(Vec2(m_sprite->getContentSize().width / 2, m_sprite->getContentSize().height / 2));
	this->addChild(m_sprite);
	//设置实体的ContentSize
	this->setContentSize(m_sprite->getContentSize());
}

Sprite* Entity::getSprite()
{
	return m_sprite;
}