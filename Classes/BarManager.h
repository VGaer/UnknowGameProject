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
	//�����з�hp
	Sprite* create(const string& hp, int tag);
	//�������HP,MP
	Sprite* create(const string& hp, const string& mp);
	//��ȡ�з�HP
	ProgressTimer* getBars(int tag);
	//��ȡ���HP,MP
	PlayerBar* getPlayerBars();
	//�ı�bar�İٷ�ֵ,offsetΪ���Ǽ��٣���Ϊ����	
	//total�ĳɵ��ô˺���Ŀ��HP����MP�����ֵ(��Player::getInstance()->getMaxHP())
	void setPercent(ProgressTimer* pSender, float total, float nowNum);

	/*void SchedulePlayerHp_Mp();*/

	//������
	//void recover(BarType sender, int tag);

	////ÿ��ظ������ü��Ϊ0.2������
	//void recoverHP(float dt);
	//void recoverMP(float dt);
	//void recoverEnemy(float dt, int i);
private:
	PlayerBar* playerBar;
	map<int, ProgressTimer*> mbars;
};