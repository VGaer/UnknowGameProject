#include "Player.h"

bool Player::init()
{
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::keyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::keyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	this->scheduleUpdate();

	StateForMove = enum_static;

	return true;
}

void Player::update(float dt)
{
	//Sprite* s = m_map->getLayer("background")->getTileAt(tiledCoordForPosition(this->getPosition()));
	//log("````%f", s->getVertexZ());
	//Sprite* ss = m_map->getLayer("foreground")->getTileAt(tiledCoordForPosition(this->getPosition()));
//	log("~~%f", ss->getVertexZ());

	if (vec.size() > 0){
		if (vec.size() > 1)
		{
			if (vec.back() == enum_up && vec[0] == enum_left
				|| vec[0] == enum_up && vec.back() == enum_left)
			{
				this->setPlayerPosition(this->getPosition() + Vec2(-6, 6));
				return;
			}
			else if (vec.back() == enum_up && vec[0] == enum_right
				|| vec[0] == enum_up && vec.back() == enum_right)
			{
				this->setPlayerPosition(this->getPosition() + Vec2(6, 6));
				return;
			}
			else if (vec.back() == enum_down && vec[0] == enum_left
				|| vec[0] == enum_down && vec.back() == enum_left)
			{
				this->setPlayerPosition(this->getPosition() + Vec2(-6, -6));
				return;
			}
			else if ((vec.back() == enum_down && vec[0] == enum_right
				|| vec[0] == enum_down && vec.back() == enum_right))
			{
				this->setPlayerPosition(this->getPosition() + Vec2(6, -6));
				return;
			}
		}
		switch (vec.back())
		{
		case enum_up:{
			Vec2 dstPos = this->getPosition();
			dstPos.y += 2;
			
			this->setPlayerPosition(this->getPosition() + Vec2(0,6));
			break;
		}
		case enum_down:{
			Vec2 dstPos = this->getPosition();
			dstPos.y -= 2;
			Vec2 tiledPos = tiledCoordForPosition(dstPos);
			
			this->setPlayerPosition(this->getPosition() + Vec2(0,-6));
			break;
		}
		case enum_left:{
			Vec2 dstPos = this->getPosition();
			dstPos.x -= 2;
			Vec2 tiledPos = tiledCoordForPosition(dstPos);
			
			this->setPlayerPosition(this->getPosition() + Vec2(-6, 0));
			break;
		}
		case enum_right:{
			Vec2 dstPos = this->getPosition();
			dstPos.x += 2;
			Vec2 tiledPos = tiledCoordForPosition(dstPos);
			
			this->setPlayerPosition(this->getPosition() + Vec2(6, 0));
			break;
		}
		case enum_static:
			break;
		default:
			break;
		}
	}
	else
	{
		//Ö÷½Ç¾²Ö¹
	}
}

void Player::keyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		StateForMove = enum_up;
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_up)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_up);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		StateForMove = enum_down;
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_down)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_down);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		StateForMove = enum_left;
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_left)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_left);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		StateForMove = enum_right;
		bool s = true;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_right)
			{
				s = false;
				break;
			}
		}
		if (s == true)
			vec.push_back(enum_right);
	}
}

void Player::keyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		if (StateForMove == enum_up)
			StateForMove = enum_static;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_up)
			{
				vec.erase(it);
				break;
			}
		}	
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
	{
		if (StateForMove == enum_down)
			StateForMove = enum_static;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_down)
			{
				vec.erase(it);
				break;
			}
		}
		
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		if (StateForMove == enum_left)
			StateForMove = enum_static;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_left)
			{
				vec.erase(it);
				break;
			}
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		if (StateForMove == enum_right)
			StateForMove = enum_static;
		for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			if ((*it) == enum_right)
			{
				vec.erase(it);
				break;
			}
		}
	}
}

void Player::setTiledMap(TMXTiledMap* map)
{
	m_map = map;
}

Vec2 Player::tiledCoordForPosition(Vec2 pos)
{
	Size mapTiledNum = m_map->getMapSize();
	Size tiledSize = m_map->getTileSize();
	Size visiblesize = Director::getInstance()->getVisibleSize();

	int x = (int)(pos.x / tiledSize.width);
	int y = (int)(m_map->getMapSize().height * m_map->getTileSize().height - pos.y) / m_map->getTileSize().height;

	return Vec2(x,y);
}

void  Player::setPlayerPosition(Vec2 position)
{
	Vec2 tileCoord = this->tiledCoordForPosition(position);
	if (tileCoord.x >= 0 && tileCoord.x < m_map->getMapSize().width //²»³¬³öÍßÆ¬µØÍ¼×ø±ê
		&& tileCoord.y >= 0 && tileCoord.y < m_map->getMapSize().height){
		int tileGid = m_map->getLayer("barrier")->getTileGIDAt(tileCoord);
		if (tileGid > 0){
			Value prop = m_map->getPropertiesForGID(tileGid);
			ValueMap proValueMap = prop.asValueMap();

			if (proValueMap.find("Collidable") != proValueMap.end()){
				std::string collision = proValueMap.at("Collidable").asString();
				if (collision == "true"){
					log("---");
					return;
				}

			}
		}
	}
	this->setPosition(position);
}