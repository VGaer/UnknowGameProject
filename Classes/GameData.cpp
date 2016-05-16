#include "GameData.h"

GameData::GameData()
{

}

GameData::~GameData()
{

}

void GameData::addDataToMonsData(const MonsData& data)
{
	string name = data.name;
	m_mapMonsData[name] = data;
}

const MonsData* GameData::getDataFromMonsData(const string& name)
{
	MonsData* monsData = NULL;
	if (m_mapMonsData.find(name) != m_mapMonsData.end())
	{
		monsData = &m_mapMonsData[name];
	}
	return monsData;
}