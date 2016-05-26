#pragma once
/****
÷ÿ–¥Menu£¨…Ë÷√SwallowTouchesŒ™fasle
***/
#include "cocos2d.h"
USING_NS_CC;
class DBMenu :public Menu
{
public:
	bool init();

	/**
	*@brief Creates an empty Menu.
	*/
	static DBMenu* create();

	/** initializes a Menu with a NSArray of MenuItem objects */
	bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
	static DBMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
	static DBMenu* createWithItem(MenuItem* item);

	/** creates a Menu with MenuItem objects */
	static DBMenu* createWithItems(MenuItem *firstItem, va_list args);
	static DBMenu* create(MenuItem* item, ...) CC_REQUIRES_NULL_TERMINATION;
private:
};
