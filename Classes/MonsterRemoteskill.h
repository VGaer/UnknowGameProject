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
	static MonsterRemoteskillDur* createWithName_width_height_damage_duration_speed_diretion_attackNums_attackNumsInter_distance(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter,float distance);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration, float speed, int direction, int attackNums, float attckNumsInter, float distance);
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

//����Զ�̳������ܣ����ƶ��ģ����糤����һ�����⣩
class MonsterRemoteskillDur_NoMove : public Entity
{
public:
	static MonsterRemoteskillDur_NoMove* createWithName_width_height_damage_duration_diretion_attackNums_attackNumsInter(std::string projectile, std::string projectileAnimate, float width, float height, float damage, float duration, std::string direction, int attackNums, float attckNumsInter);
	virtual bool init(std::string skill, std::string projectileAnimate, float width, float height, float damage, float duration,std::string direction, int attackNums, float attckNumsInter);
	bool MyRectColli(Vec2 rect1_v0, Vec2 rect1_v1, Vec2 rect1_v2, Vec2 rect1_v3,
		Vec2 rect2_v0, Vec2 rect2_v1, Vec2 rect2_v2, Vec2 rect2_v3);//�ĵ����һ�����Σ��涨v0v1 ƽ�� v2v3 ��v1v2ƽ��v3v4
private:
	float m_damage;//�����˺�
	float m_duration; //���ܳ���ʱ��
	std::string m_direction; //���ܷ���
	int m_attckNums; //���ܹ�������
	float m_attckNumsInter; //��ι����ӳ�
	float m_dt; // ��ʱ�����Ƿ����
	TimeCounter* m_attackCounter;//��ʱ��ι���
	void update(float dt);
	Player* m_player;
	string name;
};

#endif