#include "StateMachine.h"

void Idle::Enter(Monster* target)
{
	int direction = target->getAnimBase()->getCurDirection();
	target->cmd_stop();
	duration = rand() % 3 + 2;  // ����ʱ��( 2 - 4 ) ��
	timeCounter.start();
}

void Idle::Excute(Monster* target)
{
	// �����ҽ�����Ұ��Χ������׷��״̬
	if (target->checkInEyeRange())
	{
		target->getStateMachine()->ChangeState(Track::Instance());
	}
	// ����ʱ���ѵ�������Ѳ��״̬
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
	// ������빥����Χ�ڣ�ת������״̬
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