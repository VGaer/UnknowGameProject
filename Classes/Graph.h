#ifndef _Graph_H_
#define _Graph_H_

#include "Vertex.h"
#include "Edge.h"
#include "vector"
#include "map"

#define NONE_ID (Vec2(-1, -1))

class Graph
{
public:
	static Graph* getInstance();
	std::map <Vec2, Vertex*> vertices;
	std::vector <Vec2> path;
	void init(Vec2); // 从某顶点开始填充图
	void addEdgesForVertex(Vec2 startId, Vec2 endId);
	void addVertex(Vec2 vertexId, Vec2 point);
	void removeVertex(Vec2 vertexId);
	bool findPath(Vec2 startId, Vec2 endId);
	bool relax(Vec2 startId, Vec2 endId, float weight);
	float getDistance(Vec2 startId, Vec2 endId);
	Vertex* findMinHeurisVertex(std::vector <Vertex*>&);
	std::vector <Vec2> getPathResult();
	void setTildMap(TMXTiledMap*);
	TMXTiledMap* getMap();
private:
	Vec2 targetId;
	int rowNum;
	int columnNum;
	TMXTiledMap* m_map;
	Vec2 positionForTiledCoord(Vec2 pos);
};

#endif