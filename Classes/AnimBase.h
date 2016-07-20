#ifndef _AnimBase_H_
#define _AnimBase_H_

#include "Entity.h"
#include "Monster.h"

enum AnimDirection
{
	Dir_up = 0,
	Dir_down = 1,
	Dir_left = 2,
	Dir_right = 3,
	Dir_upleft = 4,//˭��ǰ˭���������򣬷���Ϊ������
	Dir_upright = 5,
	Dir_downleft = 6,
	Dir_downright = 7,
	Dir_leftup = 8,
	Dir_rightup = 9,
	Dir_leftdown = 10,
	Dir_rightdown = 11
};

class Monster;

class AnimBase
{
public:
	AnimBase(Entity* target);
	~AnimBase();
	int getDirectionByTargetPos(Point tarPos);		// ��ȡ�ƶ�����
	void playStanAnim();				// ���ž�ֹ����
	void playMoveAnim();				// �����ƶ�����
	void playAttaAnim();				// ���Ź�������
	int getCurDirection();							// ��ȡ��ǰ����
	void setCurDirection(Point tarPos);							// ���õ�ǰ����
	void setAction();
public:
	Sprite* m_sprite;
	Monster* m_monster;
	AnimDirection m_curDirection;		// ��ǰ����
	Action* m_stanAnimArray[3];         // ��ֹ��������
	Action* m_moveAnimArray[3];			// �ƶ���������
	Action* m_attaAnimArray[3];			// ������������
	Action* m_attaBigAnimArray[3];      // ��������
	Action* m_attaRemoteAnimArray[3];   // Զ�̼���
	Action* m_attaRemoteDurAnimArray[3]; //Զ�̼��ܣ�������
};

#endif