#include "QuestDispatcher.h"
#include "GameScene.h"
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
	map<int, QuestListData*> temp;
	if (!GameData::getInstance()->isExistSaveDoc())
		temp = GameData::getInstance()->m_mapQuestsData;
	else
		temp = GameData::getInstance()->getQuestSaveData();
	for (auto& i : temp) {
		if (i.second->status == QuestStatus::active)
			activeQuestList.push_back(i.second);
	}
	for (auto& a : this->getQuestListVec()) {
		if (a->type == QuestTypes::defeat && a->status == QuestStatus::active) {
			auto questDat = GameData::getInstance()->getDataFromQuestsData(a->id);
			schedule(schedule_selector(QuestDispatcher::questsUpdate));
		}
	}
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

int QuestDispatcher::getQuestType(NPC* pSender, const int id)
{
	auto npc_id = pSender->getData()->id;
	return mData[id]->type;
}

QuestListData* QuestDispatcher::getQuest(NPC* pSender, const int id)
{
	auto npc_id = pSender->getData()->id;
	return mData[id];
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
	if (mNpc.find(name) != mNpc.end())
		return mNpc.at(name);
	else
		return NULL;
}

void QuestDispatcher::addNpc(string& name, NPC * pSender)
{
	if (mNpc.find(name) == mNpc.end())
	{
		mNpc[name] = pSender;
	}
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

void QuestDispatcher::mNpcClear()
{
	NPC* temp = NULL;
	bool isExist = false;
	for (auto i = mNpc.begin(); i != mNpc.end();++i) {
		if (i->second->getIsRetain() == true)
		{
			temp = i->second;
			isExist = true;
			break;
		}
	}
	mNpc.clear();
	if(isExist)
		mNpc[temp->getData()->name] = temp;
}

void QuestDispatcher::questsUpdate(float dt)
{
	bool isFind = false;
	for (auto& i : activeQuestList)
	{
		if (i->type == QuestTypes::defeat)
		{
			isFind = true;
			if (i->mapID == GameScene::sceneId) {
				if (MonsterManager::getInstance()->getMonsterVec().size() == 0) {
					i->status = QuestStatus::commit;
				}
			}
		}
	}
	if (!isFind)
		unschedule(schedule_selector(QuestDispatcher::questsUpdate));
}

void QuestDispatcher::openUpdate()
{
	/*schedule(CC_CALLBACK_1(QuestDispatcher::questsUpdate, QuestDispatcher::getInstance(), pSender), name);*/
	schedule(schedule_selector(QuestDispatcher::questsUpdate));
}
