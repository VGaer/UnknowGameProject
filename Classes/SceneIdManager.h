#ifndef __SceneIdManager_H__
#define __SceneIdManager_H__

#include <map>
#include <vector>

#define Scene_DATA_PATH "JsonText/SceneId.json"	

struct SceneData
{
	std::string name;
	int Id;
	int Level;//����Ҫ��ĵȼ�
	std::vector<int> Misson;//Ҫ�����������嵥
	std::vector<int> MissonActive;//Ҫ�������active(����)�嵥
};


class SceneIdManager
{
public:
	static SceneIdManager*  getInstance();
	SceneIdManager();
	~SceneIdManager();
public:
	std::map<std::string, SceneData> map_scenenameToSceneData;
	std::map<int, SceneData>  map_sceneIdToSceneData;
public:
	// ����һ����������
	void addDataToSceneData(SceneData scenedata);
private:
	
	// ��ȡ�ļ�����
	void readSceneDataFile();
};

#endif