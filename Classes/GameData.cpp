#include "GameData.h"
#include "cocos2d.h"
#include "json\document.h"
USING_NS_CC;

GameData* GameData::getInstance()
{
	static GameData* instance = NULL;
	if (instance == NULL)
		instance = new GameData();
	return instance;
}

GameData::GameData()
{
	readMonsDataFile();
}

GameData::~GameData()
{

}

void GameData::addDataToMonsData(MonsData* data)
{
	string name = data->name;
	m_mapMonsData[name] = data;
}

const MonsData* GameData::getDataFromMonsData(const string& name)
{
	MonsData* monsData = NULL;
	if (m_mapMonsData.find(name) != m_mapMonsData.end())
	{
		monsData = m_mapMonsData[name];
	}
	return monsData;
}

void GameData::readMonsDataFile()
{
	std::string jsonPath = MONS_DATA_PATH;
	rapidjson::Document _doc;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	do {
		pBytes = FileUtils::getInstance()->getFileData(jsonPath, "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());
		// 判断是否为一个数组
		if (!_doc.IsArray())
			return;
		const rapidjson::Value& pArray = _doc;
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
		{
			// 读取对象属性
			MonsData* data = new MonsData();
			const rapidjson::Value &value = pArray[i];  // value为一个对象
			data->name = value["name"].GetString();
			data->hp = value["hp"].GetDouble();		
			data->damage = value["damage"].GetDouble();
			data->moveSpeed = value["moveSpeed"].GetDouble();
			data->attackInter = value["attackInter"].GetDouble();
			data->attackRange = value["attackRange"].GetDouble();
			addDataToMonsData(data);
		}
	} while (0);
}
