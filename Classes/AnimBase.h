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
	int getDirection(Point tarPos);					// 获取移动方向
	void playMoveAnim(int direction);				// 播放
	void playAttackAnim(int direction);				// 播放攻击动画
public:
	Sprite* m_sprite;
	AnimDirection m_curDirection;		// 当前方向
	Action* m_moveAnimArray[3];			// 存放移动动作
	Action* m_attaAnimArray[3];			// 存放攻击动作
};

#endif