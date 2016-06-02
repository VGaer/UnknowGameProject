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
	// 加入一条场景数据
	void addDataToSceneData(SceneData scenedata);
private:
	
	// 读取文件数据
	void readSceneDataFile();
};

#endif