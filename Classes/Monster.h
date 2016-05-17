#ifndef _Monster_H_
#define _Monster_H_

#include "Entity.h"
#include "Player.h"
#include "FindPath.h"
#include "AnimBase.h"
#include "StateMachine.h"

class FindPath;

class Monster : public Entity
{
public:
	// ���Գ�ʼ��
	static Monster* create(const std::string& name);
	bool init(const std::string& name);
public:
	// ���Բ���
	std::string name;	   // ����
	float hp;		   // ����ֵ
	float damage;	   // ������
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
	float eyeRange;    // ��Ұ�뾶
	// ����ӿ�
	AnimBase* getAnimBase();
	StateMachine* getStateMachine();
	Player* getPlayer();
public:
	// �����
	void cmd_moveTo(Point tarPos);	// �����ƶ�
	void cmd_stop();                // ��ֹ
	bool cmd_hurt(float damage);	// ����ֵ��ʾ�Ƿ�����
	void cmd_attack();				// ���й���	
	bool checkInEyeRange();			// �Ƿ�����Ұ��Χ��
	bool checkInAttaRange();		// �Ƿ��ڹ�����Χ��
private:
	Player* player;	
	AnimBase* animBase;             // ����������
	FindPath* findPath;             // �Զ�Ѱ·
	StateMachine* stateMachine;		// ״̬��
	void update(float);				// ����״̬��
};

#endif