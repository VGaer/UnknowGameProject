#pragma once
#include "Entity.h"
#include "Player.h"
#include "cocos2d.h"
#include "PopLayer.h"
#include "GameData.h"
#include "QuestList.h"
USING_NS_CC;

class QuestList;

enum NpcStatus {
	normal = 0,
	actived
};

class NPC : public Entity {
public:
	//  Ù–‘≥ı ºªØ
	static NPC* create(const std::string& name);
	bool init(const std::string& name);
	//static NPC* createWithparent(TMXTiledMap* parent);
	//virtual bool init();
	//void update(float dt);
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
	bool getIsRetain() { return isRetain; }
private:
	int activeQuest;
	int page;
	bool isRetain;
	bool bActive;
	string sourceName;
	NpcsData* data;
	map<int, QuestListData*> quests;
	QuestList* items;
	Player* m_player;
	TMXTiledMap* m_map;
	TMXTiledMap* m_parrent;
};