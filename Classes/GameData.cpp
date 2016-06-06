#include "GameData.h"
#include "cocos2d.h"
#include "json\document.h"
#include "json\prettywriter.h"
#include "json\stringbuffer.h"
#include "fstream"
#include "Player.h"
#include "GameScene.h"

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
	readPlayerDataFile();
}

GameData::~GameData()
{

}

PlayerData* GameData::getPlayerData()
{
	return playerData;
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
	if (m_mapIDtoNpc.find(data->mapID) != m_mapIDtoNpc.end()) {
		m_mapIDtoNpc[data->mapID].push_back(data);
	}
	else {
		vector<NpcsData*> vec;
		vec.push_back(data);
		m_mapIDtoNpc[data->mapID] = vec;
	}
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

map<int, vector<NpcsData*>> GameData::getMapIDtoNpcData()
{
	return m_mapIDtoNpc;
}

QuestListData * GameData::getDataFromQuestsData(const int id)
{
	QuestListData* questlistData = NULL;
	if (m_mapQuestSaveData.size() == 0)
	{
		if (m_mapQuestsData.find(id) != m_mapQuestsData.end()) {
			questlistData = m_mapQuestsData[id];
		}
	}
	else {
		if (m_mapQuestSaveData.find(id) != m_mapQuestSaveData.end()) {
			questlistData = m_mapQuestSaveData[id];
		}
	}
	return questlistData;
}

map<int, QuestDlgsData*> GameData::getDataFromQuestDlgsData()
{
	return m_mapQuestDlgsData;
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
			//读取对象属性
			MonsData* data = new MonsData();
			const rapidjson::Value& value = pArray[i]; //value为一个对象
			data->id = value["id"].GetInt();
			data->name = value["name"].GetString();
			data->hp = value["hp"].GetDouble();
			data->moveSpeed = value["moveSpeed"].GetDouble();
			data->eyeRange = value["eyeRange"].GetDouble();
			data->perceptionRange = value["perceptionRange"].GetDouble();
			data->attackedrestoretime = value["attackedrestoretime"].GetDouble();
			data->imagePath = value["imagePath"].GetString();
			data->exp = value["exp"].GetDouble();
			const rapidjson::Value& baseskillArray = value["baseskill"];
			if (baseskillArray.Size() > 0)
			{
				const rapidjson::Value& baseskill = baseskillArray[0];//获取数组对象
				MonSkill skill;
				skill.skilltype = baseskill["skilltype"].GetString();
				skill.attackRange = baseskill["attackRange"].GetDouble();
				skill.beforeattacktimes = baseskill["beforeattacktimes"].GetDouble();
				skill.attackAnimatetimePer = baseskill["attackAnimatetimePer"].GetDouble();
				skill.attackInter = baseskill["attackInter"].GetDouble();
				skill.attackEnd = baseskill["attackEnd"].GetDouble();
				skill.Isinter = baseskill["Isinter"].GetBool();
				skill.damage = baseskill["damage"].GetDouble();
				skill.attackNums = baseskill["attackNums"].GetInt();
				skill.attackNumsInter = baseskill["attackNumsInter"].GetDouble();
				skill.hAnimateName = baseskill["hAnimateName"].GetString();
				skill.dAnimateName = baseskill["dAnimateName"].GetString();
				skill.uAnimateName = baseskill["uAnimateName"].GetString();
				data->skillmap[skill.skilltype] = skill;
			}
			const rapidjson::Value& bigskillArray = value["bigskill"];
			if (bigskillArray.Size() > 0)
			{
				const rapidjson::Value& bigskill = bigskillArray[0];//获取数组对象
				MonSkill skill;
				skill.skilltype = bigskill["skilltype"].GetString();
				skill.attackRange = bigskill["attackRange"].GetDouble();
				skill.beforeattacktimes = bigskill["beforeattacktimes"].GetDouble();
				skill.attackAnimatetimePer = bigskill["attackAnimatetimePer"].GetDouble();
				skill.attackInter = bigskill["attackInter"].GetDouble();
				skill.attackEnd = bigskill["attackEnd"].GetDouble();
				skill.Isinter = bigskill["Isinter"].GetBool();
				skill.damage = bigskill["damage"].GetDouble();
				skill.attackNums = bigskill["attackNums"].GetInt();
				skill.attackNumsInter = bigskill["attackNumsInter"].GetDouble();
				skill.hAnimateName = bigskill["hAnimateName"].GetString();
				skill.dAnimateName = bigskill["dAnimateName"].GetString();
				skill.uAnimateName = bigskill["uAnimateName"].GetString();
				data->skillmap[skill.skilltype] = skill;
			}
			const rapidjson::Value& remoteskillArray = value["remoteskill"];
			if (remoteskillArray.Size() > 0)
			{
				const rapidjson::Value& remoteskill = remoteskillArray[0];//获取数组对象
				MonRemoteSkill skill;
				skill.skilltype = remoteskill["skilltype"].GetString();
				skill.projectileName = remoteskill["projectileName"].GetString();
				skill.projectileAnimateName = remoteskill["projectileAnimateName"].GetString();
				skill.skillwidth = remoteskill["skillwidth"].GetDouble();
				skill.skillheight = remoteskill["skillheight"].GetDouble();
				skill.projmovespeed = remoteskill["projmovespeed"].GetDouble();
				skill.eyeRangeForstartskill = remoteskill["eyeRangeForstartskill"].GetDouble();
				skill.beforeattacktimes = remoteskill["beforeattacktimes"].GetDouble();
				skill.attackAnimatetimePer = remoteskill["attackAnimatetimePer"].GetDouble();
				skill.attackInter = remoteskill["attackInter"].GetDouble();
				skill.attackEnd = remoteskill["attackEnd"].GetDouble();
				skill.Isinter = remoteskill["Isinter"].GetBool();
				skill.damage = remoteskill["damage"].GetDouble();
				skill.duration = remoteskill["duration"].GetDouble();
				skill.hAnimateName = remoteskill["hAnimateName"].GetString();
				skill.dAnimateName = remoteskill["dAnimateName"].GetString();
				skill.uAnimateName = remoteskill["uAnimateName"].GetString();
				data->remoteskillmap[skill.skilltype] = skill;
			}
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
			data->status = value["status"].GetInt();
			data->mapID = value["mapID"].GetInt();
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
			data->mapID = value["mapID"].GetInt();
			data->title = value["title"].GetString();
			data->instruct = value["instruct"].GetString();
			data->type = value["type"].GetInt();
			data->status = value["status"].GetInt();
			const rapidjson::Value &mArray = value["mons_id"];
			for (int i = 0; i < mArray.Size(); i++) {
				data->mons_id.push_back(mArray[i].GetInt());
			}

			const rapidjson::Value &fArray = value["forgeID"];
			for (int i = 0; i < fArray.Size(); i++) {
				data->forgeID.push_back(fArray[i].GetInt());
			}

			data->targetNpc = value["targetNpc"].GetString();
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

void GameData::writePlayerData()
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	auto player = Player::getInstance();

	document.AddMember("sceneId", GameScene::sceneId, allocator);
	document.AddMember("posX", player->getPositionX(), allocator);
	document.AddMember("posY", player->getPositionY(), allocator);
	document.AddMember("direction", player->getPlayerDir(), allocator);
	document.AddMember("level", player->m_playerlevel, allocator);
	document.AddMember("hp", player->m_hp, allocator);
	document.AddMember("mp", player->m_mp, allocator);
	document.AddMember("exp", player->m_exp, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	auto out = buffer.GetString();
	ofstream fp(PLAYER_DATA_PATH, ios_base::out);
	fp << out;
	fp.close();
}

void GameData::readPlayerDataFile()
{
	playerData = NULL;
	std::string jsonPath = PLAYER_DATA_PATH;
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
		if (!_doc.IsObject())
			return;
		const rapidjson::Value& value = _doc;
		PlayerData* data = new PlayerData();
		data->sceneId = value["sceneId"].GetInt();
		data->posX = value["posX"].GetDouble();
		data->posY = value["posY"].GetDouble();
		data->direction = value["direction"].GetInt();
		data->level = value["level"].GetInt();
		data->hp = value["hp"].GetDouble();
		data->mp = value["mp"].GetDouble();
		data->exp = value["exp"].GetDouble();
		playerData = data;
	} while (0);
}

bool GameData::isExistSaveDoc()
{
	return !playerData == NULL;
}

void GameData::writeQuestData()
{
	//write json
	rapidjson::Document document;
	document.SetArray();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value str;
	for (auto& i : m_mapQuestsData)
	{
		auto second = i.second;
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("id", second->id, allocator);

		str.SetString(second->title.c_str(), second->title.length(), allocator);
		object.AddMember("title", str, allocator);

		str.SetString(second->instruct.c_str(), second->instruct.length(), allocator);
		object.AddMember("instruct", str, allocator);

		object.AddMember("status", second->status, allocator);

		str.SetString(second->targetNpc.c_str(), second->targetNpc.length(), allocator);
		object.AddMember("targetNpc", str, allocator);

		rapidjson::Value farray(rapidjson::kArrayType);
		for (auto& j : second->forgeID) {
			rapidjson::Value object(rapidjson::kObjectType);
			object.SetInt(j);
			farray.PushBack(object, allocator);
		}
		object.AddMember("forgeID", farray, allocator);

		rapidjson::Value marray(rapidjson::kArrayType);
		for (auto& j : second->mons_id) {
			rapidjson::Value object(rapidjson::kObjectType);
			object.SetInt(j);
			marray.PushBack(object, allocator);
		}
		object.AddMember("mons_id", marray, allocator);

		object.AddMember("mapID", str, allocator);

		object.AddMember("type", second->type, allocator);
		document.PushBack(object, allocator);
	}
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	auto out = buffer.GetString();
	ofstream fp(QUESTSAVE_DATA_PATH, ios_base::out);
	fp << out;
}

void GameData::readQuestSaveDataFile()
{
	std::string jsonPath = QUESTSAVE_DATA_PATH;
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
			const rapidjson::Value &mArray = value["mons_id"];
			for (int i = 0; i < mArray.Size(); i++) {
				data->mons_id.push_back(mArray[i].GetInt());
			}
			const rapidjson::Value &fArray = value["forgeID"];
			for (int i = 0; i < fArray.Size(); i++) {
				data->forgeID.push_back(fArray[i].GetInt());
			}
			data->targetNpc = value["targetNpc"].GetString();
			m_mapQuestSaveData[data->id] = data;
		}
	} while (0);
}

