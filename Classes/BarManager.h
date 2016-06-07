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
	//创建敌方hp
	Sprite* create(const string& hp, int tag, float maxhp);
	//创建玩家HP,MP
	Sprite* create(const string& hp, const string& mp, float maxhp, float maxmp);
	//创建玩家exp
	Sprite* create(const string & exp, float lexp);
	//获取敌方HP
	ProgressTimer* getBars(int tag);
	Label* getBarsLabel(int tag);
	//获取玩家HP,MP
	PlayerBar* getPlayerBars();

	//传入指定条，最大值，当前值
	void setPercent(ProgressTimer* pSender, float total, float nowNum);
	void setBarLabel(Label* pSender, float cur, float max);
	void releaseEnemyBar(int tag);
private:
	PlayerBar* playerBar;
	map<int, EnemyBar*> mbars;
};