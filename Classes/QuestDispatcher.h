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
	defeat
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
	void initQuestsDataWithID(const int id);
	void initQuestDlgsData();
	void initQuest(NPC* pSender);
	int getQuestStatus(NPC* pSender, const int id);
	int getQuestType(NPC* pSender, const int id);
	QuestListData* getQuest(NPC* pSender, const int id);
	void questsUpdate(float dt, QuestListData* pSender);
	void openUpdate(QuestListData* pSender, string name);
	//��������NPC, ��������� ����id
	void QuestStatusControl(NPC* pSender, QuestControl ctr, const int id);
	NPC* getNpc(string& name);
	void addNpc(string& name, NPC* pSender);
	NpcsData* recoverNpc(string & name);
	vector<QuestListData*> getQuest(NPC* pSender);
	// ��ȡ�ѽ���������
	vector<QuestListData*>& getQuestListVec();
	map<int, QuestDlgsData*> getQuestDlgs();
private:
	map<string, NPC*> mNpc;
	//�����б�����
	map<int, QuestListData*> mData;
	//����Ի�����
	map<int, QuestDlgsData*> qDlgsdata;

	vector<QuestListData*> activeQuestList;
	vector<QuestListData*>::iterator it;
};
