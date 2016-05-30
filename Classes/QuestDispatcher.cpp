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
	initQuestDlgsData();
}

QuestDispatcher::~QuestDispatcher()
{
}

void QuestDispatcher::initQuestsDataWithID(const int id)
{
	mData[id] = (GameData::getInstance()->getDataFromQuestsData(id));
}

void QuestDispatcher::initQuestDlgsData()
{
	qDlgsdata = GameData::getInstance()->getDataFromQuestDlgsData();
}


void QuestDispatcher::initQuest(NPC* pSender)
{
	auto quest_id = pSender->getData()->quest_id;
	for (int i = 0; i < quest_id.size(); i++) {
		initQuestsDataWithID(quest_id[i]);
	}
}

int QuestDispatcher::getQuestStatus(NPC* pSender, const int id)
{
	auto npc_id = pSender->getData()->id;
	return mData[id]->status;
}

void QuestDispatcher::QuestStatusControl(NPC * pSender, QuestControl ctr, const int id)
{
	switch (ctr)
	{
	case accpet:
		mData[id]->status = QuestStatus::active;
		activeQuestList.push_back(mData[id]);
		break;
	case complete:
		mData[id]->status = QuestStatus::finish;
		for (it = activeQuestList.begin(); it != activeQuestList.end(); it++) {
			if (*it == mData[id]) {
				activeQuestList.erase(it);
				break;
			}
		}
		break;
	case cancel:
		mData[id]->status = QuestStatus::start;
		for (it = activeQuestList.begin(); it != activeQuestList.end(); it++) {
			if (*it == mData[id]) {
				activeQuestList.erase(it);
			}
		}
		break;
	default:
		break;
	}
}

NPC * QuestDispatcher::getNpc(string& name)
{
	return mNpc[name];
}

void QuestDispatcher::addNpc(string& name, NPC * pSender)
{
	mNpc[name] = pSender;
}

vector<QuestListData*> QuestDispatcher::getQuest(NPC* pSender)
{
	auto id = pSender->getData()->quest_id;
	vector<QuestListData*> temp;
	for (auto i : id) {
		temp.push_back(mData[i]);
	}
	return temp;
}

vector<QuestListData*>& QuestDispatcher::getQuestListVec()
{
	return activeQuestList;
}

map<int, QuestDlgsData*> QuestDispatcher::getQuestDlgs()
{
	return qDlgsdata;
}
