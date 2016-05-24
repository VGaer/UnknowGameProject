#ifndef _GameScene_H_
#define _GameScene_H_

#include "Player.h"
#include "MonsterManager.h"

class GameScene : public Layer
{
public:
	// 场景初始化
	static Scene* createSceneWithId(int id);
	virtual bool init(int id);
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt);
public:	
	void setMapInfo(int id);
	void setViewpointCenter(Vec2 Position);
	void addMonster(const std::string& name, Point pos);  // 还需要更改详细属性
	void loadPlistFile(); // 应该放在游戏开始界面加载。只加载一次
private:
	// 基本对象
	Player* m_player;
	TMXTiledMap* m_map;
	Monster* m_monster;
};

#endif