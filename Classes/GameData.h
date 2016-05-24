#ifndef _GameData_H_
#define _GameData_H_

#include "string"
#include "map"
#include <vector>
using namespace std;

#define MONS_DATA_PATH "monsterset.json"

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
	string hAnimatePath; //ˮƽ��������
	string dAnimatePath; //�¹�������
	string uAnimatePath; //�Ϲ�������
};

struct MonsData
{
	string name;		//����
	float hp;			//����ֵ
	float moveSpeed;	//����һ���ߵ�ʱ��
	float eyeRange;		//��Ұ�뾶
	float perceptionRange;//��֪�뾶
	float attackedrestoretime;//��ֱʱ��
	string imagePath; //ͼƬ·��
	std::map<string,MonSkill> skillmap; //��������
};

class GameData
{
public:
	//���ݳ�ʼ��
	static GameData* getInstance();
	GameData();
	~GameData();
	//�������ݴ��
	map<string, MonsData*> m_mapMonsData;
public:
	void addDataToMonsData(MonsData* data);
	const MonsData* getDataFromMonsData(const string& name);
private:
	//��ȡ�ļ�����
	void readMonsDataFile();
};

#endif