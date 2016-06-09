#pragma once
#include "cocos2d.h"
#include "map"
#include "string"
#include "Player.h"
#include "ui\UIButton.h"
#include "QuestList.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d::ui;
using namespace std;
USING_NS_CC;

class QuestList;

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
	Sprite* create(const string& exp, float lexp);
	//创建玩家行动条
	Sprite* create(const string& inv, const string& quest);
	//获取敌方HP
	ProgressTimer* getBars(int tag);
	Label* getBarsLabel(int tag);
	//获取玩家HP,MP
	PlayerBar* getPlayerBars();

	//传入指定条，最大值，当前值
	void setPercent(ProgressTimer* pSender, float total, float nowNum);
	void setBarLabel(Label* pSender, float cur, float max);
	void releaseEnemyBar(int tag);

	//按钮调用
	void buttonCallback(Ref * pSender, Widget::TouchEventType type);
	//弹出任务窗口
	void popQuest();
	void ItemCallback(Node *pNode);
private:
	PlayerBar* playerBar;
	QuestList* questView;
	Sprite* actBar;
	map<int, EnemyBar*> mbars;
};