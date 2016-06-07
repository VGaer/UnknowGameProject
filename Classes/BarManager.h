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
};

class BarManager : public Node {
public:
	static BarManager* getInstance();
	BarManager();
	//创建敌方hp
	Sprite* create(const string& hp, int tag);
	//创建玩家HP,MP
	Sprite* create(const string& hp, const string& mp);
	//获取敌方HP
	ProgressTimer* getBars(int tag);
	//获取玩家HP,MP
	PlayerBar* getPlayerBars();
	//改变bar的百分值,offset为正是减少，负为增加	
	//total改成调用此函数目标HP或者MP的最大值(如Player::getInstance()->getMaxHP())
	void setPercent(ProgressTimer* pSender, float total, float nowNum);

	/*void SchedulePlayerHp_Mp();*/

	//测试用
	//void recover(BarType sender, int tag);

	////每秒回复，设置间隔为0.2或以上
	//void recoverHP(float dt);
	//void recoverMP(float dt);
	//void recoverEnemy(float dt, int i);
private:
	PlayerBar* playerBar;
	map<int, ProgressTimer*> mbars;
};