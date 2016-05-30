#include "Pop.h"

Pop * Pop::create(Vec2 sender)
{
	auto unit = new Pop();
	if (unit && unit->init(sender)) {
		unit->autorelease();
	}
	else {
		CC_SAFE_DELETE(unit);
		unit = NULL;
	}
	return unit;
}

bool Pop::init(Vec2 sender)
{
	this->setAnchorPoint(Vec2(0.5, 0));
	this->setPosition(sender);
	return true;
}

void Pop::addLayer(int tag, Layer * sender, bool isPop)
{
	auto a = new pop();
	a->layer = sender;
	a->isPop = isPop;
	layer[tag] = a;
}

pop* Pop::getLayerByTag(int tag)
{
	if (!layer[tag]) return NULL;
	return layer[tag];
}

void Pop::releaseLayer(int tag)
{
	if (!layer[tag]) return;
	auto a = layer[tag];
	layer.erase(tag);
	delete a;
}

bool Pop::getPopped(int tag)
{
	if (!layer[tag]) return NULL;
	return layer[tag]->isPop;
}

void Pop::setPopped(int tag, bool isPop)
{
	if (!layer[tag]) return;
	layer[tag]->isPop = isPop;
}