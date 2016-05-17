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
	int getDirectionByTargetPos(Point tarPos);		// 获取移动方向
	void playStanAnim(int direction);				// 播放静止动画
	void playMoveAnim(int direction);				// 播放移动动画
	void playAttaAnim(int direction);				// 播放攻击动画
	int getCurDirection();							// 获取当前方向
public:
	Sprite* m_sprite;
	AnimDirection m_curDirection;		// 当前方向
	Action* m_stanAnimArray[3];         // 静止动作容器
	Action* m_moveAnimArray[3];			// 移动动作容器
	Action* m_attaAnimArray[3];			// 攻击动作容器
};

#endif