#ifndef __Graph_H__
#define __Graph_H__

#include "Vertex.h"
#include "Edge.h"
#include "vector"
#include "map"

#define  NONE_ID (Vec2(-1,-1))

class Graph
{
public:
	// 初始化图
	static Graph* getInstance(); 
	void init(Vec2 start);
	// 添加删除顶点
	void addVertex(Vec2 vertexId, Vec2 point);
	void addEdgesForVertex(Vec2 startId, Vec2 endId, float weight);
	void removeVertex(Vec2 vertexId);
	Vertex* getGraphVertexByVertexId(Vec2 vertexId);
	// 寻路相关
	bool findPath(Vec2 startId, Vec2 endId);
	std::vector<Vec2> getPathResult();
	bool relax(Vec2 startId, Vec2 endId, float weight);
	float getDistance(Vec2 startId,Vec2 endId);
	Vertex* findMinHeurisVertex(std::vector < Vertex*>&);
	Vec2 positionForTiledCoord(Vec2 pos);
	Vec2 tiledCoordForPosition(Vec2 pos);
	// 绑定瓦片地图
	void setTildMap(TMXTiledMap* map);
	TMXTiledMap* getMap();
private:	
	// 瓦片地图数据
	int rowNum;
	int columnNum;	
	Vec2 targetId;
	// 顶点容器
	std::map<Vec2, Vertex*> vertices;
	std::vector<Vec2> path;
	TMXTiledMap* m_map;
};

#endif