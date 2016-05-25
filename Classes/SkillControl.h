#ifndef _SkillControl_H_
#define _SkillControl_H_

#include "Player.h"
#include "TimeCounter.h"

#define MAX_SKILL_NUM 1		// δ��



class Player;

enum SkillType
{
	skillType_laser = 0,
};

class SkillControl : public Node
{
public:
	// ��ʼ������
	CREATE_FUNC(SkillControl);
	virtual bool init();
public:
	void useSkill(int id);  // �󶨿�ݼ������õ���
	// �����б�
	bool skill_laser();
private:
	// ���
	Player* m_player;
	// ���ܼ�ʱ��
	TimeCounter* skillCounter[MAX_SKILL_NUM];
};

#endif