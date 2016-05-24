#include "Gamedata.h"
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
	for (auto it = m_mapMonsData.begin(); it != m_mapMonsData.end(); it++)
	{
		delete (*it).second;
		(*it).second = NULL;
	}
	m_mapMonsData.clear();
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
	unsigned char* pBytes = NULL;

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
			MonsData* data = new MonsData();
			const rapidjson::Value& value = pArray[i]; //value为一个对象
			data->name = value["name"].GetString();
			data->hp = value["hp"].GetDouble();
			data->moveSpeed = value["moveSpeed"].GetDouble();
			data->eyeRange = value["eyeRange"].GetDouble();
			data->perceptionRange = value["perceptionRange"].GetDouble();
			data->attackedrestoretime = value["attackedrestoretime"].GetDouble();
			data->imagePath = value["imagePath"].GetString();
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
				skill.hAnimatePath = baseskill["hAnimatePath"].GetString();
				skill.dAnimatePath = baseskill["dAnimatePath"].GetString();
				skill.uAnimatePath = baseskill["uAnimatePath"].GetString();
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
				skill.hAnimatePath = bigskill["hAnimatePath"].GetString();
				skill.dAnimatePath = bigskill["dAnimatePath"].GetString();
				skill.uAnimatePath = bigskill["uAnimatePath"].GetString();
				data->skillmap[skill.skilltype] = skill;
			}
			const rapidjson::Value& remoteskillArray = value["remoteskill"];
			if (remoteskillArray.Size() > 0)
			{
				const rapidjson::Value& remoteskill = remoteskillArray[0];//获取数组对象
				MonSkill skill;
				skill.skilltype = remoteskill["skilltype"].GetString();
				skill.attackRange = remoteskill["attackRange"].GetDouble();
				skill.beforeattacktimes = remoteskill["beforeattacktimes"].GetDouble();
				skill.attackAnimatetimePer = remoteskill["attackAnimatetimePer"].GetDouble();
				skill.attackInter = remoteskill["attackInter"].GetDouble();
				skill.attackEnd = remoteskill["attackEnd"].GetDouble();
				skill.Isinter = remoteskill["Isinter"].GetBool();
				skill.damage = remoteskill["damage"].GetDouble();
				skill.hAnimatePath = remoteskill["hAnimatePath"].GetString();
				skill.dAnimatePath = remoteskill["dAnimatePath"].GetString();
				skill.uAnimatePath = remoteskill["uAnimatePath"].GetString();
				data->skillmap[skill.skilltype] = skill;
			}
			addDataToMonsData(data);
		}
	} while (0);

}