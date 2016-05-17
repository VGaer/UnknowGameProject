#ifndef __FindPath_H__
#define __FindPath_H__

#include "Monster.h"
class Monster;

class FindPath
{
public:
	void run(Vec2 startPos, Vec2 endPos); // 参数为像素坐标
	void bindMonster(Monster* mon);
private:
	Monster* monster;
};

#endif