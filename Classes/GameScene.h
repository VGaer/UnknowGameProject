#ifndef _GameScene_H_
#define _GameScene_H_

#include "Player.h"
#include "MonsterManager.h"

class GameScene : public Layer
{
public:
	// ������ʼ��
	static Scene* createSceneWithId(int id);
	virtual bool init(int id);
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt);
public:	
	void setMapInfo(int id);
	void setViewpointCenter(Vec2 Position);
	void addMonster(const std::string& name, Point pos);  // ����Ҫ������ϸ����
	void loadPlistFile(); // Ӧ�÷�����Ϸ��ʼ������ء�ֻ����һ��
private:
	// ��������
	Player* m_player;
	TMXTiledMap* m_map;
	Monster* m_monster;
};

#endif