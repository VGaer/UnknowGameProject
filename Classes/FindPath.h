#ifndef __FindPath_H__
#define __FindPath_H__

#include "Monster.h"
#include "vector"

class Monster;

class FindPath
{
public:
	void run(Vec2 startId, Vec2);
	void bindMonster(Monster* mon);
private:
	Monster* monster;
};

#endif