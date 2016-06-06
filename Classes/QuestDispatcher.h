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
	//任务所属NPC, 任务操作， 任务id
	void QuestStatusControl(NPC* pSender, QuestControl ctr, const int id);
	NPC* getNpc(string& name);
	void addNpc(string& name, NPC* pSender);
	NpcsData* recoverNpc(string & name);
	vector<QuestListData*> getQuest(NPC* pSender);
	// 获取已接任务容器
	vector<QuestListData*>& getQuestListVec();
	map<int, QuestDlgsData*> getQuestDlgs();
private:
	map<string, NPC*> mNpc;
	//任务列表容器
	map<int, QuestListData*> mData;
	//任务对话容器
	map<int, QuestDlgsData*> qDlgsdata;

	vector<QuestListData*> activeQuestList;
	vector<QuestListData*>::iterator it;
};
