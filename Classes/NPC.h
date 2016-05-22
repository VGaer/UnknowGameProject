#pragma once
#include "Entity.h"
#include "Player.h"
#include "cocos2d.h"
#include "PopLayer.h"
#include "GameData.h"

USING_NS_CC;

class NPC : public Entity {
public:
	static NPC* createWithparent(TMXTiledMap* parent);
	virtual bool init();
	//virtual void update(float dt);
	virtual bool onTouchBegan(Touch* _touch, Event* _event);
	virtual void onTouchEnded(Touch* _touch, Event* _event);
	void popLayer();
	void questLayer();
	void buttonCallback(Node *pNode);
	void ItemCallback(Node *pNode);
	void setPlayer(Player* sender) { m_player = sender; }
	void initDataWithName(const string& sender);
	void setTiledMap(TMXTiledMap* map);
	Vec2 tiledCoordForPosition(Vec2 pos);
	const NpcsData* getData() { return data; }
private:
	bool isPop;
	bool isPop2;
	int page;
	const NpcsData* data;
	Player* m_player;
	TMXTiledMap* m_map;
	TMXTiledMap* m_parrent;
};