#include "Monster.h"

bool Monster::init()
{
	bindSprite(Sprite::create("player.png"));
	setAnchorPoint(Vec2(0.5, 0.5));
	findPath = new FindPath();
	findPath->bindMonster(this);
	return true;
}

Monster::~Monster()
{
	CC_SAFE_DELETE(findPath);
}

void Monster::track(float dt)
{
	if (m_player != NULL)
	{
		Vec2 start = m_player->tiledCoordForPosition(this->getPosition());
		Vec2 end = m_player->tiledCoordForPosition(m_player->getPosition());
		findPath->run(start,end);
	}
}

void Monster::bindPlayer(Player* player)
{
	m_player = player;
}