#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include "vector"
using namespace std;

#define QUESTSAVE_DATA_PATH "JsonText/QuestSave.json"	// 任务数据文件保存路径
#define MONS_DATA_PATH "JsonText/monsterset.json"	// 怪物skill文件路径
#define NPCS_DATA_PATH "JsonText/NpcDlgs.json"	// NPC数据文件路径
#define QUESTS_DATA_PATH "JsonText/QuestList.json"	// 任务数据文件路径
#define QDLGS_DATA_PATH "JsonText/QuestDlgs.json"	// 任务对话数据文件路径
#define PLAYER_DATA_PATH "JsonText/PlayerData.json" // 玩家存档文件路径
#define PLAYER_DLGS_DATA_PATH "JsonText/PlayerDlgs.json" // 玩家对话数据路径

struct NpcsData {
	int id;				// id
	int status;			//npc状态（0：无、1：任务过程中）
	int mapID;
	vector<int> quest_id;		//任务id
	string name;		// 名字
	vector<string> dlgs;		// 对话s
	string imagePath;	// 图片路径
};

struct MonSkill
{
	string skilltype; //技能类型
	float attackRange; //攻击范围
	float beforeattacktimes;	//攻击前摇时间
	float attackAnimatetimePer; //攻击每帧动画时间
	float attackInter;    //攻击间隔
	float attackEnd; //攻击动画结束时间
	bool Isinter;		  //是否在前摇时可以中断
	float damage;		//攻击力
	int attackNums;   //攻击段数
	float attackNumsInter; //多段攻击的间隔时间(单段攻击的时候安排间隔时间为过了前摇的真正攻击时间,多段时时间安排为真正攻击时间/段数)
	string hAnimateName; //水平攻击动画名字
	string dAnimateName; //下攻击动画名字
	string uAnimateName; //上攻击动画名字
};

struct MonRemoteSkill
{
	string skilltype; //技能类型
	string projectileName;
	string projectileAnimateName;
	float skillwidth; //投掷技能宽度
	float skillheight;//投掷技能高度
	float projmovespeed;//投掷技能移动速度
	float eyeRangeForstartskill; //触发技能主角与怪物差距
	float beforeattacktimes;	//攻击前摇时间
	float attackAnimatetimePer; //攻击每帧动画时间
	float attackInter;    //攻击间隔
	float attackEnd; //攻击动画结束时间
	bool Isinter;		  //是否在前摇时可以中断
	float damage;		//攻击力
	float duration; //释放出来的技能持续时间
	string hAnimateName; //水平攻击动画名字
	string dAnimateName; //下攻击动画名字
	string uAnimateName; //上攻击动画名字
};

struct MonsData
{
	int id;
	string name;		//名字
	float hp;			//生命值
	float moveSpeed;	//行走一条边的时间
	float eyeRange;		//视野半径
	float perceptionRange;//感知半径
	float attackedrestoretime;//僵直时间
	float exp;
	string imagePath; //图片路径
	std::map<string, MonSkill> skillmap; //技能容器
	std::map<string, MonRemoteSkill> remoteskillmap; //远程技能容器
};


struct QuestListData {
	int id;			//任务id
	string title;	//任务标题
	string instruct; //任务简介
	int type;		//任务类型(0:跑腿，1：杀怪)
	int status;		//激活状态 （0：未接、1：激活、2：完成、3：可提交）
	int mapID;
	bool isSameMap;		//是否与发起人在同一个地图
	vector<int> mons_id;	//任务目标（如果有）
	vector<int> forgeID;
	string targetNpc; //任务目标（如果有）
};

struct QuestDlgsData {
	int id;			//对话编号
	string start;	//任务开始前对话
	string active;	//任务激活后对话
	string finish;	//任务完成后对话
	string answer;	//目标NPC回复（如果有）
};

struct PlayerData
{
	int sceneId;
	int direction;
	float posX;
	float posY;
	float level;
	float hp;
	float mp;
	float exp;//当前经验
};

struct PlayerTaskDlgsData
{
	int taskId;
	vector<string> acceptTaskDlgs;	// 接受任务时对话
	vector<string> commitTaskDlgs;  // 可提交任务时对话
	vector<string> finishTaskDlgs;	// 完成任务时对话
	bool isSaidAcTsDlgs;
	bool isSaidCmTsDlgs;
	bool isSaidFiTsDlgs;
	bool isSaying;
};

struct EnterSceneDlgsData
{
	int sceneId;
	vector<string> cannotEnterDlgs; // 无法进入场景时对话
	vector<string> enterSceneDlgs;  // 第一次进入场景时对话
	bool isSaid;
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
	map<string, NpcsData*> m_mapNpcsData;
	map<int, QuestListData*> m_mapQuestsData;
	map<int, QuestDlgsData*> m_mapQuestDlgsData;
	map<int, vector<NpcsData*>> m_mapIDtoNpc;
	map<int, PlayerTaskDlgsData*> m_mapPlayerTaskDlgs;
	map<int, EnterSceneDlgsData*> m_mapEnterSceneDlgs;
public:
	// 加入一条怪物数据
	void addDataToMonsData(MonsData* data);
	// 获取一条怪物数据
	MonsData* getDataFromMonsData(const string& name);
	// 加入一条NPC数据
	void addDataToNpcsData(NpcsData* data);
	// 获取一条NPC数据
	NpcsData* getDataFromNpcsData(const string& name);
	map<int, vector<NpcsData*>> getMapIDtoNpcData();
	// 加入一条任务数据
	void addDataToQuestsData(QuestListData* data);
	// 获取一条任务数据
	QuestListData* getDataFromQuestsData(const int id);
	// 加入一条任务对话数据
	void addDataToQuestDlgsData(QuestDlgsData* data);
	// 获取一条任务对话数据
	map<int, QuestDlgsData*> getDataFromQuestDlgsData();
	// 加入、获取玩家对话
	void addDataToPlayerTaskDlgsData(PlayerTaskDlgsData* data);		// 加入一条玩家对话数据
	PlayerTaskDlgsData* getDataFromPlayerTaskDlgsData(int taskId);	// 获取一条玩家对话数据
	void addDataToEnterSceneDlgsData(EnterSceneDlgsData* data);		// 加入一条进入场景对话
	EnterSceneDlgsData* getDataFromEnterSceneDlgsData(int sceneId);	// 获取一条进入场景对话
	// 保存玩家数据
	void writePlayerData();
	// 获取玩家数据
	PlayerData* getPlayerData();
	// 是否有存档
	bool isExistSaveDoc();
	//保存任务数据
	void writeQuestData();
	void readQuestSaveDataFile();
	map<int, QuestListData*> getQuestSaveData();
private:
	// 读取文件数据
	void readMonsDataFile();
	void readNpcsDataFile();
	void readQuestsDataFile();
	void readQuestDlgsDataFile();
	void readPlayerDataFile();			// 玩家存档文件
	void readPlayerDlgsDataFile();		// 玩家对话数据文件
private:
	PlayerData* playerData;
	map<int, QuestListData*> m_mapQuestSaveData;
};

#endif