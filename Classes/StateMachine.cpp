#include "StateMachine.h"
#include "Monster.h"
#define Pi 3.141592653

void Idle::Enter(Monster* target)
{
	//log("Idle");
	//记录当前状态
	target->setMachineState(enum_MonsterIdle);
	target->cmd_stop();
	target->duration = rand() % 3 + 2;  // 滞留时间( 2 - 4 ) 秒
	target->m_timecounter->start();	
}

void Idle::Excute(Monster* target)
{
//	log("Idle");

	//如果被主角攻击了，进入被击状态
	if (target->IsattackedByPlayer()){
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	// 如果玩家进入视野范围或者感知范围，进入追击状态
	if (target->checkInEyeRange() || target->checkInPerceptionRange())
	{
		target->getStateMachine()->ChangeState(new Track());
	}
	//// 滞留时间已到，进入巡逻状态
	else if (target->m_timecounter->getCurTime() >= target->duration)
	{
		target->getStateMachine()->ChangeState(new Patrol());
	}	
}

void Idle::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();//停掉精灵的动画
}


void Patrol::Enter(Monster* target)
{
	//记录当前状态
	target->setMachineState(enum_MonsterPatrol);
	//log("patrol");

	std::vector<Vec2> patrolvec = target->getPatrolpointvec();
	if (patrolvec.size() > 0)
	{
		//srand(time(0));
		target->index = rand() % patrolvec.size();
		target->cmd_moveTo(patrolvec[target->index]);
		//设置巡逻的寻路延迟duration
		target->duration = target->moveSpeed;
		target->m_timecounter->start();
	}
	
	target->IspatrolEnd = false;//开始巡逻后，设置为false
}

void Patrol::Excute(Monster* target)
{
	//log("patrol");

	//如果被主角攻击了，进入被击状态
	if (target->IsattackedByPlayer()){
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	//巡逻过程中优先判断主角是否在视野
	// 如果玩家进入视野范围或者感知范围，进入追击状态
	if (target->checkInEyeRange() || target->checkInPerceptionRange())
	{
		target->getStateMachine()->ChangeState(new Track());
	}
	//否则继续巡逻
	else if (target->m_timecounter->getCurTime() > target->duration)
	{
		if (target->IspatrolEnd == true){
			//已寻路到巡逻的目的地，重新随机一个巡逻目的地
			std::vector<Vec2> patrolvec = target->getPatrolpointvec();
			if (patrolvec.size() > 0)
			{
			//	srand(time(0));
				target->index = rand() % patrolvec.size();
				target->cmd_moveTo(patrolvec[target->index]);
				//设置巡逻的寻路延迟duration
				target->duration = target->moveSpeed;
				target->m_timecounter->start();
			}
		}
		/*未巡逻到目的地 继续巡逻*/
		else{
			target->cmd_moveTo(target->getPatrolpointvec()[target->index]);
			target->duration = target->moveSpeed;
			target->m_timecounter->start();
		}			
	}
}

void Patrol::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();//停掉寻路的运动
	target->getSprite()->stopAllActions();//停掉精灵的动画
}

void Attack::Enter(Monster* target)
{
	//记录当前状态
	target->setMachineState(enum_MonsterAttack);

	//设置攻击延迟duration//攻击动画设置在攻击间隔时间内
	target->duration = target->attackInter;
	target->m_timecounter->start();

	target->cmd_attack();
}

void Attack::Excute(Monster* target)
{
//	log("attack");
	//记录当前状态

	//在攻击范围，攻击间隔时间到了，继续攻击
	if (target->m_timecounter->getCurTime() > target->attackInter && target->checkInAttaRange())
	{
		target->duration = target->attackInter;
		target->m_timecounter->start();
		//log("time----%f", target->m_timecounter->getCurTime());
		target->cmd_attack();
		return;
	}

	//如果怪物还在前摇时间，主角可以打断怪物攻击
	//如果被主角攻击了，进入被击状态
	if (target->m_timecounter->getCurTime() < target->beforeattacktimes && target->IsattackedByPlayer()){
	//	log("time %f", target->m_timecounter->getCurTime());
		target->getStateMachine()->ChangeState(new Attacked());
		return;
		}
	

	//不在攻击范围了
	if (!target->checkInAttaRange())
	{
		// 如果玩家进入视野范围或者感知范围，进入追击状态
		if (target->checkInEyeRange() || target->checkInPerceptionRange())
		{
			target->getStateMachine()->ChangeState(new Track());
		}
		//随机进入patol或者idle
		else
		{
			switch (rand() % 2)
			{
			case 0:{
				target->getStateMachine()->ChangeState(new Idle());
				break;
			}
			case 1:{
				target->getStateMachine()->ChangeState(new Patrol());
				break;
			}
			}
		}
		return;
	}
	
}

void Attack::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();//停掉精灵的动画
}

void Track::Enter(Monster* target)
{
	//log("track");
	//记录当前状态
	target->setMachineState(enum_MonsterTrack);

	Vec2 tarPos = target->getPlayer()->getPosition();
	target->cmd_moveTo(tarPos);

	//设置寻路延迟duration
	target->duration = target->moveSpeed;
	target->m_timecounter->start();

	target->Is_firstFindplayer_Track = false;//设置第一次发现主角并追踪为false
}

void Track::Excute(Monster* target)
{
//	log("track");

	//如果被主角攻击了，进入被击状态
	if (target->IsattackedByPlayer()){
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	// 如果进入攻击范围内，转换攻击状态
	if (target->checkInAttaRange())
	{
		target->getStateMachine()->ChangeState(new Attack());
	}
	 // 如果玩家不在视野范围了，等当前一步寻路执行完再进入巡逻状态或者静止状态
	else if(!target->checkInEyeRange() && target->m_timecounter->getCurTime() > target->duration)
	{
		switch (rand() % 2)
		{
		case 0:{
			target->getStateMachine()->ChangeState(new Idle());
			break;
		}
		case 1:{
			target->getStateMachine()->ChangeState(new Patrol());
			break; 
		}
		}
	}
	//还是视野范围，继续寻路
	else if (target->m_timecounter->getCurTime() > target->duration)
	{
		Vec2 tarPos = target->getPlayer()->getPosition();
		target->duration = target->moveSpeed;
		target->m_timecounter->start();
		target->cmd_moveTo(tarPos);
	}			
	
}

void Track::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();//停掉精灵的动画
	target->Is_firstFindplayer_Track = true;//设置第一次发现主角并追踪为true
}


void Attacked::Enter(Monster* target)
{
	log("Attacked!!");

	target->setMachineState(enum_MonsterAttacked);
	
	//设置硬直时间
	target->duration = target->attackedrestoretimes;
	target->m_timecounter->start();

	/*播放染色动作*/
	//两个染色时间合起来小于恢复时间就行
	CCTintTo* action1 = CCTintTo::create(target->attackedrestoretimes / 3, 255, 0, 0);
	CCTintTo* action2 = CCTintTo::create(target->attackedrestoretimes / 3, target->m_monstercolor);
	target->getSprite()->runAction(Sequence::create(action1,action2,NULL));

	//这个参数应该根据主角的攻击力和怪物防御力来算
	target->cmd_hurt(target->damage);
}

void Attacked::Excute(Monster* target)
{
	//如果还是被主角攻击,Excute针对的是多段攻击，即大技能,普通攻击判断写成单段攻击
	if (target->IsattackedByPlayer()){
		log("Attacked!!");
		//重新设置硬直时间
		target->duration = target->attackedrestoretimes;
		target->m_timecounter->start();
		/*播放染色动作*/
		CCTintTo* action1 = CCTintTo::create(target->attackedrestoretimes / 2, 255, 0, 0);
		CCTintTo* action2 = CCTintTo::create(target->attackedrestoretimes / 2, target->m_monstercolor);
		target->getSprite()->runAction(Sequence::create(action1, action2, NULL));

		//这个参数应该是根据主角的攻击力和怪物防御力来算
		target->cmd_hurt(target->damage);
	}
	//如果过了僵直时间而且没有被主角攻击
	if (target->m_timecounter->getCurTime() > target->duration && !target->IsattackedByPlayer()){

		//如果主角在攻击范围
		if (target->checkInAttaRange()){
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}

		//随机静止或者巡逻状态
		switch (rand () % 2)
		{
		case 0:{
			target->getStateMachine()->ChangeState(new Idle());
			break;
		}
		case 1:{
			target->getStateMachine()->ChangeState(new Patrol());
			break;
		}
		}
	}
}

void Attacked::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();
}