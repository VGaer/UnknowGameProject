#ifndef __CubeBoss_H__
#define __CubeBoss_H__

#include "cocos2d.h"
#include "Entity.h"
#include "Player.h"
#include "TimeCounter.h"
#include "BossABC.h"

USING_NS_CC;


class CubeBoss;

class CubeBossFindPath
{
public:
	void run(Vec2 startId, Vec2);
	void bindBoss(CubeBoss* );
private:
	CubeBoss* m_cubeboss;
};

class CubeBossFindPath;

class CubeBoss : public BossABC
{
public:
	static CubeBoss* create(const std::string name);
	CubeBoss();
	bool init(const std::string name);
	void update(float dt);
	void JumpUpdate(float dt);
	void setParent(TMXTiledMap* parent);
	TMXTiledMap* getParent();
	void setJumping(bool);
	bool getJumping();
	bool IsColliWithPlayer();
	float jumptime;
	float jumpheight;
	TimeCounter* m_timecounter; //计算当前跳跃的高度
	int caljumpNums; //计算跳跃次数
	void ControlPlayerMove();//控制主角是否能移动
	bool GetCanBeAttacked();
	void SetCanBeAttacked();//实现虚函数
	Rect getBoundingBox();//重写boundingbox
	void cmd_hurt(float damage);//实现虚函数

	int monsterIdForBar;
	float monMaxHp;
private:
	bool isJumping; //是否正在跳跃，跳跃时候设置无敌
	CubeBossFindPath* m_cubebossfindpath;
	Player* m_player;
	TMXTiledMap* m_parrent;
	bool isLasering;//是否正在发射激光
	bool OneOnLand;//砸到地面时，只判断一次即可
	bool CanBeAttacked; //true可以被主角攻击，false不可以被主角攻击
	float m_hp;
	Color3B m_mycolor;
};

#endif