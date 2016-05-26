#pragma once
#include "GameData.h"
#include "cocos2d.h"
#include "NPC.h"
USING_NS_CC;

class NPC;

enum QuestStatus
{
	start = 0,
	active,
	finish
};

enum QuestControl {
	accpet = 0,
	complete,
	cancel
};

struct QuestData {
	map<int, QuestListData*> questData;
	map<int, QuestDlgsData*> qDlgsdata;
};

class QuestDispatcher
{
public:
	static QuestDispatcher* getInstance();
	QuestDispatcher();
	~QuestDispatcher();
	void initQuestsDataWithID(int tag, const int id);
	void initQuestDlgsDataWithID(int tag, const int id);
	void initQuest(NPC* pSender);
	int getQuestStatus(NPC* pSender, const int id);
	//任务所属NPC, 任务操作， 任务id
	void QuestStatusControl(NPC* pSender, QuestControl ctr, const int id);
	QuestData* getQuest(NPC* pSender);
private:
	map<int, QuestData*> mData;
};
