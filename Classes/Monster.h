#ifndef __Monster_H__
#define __Monster_H__

#include "Entity.h"
#include "Player.h"
#include "FindPath.h"
#include "AnimBase.h"
#include "StateMachine.h"
#include "TimeCounter.h"
#include "cocos2d.h"
#include <vector>

class FindPath;
class StateMachine;

typedef enum 
{
	enum_MonsterIdle = 0,
	enum_MonsterPatrol,
	enum_MonsterAttack,
	enum_MonsterTrack,
	enum_MonsterAttacked
};

class AnimBase;

class Monster : public Entity
{
public:
	// ���Գ�ʼ��
	static Monster* create(const std::string& name);
	bool init(const std::string& name);
public:
	// ���Բ���
	std::string name;  // ����
	float hp;		   // ����ֵ
	float damage;	   // ������
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
	float patrolRange; // Ѳ�߰뾶
	float eyeRange;    // ��Ұ�뾶
	float perceptionRange; //��֪�뾶
	float attackedrestoretimes; //Ӳֱʱ��
	float beforeattacktimes;      //ǰҡʱ��
	float attackAnimatetimePer; //������������ʱ��
	// ����ӿ�
	AnimBase* getAnimBase();
	StateMachine* getStateMachine();
	Player* getPlayer();
	void bindPlayer(Player* player);//������
public:
	// �����
	void cmd_moveTo(Point tarPos);	// �����ƶ�
	void cmd_stop();                // ��ֹ
	bool cmd_hurt(float damage);	// ����ֵ��ʾ�Ƿ�����
	void cmd_attack();				// ���й���	
	bool checkInEyeRange();			// �Ƿ�����Ұ��Χ��
	bool checkInPerceptionRange();  // �Ƿ��ڸ�֪����
	bool checkInAttaRange();		// �Ƿ��ڹ�����Χ��	
public:
	void setMachineState(int i);
	int checkInSectorRange();   //������Ұ����
	int getCurMachineState();
	void setMonsterParent(TMXTiledMap* parent);
	int getMonster_magnification();
	bool Is_firstFindplayer_Track;
	void setvecPatrolpoint();
	~Monster();
	std::vector<Vec2> getPatrolpointvec();
	TimeCounter* m_timecounter;
	float duration;
	bool IspatrolEnd;//Ѳ�����
	int index; //Ѳ�ߵ��¼
	bool IsattackedByPlayer();	//�Ƿ����ǹ���
	Color3B m_monstercolor; //������ɫ
	bool Isrestartbaseattack;	//��¼�����Ƿ�������ͨ����
	Vec2 prePosition;    //׷��ʱ��¼����ǰһ�ε�λ��
private:
	Player* player;
	AnimBase* animBase;             // ����������
	FindPath* findPath;             // �Զ�Ѱ·
	StateMachine* stateMachine;		// ״̬��
	void update(float);				// ����״̬��
	int m_CurMachineState;								// ״̬����״̬
	TMXTiledMap* m_parrent;		//
	int m_magnification;//���ﾫ��Ŵ���
	std::vector<Vec2> vecPatrolpoint; //Ѳ�ߵ�����
};
#endif