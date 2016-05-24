#include "StateMachine.h"
#include "Monster.h"
#include "Player.h"
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
	}
	else if (target->m_curskill == "bigskill")
	{
		//���ܵ���ȴ��ʱ
		target->m_bigskill_timecounter->start();
	}
	else if (target->m_curskill == "remoteskill")
	{
		//Զ�̼��ܵ���ȴ��ʱ
		target->m_remoteskill_timecounter->start();
	}

	//���ù��ﳯ��
	target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());
	//cmd_attack����ݹ���m_curskill���ܲ��Ų�ͬ�Ķ���
	target->cmd_attack();
	target->Isrestartbaseattack = true;
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
			//�����ʱ����ʱ�䵽�ˣ��������ڴ��ܹ�����Χ�����ȴ���
			if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].attackInter && target->checkInBigSkillRange())
			{
				target->m_curskill = "bigskill";
				target->getStateMachine()->ChangeState(new Attack());
				return;
			}

			//�������ͨ������Χ������ͨ����cd����,������ͨ����
			if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
			{
				target->m_baseskill_timecounter->start();
				target->cmd_attack();
				target->Isrestartbaseattack = true;
				return;
			}

			//����ͨ������������ǰ���£��Ҳ�����ͨ������Χ��
			if (!target->checkInAttaRange())
			{
				// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
				if (target->checkInEyeRange() || target->checkInPerceptionRange())
				{
					target->getStateMachine()->ChangeState(new Track());
				}
				//�������patol����idle
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

		//�������ǰҡ����������,���ں�ҡ���������ڹ�����Χ�ڣ�
		if (target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].beforeattacktimes 
			&& target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].attackEnd &&
			target->checkInAttaRange())
		{
			//��ͨ����Ϊ���ι���
			if (target->Isrestartbaseattack == true)
			{
				target->Isrestartbaseattack = false;
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
			}
		}

		//������ﻹ��ǰҡʱ�䣬���ǿ��Դ�Ϲ��﹥��
		//��������ǹ����ˣ����뱻��״̬
		if (target->m_baseskill_timecounter->getCurTime() < target->monsdata.skillmap["baseskill"].beforeattacktimes && target->IsattackedByPlayer()){
			target->getStateMachine()->ChangeState(new Attacked());
			return;
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

			//�������ͨ������Χ������ͨ����cd����
			if (target->checkInAttaRange() && target->m_baseskill_timecounter->getCurTime() > target->monsdata.skillmap["baseskill"].attackInter)
			{
				target->m_curskill = "baseskill";
				target->m_timecounter->start();
				target->cmd_attack();
				target->Isrestartbaseattack = true;
				return;
			}

			//���ܶ����������ˣ��Ҳ�����ͨ������Χ���ˣ���״̬
			if (!target->checkInAttaRange())
			{
				// �����ҽ�����Ұ��Χ���߸�֪��Χ������׷��״̬
				if (target->checkInEyeRange() || target->checkInPerceptionRange())
				{
					target->getStateMachine()->ChangeState(new Track());
				}
				//�������patol����idle
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

		/*�����ں�ҡ��*/

		//�������ǰҡ����������,��ʱ�����ڴ��ܹ�����Χ�ڣ�
		if (target->m_bigskill_timecounter->getCurTime() > target->monsdata.skillmap["bigskill"].beforeattacktimes 
			&& target->m_bigskill_timecounter->getCurTime() < target->monsdata.skillmap["bigskill"].attackEnd &&
			target->checkInBigSkillRange())
		{
			//���ܹ���Ϊ�����Ķ�ι���
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
		}
		
		/*���ܲ��ܱ������жϣ�����Ҫд*/
		
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

	target->Is_firstFindplayer_Track = false;//���õ�һ�η������ǲ�׷��Ϊfalse
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

	//// ���������ͨ������Χ�ڣ�
	if (target->checkInAttaRange())
	{
		//�����������֤������ͨ���ܴ���
		float cd = 0.0f;
		if (target->monsdata.skillmap.find("baseskill") != target->monsdata.skillmap.end())
			cd = target->monsdata.skillmap["baseskill"].attackInter;
		//��ͨ����cd��ȴ����
		if (target->m_baseskill_timecounter->getCurTime() > cd)
		{
			target->m_curskill = "baseskill";
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}
		
	}
	////�ڴ��ܵĹ�����Χ
	if (target->checkInBigSkillRange())
	{
		//�����������֤���д��ܴ���,
		float cd = 0.0f;
		if (target->monsdata.skillmap.find("bigskill") != target->monsdata.skillmap.end())
			cd = target->monsdata.skillmap["bigskill"].attackInter;
		//����cd��ȴ����
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
		//Զ�̼���cd��ȴ����
		if (target->m_remoteskill_timecounter->getCurTime() > cd)
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

	/*����Ⱦɫ����*/
	//����Ⱦɫʱ�������С�ڻָ�ʱ�����
	CCTintTo* action1 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, 255, 0, 0);
	CCTintTo* action2 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, target->m_monstercolor);
	target->getSprite()->runAction(Sequence::create(action1,action2,NULL));

	//�������Ӧ�ø������ǵĹ������͹������������
	//target->cmd_hurt(target->damage);
}

void Attacked::Excute(Monster* target)
{
	//������Ǳ����ǹ���,Excute��Ե��Ƕ�ι�����������,��ͨ�����ж�д�ɵ��ι���
	if (target->IsattackedByPlayer()){
		log("Attacked!!");
		//��������Ӳֱʱ��
		target->duration = target->monsdata.attackedrestoretime;
		target->m_timecounter->start();
		/*����Ⱦɫ����*/
		CCTintTo* action1 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, 255, 0, 0);
		CCTintTo* action2 = CCTintTo::create(target->monsdata.attackedrestoretime / 3, target->m_monstercolor);
		target->getSprite()->runAction(Sequence::create(action1, action2, NULL));

		//�������Ӧ���Ǹ������ǵĹ������͹������������
		//target->cmd_hurt(target->damage);
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