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
	static Graph* getInstance();
	void init(Vec2); //从某顶点开始填充图
	void addEdgesForVertex(Vec2 startId, Vec2 endId,float weight);
	void addVertex(Vec2 vertexId,Vec2 point);
	void removeVertex(Vec2 vertexId);
	bool findPath(Vec2 startId, Vec2 endId);
	bool relax(Vec2 startId, Vec2 endId, float weight);
	float getDistance(Vec2 startId,Vec2 endId);
	Vertex* findMinHeurisVertex(std::vector < Vertex*>&);
	std::vector<Vec2> getPathResult();
	void setTildMap(TMXTiledMap* map);
	TMXTiledMap* getMap();
	Vertex* getGraphVertexByVertexId(Vec2 vertexId);
private:
	std::map<Vec2, Vertex*> vertices;
	std::vector<Vec2> path;
	Vec2 targetId;
	int rowNum;
	int columnNum;
	TMXTiledMap* m_map;
	Vec2 positionForTiledCoord(Vec2 pos);
};

#endif