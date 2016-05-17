#ifndef _AnimBase_H_
#define _AnimBase_H_

#include "Entity.h"

enum AnimDirection
{
	Dir_up = 0,
	Dir_down = 1,
	Dir_left = 2,
	Dir_right = 3
};

class AnimBase
{
public:
	AnimBase(Entity* target);
	int getDirectionByTargetPos(Point tarPos);		// ��ȡ�ƶ�����
	void playStanAnim(int direction);				// ���ž�ֹ����
	void playMoveAnim(int direction);				// �����ƶ�����
	void playAttaAnim(int direction);				// ���Ź�������
	int getCurDirection();							// ��ȡ��ǰ����
public:
	Sprite* m_sprite;
	AnimDirection m_curDirection;		// ��ǰ����
	Action* m_stanAnimArray[3];         // ��ֹ��������
	Action* m_moveAnimArray[3];			// �ƶ���������
	Action* m_attaAnimArray[3];			// ������������
};

#endif