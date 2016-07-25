#include "TalkManager.h"

TalkManager* TalkManager::getInstance()
{
	static TalkManager* instance = NULL;
	if (instance == NULL)
		instance = new TalkManager();
	return instance;
}

TalkManager::TalkManager()
{
	initCurEnterSceneDlgs();
	initCurPlayerTaskDlgs();
}

void TalkManager::initCurEnterSceneDlgs()
{
	for (auto m : GameData::getInstance()->m_mapPlayerTaskDlgs)
	{
		PlayerTaskDlgsData* playertaskdlgsdata = new PlayerTaskDlgsData();
		playertaskdlgsdata->acceptTaskDlgs = m.second->acceptTaskDlgs;
		playertaskdlgsdata->commitTaskDlgs = m.second->commitTaskDlgs;
		playertaskdlgsdata->finishTaskDlgs = m.second->finishTaskDlgs;
		playertaskdlgsdata->isSaidAcTsDlgs = m.second->isSaidAcTsDlgs;
		playertaskdlgsdata->isSaidCmTsDlgs = m.second->isSaidCmTsDlgs;
		playertaskdlgsdata->isSaidFiTsDlgs = m.second->isSaidFiTsDlgs;
		playertaskdlgsdata->isSaying = m.second->isSaying;
		playertaskdlgsdata->taskId = m.second->taskId;

		m_mapCurPlayerTaskDlgs[m.first] = playertaskdlgsdata;
	}
}

void TalkManager::initCurPlayerTaskDlgs()
{
	for (auto m : GameData::getInstance()->m_mapEnterSceneDlgs)
	{
		EnterSceneDlgsData* enterscenedlgsdata = new EnterSceneDlgsData();
		enterscenedlgsdata->cannotEnterDlgs = m.second->cannotEnterDlgs;
		enterscenedlgsdata->enterSceneDlgs = m.second->enterSceneDlgs;
		enterscenedlgsdata->isSaid = m.second->isSaid;
		enterscenedlgsdata->sceneId = m.second->sceneId;

		m_mapCurEnterSceneDlgs[m.first ]= enterscenedlgsdata;
	}
}

PlayerTaskDlgsData* TalkManager::getDataFromPlayerTaskDlgsData(int id)
{
	if (m_mapCurPlayerTaskDlgs.find(id) != m_mapCurPlayerTaskDlgs.end())
	{
		return m_mapCurPlayerTaskDlgs[id];
	}
	else
	{
		return NULL;
	}
}

EnterSceneDlgsData* TalkManager::getDataFromEnterSceneDlgsData(int id)
{
	if (m_mapCurEnterSceneDlgs.find(id) != m_mapCurEnterSceneDlgs.end())
	{
		return m_mapCurEnterSceneDlgs[id];
	}
	else //如果没有加上else,在Release版本会崩溃，因为如果不存在，返回的指针Debug版本会自动为NULL，但Release版本是随机的， 那么就可能造成非法操作内存
	{
		return NULL;
	}
}

void TalkManager::InitTalkForPlayerDied()
{
	for (auto m : GameData::getInstance()->m_mapPlayerTaskDlgs)
	{
		PlayerTaskDlgsData* playertaskdlgsdata = m_mapCurPlayerTaskDlgs[m.first];
		playertaskdlgsdata->acceptTaskDlgs = m.second->acceptTaskDlgs;
		playertaskdlgsdata->commitTaskDlgs = m.second->commitTaskDlgs;
		playertaskdlgsdata->finishTaskDlgs = m.second->finishTaskDlgs;
		playertaskdlgsdata->isSaidAcTsDlgs = m.second->isSaidAcTsDlgs;
		playertaskdlgsdata->isSaidCmTsDlgs = m.second->isSaidCmTsDlgs;
		playertaskdlgsdata->isSaidFiTsDlgs = m.second->isSaidFiTsDlgs;
		playertaskdlgsdata->isSaying = m.second->isSaying;
		playertaskdlgsdata->taskId = m.second->taskId;
	}

	for (auto m : GameData::getInstance()->m_mapEnterSceneDlgs)
	{
		EnterSceneDlgsData* enterscenedlgsdata = m_mapCurEnterSceneDlgs[m.first];
		enterscenedlgsdata->cannotEnterDlgs = m.second->cannotEnterDlgs;
		enterscenedlgsdata->enterSceneDlgs = m.second->enterSceneDlgs;
		enterscenedlgsdata->isSaid = m.second->isSaid;
		enterscenedlgsdata->sceneId = m.second->sceneId;
	}
}





