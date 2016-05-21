#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
using namespace std;

//#define MONS_DATA_PATH "..."	// ���������ļ�·��
#define MONS_DATA_PATH "monattribute.json"

struct MonsData
{
	string name;	   // ����
	float hp;		   // ����ֵ
	float damage;	   // ������
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
	float eyeRange;	   // ��Ұ�뾶��
	float patrolRange; // Ѳ�߰뾶��
	float perceptionRange; //��֪�뾶
	float attackedrestoretimes; //�������ָ�ʱ�� //�ָ�ʱ��Լ��˵��ӲֱԽ��
	string imagePath;  // ͼƬ·��
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
public:
	// ����һ����������
	void addDataToMonsData(MonsData* data);
	// ��ȡһ����������
	const MonsData* getDataFromMonsData(const string& name);
private:
	// ��ȡ�ļ�����
	void readMonsDataFile();
};

#endif