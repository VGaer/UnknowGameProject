#include "Monster.h"

Monster* Monster::create(const std::string& file)
{
	auto unit = new Monster();
	if (unit && unit->init(file)){
		unit->autorelease();
	}
	else{
		CC_SAFE_DELETE(unit);
		unit = NULL;
	}
	return unit;
}

bool Monster::init(const std::string& file)
{
	bindSprite(Sprite::create(file));
	return true;
}