#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
using namespace std;

struct MonsData
{
	string name;	   // 名字
	float hp;		   // 生命值
	float damage;	   // 攻击力
	float moveSpeed;   // 行走一条边的时间
	float attackInter; // 攻击间隔
	float attackRange; // 攻击范围
};

class GameData
{
public:
	// 数据初始化
	GameData();
	~GameData();
	// 基础数据存放
	typedef map<string, MonsData> MonsData_t;
	static MonsData_t m_mapMonsData;
public:
	// 加入一条怪物数据
	static void addDataToMonsData(const MonsData& data);
	// 获取一条怪物数据
	static const MonsData* getDataFromMonsData(const string& name);
};

#endif