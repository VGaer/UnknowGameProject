#include "QuestDispatcher.h"

QuestDispatcher * QuestDispatcher::getInstance()
{
	static QuestDispatcher* instance = NULL;
	if (instance == NULL)
		instance = new QuestDispatcher();
	return instance;
}

QuestDispatcher::QuestDispatcher()
{
}

QuestDispatcher::~QuestDispatcher()
{
}

void QuestDispatcher::initQuestsDataWithID(int tag, const int id)
{
	mData[tag]->questData[id] = (GameData::getInstance()->getDataFromQuestsData(id));
}

void QuestDispatcher::initQuestDlgsDataWithID(int tag, const int id)
{
	mData[tag]->qDlgsdata[id] = GameData::getInstance()->getDataFromQuestDlgsData(id);
}


void QuestDispatcher::initQuest(NPC* pSender)
{
	auto npc_id = pSender->getData()->id;
	auto quest_id = pSender->getData()->quest_id;
	mData[npc_id] = new QuestData();
	for (int i = 0; i < quest_id.size(); i++) {
		initQuestsDataWithID(npc_id, quest_id[i]);
		initQuestDlgsDataWithID(npc_id, quest_id[i]);
	}
}

int QuestDispatcher::getQuestStatus(NPC* pSender, const int id)
{
	auto npc_id = pSender->getData()->id;
	return mData[npc_id]->questData[id]->status;
}

void QuestDispatcher::QuestStatusControl(NPC * pSender, QuestControl ctr, const int id)
{
	auto npc_id = pSender->getData()->id;
	switch (ctr)
	{
	case accpet:
		mData[npc_id]->questData[id]->status = QuestStatus::active;
		break;
	case complete:
		mData[npc_id]->questData[id]->status = QuestStatus::finish;
		break;
	case cancel:
		mData[npc_id]->questData[id]->status = QuestStatus::start;
		break;
	default:
		break;
	}
}

QuestData * QuestDispatcher::getQuest(NPC* pSender)
{
	auto npc_id = pSender->getData()->id;
	return mData[npc_id];
}
