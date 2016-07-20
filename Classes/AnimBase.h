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
	Dir_upleft = 4,//谁在前谁就是主方向，否则为副方向
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
	int getDirectionByTargetPos(Point tarPos);		// 获取移动方向
	void playStanAnim();				// 播放静止动画
	void playMoveAnim();				// 播放移动动画
	void playAttaAnim();				// 播放攻击动画
	int getCurDirection();							// 获取当前方向
	void setCurDirection(Point tarPos);							// 设置当前方向
	void setAction();
public:
	Sprite* m_sprite;
	Monster* m_monster;
	AnimDirection m_curDirection;		// 当前方向
	Action* m_stanAnimArray[3];         // 静止动作容器
	Action* m_moveAnimArray[3];			// 移动动作容器
	Action* m_attaAnimArray[3];			// 攻击动作容器
	Action* m_attaBigAnimArray[3];      // 攻击大技能
	Action* m_attaRemoteAnimArray[3];   // 远程技能
	Action* m_attaRemoteDurAnimArray[3]; //远程技能（持续）
};

#endif