#include "QuestDispatcher.h"
#include "GameScene.h"
#include "Talk.h"
#include "TalkManager.h"

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
	
	//���ù����޴浵,������Ϣ������m_mapQuestsData
	initQuestFromGameData();
	
	for (auto& i : qData) {
		if (i.second->status == QuestStatus::active)
			activeQuestList.push_back(i.second);
	}
	for (auto& a : this->getQuestListVec()) {
		if (a->type == QuestTypes::defeat && a->status == QuestStatus::active) {
			schedule(schedule_selector(QuestDispatcher::questsUpdate));
			break;
		}
	}
}

QuestDispatcher::~QuestDispatcher()
{
}


void QuestDispatcher::initQuestDlgsData()
{
	qDlgsdata = GameData::getInstance()->getDataFromQuestDlgsData();
}

int QuestDispatcher::getQuestStatus(const int id)
{
	if (qData.find(id) != qData.end())
		return qData[id]->status;
	else return -1;
}

int QuestDispatcher::getQuestType(const int id)
{
	if (qData.find(id) != qData.end())
		return qData[id]->type;
	else return -1;
}

QuestListData* QuestDispatcher::getQuest(const int id)
{
		return qData[id];
}


void QuestDispatcher::QuestStatusControl(NPC * pSender, QuestControl ctr, const int id)
{
	switch (ctr)
	{
	case accpet:
		qData[id]->status = QuestStatus::active;
		activeQuestList.push_back(qData[id]);
		break;
	case complete:
		qData[id]->status = QuestStatus::finish;
		for (it = activeQuestList.begin(); it != activeQuestList.end(); it++) {
			if (*it == qData[id]) {
				activeQuestList.erase(it);
				break;
			}
		}
		break;
	case cancel:
		qData[id]->status = QuestStatus::start;
		for (it = activeQuestList.begin(); it != activeQuestList.end(); it++) {
			if (*it == qData[id]) {
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
		temp.push_back(qData[i]);
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


void QuestDispatcher::questsUpdate(float dt)
{
	bool isFind = false;
	QuestListData* temp = NULL;
	for (auto& i : activeQuestList)
	{
		if (i->type == QuestTypes::defeat)
		{
			isFind = true;
			if (i->mapID == GameScene::sceneId) {
				if (MonsterManager::getInstance()->getMonsterVec().size() == 0) {
					i->status = QuestStatus::commit;
					if (i->id == 4)
					{
						// ������ڶԻ�
						auto taskDlgs = GameData::getInstance()->getDataFromPlayerTaskDlgsData(i->id);
						if (taskDlgs && !taskDlgs->isSaying && !taskDlgs->isSaidCmTsDlgs)
						{
							if (taskDlgs->commitTaskDlgs.empty() || taskDlgs->commitTaskDlgs.at(0) == "NULL")
								return;
							taskDlgs->isSaying = true;
							auto talk = Talk::create(taskDlgs->commitTaskDlgs, taskDlgs->taskId, Talk_CmTask);
							Player::getInstance()->getTiledMap()->addChild(talk, 666);
						}
						i->status = QuestStatus::finish;
						temp = i;
					}				
				}
			}
		}
	}
	if (temp)
	{
		for (auto j = activeQuestList.begin(); j != activeQuestList.end(); ++j)
		{
			if (*j == temp)
			{
				activeQuestList.erase(j);
				break;
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

map<int, QuestListData*>& QuestDispatcher::getqData()
{
	return qData;
}

void QuestDispatcher::initQuestFromGameData()
{
	for (auto i : GameData::getInstance()->m_mapQuestsData)
	{		
		//new һ���µ��ڴ棬������������µ�����״̬������new������
		QuestListData* data = new QuestListData();
		data->id = (i.second)->id;
		data->level = (i.second)->level;
		data->mapID = (i.second)->mapID;
		data->title = (i.second)->title;
		data->instruct = (i.second)->instruct;
		data->type = (i.second)->type;
		data->status = (i.second)->status;
		for (auto monid : (i.second)->mons_id)
		{
			(data->mons_id).push_back(monid);
		}
		for (auto forgeid : (i.second)->forgeID)
		{
			data->forgeID.push_back(forgeid);
		}
		data->targetNpc = (i.second)->targetNpc;
		data->launchNpc = (i.second)->launchNpc;

		qData[i.first] = data;
	}
}

void QuestDispatcher::InitQuestDispathcerForPlayerDied()
{
	//�������ˣ���������״̬�����˵�GameData�����һ�δ浵
	//���û������Ĵ浵��������浵�������ļ�����Ҳ�Ƕ���m_mapQuestsData�еģ���m_mapQuestsData���ǶԵ�
	for (auto i : GameData::getInstance()->m_mapQuestsData)
	{
		QuestListData* data = qData[i.first];
		data->id = (i.second)->id;
		data->level = (i.second)->level;
		data->mapID = (i.second)->mapID;
		data->title = (i.second)->title;
		data->instruct = (i.second)->instruct;
		data->type = (i.second)->type;
		data->status = (i.second)->status;
		for (auto monid : (i.second)->mons_id)
		{
			(data->mons_id).push_back(monid);
		}
		for (auto forgeid : (i.second)->forgeID)
		{
			data->forgeID.push_back(forgeid);
		}
		data->targetNpc = (i.second)->targetNpc;
		data->launchNpc = (i.second)->launchNpc;
	}
		

	//�����activeQuestList
	activeQuestList.clear();
	for (auto& i : qData) {
		if (i.second->status == QuestStatus::active)
			activeQuestList.push_back(i.second);
	}	
}


bool QuestDispatcher::IsExistDefeatQuest()
{
	bool isFind = false;
	QuestListData* temp = NULL;
	for (auto& i : activeQuestList)
	{
		if (i->type == QuestTypes::defeat)
		{
			isFind = true;
		}
	}
	return isFind;
}

void QuestDispatcher::twoIdSceneTalk(TMXTiledMap* map)
{
	for (auto& a : this->getQuestListVec()) {
		if (a->id != 4) continue; //��ͷ�����ͼ�����ǶԻ���
		if (a->type == QuestTypes::defeat && a->status == QuestStatus::active) {
			////��ȡ�������Ի�
			auto playerDlgs = TalkManager::getInstance()->getDataFromPlayerTaskDlgsData(a->id);
			if (playerDlgs && !playerDlgs->isSaidAcTsDlgs)
			{
				if (playerDlgs->acceptTaskDlgs.empty() || playerDlgs->acceptTaskDlgs.at(0) == "NULL")
					;
				else
				{
					auto talk = Talk::create(playerDlgs->acceptTaskDlgs, playerDlgs->taskId, Talk_AcTask);
					if (map)
					{
						map->addChild(talk, 100);
						break;
					}
				}
			}
		}
	}
}
