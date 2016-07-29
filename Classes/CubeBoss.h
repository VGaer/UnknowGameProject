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
	TimeCounter* m_timecounter; //���㵱ǰ��Ծ�ĸ߶�
	int caljumpNums; //������Ծ����
	void ControlPlayerMove();//���������Ƿ����ƶ�
	bool GetCanBeAttacked();
	void SetCanBeAttacked();//ʵ���麯��
	Rect getBoundingBox();//��дboundingbox
	void cmd_hurt(float damage);//ʵ���麯��

	int monsterIdForBar;
	float monMaxHp;
private:
	bool isJumping; //�Ƿ�������Ծ����Ծʱ�������޵�
	CubeBossFindPath* m_cubebossfindpath;
	Player* m_player;
	TMXTiledMap* m_parrent;
	bool isLasering;//�Ƿ����ڷ��伤��
	bool OneOnLand;//�ҵ�����ʱ��ֻ�ж�һ�μ���
	bool CanBeAttacked; //true���Ա����ǹ�����false�����Ա����ǹ���
	float m_hp;
	Color3B m_mycolor;
};

#endif