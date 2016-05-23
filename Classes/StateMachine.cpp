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
		target->duration = target->moveSpeed;
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
				target->duration = target->moveSpeed;
				target->m_timecounter->start();
			}
		}
		/*δѲ�ߵ�Ŀ�ĵ� ����Ѳ��*/
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
	target->stopAllActions();//ͣ��Ѱ·���˶�
	target->getSprite()->stopAllActions();//ͣ������Ķ���
}

void Attack::Enter(Monster* target)
{
	log("attack");
	//��¼��ǰ״̬
	target->setMachineState(enum_MonsterAttack);

	//���ù����ӳ�duration//�������������ڹ������ʱ����
	target->duration = target->attackInter;
	target->m_timecounter->start();

	target->cmd_attack();
	target->Isrestartbaseattack = true;
}

void Attack::Excute(Monster* target)
{
	log("attack");
	//��¼��ǰ״̬

	//���ù��ﳯ��
	target->getAnimBase()->setCurDirection(target->getPlayer()->getPosition());

	//�ڹ�����Χ���������ʱ�䵽�ˣ���������
	if (target->m_timecounter->getCurTime() > target->attackInter && target->checkInAttaRange())
	{
		target->duration = target->attackInter;
		target->m_timecounter->start();
		//log("time----%f", target->m_timecounter->getCurTime());
		target->cmd_attack();
		target->Isrestartbaseattack = true;
		return;
	}

	//�������ǰҡ����������,��ʱ�����ڹ�����Χ�ڣ�
	if (target->m_timecounter->getCurTime() > target->beforeattacktimes && target->checkInAttaRange())
	{
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
	if (target->m_timecounter->getCurTime() < target->beforeattacktimes && target->IsattackedByPlayer()){
	//	log("time %f", target->m_timecounter->getCurTime());
		target->getStateMachine()->ChangeState(new Attacked());
		return;
		}
	

	//���ڹ�����Χ��
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
	target->duration = target->moveSpeed;
	target->m_timecounter->start();

	target->Is_firstFindplayer_Track = false;//���õ�һ�η������ǲ�׷��Ϊfalse
}

void Track::Excute(Monster* target)
{
	log("track");

	//��������ǹ����ˣ����뱻��״̬
	if (target->IsattackedByPlayer()){
		target->getStateMachine()->ChangeState(new Attacked());
		return;
	}

	//��¼����λ��
	target->prePosition = target->getPosition();
	
	// ������빥����Χ�ڣ�ת������״̬
	if (target->checkInAttaRange())
	{
		target->getStateMachine()->ChangeState(new Attack());
	}
	 // �����Ҳ�����Ұ��Χ�ˣ��ȵ�ǰһ��Ѱ·ִ�����ٽ���Ѳ��״̬���߾�ֹ״̬
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
	//������Ұ��Χ������Ѱ·
	else if (target->m_timecounter->getCurTime() > target->duration)
	{
		Vec2 tarPos = target->getPlayer()->getPosition();
		target->duration = target->moveSpeed;
		target->m_timecounter->start();
		target->cmd_moveTo(tarPos);
		//������ﲻ���ˣ������ִ򲻵����ǣ�˵������ȷʵ��Ѱ·�����Ǹ����ˣ����ǹ�����Χ�ﲻ�ϣ���ô΢������λ�� 
		if (target->getPosition() == target->prePosition)
		{
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
	target->duration = target->attackedrestoretimes;
	target->m_timecounter->start();

	/*����Ⱦɫ����*/
	//����Ⱦɫʱ�������С�ڻָ�ʱ�����
	CCTintTo* action1 = CCTintTo::create(target->attackedrestoretimes / 3, 255, 0, 0);
	CCTintTo* action2 = CCTintTo::create(target->attackedrestoretimes / 3, target->m_monstercolor);
	target->getSprite()->runAction(Sequence::create(action1,action2,NULL));

	//�������Ӧ�ø������ǵĹ������͹������������
	target->cmd_hurt(target->damage);
}

void Attacked::Excute(Monster* target)
{
	//������Ǳ����ǹ���,Excute��Ե��Ƕ�ι�����������,��ͨ�����ж�д�ɵ��ι���
	if (target->IsattackedByPlayer()){
		log("Attacked!!");
		//��������Ӳֱʱ��
		target->duration = target->attackedrestoretimes;
		target->m_timecounter->start();
		/*����Ⱦɫ����*/
		CCTintTo* action1 = CCTintTo::create(target->attackedrestoretimes / 3, 255, 0, 0);
		CCTintTo* action2 = CCTintTo::create(target->attackedrestoretimes / 3, target->m_monstercolor);
		target->getSprite()->runAction(Sequence::create(action1, action2, NULL));

		//�������Ӧ���Ǹ������ǵĹ������͹������������
		target->cmd_hurt(target->damage);
	}
	//������˽�ֱʱ�����û�б����ǹ���
	if (target->m_timecounter->getCurTime() > target->duration && !target->IsattackedByPlayer()){

		//��������ڹ�����Χ
		if (target->checkInAttaRange()){
			target->getStateMachine()->ChangeState(new Attack());
			return;
		}

		//�����ֹ����Ѳ��״̬
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