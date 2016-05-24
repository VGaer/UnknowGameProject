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
	readNpcsDataFile();
	readQuestsDataFile();
	readQuestDlgsDataFile();
}

GameData::~GameData()
{

}

void GameData::addDataToMonsData(MonsData* data)
{
	string name = data->name;
	m_mapMonsData[name] = data;
}

void GameData::addDataToNpcsData(NpcsData* data)
{
	string name = data->name;
	m_mapNpcsData[name] = data;
}

void GameData::addDataToQuestsData(QuestListData * data)
{
	int id = data->id;
	m_mapQuestsData[id] = data;
}

void GameData::addDataToQuestDlgsData(QuestDlgsData * data)
{
	int id = data->id;
	m_mapQuestDlgsData[id] = data;
}

MonsData* GameData::getDataFromMonsData(const string& name)
{
	MonsData* monsData = NULL;
	if (m_mapMonsData.find(name) != m_mapMonsData.end())
	{
		monsData = m_mapMonsData[name];
	}
	return monsData;
}

NpcsData* GameData::getDataFromNpcsData(const string& name)
{
	NpcsData* npcsData = NULL;
	if (m_mapNpcsData.find(name) != m_mapNpcsData.end())
	{
		npcsData = m_mapNpcsData[name];
	}
	return npcsData;
}

QuestListData * GameData::getDataFromQuestsData(const int id)
{
	QuestListData* questlistData = NULL;
	if (m_mapQuestsData.find(id) != m_mapQuestsData.end()) {
		questlistData = m_mapQuestsData[id];
	}
	return questlistData;
}

QuestDlgsData * GameData::getDataFromQuestDlgsData(const int id)
{
	QuestDlgsData* questdlgsData = NULL;
	if (m_mapQuestDlgsData.find(id) != m_mapQuestDlgsData.end()) {
		questdlgsData = m_mapQuestDlgsData[id];
	}
	return questdlgsData;
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
			data->eyeRange = value["eyeRange"].GetDouble();
			data->patrolRange = value["patrolRange"].GetDouble();
			data->perceptionRange = value["perceptionRange"].GetDouble();
			data->attackedrestoretimes = value["attackedrestoretimes"].GetDouble();
			data->beforeattacktimes = value["beforeattacktimes"].GetDouble();
			data->attackAnimatetimePer = value["attackAnimatetimePer"].GetDouble();
			data->imagePath = value["imagePath"].GetString();
			addDataToMonsData(data);
		}
	} while (0);
}

void GameData::readNpcsDataFile()
{
	std::string jsonPath = NPCS_DATA_PATH;
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
		log("%d", pArray.Size());
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
		{
			// 读取对象属性
			NpcsData* data = new NpcsData();
			const rapidjson::Value &value = pArray[i];  // value为一个对象
			data->id = value["id"].GetInt();
			data->name = value["name"].GetString();
			const rapidjson::Value &intArray = value["quest_id"];
			for (int i = 0; i < intArray.Size(); i++) {
				data->quest_id.push_back(intArray[i].GetInt());
			}
			const rapidjson::Value &strArray = value["dlgs"];
			for (int i = 0; i < strArray.Size(); i++) {
				data->dlgs.push_back(strArray[i].GetString());
			}
			//data->imagePath = value["imagePath"].GetString();
			addDataToNpcsData(data);
		}
	} while (0);
}

void GameData::readQuestsDataFile()
{
	std::string jsonPath = QUESTS_DATA_PATH;
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
			QuestListData* data = new QuestListData();
			const rapidjson::Value &value = pArray[i];  // value为一个对象
			data->id = value["id"].GetInt();
			data->title = value["title"].GetString();
			data->instruct = value["instruct"].GetString();
			data->type = value["type"].GetInt();
			data->status = value["status"].GetInt();
			data->mons_id = value["mons_id"].GetString();
			addDataToQuestsData(data);
		}
	} while (0);
}

void GameData::readQuestDlgsDataFile()
{
	std::string jsonPath = QDLGS_DATA_PATH;
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
			QuestDlgsData* data = new QuestDlgsData();
			const rapidjson::Value &value = pArray[i];  // value为一个对象
			data->id = value["id"].GetInt();
			data->start = value["start"].GetString();
			data->active = value["active"].GetString();
			data->finish = value["finish"].GetString();
			data->answer = value["answer"].GetString();
			addDataToQuestDlgsData(data);
		}
	} while (0);
}
