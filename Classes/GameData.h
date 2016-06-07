#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include "vector"
using namespace std;

#define QUESTSAVE_DATA_PATH "JsonText/QuestSave.json"	// ���������ļ�����·��
#define MONS_DATA_PATH "JsonText/monsterset.json"	// ����skill�ļ�·��
#define NPCS_DATA_PATH "JsonText/NpcDlgs.json"	// NPC�����ļ�·��
#define QUESTS_DATA_PATH "JsonText/QuestList.json"	// ���������ļ�·��
#define QDLGS_DATA_PATH "JsonText/QuestDlgs.json"	// ����Ի������ļ�·��
#define PLAYER_DATA_PATH "JsonText/PlayerData.json" // ��Ҵ浵�ļ�·��
#define PLAYER_DLGS_DATA_PATH "JsonText/PlayerDlgs.json" // ��ҶԻ�����·��

struct NpcsData {
	int id;				// id
	int status;			//npc״̬��0���ޡ�1����������У�
	int mapID;
	vector<int> quest_id;		//����id
	string name;		// ����
	vector<string> dlgs;		// �Ի�s
	string imagePath;	// ͼƬ·��
};

struct MonSkill
{
	string skilltype; //��������
	float attackRange; //������Χ
	float beforeattacktimes;	//����ǰҡʱ��
	float attackAnimatetimePer; //����ÿ֡����ʱ��
	float attackInter;    //�������
	float attackEnd; //������������ʱ��
	bool Isinter;		  //�Ƿ���ǰҡʱ�����ж�
	float damage;		//������
	int attackNums;   //��������
	float attackNumsInter; //��ι����ļ��ʱ��(���ι�����ʱ���ż��ʱ��Ϊ����ǰҡ����������ʱ��,���ʱʱ�䰲��Ϊ��������ʱ��/����)
	string hAnimateName; //ˮƽ������������
	string dAnimateName; //�¹�����������
	string uAnimateName; //�Ϲ�����������
};

struct MonRemoteSkill
{
	string skilltype; //��������
	string projectileName;
	string projectileAnimateName;
	float skillwidth; //Ͷ�����ܿ��
	float skillheight;//Ͷ�����ܸ߶�
	float projmovespeed;//Ͷ�������ƶ��ٶ�
	float eyeRangeForstartskill; //�������������������
	float beforeattacktimes;	//����ǰҡʱ��
	float attackAnimatetimePer; //����ÿ֡����ʱ��
	float attackInter;    //�������
	float attackEnd; //������������ʱ��
	bool Isinter;		  //�Ƿ���ǰҡʱ�����ж�
	float damage;		//������
	float duration; //�ͷų����ļ��ܳ���ʱ��
	string hAnimateName; //ˮƽ������������
	string dAnimateName; //�¹�����������
	string uAnimateName; //�Ϲ�����������
};

struct MonsData
{
	int id;
	string name;		//����
	float hp;			//����ֵ
	float moveSpeed;	//����һ���ߵ�ʱ��
	float eyeRange;		//��Ұ�뾶
	float perceptionRange;//��֪�뾶
	float attackedrestoretime;//��ֱʱ��
	float exp;
	string imagePath; //ͼƬ·��
	std::map<string, MonSkill> skillmap; //��������
	std::map<string, MonRemoteSkill> remoteskillmap; //Զ�̼�������
};


struct QuestListData {
	int id;			//����id
	string title;	//�������
	string instruct; //������
	int type;		//��������(0:���ȣ�1��ɱ��)
	int status;		//����״̬ ��0��δ�ӡ�1�����2����ɡ�3�����ύ��
	int mapID;
	bool isSameMap;		//�Ƿ��뷢������ͬһ����ͼ
	vector<int> mons_id;	//����Ŀ�꣨����У�
	vector<int> forgeID;
	string targetNpc; //����Ŀ�꣨����У�
};

struct QuestDlgsData {
	int id;			//�Ի����
	string start;	//����ʼǰ�Ի�
	string active;	//���񼤻��Ի�
	string finish;	//������ɺ�Ի�
	string answer;	//Ŀ��NPC�ظ�������У�
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
	float exp;//��ǰ����
};

struct PlayerTaskDlgsData
{
	int taskId;
	vector<string> acceptTaskDlgs;	// ��������ʱ�Ի�
	vector<string> commitTaskDlgs;  // ���ύ����ʱ�Ի�
	vector<string> finishTaskDlgs;	// �������ʱ�Ի�
	bool isSaidAcTsDlgs;
	bool isSaidCmTsDlgs;
	bool isSaidFiTsDlgs;
	bool isSaying;
};

struct EnterSceneDlgsData
{
	int sceneId;
	vector<string> cannotEnterDlgs; // �޷����볡��ʱ�Ի�
	vector<string> enterSceneDlgs;  // ��һ�ν��볡��ʱ�Ի�
	bool isSaid;
};

class GameData
{
public:
	// ���ݳ�ʼ��
	static GameData* getInstance();
	GameData();
	~GameData();
	// �������ݴ��
	map<string, MonsData*> m_mapMonsData;
	map<string, NpcsData*> m_mapNpcsData;
	map<int, QuestListData*> m_mapQuestsData;
	map<int, QuestDlgsData*> m_mapQuestDlgsData;
	map<int, vector<NpcsData*>> m_mapIDtoNpc;
	map<int, PlayerTaskDlgsData*> m_mapPlayerTaskDlgs;
	map<int, EnterSceneDlgsData*> m_mapEnterSceneDlgs;
public:
	// ����һ����������
	void addDataToMonsData(MonsData* data);
	// ��ȡһ����������
	MonsData* getDataFromMonsData(const string& name);
	// ����һ��NPC����
	void addDataToNpcsData(NpcsData* data);
	// ��ȡһ��NPC����
	NpcsData* getDataFromNpcsData(const string& name);
	map<int, vector<NpcsData*>> getMapIDtoNpcData();
	// ����һ����������
	void addDataToQuestsData(QuestListData* data);
	// ��ȡһ����������
	QuestListData* getDataFromQuestsData(const int id);
	// ����һ������Ի�����
	void addDataToQuestDlgsData(QuestDlgsData* data);
	// ��ȡһ������Ի�����
	map<int, QuestDlgsData*> getDataFromQuestDlgsData();
	// ���롢��ȡ��ҶԻ�
	void addDataToPlayerTaskDlgsData(PlayerTaskDlgsData* data);		// ����һ����ҶԻ�����
	PlayerTaskDlgsData* getDataFromPlayerTaskDlgsData(int taskId);	// ��ȡһ����ҶԻ�����
	void addDataToEnterSceneDlgsData(EnterSceneDlgsData* data);		// ����һ�����볡���Ի�
	EnterSceneDlgsData* getDataFromEnterSceneDlgsData(int sceneId);	// ��ȡһ�����볡���Ի�
	// �����������
	void writePlayerData();
	// ��ȡ�������
	PlayerData* getPlayerData();
	// �Ƿ��д浵
	bool isExistSaveDoc();
	//������������
	void writeQuestData();
	void readQuestSaveDataFile();
	map<int, QuestListData*> getQuestSaveData();
private:
	// ��ȡ�ļ�����
	void readMonsDataFile();
	void readNpcsDataFile();
	void readQuestsDataFile();
	void readQuestDlgsDataFile();
	void readPlayerDataFile();			// ��Ҵ浵�ļ�
	void readPlayerDlgsDataFile();		// ��ҶԻ������ļ�
private:
	PlayerData* playerData;
	map<int, QuestListData*> m_mapQuestSaveData;
};

#endif