#ifndef _SkillControl_H_
#define _SkillControl_H_

#include "Player.h"
#include "TimeCounter.h"

#define MAX_SKILL_NUM 5		// δ��

#define SKILL_LASER_CD 2

class Player;

enum SkillType
{
	skillType_laser = 0,
};

class SkillControl
{
public:
	// ��ʼ������
	SkillControl(Player* player);
public:
	void useSkill(int id);  // �󶨿�ݼ������õ���
	// �����б�
	void skill_laser();
private:
	// ���
	Player* m_player;
	// ���ܼ�ʱ��
	TimeCounter skillCounter[MAX_SKILL_NUM];
};

#endif