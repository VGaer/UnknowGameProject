#include "ChangeScenePointManager.h"
#include "GameScene.h"
#include "SceneIdManager.h"
#include "Player.h"

ChangeScenePointManager* ChangeScenePointManager::getInstance()
{
	static ChangeScenePointManager* instance = nullptr;
	if (instance == nullptr)
		instance = new ChangeScenePointManager();
	return instance;
}

bool ChangeScenePointManager::IsReachCondition(int newSceneId)
{
	return (IsReachLevel(newSceneId) && IsReachMission(newSceneId));
}
	
bool ChangeScenePointManager::IsReachLevel(int newSceneId)
{
	//获取新场景id对应的SceneData
	if (SceneIdManager::getInstance()->map_sceneIdToSceneData.find(newSceneId) != SceneIdManager::getInstance()->map_sceneIdToSceneData.end())
	{
		auto scenedata = SceneIdManager::getInstance()->map_sceneIdToSceneData[newSceneId];
		if (scenedata.Level == 0)
			return true;
		else
		{
			if (Player::getInstance()->m_playerlevel >= scenedata.Level)
				return true;
			else
				return false;
		}
	}
}
	
bool ChangeScenePointManager::IsReachMission(int newSceneId)
{
	//获取新场景id对应的SceneData
	if (SceneIdManager::getInstance()->map_sceneIdToSceneData.find(newSceneId) != SceneIdManager::getInstance()->map_sceneIdToSceneData.end())
	{
		auto scenedata = SceneIdManager::getInstance()->map_sceneIdToSceneData[newSceneId];
		std::vector<int>missonvec = scenedata.Misson;
		//如果当前场景没有前置任务要求
		if (missonvec.size() == 0)
		{
			return true;
		}
		else
		{
			bool OK = true;
			//遍历任务，
			for (auto &i : missonvec)
			{
				auto questData = GameData::getInstance()->getDataFromQuestsData(i);
				//判断任务是否全部完成
				//if (任务完成) continue;
				if (questData->status == QuestStatus::finish)	continue;
				else {
					OK = false;
					break;
				}
				//else {OK = falss;break;}
			}
			return OK;
		}
	}
}
