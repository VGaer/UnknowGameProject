#ifndef _State_H_
#define _State_H_

#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;

class Monster;

class State {
public:
	virtual void Enter(Monster*) = 0;
	virtual void Excute(Monster*) = 0;
	virtual void Exit(Monster*) = 0;
};

#endif