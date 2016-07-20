#include "PopManager.h"

PopManager* PopManager::getInstance()
{
	static PopManager* instance = NULL;
	if (instance == NULL)
		instance = new PopManager();
	return instance;
}

map<string,Pop*>& PopManager::getPopsMap()
{
	return m_popMap;
}