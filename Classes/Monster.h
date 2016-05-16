#ifndef __Monster_H__
#define __Monster_H__

#include "Entity.h"
#include "Player.h"
#include "FindPath.h"

class FindPath;

class Monster : public Entity
{
public:
	CREATE_FUNC(Monster);
	~Monster();
	FindPath* findPath;
	virtual bool init();
	void track(float dt);
	void bindPlayer(Player* player);
private:
	Player * m_player;
};
#endif