#include "StateMachine.h"
#include "Monster.h"
#include "Player.h"
#include "MonsterRemoteskill.h"

#define Pi 3.141592653

void Idle::Enter(Monster* target)
{
	log("Idle");
	//��¼��ǰ״̬
	target->setMachineState(enum_MonsterIdle);
	target->cmd_stop();
	target->duration = rand() % 3 + 2;  // ����ʱ��( 2 - 4 ) ��
	target->m_timecounter->start();	
}

void Idle::Excute(Monster* target)
{
	log("Idle");

	//��������ǹ����ˣ����뱻��״̬
	if (target->IsattackedByPlayer()){
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
	if (target->checkInEyeRange() || target->checkInPerceptionRange())
	{
		target->getStateMachine()->ChangeState(new Track());
	}
	//// ����ʱ���ѵ�������Ѳ��״̬
	else if (target->m_timecounter->getCurTime() >= target->duration)
	{
		target->getStateMachine()->ChangeState(new Patrol());
	}	
}

void Idle::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();//ͣ������Ķ���
}


void Patrol::Enter(Monster* target)
{
	//��¼��ǰ״̬
	target->setMachineState(enum_MonsterPatrol);
	log("patrol");

	std::vector<Vec2> patrolvec = target->getPatrolpointvec();
	if (patrolvec.size() > 0)
	{
		//srand(time(0));
		target->index = rand() % patrolvec.size();
		target->cmd_moveTo(patrolvec[target->index]);
		//����Ѳ�ߵ�Ѱ·�ӳ�duration
		target->duration = target->monsdata.moveSpeed;
		target->m_timecounter->start();
	}
	
	target->IspatrolEnd = false;//��ʼѲ�ߺ�����Ϊfalse
}

void Patrol::Excute(Monster* target)
{
	log("patrol");

	//��������ǹ����ˣ����뱻��״̬
	if (target->IsattackedByPlayer()){
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	//Ѳ�߹����������ж������Ƿ�����Ұ
	// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
	if (target->checkInEyeRange() || target->checkInPerceptionRange())
	{
		target->getStateMachine()->ChangeState(new Track());
	}
	//�������Ѳ��
	else if (target->m_timecounter->getCurTime() > target->duration)
	{
		if (target->IspatrolEnd == true){
			//��Ѱ·��Ѳ�ߵ�Ŀ�ĵأ��������һ��Ѳ��Ŀ�ĵ�
			std::vector<Vec2> patrolvec = target->getPatrolpointvec();
			if (patrolvec.size() > 0)
			{
			//	srand(time(0));
				target->index = rand() % patrolvec.size();
				target->cmd_moveTo(patrolvec[target->index]);
				//����Ѳ�ߵ�Ѱ·�ӳ�duration
				target->duration = target->monsdata.moveSpeed;
				target->m_timecounter->start();
			}
		}
		/*δѲ�ߵ�Ŀ�ĵ� ����Ѳ��*/
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
	target->stopAllActions();//ͣ��Ѱ·���˶�
	target->getSprite()->stopAllActions();//ͣ������Ķ���
}

/*�����ļ��ܴ��ڻ��������״̬*/
void Attack::Enter(Monster* target)
{
	log("attack");
	//��¼��ǰ״̬
	target->setMachineState(enum_MonsterAttack);

	//���ù����ӳ�duration//�������������ڹ������ʱ����
	if (target->m_curskill == "baseskill")
	{
		//��ͨ��������ȴ��ʱ
		target->m_baseskill_timecounter->start();
		//���ù��ﳯ��
		target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());
	}
	else if (target->m_curskill == "bigskill")
	{
		//���ܵ���ȴ��ʱ
		target->m_bigskill_timecounter->start();
		//���ù��ﳯ��
		target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());
	}
	else if (target->m_curskill == "remoteskill")
	{
		//Զ�̼��ܵ���ȴ��ʱ
		target->m_remoteskill_timecounter->start();
		//���ù��ﳯ��
		target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());	
	}
	//cmd_attack����ݹ���m_curskill���ܲ��Ų�ͬ�Ķ���
	target->cmd_attack();
}

void Attack::Excute(Monster* target)
{
	log("attack");
	log("%s", target->m_curskill.c_str());

	//���ù��ﳯ��
	target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());

	//��ǰ����ͨ����
	if (target->m_curskill == "baseskill"){
		//��ͨ��������ʱ�䵽��
		if (target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackEnd)
		{
			//������ܴ���
			if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
			{
				//�����ʱ����ʱ�䵽�ˣ��������ڴ��ܹ�����Χ�����ȴ���
				if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
				{
					target->m_curskill = "bigskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}
						
			//�������ͨ������Χ������ͨ����cd����,������ͨ����
			if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
			{
				target->m_baseskill_timecounter->start();
				target->cmd_attack();
				return;
			}

			//���Զ�̼��ܴ���
			if (target->monsdata.remoteskillmap.find("remoteskill") != target->monsdata.remoteskillmap.end())
			{
				/*�����Զ�̼��ܹ�����Χ�£���������Զ�̼��ܹ���,���Զ�̹���*/
				if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
				{
					target->m_curskill = "remoteskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}
			
			/*���������κμ��ܵĹ��ܷ�Χ*/
			
			// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
			if (target->checkInEyeRange() || target->checkInPerceptionRange())
			{
				target->getStateMachine()->ChangeState(new Track());
				return;
			}
			//�������patol����idle
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
		/*���﹥������δ�������*/

		//�������ǰҡ����������,���ں�ҡ������������ͨ������Χ�ڣ�
		if (target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].beforeattacktimes 
			&& target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].attackEnd &&
			target->checkInAttaRange())
		{
			/*��������ͨ�������������ڶ�ι������ʱ����*/
			if (target->m_baseskill_attackNumsInter_timecounter->getCurTime() == 0.0f ||
				target->m_baseskill_attackNumsInter_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackNumsInter)
			{
				//�����Ǳ�������Ϣ�Ž�����
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

				//���깥����Ϣ���к����¿�����ͨ�����Ķ�ι��������ʱ��
				target->m_baseskill_attackNumsInter_timecounter->start();
			}

		}
	
		//������ﻹ��ǰҡʱ�䣬
		if (target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].beforeattacktimes)
		{
			//�����ǹ�����
			if (target->IsattackedByPlayer())
			{
				//���ǿ��Դ�Ϲ��﹥��
				if (target->monsdata.skillmap["baseskill"].Isinter == true)
				{
					//���뱻��״̬
					target->getStateMachine()->ChangeState(new Attacked());
					return;
				}
				//����ʲô���鶼����,�۹����Ѫ��д��IsattackedByPlayer������
				else
				{
					;
				}
			}		
		}
		//��ǰҡ
		else
		{
			//����target->IsattackedByPlayer
			target->IsattackedByPlayer();
		}
			
	}	

	//�����ǰ�Ǵ���
	if (target->m_curskill == "bigskill"){
		//������ܹ�����������
		if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackEnd)
		{
			//�����ʱ����ʱ�䵽�ˣ��������ڴ��ܹ�����Χ,���ȴ���
			if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
			{
				target->m_curskill = "bigskill";
				target->getStateMachine()->ChangeState(new Attack());
				return;
			}
	
			//�����ͨ�������ܴ���
			if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
			{
				//�������ͨ������Χ������ͨ����cd����
				if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
				{
					target->m_curskill = "baseskill";
					target->m_baseskill_timecounter->start();
					target->cmd_attack();
					return;
				}
			}

			//���Զ�̹������ܴ���
			if (target->monsdata.remoteskillmap.find("remoteskill") != target->monsdata.remoteskillmap.end())
			{
				/*�����Զ�̼��ܹ�����Χ�£���������Զ�̼��ܹ���,���Զ�̹���*/
				if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
				{
					target->m_curskill = "remoteskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}
			/*���������κμ��ܵĹ��ܷ�Χ*/
		
			// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
			if (target->checkInEyeRange() || target->checkInPerceptionRange())
			{
				target->getStateMachine()->ChangeState(new Track());
				return;
			}
			//�������patol����idle
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
		/*���﹥������δ�������*/

		//�������ǰҡ����������,��ʱ�����ڴ��ܹ�����Χ�ڣ�
		if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].beforeattacktimes 
			&& target->m_bigskill_timecounter->getCurTime() < target->monsdata.skillmap["bigskill"].attackEnd &&
			target->checkInBigSkillRange())
		{
			/*�����δ��ܹ������������ڴ��ܶ�ι������ʱ����*/
			if (target->m_bigskill_attackNumsInter_timecounter->getCurTime() == 0 ||
				target->m_bigskill_attackNumsInter_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackNumsInter)
			{
				//�����Ǳ�������Ϣ�Ž�����
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

				//���깥����Ϣ���к����¿������ܹ����Ķ�ι��������ʱ��
				target->m_bigskill_attackNumsInter_timecounter->start();
			}

		}
	
		//������ﻹ��ǰҡʱ��	
		if (target->m_bigskill_timecounter->getCurTime() < target->monsdata.skillmap["bigskill"].beforeattacktimes)
		{
			//��������ǹ�����
			if (target->IsattackedByPlayer())
			{
				//�����жϣ������뱻��״̬
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
		//��ǰҡ
		else
		{
			target->IsattackedByPlayer();
		}
		
	}

	//�����ǰ������Զ�̼���
	if (target->m_curskill == "remoteskill"){
		/*Զ�̼��ܶ������������ں�ҡ����Ͷ����Ʒ*/
		if (target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].beforeattacktimes
			&& target->m_remoteskill_timecounter->getCurTime() < target->monsdata.remoteskillmap["remoteskill"].attackEnd)
		{
			if (target->m_remoteskill_projectile_timecounter->getCurTime() == 0.0f ||
				target->m_remoteskill_projectile_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
			{
				target->m_remoteskill_projectile_timecounter->start();

				//Զ�̼���newһ��Ͷ����
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
				target->getParent()->addChild(projectile, target->getParent()->getChildren().size()); //Ͷ������ӵ���ͼ����
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

				projectile->scheduleUpdate();//Ͷ���￪����ʱ��
			}		
		}

		//Զ�̼��ܶ�����������
		if (target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackEnd)
		{
			//������ܴ���
			if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
			{
				//���������ȴʱ�䵽���������ڴ��ܹ�����Χ
				if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
				{
					target->m_curskill = "bigskill";
					target->getStateMachine()->ChangeState(new Attack());
					return;
				}
			}

			//�����ͨ�������ܴ���
			if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
			{
				//�����ͨcd���˶�����������ͨ������Χ��
				if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
				{
					target->m_curskill = "baseskill";
					target->m_baseskill_timecounter->start();
					target->cmd_attack();
					return;
				}
			}

			//���Զ�̼��ܹ���cdʱ����������Զ�̼��ܹ�����Χ��
			if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
			{
				target->m_curskill = "remoteskill";
				target->getStateMachine()->ChangeState(new Attack());
				return;
			}
			/*���������κμ��ܵĹ��ܷ�Χ*/

			// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
			if (target->checkInEyeRange() || target->checkInPerceptionRange())
			{
				target->getStateMachine()->ChangeState(new Track());
				return;
			}
			//�������patol����idle
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

		/*Զ�̼��ܵĹ������ǵ��ж���Ͷ��������*/

		//������ﻹ��ǰҡʱ��
		if (target->m_bigskill_timecounter->getCurTime() < target->monsdata.remoteskillmap["remoteskill"].beforeattacktimes)
		{
			//��������ǹ�����
			if (target->IsattackedByPlayer())
			{
				//�����жϣ����뱻��״̬//���������ˣ�Ͷ����ͷ��䲻��ȥ��
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
		//��ǰҡ,����ҡ
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
	target->getSprite()->stopAllActions();//ͣ������Ķ���
}

void Track::Enter(Monster* target)
{
	log("track");
	//��¼��ǰ״̬
	target->setMachineState(enum_MonsterTrack);

	Vec2 tarPos = target->getPlayer()->getPosition();
	target->cmd_moveTo(tarPos);

	//����Ѱ·�ӳ�duration
	target->duration = target->monsdata.moveSpeed;
	target->m_timecounter->start();

	target->Is_firstFindplayer_Track = false;//����(��һ�η������ǲ�׷��)Ϊfalse
}

void Track::Excute(Monster* target)
{
	log("track");
	//��������ǹ����ˣ����뱻��״̬
	if (target->IsattackedByPlayer()){
		target->m_curskill = "baseskill";
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}
	
	//�����������֤������ͨ���ܴ���
	if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
	{
		// ���������ͨ������Χ�ڣ��Ҳ�����ͨ����cdʱ��
		if (target->checkInAttaRange() && !(target->m_baseskill_timecounter->getCurTime() > 0 &&
			target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].attackInter))
		{
			target->m_curskill = "baseskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}
	
	//�����������֤���д��ܴ���
	if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
	{
		//���������ܷ�Χ�ڣ��Ҵ���û��ȴ��
		if (target->checkInBigSkillRange() && target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter)
		{
			target->m_curskill = "bigskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}
	
	//�����������֤����Զ�̼��ܴ���
	if (target->monsdata.remoteskillmap.find("remoteskill") != target->monsdata.remoteskillmap.end())
	{
		//�������Զ�̼��ܹ�����Χ����Զ�̼���û��ȴ��
		if (target->checkInRemoteSkillRange() && target->m_remoteskill_timecounter->getCurTime() > target->monsdata.remoteskillmap["remoteskill"].attackInter)
		{
			target->m_curskill = "remoteskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	}
	

	 // �����Ҳ�����Ұ��Χ�ˣ��ȵ�ǰһ��Ѱ·ִ�����ٽ���Ѳ��״̬���߾�ֹ״̬
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
	//������Ұ��Χ������Ѱ·
	else if (target->m_timecounter->getCurTime() > target->duration)
	{
		log("track");
		Vec2 tarPos = target->getPlayer()->getPosition();
		target->duration = target->monsdata.moveSpeed;
		target->m_timecounter->start();
		target->cmd_moveTo(tarPos);
		//������ﲻ���ˣ������ִ򲻵����ǣ�˵������ȷʵ��Ѱ·�����Ǹ����ˣ����ǹ�����Χ�ﲻ�ϣ���ô΢������λ�� 
		if (target->IstrackNoresult == true)
		{
			log("------------------------------------------");
			//ָ�����ǵ�λ��,���벻��100�ģ���Ƭ��С��64��
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
	target->getSprite()->stopAllActions();//ͣ������Ķ���
	target->Is_firstFindplayer_Track = true;//���õ�һ�η������ǲ�׷��Ϊtrue
}


void Attacked::Enter(Monster* target)
{
	log("Attacked!!");

	target->setMachineState(enum_MonsterAttacked);
	
	//����Ӳֱʱ��
	target->duration = target->monsdata.attackedrestoretime;
	target->m_timecounter->start();
}

void Attacked::Excute(Monster* target)
{
	//������Ǳ����ǹ���
	if (target->IsattackedByPlayer()){
		log("Attacked!!");
		//��������Ӳֱʱ��
		target->duration = target->monsdata.attackedrestoretime;
		target->m_timecounter->start();
	}
	//������˽�ֱʱ�����û�б����ǹ���
	if (target->m_timecounter->getCurTime() > target->duration && !target->IsattackedByPlayer()){

		//��������ڹ�����Χ
		if (target->checkInAttaRange()){
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
	
		// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
		//�ӱ���״̬����׷�ٵ��ж�ֱ������Բ�������׷���ж�
		target->Is_firstFindplayer_Track = false;
		if (target->checkInEyeRange() || target->checkInPerceptionRange())
		{
			target->getStateMachine()->ChangeState(new Track()); 
			return;
		}

		//�����������û�������ǣ�����Ѳ��״̬
		target->getStateMachine()->ChangeState(new Patrol());
		
	}
}

void Attacked::Exit(Monster* target)
{
	delete this;
	target->stopAllActions();
	target->getSprite()->stopAllActions();
}