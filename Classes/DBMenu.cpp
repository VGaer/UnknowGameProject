#include "DBMenu.h"

bool DBMenu::init()
{
	return initWithArray(Vector<MenuItem*>());
}

DBMenu * DBMenu::create()
{
	return DBMenu::create(nullptr, nullptr);
}

bool DBMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
	if (Layer::init())
	{
		_enabled = true;
		// menu in the center of the screen
		Size s = Director::getInstance()->getWinSize();

		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(Vec2(0.5f, 0.5f));
		this->setContentSize(s);

		setPosition(Vec2(s.width / 2, s.height / 2));

		int z = 0;

		for (auto& item : arrayOfItems)
		{
			this->addChild(item, z);
			z++;
		}

		_selectedItem = nullptr;
		_state = Menu::State::WAITING;

		// enable cascade color and opacity on menus
		setCascadeColorEnabled(true);
		setCascadeOpacityEnabled(true);


		auto touchListener = EventListenerTouchOneByOne::create();
		touchListener->setSwallowTouches(false);

		touchListener->onTouchBegan = CC_CALLBACK_2(DBMenu::onTouchBegan, this);
		touchListener->onTouchMoved = CC_CALLBACK_2(DBMenu::onTouchMoved, this);
		touchListener->onTouchEnded = CC_CALLBACK_2(DBMenu::onTouchEnded, this);
		touchListener->onTouchCancelled = CC_CALLBACK_2(DBMenu::onTouchCancelled, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

		return true;
	}
	return false;
}

DBMenu* DBMenu::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
	auto ret = new DBMenu();
	if (ret && ret->initWithArray(arrayOfItems))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

DBMenu* DBMenu::createWithItem(MenuItem* item)
{
	return DBMenu::create(item, nullptr);
}

DBMenu* DBMenu::createWithItems(MenuItem *item, va_list args)
{
	Vector<MenuItem*> items;
	if (item)
	{
		items.pushBack(item);
		MenuItem *i = va_arg(args, MenuItem*);
		while (i)
		{
			items.pushBack(i);
			i = va_arg(args, MenuItem*);
		}
	}

	return DBMenu::createWithArray(items);
}

DBMenu* DBMenu::create(MenuItem* item, ...) CC_REQUIRES_NULL_TERMINATION
{
	va_list args;
	va_start(args, item);

	DBMenu *ret = DBMenu::createWithItems(item, args);

	va_end(args);

	return ret;
}