#include "SceneIdManager.h"
#include "json\document.h"
#include "Player.h"
#include "cocos2d.h"
USING_NS_CC;

SceneIdManager* SceneIdManager::getInstance()
{
	static SceneIdManager* sceneId = NULL;
	if (sceneId == NULL)
		sceneId = new SceneIdManager();
	return sceneId;
}

SceneIdManager::SceneIdManager()
{
	readSceneDataFile();
}

SceneIdManager::~SceneIdManager()
{
	
}

void SceneIdManager::addDataToSceneData(SceneData scenedata)
{
	map_scenenameToId[scenedata.name] = scenedata.Id;
	map_sceneIdToname[scenedata.Id] = scenedata.name;
}

void SceneIdManager::readSceneDataFile()
{
	std::string jsonPath = Scene_DATA_PATH;
	rapidjson::Document _doc;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	do{
		pBytes = FileUtils::getInstance()->getFileData(jsonPath, "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());
		//�ж��Ƿ�Ϊһ������
		if (!_doc.IsArray())
			return;
		const rapidjson::Value& pArray = _doc;
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
		{
			//��ȡ��������
			SceneData scenedata;
			const rapidjson::Value& value = pArray[i];//valueΪһ������
			scenedata.name = value["scenename"].GetString();
			scenedata.Id = value["Id"].GetInt();
			addDataToSceneData(scenedata);
		}
	} while (0);
}
