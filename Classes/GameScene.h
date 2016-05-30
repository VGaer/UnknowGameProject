#ifndef _GameScene_H_
#define _GameScene_H_

#include "Player.h"
#include "MonsterManager.h"
#include "NpcManager.h"
#include "PopManager.h"

class GameScene : public Layer
{
public:
	// ������ʼ��
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
	void addMonster(const std::string& name, Point pos);  // ����Ҫ������ϸ����
	void loadPlistFile(); // Ӧ�÷�����Ϸ��ʼ������ء�ֻ����һ��
private:
	// ��������
	NPC* m_npc;
	Player* m_player;	
	TMXTiledMap* m_map;
	Monster* m_monster;
	static int curSceneId; // ��ǰ����id
};

#endif