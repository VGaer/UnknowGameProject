#ifndef __MonsterRemoteskill_H__
#define __MonsterRemoteskill_H__

#include "Entity.h"
#include "Player.h"
#include "TimeCounter.h"

//����Զ�̷��м���
class MonsterRemoteskill : public Entity
{
public:
	static MonsterRemoteskill* createWithName_width_height_damage_duration_speed_diretion(std::string projectile,std::string projectileAnimate ,float width, float height, float damage, float duration, float speed, int direction);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction);
private:
	float m_damage;//�����˺�
	float m_duration; //���ܳ���ʱ��
	float m_movespeed; //�����ٶ�
	int m_direction; //Ͷ�����ܷ���
	float m_dt; // ��ʱ
	void update(float dt);
	Player* m_player;
	string name;
};

//����Զ�̳�������(�ƶ�һ�������ͣ������)
class MonsterRemoteskillDur : public Entity
{
public:
	static MonsterRemoteskillDur* createWithName_width_height_damage_duration_speed_diretion_attackNums_attackNumsInter(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter);
private:
	float m_damage;//�����˺�
	float m_duration; //���ܳ���ʱ��
	float m_movespeed; //�����ٶ�
	int m_direction; //���ܷ���
	int m_attckNums; //���ܹ�������
	float m_attckNumsInter; //��ι����ӳ�
	float m_dt; // ��ʱ�����Ƿ����
	TimeCounter* m_attackCounter;//��ʱ��ι���
	void update(float dt);
	Player* m_player;
	string name;
};

#endif