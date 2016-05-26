#include "StateMachine.h"
#include "Monster.h"
#include "Player.h"
#include "MonsterRemoteskill.h"

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

/*攻击的技能存在会进来攻击状态*/
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
		//设置怪物朝向
		target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());
	}
	else if (target->m_curskill == "bigskill")
	{
		//大技能的冷却计时
		target->m_bigskill_timecounter->start();
		//设置怪物朝向
		target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());
	}
	else if (target->m_curskill == "remoteskill")
	{
		//远程技能的冷却计时
		target->m_remoteskill_timecounter->start();
		//设置怪物朝向
		target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());	
	}
	//cmd_attack会根据怪物m_curskill技能播放不同的动画
	target->cmd_attack();
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
			//如果大技能存在
			if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
			{
				//如果此时大技能时间到了，且主角在大技能攻击范围，优先大技能
				if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
				{
					target->m_curskill = "bigskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}
						
			//如果在普通攻击范围内且普通攻击cd到了,继续普通攻击
			if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
			{
				target->m_baseskill_timecounter->start();
				target->cmd_attack();
				return;
			}

			//如果远程技能存在
			if (target->monsdata.remoteskillmap.find("remoteskill") != target->monsdata.remoteskillmap.end())
			{
				/*如果在远程技能攻击范围下，且主角在远程技能攻击,变成远程攻击*/
				if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
				{
					target->m_curskill = "remoteskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}
			
			/*都不处于任何技能的功能范围*/
			
			// 如果玩家进入视野范围或者感知范围，进入追击状态
			if (target->checkInEyeRange() || target->checkInPerceptionRange())
			{
				target->getStateMachine()->ChangeState(new Track());
				return;
			}
			//随机进入patol或者idle
			else
			{
				switch (rand() % 2)
				{
				case 0:{
					target->getStateMachine()->ChangeState(new Idle());
					return;
				}
				case 1:{
					target->getStateMachine()->ChangeState(new Patrol());
					return;
				}
				}
			}			
			
		}
		/*怪物攻击动画未结束情况*/

		//怪物过了前摇的真正攻击,处于后摇，且主角在普通攻击范围内，
		if (target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].beforeattacktimes 
			&& target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].attackEnd &&
			target->checkInAttaRange())
		{
			/*如果多段普通攻击计数器不在多段攻击间隔时间内*/
			if (target->m_baseskill_attackNumsInter_timecounter->getCurTime() == 0.0f ||
				target->m_baseskill_attackNumsInter_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackNumsInter)
			{
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

				//放完攻击消息队列后，重新开启普通攻击的多段攻击间隔计时器
				target->m_baseskill_attackNumsInter_timecounter->start();
			}

		}
	
		//如果怪物还在前摇时间，
		if (target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].beforeattacktimes)
		{
			//被主角攻击了
			if (target->IsattackedByPlayer())
			{
				//主角可以打断怪物攻击
				if (target->monsdata.skillmap["baseskill"].Isinter == true)
				{
					//进入被击状态
					target->getStateMachine()->ChangeState(new Attacked());
					return;
				}
				//否则什么事情都不干,扣怪物的血已写在IsattackedByPlayer函数上
				else
				{
					;
				}
			}		
		}
		//非前摇
		else
		{
			//调用target->IsattackedByPlayer
			target->IsattackedByPlayer();
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
	
			//如果普通攻击技能存在
			if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
			{
				//如果在普通攻击范围内且普通攻击cd到了
				if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
				{
					target->m_curskill = "baseskill";
					target->m_baseskill_timecounter->start();
					target->cmd_attack();
					return;
				}
			}

			//如果远程攻击技能存在
			if (target->monsdata.remoteskillmap.find("remoteskill") != target->monsdata.remoteskillmap.end())
			{
				/*如果在远程技能攻击范围下，且主角在远程技能攻击,变成远程攻击*/
				if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
				{
					target->m_curskill = "remoteskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}
			/*都不处于任何技能的功能范围*/
		
			// 如果玩家进入视野范围或者感知范围，进入追击状态
			if (target->checkInEyeRange() || target->checkInPerceptionRange())
			{
				target->getStateMachine()->ChangeState(new Track());
				return;
			}
			//随机进入patol或者idle
			else
			{
				switch (rand() % 2)
				{
				case 0:{
					target->getStateMachine()->ChangeState(new Idle());
					return;
				}
				case 1:{
					target->getStateMachine()->ChangeState(new Patrol());
					return;
				}
				}
			}			
		}
		/*怪物攻击动画未结束情况*/

		//怪物过了前摇的真正攻击,此时主角在大技能攻击范围内，
		if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].beforeattacktimes 
			&& target->m_bigskill_timecounter->getCurTime() < target->monsdata.skillmap["bigskill"].attackEnd &&
			target->checkInBigSkillRange())
		{
			/*如果多段大技能攻击计数器不在大技能多段攻击间隔时间内*/
			if (target->m_bigskill_attackNumsInter_timecounter->getCurTime() == 0 ||
				target->m_bigskill_attackNumsInter_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackNumsInter)
			{
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

				//放完攻击消息队列后，重新开启大技能攻击的多段攻击间隔计时器
				target->m_bigskill_attackNumsInter_timecounter->start();
			}

		}
	
		//如果怪物还在前摇时间	
		if (target->m_bigskill_timecounter->getCurTime() < target->monsdata.skillmap["bigskill"].beforeattacktimes)
		{
			//如果被主角攻击了
			if (target->IsattackedByPlayer())
			{
				//可以中断，，进入被击状态
				if (target->monsdata.skillmap["bigskill"].Isinter == true)
				{
					target->getStateMachine()->ChangeState(new Attacked());
					return;
				}
				else
				{
					;
				}
			}				
		}
		//非前摇
		else
		{
			target->IsattackedByPlayer();
		}
		
	}

	//如果当前技能是远程技能
	if (target->m_curskill == "remoteskill"){
		/*远程技能动作动画播放在后摇，才投掷物品*/
		if (target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].beforeattacktimes
			&& target->m_remoteskill_timecounter->getCurTime() < target->monsdata.remoteskillmap["remoteskill"].attackEnd)
		{
			if (target->m_remoteskill_projectile_timecounter->getCurTime() == 0.0f ||
				target->m_remoteskill_projectile_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
			{
				target->m_remoteskill_projectile_timecounter->start();

				//远程技能new一个投掷物
				auto remoteskillstr = target->monsdata.remoteskillmap["remoteskill"];
				std::string projectileName = remoteskillstr.projectileName;
				std::string projectileAniamteName = remoteskillstr.projectileAnimateName;
				float width = remoteskillstr.skillwidth;
				float height = remoteskillstr.skillheight;
				float damage = remoteskillstr.damage;
				float duration = remoteskillstr.duration;
				float speed = remoteskillstr.projmovespeed;
				int diretion = target->getAnimBase()->getCurDirection();
				auto projectile = MonsterRemoteskill::createWithName_width_height_damage_duration_speed_diretion(projectileName, projectileAniamteName, width, height, damage, duration, speed, diretion);
				target->getParent()->addChild(projectile, target->getParent()->getChildren().size()); //投掷物添加到地图层上
				Point pos = target->getPosition();
				Point size = target->getContentSize();
				switch (diretion)
				{
				case Dir_up:
					pos.y += size.y * target->getAnchorPoint().y;
					projectile->setRotation(-90);
					break;
				case Dir_down:
					pos.y -= size.y * target->getAnchorPoint().y;
					projectile->setRotation(90);
					break;
				case Dir_left:
					pos.x -= size.x * target->getAnchorPoint().x;
					projectile->setRotation(180);
					break;
				case Dir_right:
					pos.x += size.x * target->getAnchorPoint().x;
					break;
				case Dir_upleft:
					pos.y += size.y * target->getAnchorPoint().y;
					projectile->setRotation(-90);
					break;
				case Dir_upright:
					pos.y += size.y * target->getAnchorPoint().y;
					projectile->setRotation(-90);
					break;
				case Dir_downleft:
					pos.y -= size.y * target->getAnchorPoint().y;
					projectile->setRotation(90);
					break;
				case Dir_downright:
					pos.y -= size.y * target->getAnchorPoint().y;
					projectile->setRotation(90);
					break;
				case Dir_leftup:
					pos.x -= size.x * target->getAnchorPoint().x;
					projectile->setRotation(180);
					break;
				case Dir_rightup:
					pos.x -= size.x * target->getAnchorPoint().x;
					projectile->setRotation(180);
					break;
				case Dir_leftdown:
					pos.x -= size.x * target->getAnchorPoint().x;
					projectile->setRotation(180);
					break;
				case Dir_rightdown:
					pos.x -= size.x * target->getAnchorPoint().x;
					projectile->setRotation(180);
					break;
				}
				projectile->setPosition(pos);

				projectile->scheduleUpdate();//投掷物开启定时器
			}		
		}

		//远程技能动画播放完了
		if (target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackEnd)
		{
			//如果大技能存在
			if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
			{
				//如果大技能冷却时间到了且主角在大技能攻击范围
				if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
				{
					target->m_curskill = "bigskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}

			//如果普通攻击技能存在
			if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
			{
				//如果普通cd过了而且主角在普通攻击范围内
				if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
				{
					target->m_curskill = "baseskill";
					target->m_baseskill_timecounter->start();
					target->cmd_attack();
					return;
				}
			}

			//如果远程技能过了cd时间且主角在远程技能攻击范围内
			if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
			{
				target->m_curskill = "remoteskill";
				target->getStateMachine()->ChangeState(new Attack());
				return;
			}
			/*都不处于任何技能的功能范围*/

			// 如果玩家进入视野范围或者感知范围，进入追击状态
			if (target->checkInEyeRange() || target->checkInPerceptionRange())
			{
				target->getStateMachine()->ChangeState(new Track());
				return;
			}
			//随机进入patol或者idle
			else
			{
				switch (rand() % 2)
				{
				case 0:{
					target->getStateMachine()->ChangeState(new Idle());
					return;
				}
				case 1:{
					target->getStateMachine()->ChangeState(new Patrol());
					return;
				}
				}
			}
			
		}

		/*远程技能的攻击主角的判定在投掷物类里*/

		//如果怪物还在前摇时间
		if (target->m_bigskill_timecounter->getCurTime() < target->monsdata.remoteskillmap["remoteskill"].beforeattacktimes)
		{
			//如果被主角攻击了
			if (target->IsattackedByPlayer())
			{
				//可以中断，进入被击状态//如果被打断了，投掷物就发射不出去的
				if (target->monsdata.remoteskillmap["remoteskill"].Isinter == true)
				{
					target->getStateMachine()->ChangeState(new Attacked());
					return;
				}
				else
				{
					;
				}
			}			
		}
		//非前摇,即后摇
		else
		{
			target->IsattackedByPlayer();
		}
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

	target->Is_firstFindplayer_Track = false;//设置(第一次发现主角并追踪)为false
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
	
	//如果条件成立证明有普通技能存在
	if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
	{
		// 如果进入普通攻击范围内，且不在普通攻击cd时间
		if (target->checkInAttaRange() && !(target->m_baseskill_timecounter->getCurTime() > 0 &&
			target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].attackInter))
		{
			target->m_curskill = "baseskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}
	
	//如果条件成立证明有大技能存在
	if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
	{
		//如果进入大技能范围内，且大技能没冷却了
		if (target->checkInBigSkillRange() && target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter)
		{
			target->m_curskill = "bigskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}
	
	//如果条件成立证明有远程技能存在
	if (target->monsdata.remoteskillmap.find("remoteskill") != target->monsdata.remoteskillmap.end())
	{
		//如果进入远程技能攻击范围，且远程技能没冷却了
		if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
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
}

void Attacked::Excute(Monster* target)
{
	//如果还是被主角攻击
	if (target->IsattackedByPlayer()){
		log("Attacked!!");
		//重新设置硬直时间
		target->duration = target->monsdata.attackedrestoretime;
		target->m_timecounter->start();
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