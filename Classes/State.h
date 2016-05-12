#pragma once
#include "cocos2d.h"
USING_NS_CC;
template<class T>
class State {
public:
	virtual ~State(){}

	virtual void Enter(T*) = 0;
	virtual void Excute(T*) = 0;
	virtual void Exit(T*) = 0;
};