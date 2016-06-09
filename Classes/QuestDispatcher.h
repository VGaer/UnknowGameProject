#pragma once
#include "GameData.h"
#include "cocos2d.h"
#include "NPC.h"
#include "map"
USING_NS_CC;

class NPC;

enum QuestTypes
{
	search = 0,
	defeat,
	level
};

enum QuestStatus
{
	start = 0,
	active,
	finish,
	commit
};

enum QuestControl {
	accpet = 0,
	complete,
	cancel
};

//struct QuestData {
//	map<int, QuestListData*> questData;
//};

class QuestDispatcher : public Node
{
public:
	static QuestDispatcher* getInstance();
	QuestDispatcher();
	~QuestDispatcher();
	void initQuestDlgsData();
	void initQuestFromGameData();
	int getQuestStatus(const int id);
	int getQuestType(const int id);
	QuestListData* getQuest(const int id);
	void questsUpdate(float dt);
	void openUpdate();
	//��������NPC, ��������� ����id
	void QuestStatusControl(NPC* pSender, QuestControl ctr, const int id);
	NPC* getNpc(string& name);
	void addNpc(string& name, NPC* pSender);
	vector<QuestListData*> getQuest(NPC* pSender);
	// ��ȡ�ѽ���������
	vector<QuestListData*>& getQuestListVec();
	map<int, QuestDlgsData*> getQuestDlgs();
	map<int, QuestListData*>& getqData();
	//void mNpcClear();
private:
	map<string, NPC*> mNpc;
	//�����б�����
	map<int, QuestListData*> qData;
	//����Ի�����
	map<int, QuestDlgsData*> qDlgsdata;

	vector<QuestListData*> activeQuestList;
	vector<QuestListData*>::iterator it;
};
