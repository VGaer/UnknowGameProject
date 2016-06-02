#ifndef __MonDiedThings_H__
#define __MonDiedThings_H__

#include "Entity.h"
#include "Player.h"

class Player;

class t_HP : public Entity
{
public:
	CREATE_FUNC(t_HP);
	virtual bool init();
	void sethpNum(int h);
	void bindPlayer(Player* player);
	void update(float dt);
private:
	Player* m_player;
	int hpNum;
};

class t_MP : public Entity
{
public:
	CREATE_FUNC(t_MP);
	virtual bool init();
	void setmpNum(int m);
	void bindPlayer(Player* player);
	void update(float dt);
private:
	Player* m_player;
	int mpNum;
};

#endif