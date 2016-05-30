#ifndef _GameScene_H_
#define _GameScene_H_

#include "Player.h"
#include "MonsterManager.h"
#include "NpcManager.h"
#include "PopManager.h"

class GameScene : public Layer
{
public:
	// 场景初始化
	static Scene* createSceneWithId(int id);
	static Scene* createSceneWithSaveData();		
	bool init(int id);
	bool initWithSaveData();
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt);
public:
	static int getCurSceneId();
	void setMapInfo(int id);
	void setViewpointCenter(Vec2 Position);
	void addPlayer(Point pos, int direction = em_down);
	void addNpc(int npcId, Point pos);
	void addMonster(const std::string& name, Point pos);  // 还需要更改详细属性
	void loadPlistFile(); // 应该放在游戏开始界面加载。只加载一次
private:
	// 基本对象
	NPC* m_npc;
	Player* m_player;	
	TMXTiledMap* m_map;
	Monster* m_monster;
	static int curSceneId; // 当前场景id
};

#endif