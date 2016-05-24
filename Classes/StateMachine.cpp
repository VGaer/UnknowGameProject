#include "StateMachine.h"
#include "Monster.h"
#include "Player.h"
#define Pi 3.141592653

void Idle::Enter(Monster* target)
{
	log("Idle");
	//记录当前状态
	target->setMachineState(enum_MonsterIdle);
	target->cmd_stop();
	target->duration = rand() % 3 + 2;  // 滞留时间( 2 - 4 ) 秒
	target->m_timecounter->start();	
}

void Idle::Excute(Monster* target)
{
	log("Idle");

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
	log("patrol");

	std::vector<Vec2> patrolvec = target->getPatrolpointvec();
	if (patrolvec.size() > 0)
	{
		//srand(time(0));
		target->index = rand() % patrolvec.size();
		target->cmd_moveTo(patrolvec[target->index]);
		//设置巡逻的寻路延迟duration
		target->duration = target->monsdata.moveSpeed;
		target->m_timecounter->start();
	}
	
	target->IspatrolEnd = false;//开始巡逻后，设置为false
}

void Patrol::Excute(Monster* target)
{
	log("patrol");

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
				target->duration = target->monsdata.moveSpeed;
				target->m_timecounter->start();
			}
		}
		/*未巡逻到目的地 继续巡逻*/
		else{
			target->cmd_moveTo(target->getPatrolpointvec()[target->index]);
			target->duration = target->monsdata.moveSpeed;
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
	log("attack");
	//记录当前状态
	target->setMachineState(enum_MonsterAttack);

	//设置攻击延迟duration//攻击动画设置在攻击间隔时间内
	if (target->m_curskill == "baseskill")
	{
		//普通攻击的冷却计时
		target->m_baseskill_timecounter->start();
	}
	else if (target->m_curskill == "bigskill")
	{
		//大技能的冷却计时
		target->m_bigskill_timecounter->start();
	}
	else if (target->m_curskill == "remoteskill")
	{
		//远程技能的冷却计时
		target->m_remoteskill_timecounter->start();
	}

	//设置怪物朝向
	target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());
	//cmd_attack会根据怪物m_curskill技能播放不同的动画
	target->cmd_attack();
	target->Isrestartbaseattack = true;
}

void Attack::Excute(Monster* target)
{
	log("attack");
	log("%s", target->m_curskill.c_str());

	//设置怪物朝向
	target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());

	//当前是普通攻击
	if (target->m_curskill == "baseskill"){
		//普通攻击动画时间到了
		if (target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackEnd)
		{
			//如果此时大技能时间到了，且主角在大技能攻击范围，优先大技能
			if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
			{
				target->m_curskill = "bigskill";
				target->getStateMachine()->ChangeState(new Attack());
				return;
			}

			//如果在普通攻击范围内且普通攻击cd到了,继续普通攻击
			if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
			{
				target->m_baseskill_timecounter->start();
				target->cmd_attack();
				target->Isrestartbaseattack = true;
				return;
			}

			//在普通攻击动画结束前提下，且不在普通攻击范围了
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

		//怪物过了前摇的真正攻击,处于后摇，且主角在攻击范围内，
		if (target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].beforeattacktimes 
			&& target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].attackEnd &&
			target->checkInAttaRange())
		{
			//普通攻击为单段攻击
			if (target->Isrestartbaseattack == true)
			{
				target->Isrestartbaseattack = false;
				//把主角被攻击信息放进队列
				switch (target->getAnimBase()->getCurDirection())
				{
				case Dir_up:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromdown);
					break;
				}
				case Dir_down:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromup);
					break;
				}
				case Dir_left:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromright);
					break;
				}
				case Dir_right:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromleft);
					break;
				}
				case Dir_upleft:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromdown);
					break;
				}
				case Dir_upright:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromdown);
					break;
				}
				case Dir_downleft:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromup);
					break;
				}
				case Dir_downright:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromup);
					break;
				}
				case Dir_leftup:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromright);
					break;
				}
				case Dir_rightup:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromleft);
					break;
				}
				case Dir_leftdown:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromright);
					break;
				}
				case Dir_rightdown:{
					target->getPlayer()->attackedqueue.push(enum_playerattackedfromleft);
					break;
				}
				}
			}
		}

		//如果怪物还在前摇时间，主角可以打断怪物攻击
		//如果被主角攻击了，进入被击状态
		if (target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].beforeattacktimes && target->IsattackedByPlayer()){
			target->getStateMachine()->ChangeState(new Attacked());
			return;
		}
		
	}	

	//如果当前是大技能
	if (target->m_curskill == "bigskill"){
		//如果大技能攻击动画结束
		if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackEnd)
		{
			//如果此时大技能时间到了，且主角在大技能攻击范围,优先大技能
			if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
			{
				target->m_curskill = "bigskill";
				target->getStateMachine()->ChangeState(new Attack());
				return;
			}

			//如果在普通攻击范围内且普通攻击cd到了
			if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
			{
				target->m_curskill = "baseskill";
				target->m_timecounter->start();
				target->cmd_attack();
				target->Isrestartbaseattack = true;
				return;
			}

			//大技能动画播放完了，且不在普通攻击范围内了，切状态
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

		/*大技能在后摇期*/

		//怪物过了前摇的真正攻击,此时主角在大技能攻击范围内，
		if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].beforeattacktimes 
			&& target->m_bigskill_timecounter->getCurTime() < target->monsdata.skillmap["bigskill"].attackEnd &&
			target->checkInBigSkillRange())
		{
			//大技能攻击为持续的多段攻击
			//把主角被攻击信息放进队列
			switch (target->getAnimBase()->getCurDirection())
			{
			case Dir_up:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromdown);
				break;
			}
			case Dir_down:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromup);
				break;
			}
			case Dir_left:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromright);
				break;
			}
			case Dir_right:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromleft);
				break;
			}
			case Dir_upleft:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromdown);
				break;
			}
			case Dir_upright:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromdown);
				break;
			}
			case Dir_downleft:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromup);
				break;
			}
			case Dir_downright:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromup);
				break;
			}
			case Dir_leftup:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromright);
				break;
			}
			case Dir_rightup:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromleft);
				break;
			}
			case Dir_leftdown:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromright);
				break;
			}
			case Dir_rightdown:{
				target->getPlayer()->attackedqueue.push(enum_playerattackedfromleft);
				break;
			}
			}	
		}
		
		/*大技能不能被主角中断，不需要写*/
		
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
	log("track");
	//记录当前状态
	target->setMachineState(enum_MonsterTrack);

	Vec2 tarPos = target->getPlayer()->getPosition();
	target->cmd_moveTo(tarPos);

	//设置寻路延迟duration
	target->duration = target->monsdata.moveSpeed;
	target->m_timecounter->start();

	target->Is_firstFindplayer_Track = false;//设置第一次发现主角并追踪为false
}

void Track::Excute(Monster* target)
{
	log("track");
	//如果被主角攻击了，进入被击状态
	if (target->IsattackedByPlayer()){
		target->m_curskill = "baseskill";
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	//// 如果进入普通攻击范围内，
	if (target->checkInAttaRange())
	{
		//如果条件成立证明有普通技能存在
		float cd = 0.0f;
		if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
			cd = target->monsdata.skillmap["baseskill"].attackInter;
		//普通攻击cd冷却好了
		if (target->m_baseskill_timecounter->getCurTime() > cd)
		{
			target->m_curskill = "baseskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
		
	}
	////在大技能的攻击范围
	if (target->checkInBigSkillRange())
	{
		//如果条件成立证明有大技能存在,
		float cd = 0.0f;
		if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
			cd = target->monsdata.skillmap["bigskill"].attackInter;
		//大技能cd冷却好了
		if (target->m_bigskill_timecounter->getCurTime() > cd)
		{
			target->m_curskill = "bigskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}

	if (target->checkInRemoteSkillRange())
	{
		float cd = 0.0f;
		if (target->monsdata.skillmap.find("remoteskill") != target->monsdata.skillmap.end())
			cd = target->monsdata.skillmap["remoteskill"].attackInter;
		//远程技能cd冷却好了
		if (target->m_remoteskill_timecounter->getCurTime() > cd)
		{
			target->m_curskill = "remoteskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}

	 // 如果玩家不在视野范围了，等当前一步寻路执行完再进入巡逻状态或者静止状态
	if(!target->checkInEyeRange() && target->m_timecounter->getCurTime() > target->duration)
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
		log("track");
		Vec2 tarPos = target->getPlayer()->getPosition();
		target->duration = target->monsdata.moveSpeed;
		target->m_timecounter->start();
		target->cmd_moveTo(tarPos);
		//如果怪物不动了，但是又打不到主角，说明怪物确实是寻路到主角附近了，但是攻击范围达不上，那么微调怪物位置 
		if (target->IstrackNoresult == true)
		{
			log("------------------------------------------");
			//指向主角的位移,距离不超100的，瓦片大小才64，
			Vec2 dif = target->getPlayer()->getPosition() - target->getPosition();
			dif = dif * 0.1;
			target->setPosition(target->getPosition() + dif);
		}
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
	target->duration = target->monsdata.attackedrestoretime;
	target->m_timecounter->start();

	/*播放染色动作*/
	//两个染色时间合起来小于恢复时间就行
	CCTintTo* action1 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, 255, 0, 0);
	CCTintTo* action2 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, target->m_monstercolor);
	target->getSprite()->runAction(Sequence::create(action1,action2,NULL));

	//这个参数应该根据主角的攻击力和怪物防御力来算
	//target->cmd_hurt(target->damage);
}

void Attacked::Excute(Monster* target)
{
	//如果还是被主角攻击,Excute针对的是多段攻击，即大技能,普通攻击判断写成单段攻击
	if (target->IsattackedByPlayer()){
		log("Attacked!!");
		//重新设置硬直时间
		target->duration = target->monsdata.attackedrestoretime;
		target->m_timecounter->start();
		/*播放染色动作*/
		CCTintTo* action1 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, 255, 0, 0);
		CCTintTo* action2 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, target->m_monstercolor);
		target->getSprite()->runAction(Sequence::create(action1, action2, NULL));

		//这个参数应该是根据主角的攻击力和怪物防御力来算
		//target->cmd_hurt(target->damage);
	}
	//如果过了僵直时间而且没有被主角攻击
	if (target->m_timecounter->getCurTime() > target->duration && !target->IsattackedByPlayer()){

		//如果主角在攻击范围
		if (target->checkInAttaRange()){
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	
		// 如果玩家进入视野范围或者感知范围，进入追击状态
		//从被击状态进入追踪的判断直接启动圆形区域的追踪判断
		target->Is_firstFindplayer_Track = false;
		if (target->checkInEyeRange() || target->checkInPerceptionRange())
		{
			target->getStateMachine()->ChangeState(new Track()); 
			return;
		}

		//被攻击后，如果没发现主角，进入巡逻状态
		target->getStateMachine()->ChangeState(new Patrol());
		
	}
}

void Attacked::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();
}