#include "PopManager.h"

PopManager * PopManager::getInstance()
{
	static PopManager* instance = NULL;
	if (instance == NULL)
		instance = new PopManager();
	return instance;
}

PopManager::PopManager(){
}


void PopManager::addLayer(int tag, Layer * sender, bool isPop)
{
	auto a = new pop();
	a->layer = sender;
	a->isPop = isPop;
	layer[tag] = a;
}

pop* PopManager::getLayerByTag(int tag)
{
	if (!layer[tag]) return NULL;
	return layer[tag];
}

void PopManager::releaseLayer(int tag)
{
	if (!layer[tag]) return;
	auto a = layer[tag];
	layer.erase(tag);
	delete a;
}

bool PopManager::getPopped(int tag)
{
	if (!layer[tag]) return NULL;
	return layer[tag]->isPop;
}

void PopManager::setPopped(int tag, bool isPop)
{
	if (!layer[tag]) return;
	layer[tag]->isPop = isPop;
}