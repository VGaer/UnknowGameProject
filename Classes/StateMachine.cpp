#include "StateMachine.h"

void Idle::Enter(Monster* target)
{
	int direction = target->getAnimBase()->getCurDirection();
	target->cmd_stop();
	duration = rand() % 3 + 2;  // 滞留时间( 2 - 4 ) 秒
	timeCounter.start();
}

void Idle::Excute(Monster* target)
{
	// 如果玩家进入视野范围，进入追击状态
	if (target->checkInEyeRange())
	{
		target->getStateMachine()->ChangeState(Track::Instance());
	}
	// 滞留时间已到，进入巡逻状态
	if (timeCounter.getCurTime() >= duration)
	{
		target->getStateMachine()->ChangeState(Patrol::Instance());
	}
}

void Idle::Exit(Monster* target)
{
	delete this;
}

Patrol* Patrol::Instance()
{
	static Patrol instance;
	return &instance;
}

void Patrol::Enter(Monster* target)
{

}

void Patrol::Excute(Monster* target)
{

}

void Patrol::Exit(Monster* target)
{

}

Attack* Attack::Instance()
{
	static Attack instance;
	return &instance;
}

void Attack::Enter(Monster* target)
{

}

void Attack::Excute(Monster* target)
{
}

void Attack::Exit(Monster* target)
{

}

Track* Track::Instance()
{
	static Track instance;
	return &instance;
}

void Track::Enter(Monster* target)
{

}

void Track::Excute(Monster* target)
{
	// 如果进入攻击范围内，转换攻击状态
	if (target->checkInAttaRange())
	{
		target->getStateMachine()->ChangeState(Attack::Instance());
	}
	Vec2 tarPos = target->getPlayer()->getPosition();
	target->cmd_moveTo(tarPos);
}

void Track::Exit(Monster* target)
{

}