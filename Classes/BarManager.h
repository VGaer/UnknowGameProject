#pragma once
#include "cocos2d.h"
#include "map"
#include "string"
#include "Player.h"
using namespace std;
USING_NS_CC;

enum BarType {
	HP = 0,
	MP,
	Enemy
};


struct PlayerBar {
	ProgressTimer* m_hp;
	ProgressTimer* m_mp;
	ProgressTimer* m_exp;
	Label* l_hp;
	Label* l_mp;
	Label* l_exp;
};

struct EnemyBar {
	ProgressTimer* m_hp;
	Label* l_hp;
};

class BarManager : public Node {
public:
	static BarManager* getInstance();
	BarManager();
	//�����з�hp
	Sprite* create(const string& hp, int tag, float maxhp);
	//�������HP,MP
	Sprite* create(const string& hp, const string& mp, float maxhp, float maxmp);
	//�������exp
	Sprite* create(const string & exp, float lexp);
	//��ȡ�з�HP
	ProgressTimer* getBars(int tag);
	Label* getBarsLabel(int tag);
	//��ȡ���HP,MP
	PlayerBar* getPlayerBars();

	//����ָ���������ֵ����ǰֵ
	void setPercent(ProgressTimer* pSender, float total, float nowNum);
	void setBarLabel(Label* pSender, float cur, float max);
	void releaseEnemyBar(int tag);
private:
	PlayerBar* playerBar;
	map<int, EnemyBar*> mbars;
};