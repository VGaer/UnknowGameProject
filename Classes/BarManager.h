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
	//�����з�hp
	Sprite* create(const string& hp, int tag, float maxhp);
	//�������HP,MP
	Sprite* create(const string& hp, const string& mp, float maxhp, float maxmp);
	//�������exp
	Sprite* create(const string& exp, float lexp);
	//��������ж���
	Sprite* create(const string& inv, const string& quest);
	//��ȡ�з�HP
	ProgressTimer* getBars(int tag);
	Label* getBarsLabel(int tag);
	//��ȡ���HP,MP
	PlayerBar* getPlayerBars();

	//����ָ���������ֵ����ǰֵ
	void setPercent(ProgressTimer* pSender, float total, float nowNum);
	void setBarLabel(Label* pSender, float cur, float max);
	void releaseEnemyBar(int tag);

	//��ť����
	void buttonCallback(Ref * pSender, Widget::TouchEventType type);
	//�������񴰿�
	void popQuest();
	void ItemCallback(Node *pNode);
private:
	PlayerBar* playerBar;
	QuestList* questView; //ע��questView��ͼ��û������create�ģ�����Ұָ�룬��ͼ֮��ֻ������create actBar,questView��create�ǿ���������ġ�
	//��ô��ͼ��questViewָ��ָ����δ�ͷŵ��ڴ�,�г���ʱ��questViewΪNULL����,����questView��addchild��actBar,�г���ʱ����create��actBar
	//������actBar����ֱ����questViewΪNULL����
	Sprite* actBar;
	map<int, EnemyBar*> mbars;
};