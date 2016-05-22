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
	Data[tag]->questData = GameData::getInstance()->getDataFromQuestsData(id);
}

void QuestDispatcher::initQuestDlgsDataWithID(int tag, const int id)
{
	Data[tag]->qDlgsdata = GameData::getInstance()->getDataFromQuestDlgsData(id);
}


void QuestDispatcher::initQuest(NPC* pSender)
{
	auto npc_id = pSender->getData()->id;
	auto quest_id = pSender->getData()->quest_id;
	for (int i = 0; i < quest_id.size(); i++) {
		Data[npc_id] = new QuestData();
		initQuestsDataWithID(npc_id, quest_id[i]);
		initQuestDlgsDataWithID(npc_id, quest_id[i]);
	}
}

void QuestDispatcher::getQuestStatus()
{

}

QuestData * QuestDispatcher::getQuest(NPC* pSender)
{
	auto npc_id = pSender->getData()->id;
	return Data[npc_id];
}
