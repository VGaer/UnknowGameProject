#ifndef _GameScene_H_
#define _GameScene_H_

#include "Player.h"
#include "MonsterManager.h"
#include "NPC.h"
#include "PopManager.h"
#include "NpcManager.h"


class GameScene : public Layer
{
public:
	// ������ʼ��
	static Scene* createSceneWithId(int id);
	static Scene* loadSceneWithSaveData();
	virtual bool init(int id);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt);
	void MonHP_MPBar_Update(float dt);
	GameScene();
public:
	void setMapInfo(int id);
	void setViewpointCenter(Vec2 Position);
	void addPlayer(Point pos, int direction = em_down);
	void addPlayer(PlayerData* data);
	void addNpc(vector<NpcsData*> nData);
	void addMonster(const std::string& name, Point pos);  // ����Ҫ������ϸ����
	void AfterPlayerSetMonsterBindPlayer();
	
	void firstEnterTalk();
	static int sceneId;
private:
	// ��������
	Player* m_player;
	TMXTiledMap* m_map;
	Monster* m_monster;
	TMXObjectGroup* objGroup;
	NPC* m_npc;
	static bool OnlyOnceSetScale;
};

#endif