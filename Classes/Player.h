#ifndef __Player_H__
#define __Player_H__

#include "cocos2d.h"
#include "Entity.h"
USING_NS_CC;
typedef enum 
{
	enum_up = 100,
	enum_down,
	enum_left,
	enum_right,
	enum_static
}MoveDir;

class Player : public Entity
{
public:
	CREATE_FUNC(Player);
	virtual bool init();
	virtual void update(float dt);
	void keyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void keyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void setTiledMap(TMXTiledMap* map);
	Vec2 tiledCoordForPosition(Vec2 pos);
	void setPlayerPosition(Vec2 position);
private:
	TMXTiledMap* m_map;
	int StateForMove;
	std::vector<int> vec;
};
#endif 
