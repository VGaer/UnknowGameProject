#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
using namespace std;

//#define MONS_DATA_PATH "..."	// 怪物数据文件路径
#define MONS_DATA_PATH "monattribute.json"

struct MonsData
{
	string name;	   // 名字
	float hp;		   // 生命值
	float damage;	   // 攻击力
	float moveSpeed;   // 行走一条边的时间
	float attackInter; // 攻击间隔
	float attackRange; // 攻击范围
	float eyeRange;	   // 视野半径，
	float patrolRange; // 巡逻半径，
	float perceptionRange; //感知半径
	float attackedrestoretimes; //被攻击恢复时间 //恢复时间约快说明硬直越高
	string imagePath;  // 图片路径
};

class GameData
{
public:
	// 数据初始化
	static GameData* getInstance();
	GameData();
	~GameData();
	// 基础数据存放
	map<string, MonsData*> m_mapMonsData;
public:
	// 加入一条怪物数据
	void addDataToMonsData(MonsData* data);
	// 获取一条怪物数据
	const MonsData* getDataFromMonsData(const string& name);
private:
	// 读取文件数据
	void readMonsDataFile();
};

#endif