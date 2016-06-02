#ifndef __SceneIdManager_H__
#define __SceneIdManager_H__

#include <map>

#define Scene_DATA_PATH "JsonText/SceneId.json"	

struct SceneData
{
	std::string name;
	int Id;
};

class SceneIdManager
{
public:
	static SceneIdManager*  getInstance();
	SceneIdManager();
	~SceneIdManager();
public:
	std::map<std::string, int> map_scenenameToId;
	std::map<int, std::string>  map_sceneIdToname;
public:
	// ����һ����������
	void addDataToSceneData(SceneData scenedata);
private:
	
	// ��ȡ�ļ�����
	void readSceneDataFile();
};

#endif