#ifndef _GameScene_H_
#define _GameScene_H_

#include "Player.h"
#include "MonsterManager.h"
#include "NpcManager.h"
#include "PopManager.h"
#include "BarManager.h"

class GameScene : public Layer
{
public:
	// ������ʼ��
	static Scene* createSceneWithId(int id);
	static Scene* loadSceneWithSaveData();
	bool init(int id);
	bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt);
public:
	void setMapInfo(int id);
	void setViewpointCenter(Vec2 Position);
	void addPlayer(Point pos, int direction = enum_down);
	void addPlayer(PlayerData* data);
	void addMonster(const std::string& name, Point pos);  // ����Ҫ������ϸ����
	void loadPlistFile(); // Ӧ�÷�����Ϸ��ʼ������ء�ֻ����һ��
	static int sceneId;
private:
	// ��������
	Player* m_player;
	TMXTiledMap* m_map;
	Monster* m_monster;
	NPC* m_npc;
};

#endif