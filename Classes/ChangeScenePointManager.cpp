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
	//��ȡ�³���id��Ӧ��SceneData
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
	//��ȡ�³���id��Ӧ��SceneData
	if (SceneIdManager::getInstance()->map_sceneIdToSceneData.find(newSceneId) != SceneIdManager::getInstance()->map_sceneIdToSceneData.end())
	{
		auto scenedata = SceneIdManager::getInstance()->map_sceneIdToSceneData[newSceneId];
		std::vector<int>missonvec = scenedata.Misson;
		//�����ǰ����û��ǰ������Ҫ��
		if (missonvec.size() == 0)
		{
			return true;
		}
		else
		{
			bool OK = true;
			//��������
			for (int i = 0; i < missonvec.size(); i++)
			{
				//�ж������Ƿ�ȫ�����
				//if (�������) continue;
				//else {OK = falss;break;}
			}
			return OK;
		}
	}
}
