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
	int getDirection(Point tarPos);					// ��ȡ�ƶ�����
	void playMoveAnim(int direction);				// ����
	void playAttackAnim(int direction);				// ���Ź�������
public:
	Sprite* m_sprite;
	AnimDirection m_curDirection;		// ��ǰ����
	Action* m_moveAnimArray[3];			// ����ƶ�����
	Action* m_attaAnimArray[3];			// ��Ź�������
};

#endif