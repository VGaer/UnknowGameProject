#include "SceneIdManager.h"
#include "json\document.h"
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
	map_scenenameToSceneData[scenedata.name] = scenedata;
	map_sceneIdToSceneData[scenedata.Id] = scenedata;
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
		//判断是否为一个数组
		if (!_doc.IsArray())
			return;
		const rapidjson::Value& pArray = _doc;
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
		{
			//读取对象属性
			SceneData scenedata;
			const rapidjson::Value& value = pArray[i];//value为一个对象
			scenedata.name = value["scenename"].GetString();
			scenedata.Id = value["Id"].GetInt();
			scenedata.Level = value["Level"].GetInt();
			const rapidjson::Value& MissonArr = value["Mission"];
			for (int i = 0; i < MissonArr.Size(); i++)
			{
				scenedata.Misson.push_back(MissonArr[i].GetInt());
			}
			const rapidjson::Value& MissonActiveArr = value["MissonActive"];
			for (int i = 0; i < MissonActiveArr.Size(); i++)
			{
				scenedata.MissonActive.push_back(MissonActiveArr[i].GetInt());
			}
			
			addDataToSceneData(scenedata);
		}
	} while (0);
}