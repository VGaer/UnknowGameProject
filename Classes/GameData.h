#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
using namespace std;

#define MONS_DATA_PATH "..."	// ���������ļ�·��

struct MonsData
{
	string name;	   // ����
	float hp;		   // ����ֵ
	float damage;	   // ������
	float moveSpeed;   // ����һ���ߵ�ʱ��
	float attackInter; // �������
	float attackRange; // ������Χ
};

class GameData
{
public:
	// ���ݳ�ʼ��
	GameData* getInstance();
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