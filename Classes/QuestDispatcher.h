#pragma once
#include "GameData.h"
#include "cocos2d.h"
#include "NPC.h"
USING_NS_CC;

enum QuestStatus
{
	start = 0,
	active,
	finish
};

struct QuestData {
	const QuestListData* questData;
	const QuestDlgsData* qDlgsdata;
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
	void getQuestStatus();
	QuestData* getQuest(NPC* pSender);
private:
	map<int, QuestData*> Data;
};
