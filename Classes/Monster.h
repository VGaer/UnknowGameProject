#ifndef _Monster_H_
#define _Monster_H_

#include "Entity.h"
#include "Player.h"
#include "AnimBase.h"

class Monster : Entity
{
public:
	// ���Գ�ʼ��
	static Monster* create(const std::string& file);
	bool init(const std::string& file);
public:
	// ���Բ���
	float hp;
	float damage;
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
public:
	// �����
	void cmd_moveTo(Point);			// �����ƶ�
	void cmd_stop();				// ֹͣ�ƶ� 
	bool cmd_hurt(float damage);	// ����ֵ��ʾ�Ƿ�����
	void cmd_attack();				// ���й���
private:
	Player* player;	
	AnimBase* animBase;             // ����������
	void changeDirection();			// �ı��ƶ�����
	bool checkRange();				// �Ƿ��ڹ�����Χ
};

#endif